#include "uv_udpclient.h"

namespace Network {

	Uv_UdpClient::Uv_UdpClient(uv_loop_t *eventLoop) :m_eventLoop(eventLoop), m_bInitSucced(false), m_allocBuff(nullptr), m_bNeedJoinGroup(false),
		m_bOpenBroadcast(false)
	{
		init();
	}

	Uv_UdpClient::~Uv_UdpClient()
	{
		if (m_allocBuff) {
			delete m_allocBuff;
		}
	}

	bool Uv_UdpClient::joinGroup(string localIp, string multiIp)
	{
		Check_Return(multiIp.size() == 0, false);

		m_bNeedJoinGroup = true;
		m_multicastPair.first = localIp;
		m_multicastPair.second = multiIp;

		return true;
	}

	bool Uv_UdpClient::leaveGroup(string localIp, string multiIp)
	{
		Check_Return(multiIp.size() == 0 || !m_bNeedJoinGroup, false);
		int ret = uv_udp_set_membership(&m_udp, m_multicastPair.second.data(), m_multicastPair.first.data(), UV_LEAVE_GROUP);
		if (ret) {
			m_errorMsg = getLastUvError(ret);
			return false;
		}
		return true;
	}

	bool Uv_UdpClient::setBroadcast(bool enabled)
	{
		int ret = uv_udp_set_broadcast(&m_udp, enabled ? 1 : 0);
		if (ret) {
			m_errorMsg = getLastUvError(ret);
			return false;
		}
		m_bOpenBroadcast = enabled;
		return true;
	}

	bool Uv_UdpClient::send(char *data, int len, string remoteIp, int remotePort)
	{
		if (!m_bInitSucced) {
			m_errorMsg = "sock is invalid!";
			return false;
		}

		if (data == nullptr || len <= 0) {
			m_errorMsg = "data or data length is incorrect!";
			return false;
		}

		if (remoteIp.size() == 0 || remotePort <= 0) {
			m_errorMsg = "dest ip or port is incorrect!";
			return false;
		}

		//NOTE libuv内部会将数据拷贝至自己的缓冲区
		uv_buf_t buf;
		buf.base = data;
		buf.len = len;

		uv_udp_send_t * req = new uv_udp_send_t;
		req->data = this;

		struct sockaddr_in dest;
		dest.sin_family = AF_INET;
		dest.sin_port = htons(remotePort);
		if (m_bOpenBroadcast) {
			dest.sin_addr.s_addr = htonl(INADDR_BROADCAST);
		}
		else {
			dest.sin_addr.s_addr = inet_addr(remoteIp.data());
		}

		int ret = uv_udp_send(req, &m_udp, &buf, 1, (struct sockaddr *)&dest, udpSend);
		if (ret) {
			m_errorMsg = getLastUvError(ret);

			free(buf.base);
			delete req;

			return false;
		}

		return true;
	}

	void Uv_UdpClient::udpSend(uv_udp_send_t* req, int status)
	{
		Uv_UdpClient * udpClient = static_cast<Uv_UdpClient *>(req->data);
		if (status < 0) {
			udpClient->m_errorMsg = getLastUvError(status);
			return;
		}

		delete req;
	}

	bool Uv_UdpClient::start(string localIp, int localPort, UdpRecvCallBack cb)
	{
		Check_Return(localPort <= 0, false);
		m_udpCallback = cb;

		struct sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = htons(localPort);

		if (localIp.size() == 0) {
			addr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		else {
			addr.sin_addr.s_addr = inet_addr(localIp.data());
		}

		int ret = 0;
		do {
			ret = uv_udp_bind(&m_udp, (struct sockaddr *)&addr, UV_UDP_REUSEADDR);
			if (ret) {
				break;
			}

			if (m_bNeedJoinGroup) {
				ret = uv_udp_set_membership(&m_udp, m_multicastPair.second.data(), m_multicastPair.first.data(), UV_JOIN_GROUP);
				if (ret) {
					break;
				}
			}

			m_udp.data = this;

			ret = uv_udp_recv_start(&m_udp, udpMultiStorage, onRecvData);
			if (ret) {
				break;
			}

			m_bInitSucced = true;

			return true;
		} while (0);

		m_errorMsg = getLastUvError(ret);
		return false;
	}

	const string &Uv_UdpClient::errorInfo() const
	{
		return m_errorMsg;
	}

	void Uv_UdpClient::init()
	{
		int ret = uv_udp_init(m_eventLoop, &m_udp);
		if (ret) {
			m_errorMsg = getLastUvError(ret);
			return;
		}
		m_bInitSucced = true;
	}

	void Uv_UdpClient::udpMultiStorage(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf)
	{
		Uv_UdpClient * udpClient = static_cast<Uv_UdpClient*>(handle->data);
		if (udpClient) {
			if (udpClient->m_allocBuff == nullptr) {
				udpClient->m_allocBuff = new char[65536];
			}

			buf->base = udpClient->m_allocBuff;
			buf->len = 65536;
		}
	}

	void Uv_UdpClient::onRecvData(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const sockaddr *senderAddr, unsigned flags)
	{
		Uv_UdpClient * udpClient = static_cast<Uv_UdpClient*>(handle->data);

		if (nread == -1) {
			uv_close((uv_handle_t*)handle, NULL);
			return;
		}

		if (nread > 0 && buf->base != nullptr) {
			if (udpClient->m_udpCallback) {
				udpClient->m_udpCallback(buf->base, nread, (const struct sockaddr_in *)senderAddr);
			}
			else {
				cout << "udp recv " << nread << " len data" << endl;
			}
		}
	}

}//namespace Network 
