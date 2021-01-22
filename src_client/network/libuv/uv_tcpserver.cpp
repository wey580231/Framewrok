#include "uv_tcpserver.h"

namespace Network {

	/*! 
	 * @brief 为单个数据连接分配数据管理单元
	 * @param data 服务器句柄
	 * @return 创建后的数据单元
	 */
	ClientConnHandle * allocateConnHandle(Uv_TcpServer * data)
	{
		ClientConnHandle * handle = new ClientConnHandle();
		handle->readBuff.base = new char[BUFFER_SIZE];
		handle->readBuff.len = BUFFER_SIZE;
		handle->server = data;
		handle->acceptClient = nullptr;
		return handle;
	}

	void freeConnHandle(ClientConnHandle * hdl)
	{
		delete[] hdl->readBuff.base;
		delete hdl;
	}

	Uv_TcpServer::Uv_TcpServer(uv_loop_t *eventLoop) :
		m_eventLoop(eventLoop),
		m_newConnCallback(nullptr), m_messRecvCallback(nullptr),
		m_messSendCallback(nullptr), m_closedCallback(nullptr), m_serverClosedCallback(nullptr),
		m_bForceQuit(false), m_bClosed(true)
	{
		int ret = uv_mutex_init(&m_clientMutex);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
		}
	}

	Uv_TcpServer::~Uv_TcpServer()
	{
		stop();
	}

	bool Uv_TcpServer::start(string localIp, int localPort, int backlog)
	{
		m_localIp = localIp;
		m_localPort = localPort;

		Check_Return(!init(), false);
		Check_Return(!bind(), false);
		Check_Return(!listen(backlog), false);

		cout << "server start [" << localIp.data() << ":" << localPort << "]" << endl;

		return true;
	}

	void Uv_TcpServer::stop()
	{
		if (m_bClosed)
			return;

		m_bForceQuit = true;
		uv_async_send(&m_handleClose);
	}

	string Uv_TcpServer::errorInfo() const
	{
		return m_errorMsg;
	}

	bool Uv_TcpServer::setKeepAlive(bool enable, uint delaySeconds)
	{
		int ret = uv_tcp_keepalive(&m_tcpServer, enable ? 1 : 0, delaySeconds);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
			//RLOG_ERROR_S(m_errorMsg);
			return false;
		}
		return true;
	}

	bool Uv_TcpServer::setNoDelay(bool enable)
	{
		int ret = uv_tcp_nodelay(&m_tcpServer, enable ? 1 : 0);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
			//RLOG_ERROR_S(m_errorMsg);
			return false;
		}
		return true;
	}

	bool Uv_TcpServer::init()
	{
		int ret = 0;
		do {
			ret = uv_async_init(m_eventLoop, &m_handleClose, asyncCloseCB);
			if (ret != 0) {
				break;
			}

			ret = uv_tcp_init(m_eventLoop, &m_tcpServer);
			if (ret != 0) {
				break;
			}

			m_handleClose.data = this;
			m_tcpServer.data = this;

			ret = uv_tcp_nodelay(&m_tcpServer, 1);
			if (ret != 0) {
				break;
			}

			return true;

		} while (0);

		return false;
	}

	bool Uv_TcpServer::bind()
	{
		int ret;
		do {
			struct sockaddr_in localAddr;
			ret = uv_ip4_addr(m_localIp.data(), m_localPort, &localAddr);
			if (ret != 0)
				break;

			ret = uv_tcp_bind(&m_tcpServer, (struct sockaddr *)&localAddr, 0);
			if (ret != 0)
				break;

			return true;
		} while (0);

		m_errorMsg = getLastUvError(ret);
		//RLOG_ERROR_S(m_errorMsg);
		return false;
	}

	bool Uv_TcpServer::listen(int backLog)
	{
		int ret;
		do {
			ret = uv_listen((uv_stream_t*)&m_tcpServer, backLog, acceptCB);
			if (ret != 0)
				break;

			return true;
		} while (0);

		m_errorMsg = getLastUvError(ret);
		//RLOG_ERROR_S(m_errorMsg);
		return false;
	}

	/*!
	 * @brief 关闭服务器上所持有的连接
	 */
	void Uv_TcpServer::closing()
	{
		if (m_bClosed)
			return;

		uv_mutex_lock(&m_clientMutex);
		std::for_each(m_clientList.begin(), m_clientList.end(), [&](AcceptTcpClient * client) {
			client->close();
		});
		uv_mutex_unlock(&m_clientMutex);

		uv_close((uv_handle_t*)&m_tcpServer, serverCloseCB);
	}

	int Uv_TcpServer::allocateClientId()
	{
		static int client = 20;
		return ++client;
	}

	/*! 
	 * @brief 新客户端连接回调
	 * @param server 服务器数据信息
	 */
	void Uv_TcpServer::acceptCB(uv_stream_t *server, int status)
	{
		Uv_TcpServer * tcpServer = static_cast<Uv_TcpServer *>(server->data);
		Check_Return(tcpServer == nullptr, );

		if (status != 0) {
			tcpServer->m_errorMsg = getLastUvError(status);
			//RLOG_ERROR_S(tcpServer->m_errorMsg);
			return;
		}

		ClientConnHandle * conn = allocateConnHandle(tcpServer);

		int ret;
		do {
			ret = uv_tcp_init(tcpServer->m_eventLoop, &conn->tcpConn);
			if (ret != 0)
				break;

			conn->tcpConn.data = conn;
			conn->clientId = tcpServer->allocateClientId();

			ret = uv_accept(server, (uv_stream_t*)&conn->tcpConn);
			if (ret != 0)
				break;

			ret = uv_read_start((uv_stream_t*)&conn->tcpConn, allocBufferForRecvCB, recvDataCB);
			if (ret != 0) {
				uv_close((uv_handle_t*)&conn->tcpConn, freeAcceptFailedClient);
				break;
			}

			AcceptTcpClient * remoteClient = new AcceptTcpClient(tcpServer->m_eventLoop, conn);
			remoteClient->setConnClosedCallBack(removeClient);

			uv_mutex_lock(&tcpServer->m_clientMutex);
			tcpServer->m_clientList.push_back(remoteClient);
			uv_mutex_unlock(&tcpServer->m_clientMutex);

			if (tcpServer->m_newConnCallback) {
				tcpServer->m_newConnCallback(remoteClient);
			}

			cout << "new client id=" << conn->clientId << endl;
			return;

		} while (0);

		freeConnHandle(conn);

		tcpServer->m_errorMsg = getLastUvError(ret);
		//RLOG_ERROR_S(tcpServer->m_errorMsg);
	}

	void Uv_TcpServer::allocBufferForRecvCB(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
	{
		R_UNUSED(suggested_size);
		ClientConnHandle * hdl = static_cast<ClientConnHandle*>(handle->data);
		if (hdl) {
			*buf = hdl->readBuff;
		}
	}

	/*!
	 * @brief 客户端接收数据回调
	 */
	void Uv_TcpServer::recvDataCB(uv_stream_t *stream, ssize_t nread, const uv_buf_t *buf)
	{
		ClientConnHandle * hdl = static_cast<ClientConnHandle*>(stream->data);
		if (hdl) {
			AcceptTcpClient * remoteClient = static_cast<AcceptTcpClient*>(hdl->acceptClient);
			if (nread < 0) {
				if (nread == UV_EOF) {
					printf("client %i recv eof!\n", hdl->clientId);
				}
				else if (nread == UV_ECONNRESET) {
					printf("client %i rst!\n", hdl->clientId);
				}
				else {
					printf("other recv error %s.\n", getLastUvError(nread).data());
				}

				remoteClient->close();
			}
			else if (nread > 0) {
				Uv_TcpServer * server = static_cast<Uv_TcpServer *>(hdl->server);
				if (server->m_messRecvCallback) {
					server->m_messRecvCallback(remoteClient, buf->base, nread);
				}
				else {
					cout << "client " << remoteClient->m_clientId << " recv " << nread << " bytes!" << endl;
				}
			}
		}
	}

	void Uv_TcpServer::asyncCloseCB(uv_async_t *handle)
	{
		Uv_TcpServer * tcpServer = static_cast<Uv_TcpServer *>(handle->data);
		if (tcpServer->m_bForceQuit) {
			tcpServer->closing();
		}
	}

	void Uv_TcpServer::serverCloseCB(uv_handle_t *handle)
	{
		Uv_TcpServer * tcpServer = static_cast<Uv_TcpServer *>(handle->data);
		if (tcpServer->m_serverClosedCallback) {
			tcpServer->m_serverClosedCallback();
		}
		uv_mutex_destroy(&tcpServer->m_clientMutex);

		cout << "serverCloseCB handle:" << handle << endl;
	}

	void Uv_TcpServer::freeAcceptFailedClient(uv_handle_t *handle)
	{
		ClientConnHandle * conn = static_cast<ClientConnHandle *>(handle->data);
		if (conn) {
			freeConnHandle(conn);
		}
	}

	/*!
	 * @brief 客户端断连后移除客户端
	 * @attention 在移除客户端之前需要回调用户接口后再删除客户端。
	 * @param[in] client 待移除的客户端句柄
	 */
	void Uv_TcpServer::removeClient(AcceptTcpClient *client)
	{
		Uv_TcpServer * tcpServer = static_cast<Uv_TcpServer*>(client->m_connHandle->server);

		if (tcpServer->m_closedCallback) {
			tcpServer->m_closedCallback(client);
		}

		uv_mutex_lock(&tcpServer->m_clientMutex);
		tcpServer->m_clientList.remove(client);
		uv_mutex_unlock(&tcpServer->m_clientMutex);

		cout << "remove client id:" << client->m_connHandle->clientId << endl;
		freeConnHandle(client->m_connHandle);
		delete client;

		client = nullptr;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	AcceptTcpClient::AcceptTcpClient(uv_loop_t * eventLoop, ClientConnHandle * handle) :
		m_eventLoop(eventLoop), m_connHandle(handle), m_clientId(handle->clientId), m_bIsClosed(false),
		m_closeCallback(nullptr), m_writeCallback(nullptr),
		m_fixedRingBuffer(K_BYTES(64)), m_bForceQuit(false)
	{
		m_connHandle->acceptClient = this;

		int ret = uv_async_init(m_eventLoop, &m_async, asyncCB);
		if (ret == 0) {
			m_async.data = this;
		}
	}

	AcceptTcpClient::~AcceptTcpClient()
	{
		close();
	}

	void AcceptTcpClient::close()
	{
		if (m_bIsClosed)
			return;

		m_bForceQuit = true;
		m_connHandle->tcpConn.data = this;
		uv_close((uv_handle_t*)&m_connHandle->tcpConn, clientClose);
	}

	bool AcceptTcpClient::isClosed() const
	{
		return m_bIsClosed;
	}

	int AcceptTcpClient::send(const char *data, int len)
	{
		Check_Return(data == nullptr || len <= 0, 0);

		uv_async_send(&m_async);

		int t_writeToBuff = 0;
		while (!m_bForceQuit) {
			uv_mutex_lock(&m_writeBuffMutex);
			t_writeToBuff += m_fixedRingBuffer.append(data, len);
			uv_mutex_unlock(&m_writeBuffMutex);

			if (t_writeToBuff < len) {
				//TODO 考虑等会再重试
				continue;
			}
			else {
				break;
			}
		}
		uv_async_send(&m_async);

		return t_writeToBuff;
	}

	void AcceptTcpClient::clientClose(uv_handle_t *handle)
	{
		AcceptTcpClient * tcpClient = static_cast<AcceptTcpClient *>(handle->data);
		if (tcpClient) {
			tcpClient->m_bIsClosed = true;
			//        uv_mutex_destroy(&tcpClient->m_writeBuffMutex);
			if (tcpClient->m_closeCallback) {
				tcpClient->m_closeCallback(tcpClient);
			}
		}
	}

	void AcceptTcpClient::asyncCB(uv_async_t *handle)
	{
		AcceptTcpClient * tcpClient = static_cast<AcceptTcpClient*>(handle->data);
		if (tcpClient) {
			if (tcpClient->m_bForceQuit) {
				return;
			}
			tcpClient->sendData(nullptr);
		}
	}

	void AcceptTcpClient::sendData(uv_write_t *req)
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
			int iret = uv_write(&seg->writeReq, (uv_stream_t*)&m_connHandle->tcpConn, &seg->dataBuff, 1, sendDataCB);

			if (iret) {
				m_sendList.push_back(seg);
				//RLOG_ERROR("client send error:%s", RUtil::getLastUvError(iret).data());
				fprintf(stdout, "send error. %s-%s\n", uv_err_name(iret), uv_strerror(iret));
			}
		}
	}

	void AcceptTcpClient::sendDataCB(uv_write_t *req, int status)
	{
		AcceptTcpClient* tcpClient = (AcceptTcpClient*)req->data;

		if (status < 0) {
			if (tcpClient->m_sendList.size() > MAX_SEND_LIST_SIZE) {
				freeWriteSegment((WriteSegment*)req);
			}
			else {
				tcpClient->m_sendList.push_back((WriteSegment*)req);
			}

			//RLOG_ERROR("send error:%s", RUtil::getLastUvError(status));
			//fprintf(stderr, "send error %s\n", RUtil::getLastUvError(status).data());
			return;
		}
		else {
			freeWriteSegment((WriteSegment*)req);

			if (tcpClient->m_writeCallback) {
				tcpClient->m_writeCallback(tcpClient, 0);
			}
		}
	}

}//namespace Network