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

	Uv_TcpClient::Uv_TcpClient(uv_loop_t *loop) :
		m_eventLoop(loop), 
		m_bAutoReconnect(false), m_repeatConnTime(5000),m_bIsReconnecting(false), m_reconnectTimes(0), m_maxReconnectTimes(-1),
		m_connectedCallback(nullptr), m_recvCallback(nullptr),m_writeCallback(nullptr), m_closeCallback(nullptr),
		m_connectedState(R_CLOSED),
		m_fixedRingBuffer(),
		m_bForceClosed(false),m_bExit(false), m_bindLocalPort(0), m_bIsClosed(true)
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

		stopReconnect();

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

	void Uv_TcpClient::setMaxReconnectTimes(int maxTimes)
	{
		m_maxReconnectTimes = maxTimes;
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
			tcpClient->sendData();
		}
	}

	/*! 
	 * @brief 连接远程服务器回调
	 * @param handle 客户端句柄
	 * @param status 连接结果，<0表示连接错误；=0表示正确
	 * @return 
	 */
	void Uv_TcpClient::connectCB(uv_connect_t * handle, int status)
	{
		TcpClientHandle *  thdl = static_cast<TcpClientHandle *>(handle->handle->data);
		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(thdl->uv_tcpClient);

		if (status != 0) {
			tcpClient->m_connectedState = R_ERROR;
			tcpClient->m_errorMsg = getLastUvError(status);

			uv_close((uv_handle_t*)&tcpClient->m_handle->client,nullptr);

			if (tcpClient->m_bAutoReconnect) {

				if (tcpClient->m_maxReconnectTimes > 0 && tcpClient->m_reconnectTimes >= tcpClient->m_maxReconnectTimes) {
					tcpClient->stopReconnect();

					if (tcpClient->m_connectedCallback) {
						tcpClient->m_connectedCallback(nullptr);
					}

					return;
				}

				uv_timer_stop(&tcpClient->m_reconnectTimer);
				tcpClient->m_reconnectTimes += 1;
				tcpClient->m_repeatConnTime += 1000;

				if (tcpClient->m_reconnectCallback)
					tcpClient->m_reconnectCallback(tcpClient->m_reconnectTimes);

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

		if (tcpClient->isReconnecting()) {
			tcpClient->stopReconnect();
		}

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

			//UV__EOF UV_ECONNRESET
			tcpClient->m_errorMsg = getLastUvError(nread);

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

	int Uv_TcpClient::send(const char *data, int len)
	{
		Check_Return(data == nullptr || len <= 0, 0);

		if (m_connectedState != R_ESTABLISHED) {
			return 0;
		}

		int leftLen = len;
		while (leftLen > 0) {
			WriteSegment * seg = allocWriteSegment(len);
			memcpy(seg->dataBuff.base, data + (len - leftLen), seg->dataLen);
			seg->writeReq.data = this;

			leftLen -= seg->dataLen;

			uv_mutex_lock(&m_writeBuffMutex);
			m_cachedSendList.push_back(seg);
			uv_mutex_unlock(&m_writeBuffMutex);
		}

		uv_async_send(&m_aysnc);

		return 0;
	}

	/*! 
	 * @brief 将待发送数据从发送缓冲区移动至发送队列
	 * @details  1.将待发送数据从发送缓冲区移动至发送队列；
			     2.遍历队列:
					2.1判断当前发送单元是否已经被成功发送，若是则将其删除。
					2.2否则将其通过网络发送(并不是真正的发送成功，需要结合发送的回调函数判断)
	 */
	void Uv_TcpClient::sendData()
	{
		//[1]
		{
			uv_mutex_lock(&m_writeBuffMutex);
			m_sendList.splice(m_sendList.end(),m_cachedSendList);
			m_cachedSendList.clear();
			uv_mutex_unlock(&m_writeBuffMutex);
		}

		if (m_sendList.size() > MAX_SEND_LIST_SIZE) {
			//TODO 超过高水位，应该将其关闭掉
		}

		//[2]
		auto beg = m_sendList.begin();
		
		while (beg != m_sendList.end()) {

			//[2.1]
			if ((*beg)->deleteable) {
				freeWriteSegment(*beg);
				beg = m_sendList.erase(beg);
				continue;
			}

			WriteSegment * seg = *beg;

			//[2.2]
			int iret = uv_write(&seg->writeReq, (uv_stream_t*)&m_handle->client, &seg->dataBuff, 1, sendDataCB);

			if (iret) {
				break;
			}
			
			++beg;
		}
	}

	/*! 
	 * @brief 数据发送结束回调
	 * @param req 此次发送的数据请求
	 * @param status 发送结果 ==0表示发送成功
	 * @details 若数据发送成功，则将当前发送请求标识设置为可删除，但下次发送前统一删除。
				此举的目的是避免在在对发送队列遍历发送时，同时对其修改。
	 */
	void Uv_TcpClient::sendDataCB(uv_write_t *req, int status)
	{
		Uv_TcpClient* tcpClient = (Uv_TcpClient*)req->data;

		if (status < 0) {
			cout << "send error:"<< getLastUvError(status) << endl;
			return;
		}
		else {
			WriteSegment * seg = (WriteSegment*)req;
			seg->deleteable = true;

			if (tcpClient->m_writeCallback) {
				tcpClient->m_writeCallback(tcpClient, 0);
			}
		}
	}

	void Uv_TcpClient::reconnectTimerCB(uv_timer_t *handle)
	{
		Uv_TcpClient * tcpClient = static_cast<Uv_TcpClient*>(handle->data);

		int ret = 0;

		do {
			ret = uv_tcp_init(tcpClient->m_eventLoop, &tcpClient->m_handle->client);
			if (ret != 0) {
				break;
			}

			tcpClient->m_handle->client.data = tcpClient->m_handle;
			tcpClient->m_handle->uv_tcpClient = tcpClient;

			struct sockaddr_in rsockAddr;
			ret = uv_ip4_addr(tcpClient->m_remoteIp.data(), tcpClient->m_remotePort, &rsockAddr);
			if (ret != 0) {
				break;
			}

			if (!tcpClient->bindLocalAddress()) {
				break;
			}

			ret = uv_tcp_connect(&tcpClient->m_connReq, &tcpClient->m_handle->client, (const struct sockaddr *)&rsockAddr, connectCB);

			if (ret != 0) {
				uv_close((uv_handle_t*)&tcpClient->m_handle->client, nullptr);
				break;
			}

			tcpClient->m_connectedState = R_CONNECTING;

			return;

		} while (0);

		tcpClient->m_connectedState = R_ERROR;
		tcpClient->m_errorMsg = getLastUvError(ret);
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

		if (handle == (uv_handle_t*)&tcpClient->m_handle->client && !tcpClient->m_bForceClosed) {
			int ret = uv_timer_start(&tcpClient->m_reconnectTimer, Uv_TcpClient::reconnectTimerCB, tcpClient->m_repeatConnTime, tcpClient->m_repeatConnTime);
			if (ret) {
				uv_close((uv_handle_t*)&tcpClient->m_reconnectTimer, Uv_TcpClient::remoteClientCloseCB);
				tcpClient->m_errorMsg = getLastUvError(ret);
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
		m_repeatConnTime = 5000;
		m_reconnectTimes = 0;
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