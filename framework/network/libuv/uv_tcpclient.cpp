#include "uv_tcpclient.h"

namespace Network {

	TcpClientHandle * allocateHandle(void * client)
	{
		TcpClientHandle * hdl = new TcpClientHandle;
		hdl->uv_tcpClient = client;
		hdl->readBuf.base = (char *)malloc(BUFFER_SIZE);
		hdl->readBuf.len = BUFFER_SIZE;
		return hdl;
	}

	Uv_TcpClient::Uv_TcpClient(uv_loop_t *loop) :m_eventLoop(loop), m_bAutoReconnect(false), m_repeatConnTime(5000),
		m_bIsReconnecting(false), m_fixedRingBuffer(), m_bForceClosed(false), m_connectedState(R_CLOSED),
		m_connectedCallback(nullptr), m_recvCallback(nullptr),
		m_writeCallback(nullptr), m_closeCallback(nullptr),
		m_bExit(false), m_bindLocalPort(0), m_bIsClosed(true)
	{
		m_handle = allocateHandle(this);
		int ret = uv_mutex_init(&m_writeBuffMutex);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
		}
		m_connReq.data = this;
	}

	Uv_TcpClient::~Uv_TcpClient()
	{
		close();
	}

	/*!
	* @brief 主动发起连接远程服务器
	* @attention 返回值为true只表示调用成功，不表示已经连接成功
	* @param[in] 远程主机Ip
	* @param[in] 远程主机端口
	* @param[in] 绑定本地主机ip
	* @param[in] 绑定主机中端口,0表示不绑定,
	* @return true:调用成功;false:调用失败
	*/
	bool Uv_TcpClient::connect(string remoteAddr, int port, string bindLocalIp, int bindLocalPort)
	{
		Check_Return(m_connectedState != R_CLOSED  && m_connectedState != R_ERROR, false);

		Check_Return(remoteAddr.size() == 0 || port <= 0, false);
		Check_Return(!init(), false);

		cout << "start connect:" << remoteAddr << "_" << port << " threadId:" << this_thread::get_id() << endl;

		struct sockaddr_in addr;
		Check_Return(uv_ip4_addr(remoteAddr.data(), port, &addr) != 0, false);

		m_bindLocalIp = bindLocalIp;
		m_bindLocalPort = bindLocalPort;

		Check_Return(!bindLocalAddress(), false);

		int ret = uv_tcp_connect(&m_connReq, &m_handle->client, (const struct sockaddr *)&addr, connectCB);
		if (ret != 0) {
			getLastUvError(ret);
			return false;
		}

		m_remoteIp = remoteAddr;
		m_remotePort = port;

		m_connectedState = R_CONNECTING;

		return true;
	}

	int Uv_TcpClient::send(const char *data, int len)
	{
		Check_Return(data == nullptr || len <= 0, 0);

		uv_mutex_lock(&m_writeBuffMutex);
		int t_writeToBuff = m_fixedRingBuffer.append(data, len);
		uv_mutex_unlock(&m_writeBuffMutex);

		uv_async_send(&m_aysnc);

		return t_writeToBuff;
	}

	void Uv_TcpClient::close()
	{
		if (m_bIsClosed)
			return;

		m_connectedState = R_CLOSING;
		m_bForceClosed = true;
		uv_async_send(&m_aysnc);
	}

	bool Uv_TcpClient::connected() const
	{
		return !m_bIsClosed;
	}

	bool Uv_TcpClient::isClosed() const
	{
		return m_bIsClosed;
	}

	void Uv_TcpClient::setAutoReconnect(bool flag)
	{
		m_bAutoReconnect = flag;
	}

	bool Uv_TcpClient::isAutoReconnect() const
	{
		return m_bAutoReconnect;
	}

	int Uv_TcpClient::reconnectInterval() const
	{
		return m_repeatConnTime;
	}

	string Uv_TcpClient::errorInfo() const
	{
		return m_errorMsg;
	}

	bool Uv_TcpClient::setKeepAlive(bool enable, uint delaySeconds)
	{
		int ret = uv_tcp_keepalive(&m_handle->client, enable ? 1 : 0, delaySeconds);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
			return false;
		}
		return true;
	}

	bool Uv_TcpClient::setNoDelay(bool enable)
	{
		int ret = uv_tcp_nodelay(&m_handle->client, enable ? 1 : 0);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
			return false;
		}
		return true;
	}

	bool Uv_TcpClient::init()
	{
		if (!m_bIsClosed )
			return false;

		int ret = 0;
		do {
			ret = uv_async_init(m_eventLoop, &m_aysnc, aysncCB);
			if (ret != 0) {
				break;
			}

			m_aysnc.data = this;

			ret = uv_tcp_init(m_eventLoop, &(m_handle->client));
			if (ret != 0) {
				break;
			}

			m_handle->client.data = m_handle;
			m_handle->uv_tcpClient = this;

			ret = uv_timer_init(m_eventLoop, &m_reconnectTimer);
			if (ret != 0) {
				break;
			}

			m_reconnectTimer.data = this;

			return true;

		} while (0);

		getLastUvError(ret);
		return false;
	}

	bool Uv_TcpClient::bindLocalAddress()
	{
		if (m_bindLocalIp.size() > 0 || m_bindLocalPort != 0) {
			struct sockaddr_in localAddr;
			memset((char*)&localAddr, 0, sizeof(localAddr));

			localAddr.sin_family = AF_INET;
			localAddr.sin_port = htons(m_bindLocalPort);

			if (m_bindLocalIp.size() == 0) {
				localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
			}
			else {
				localAddr.sin_addr.s_addr = inet_addr(m_bindLocalIp.data());
			}

			int ret = uv_tcp_bind(&m_handle->client, (const struct sockaddr*)&localAddr, 0);
			if (ret != 0) {
				getLastUvError(ret);
				return false;
			}
		}
		return true;
	}

	void Uv_TcpClient::aysncCB(uv_async_t *handle)
	{
		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(handle->data);
		if (tcpClient) {
			if (tcpClient->m_bForceClosed) {
				tcpClient->closing();
				return;
			}
			tcpClient->sendData(nullptr);
		}
	}

	/*! 
	 * @brief 连接远程服务器回调
	 * @param handle 客户端句柄
	 * @param status 连接结果，<0表示连接错误；=0表示正确
	 * @return 
	 */
	void Uv_TcpClient::connectCB(uv_connect_t *handle, int status)
	{
		TcpClientHandle *  thdl = static_cast<TcpClientHandle *>(handle->handle->data);
		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(thdl->uv_tcpClient);

		if (status != 0) {
			tcpClient->m_connectedState = R_ERROR;
			tcpClient->m_errorMsg = getLastUvError(status);

			if (tcpClient->m_bAutoReconnect) {
				uv_timer_stop(&tcpClient->m_reconnectTimer);

				tcpClient->m_repeatConnTime += 1000;
				tcpClient->m_bIsReconnecting = true;

				cout << "start reconnect:" << tcpClient->m_repeatConnTime << endl;
				uv_timer_start(&tcpClient->m_reconnectTimer, Uv_TcpClient::reconnectTimerCB, tcpClient->m_repeatConnTime, tcpClient->m_repeatConnTime);
			}
			else {
				if (tcpClient->m_connectedCallback) {
					tcpClient->m_connectedCallback(nullptr);
				}
			}
			return;
		}

		cout << "connect successs,[" << tcpClient->m_remoteIp.data() << ":" << tcpClient->m_remotePort << "]" << endl;

		int ret = uv_read_start(handle->handle, allocBufferForRecvCB, recvDataCB);

		if (ret != 0) {
			tcpClient->m_connectedState = R_ERROR;
			tcpClient->m_errorMsg = getLastUvError(status);
		}
		else {
			tcpClient->m_connectedState = R_ESTABLISHED;
			tcpClient->m_bIsClosed = false;

			if (tcpClient->m_connectedCallback) {
				tcpClient->m_connectedCallback(tcpClient);
			}
		}

		if (tcpClient->m_bIsReconnecting) {
			tcpClient->stopReconnect();
		}
	}

	void Uv_TcpClient::allocBufferForRecvCB(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
	{
		R_UNUSED(suggested_size);
		TcpClientHandle * hdl = static_cast<TcpClientHandle *>(handle->data);
		if (hdl != nullptr) {
			*buf = hdl->readBuf;
		}
	}

	void Uv_TcpClient::recvDataCB(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
	{
		TcpClientHandle * hdl = static_cast<TcpClientHandle*>(stream->data);
		Check_Return(hdl == nullptr, );

		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient *>(hdl->uv_tcpClient);
		if (nread < 0) {
			if (nread == UV__EOF) {
				//RLOG_DEBUG_S("server close(EOF) socket!");
			}
			else if (nread == UV_ECONNRESET) {
				//RLOG_DEBUG_S("server close(RST) socket!");
			}
			else {
				//RLOG_DEBUG("server close %s socket!", RUtil::getLastUvError(nread).data());
			}

			//关闭后开启重传设置
			if (tcpClient->m_bAutoReconnect) {
				tcpClient->m_bIsReconnecting = true;
			}

			uv_close((uv_handle_t*)stream, remoteClientCloseCB);
			return;
		}

		if (tcpClient->m_recvCallback) {
			tcpClient->m_recvCallback(tcpClient, buf->base, nread);
		}
		else {
			cout << "recv data,len is " << nread << endl;
		}
	}

	/*!
	 * @brief 通过libuv发送数据请求
	 * @attention 1.若请求req为空，则检测缓存队列中是否存在未发送的数据，若有则发送缓存的数据；
	 *            2.若请求req不为空，判断待发送缓存队列长度是否超过最大缓存个数，若超过将其释放，否则将其加入发送的队列末；
	 * @param[in]
	 * @return
	 */
	void Uv_TcpClient::sendData(uv_write_t *req)
	{
		WriteSegment * seg = (WriteSegment*)req;
		if (req) {
			if (m_sendList.size() > MAX_SEND_LIST_SIZE) {
				freeWriteSegment(seg);
			}
			else {
				m_sendList.push_back(seg);
			}
		}

		while (true) {
			uv_mutex_lock(&m_writeBuffMutex);
			if (m_fixedRingBuffer.isEmpty()) {
				uv_mutex_unlock(&m_writeBuffMutex);
				break;
			}

			if (m_sendList.empty()) {
				seg = allocWriteSegment(m_fixedRingBuffer.dataSize());
				seg->writeReq.data = this;
				seg->dataBuff.len = m_fixedRingBuffer.read(seg->dataBuff.base, seg->dataLen);
			}
			else {
				seg = m_sendList.front();
				m_sendList.pop_front();
			}
			uv_mutex_unlock(&m_writeBuffMutex);
			int iret = uv_write(&seg->writeReq, (uv_stream_t*)&m_handle->client, &seg->dataBuff, 1, sendDataCB);

			if (iret) {
				m_sendList.push_back(seg);
				//RLOG_ERROR("client send error:%s", RUtil::getLastUvError(iret).data());
				fprintf(stdout, "send error. %s-%s\n", uv_err_name(iret), uv_strerror(iret));
			}
		}
	}

	void Uv_TcpClient::sendDataCB(uv_write_t *req, int status)
	{
		Uv_TcpClient* tcpClient = (Uv_TcpClient*)req->data;

		if (status < 0) {
			if (tcpClient->m_sendList.size() > MAX_SEND_LIST_SIZE) {
				freeWriteSegment((WriteSegment*)req);
			}
			else {
				tcpClient->m_sendList.push_back((WriteSegment*)req);
			}
			//RLOG_ERROR("send error:%s", RUtil::getLastUvError(status));
			fprintf(stderr, "send error %s\n", getLastUvError(status).data());
			return;
		}
		else {
			freeWriteSegment((WriteSegment*)req);

			if (tcpClient->m_writeCallback) {
				tcpClient->m_writeCallback(tcpClient, 0);
			}
		}
	}

	void Uv_TcpClient::reconnectTimerCB(uv_timer_t *handle)
	{
		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(handle->data);

		do {
			int ret = uv_tcp_init(tcpClient->m_eventLoop, &tcpClient->m_handle->client);
			if (ret != 0) {
				//RLOG_ERROR_S(RUtil::getLastUvError(ret));
				break;
			}

			tcpClient->m_handle->client.data = tcpClient->m_handle;
			tcpClient->m_handle->uv_tcpClient = tcpClient;

			struct sockaddr_in rsockAddr;
			ret = uv_ip4_addr(tcpClient->m_remoteIp.data(), tcpClient->m_remotePort, &rsockAddr);
			if (ret != 0) {
				//RLOG_ERROR_S(RUtil::getLastUvError(ret));
				break;
			}

			if (!tcpClient->bindLocalAddress()) {
				break;
			}

			ret = uv_tcp_connect(&tcpClient->m_connReq, &tcpClient->m_handle->client, (const struct sockaddr *)&rsockAddr, connectCB);

			if (ret != 0) {
				//RLOG_ERROR_S(RUtil::getLastUvError(ret));
				uv_close((uv_handle_t*)&tcpClient->m_handle->client, nullptr);
				break;
			}
			tcpClient->m_connectedState = R_CONNECTING;
			tcpClient->m_bIsReconnecting = true;

			return;

		} while (0);

		uv_timer_stop(handle);
		tcpClient->m_repeatConnTime += 1000;
		uv_timer_start(handle, Uv_TcpClient::reconnectTimerCB, tcpClient->m_repeatConnTime, tcpClient->m_repeatConnTime);
	}

	/*!
	 * @brief 对方关闭后回调
	 * @attention 1.对方主动关闭，若设置了周期性重连动作，则触发重连定时器工作；
	 *            2.自己主动关闭，因状态标志m_bForceClosed==true，不会再进行重连动作，回调用户关闭函数
	 */
	void Uv_TcpClient::remoteClientCloseCB(uv_handle_t *handle)
	{
		TcpClientHandle* hdl = static_cast<TcpClientHandle*>(handle->data);
		assert(hdl != nullptr);

		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(hdl->uv_tcpClient);
		assert(tcpClient != nullptr);

		tcpClient->m_bIsClosed = true;
		tcpClient->m_connectedState = R_CLOSED;

		if (handle == (uv_handle_t*)&tcpClient->m_handle->client && !tcpClient->m_bForceClosed && tcpClient->m_bIsReconnecting) {
			int ret = uv_timer_start(&tcpClient->m_reconnectTimer, Uv_TcpClient::reconnectTimerCB, tcpClient->m_repeatConnTime, tcpClient->m_repeatConnTime);
			if (ret) {
				uv_close((uv_handle_t*)&tcpClient->m_reconnectTimer, Uv_TcpClient::remoteClientCloseCB);
				//RLOG_ERROR_S(RUtil::getLastUvError(ret));
				return;
			}
		}

		if (tcpClient->m_closeCallback) {
			tcpClient->m_closeCallback(tcpClient);
		}
		else {
			cout << "close CB handle" << endl;
		}
	}

	/*!
	 * @brief 本地主动关闭连接回调
	 * @attention 使用uv_walk会将当前loop下所有的handle遍历一遍，但需要从中找到当前持有socket连接的句柄，只对它进行回调处理
	 */
	void Uv_TcpClient::localClientCloseCB(uv_handle_t *handle)
	{
		TcpClientHandle * hdl = static_cast<TcpClientHandle*>(handle->data);

		if (hdl && hdl->uv_tcpClient) {
			Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(hdl->uv_tcpClient);
			tcpClient->m_bIsClosed = true;
			uv_mutex_destroy(&tcpClient->m_writeBuffMutex);

			for (auto it : tcpClient->m_sendList) {
				freeWriteSegment(it);
			}
			tcpClient->m_sendList.clear();

			if (tcpClient && tcpClient->m_closeCallback) {
				tcpClient->m_closeCallback(tcpClient);
			}
		}
		else {
			cout << "localClientCloseCB handle" << endl;
		}
	}

	void Uv_TcpClient::stopReconnect()
	{
		uv_timer_stop(&m_reconnectTimer);
		m_bIsReconnecting = false;
		m_repeatConnTime = 5000;
	}

	void Uv_TcpClient::closing()
	{
		if (m_bIsClosed)
			return;

		stopReconnect();

		uv_close((uv_handle_t*)&m_reconnectTimer, nullptr);
		uv_close((uv_handle_t*)&m_handle->client, localClientCloseCB);
	}

} //namespace Network