/*!
 *  @brief     封装libuv的udp客户端
 *  @details
 *             1.支持组播设置
 *  @author    wey
 *  @version   1.0
 *  @date      2020.02.29
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef UV_UDPCLIENT_H
#define UV_UDPCLIENT_H

#include "uv_headfile.h"

namespace Network {

	typedef std::function<void(char * data, int len, const sockaddr_in * remote)> UdpRecvCallBack;

	class Uv_UdpClient
	{
	public:
		Uv_UdpClient(uv_loop_t * eventLoop);
		~Uv_UdpClient();

		/*!< 加入/离开组播 */
		bool joinGroup(string localIp, string multiIp);
		bool leaveGroup(string localIp, string multiIp);

		/*!< 广播 */
		bool setBroadcast(bool enabled);

		bool start(string localIp, int localPort, UdpRecvCallBack cb = nullptr);
		bool send(char *data, int len, string remoteIp, int remotePort);

		const string& errorInfo()const;

	private:
		void init();

		static void udpMultiStorage(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf);
		static void onRecvData(uv_udp_t *handle, ssize_t nread, const uv_buf_t *buf, const sockaddr * senderAddr, unsigned flags);
		static void udpSend(uv_udp_send_t* req, int status);

	private:
		uv_loop_t * m_eventLoop;

		uv_udp_t m_udp;
		UdpRecvCallBack m_udpCallback;

		bool m_bOpenBroadcast;                      /*!< 是否开启广播 */
		bool m_bNeedJoinGroup;                      /*!< 是否需要加入组播 */
		pair<string, string> m_multicastPair;        /*!< first:本地网卡地址，second:组播地址 */

		char * m_allocBuff;

		bool m_bInitSucced;
		string m_errorMsg;
	};

} //namespace Network 

#endif // UV_UDPCLIENT_H
