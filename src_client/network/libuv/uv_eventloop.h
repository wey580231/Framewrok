/*!
 *  @brief     封装libuv中eventloop
 *  @details   主事件循环上可以挂载多个监控目标
 *  @author    wey
 *  @version   1.0
 *  @date      2020.02.
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef UV_EVENTLOOP_H
#define UV_EVENTLOOP_H

#include "uv_headfile.h"

namespace Network {

	typedef std::function<void()> TimerCallback;

	class NETWORKSHARED_EXPORT Uv_EventLoop
	{
	public:
		Uv_EventLoop();

		/*! 
		 * @brief 开启事件循环
		 * @param status 事件循环运行模式
		 * @return true:开启成功；false；开启失败
		 */
		bool startLoop(uv_run_mode status = UV_RUN_DEFAULT);
		void stopLoop();

		int runAt(int milliSeconds, const TimerCallback & cb);
		int runEvery(int milliSeconds, const TimerCallback & cb);
		void cancelTimer(int timerId);

		uv_loop_t * eventLoop();

	private:
		static void loopThread(void *arg);
		static void closeAll(uv_async_t *handle);
		static void walkCB(uv_handle_t* handle, void* arg);
		static void handleCB(uv_handle_t* handle);

		static void timeroutCB(uv_timer_t * handle);
		static void timercloseCB(uv_handle_t * handle);

		void runLoop();
		void closing();
		int createInnerTimer(const TimerCallback & cb, int timeout, int repeat = 0);
		int allocateTimerId();

	private:
		enum StartProgress {
			START_SUCCESS,
			START_ERROR,
			START_INIT
		};

		struct TimerHandle {
			TimerHandle() :callBack(nullptr), m_bSingleShot(true), timer(nullptr), m_timerId(-1) {}
			int m_timerId;
			bool m_bSingleShot;     /*!< 是否只运行一次 */
			uv_timer_t * timer;
			Uv_EventLoop * loop;
			TimerCallback callBack;
		};

	private:
		uv_thread_t m_serverThread;         /*!< 事件循环所在线程 */
		uv_loop_t m_eventLoop;
		uv_run_mode m_runMode;

		StartProgress m_startProgress;
		uv_async_t m_handleClose;

		list<TimerHandle *> m_timers;       /*!< 定时器队列 */
		array<int, 100> m_timerIdArray;      /*!< 已分配的定时器下标为1，未分配为0 */

		bool m_bClosed;         /*!< 事件循环是否关闭 */
		bool m_bForceQuit;      /*!< 主动退出 */
		string m_errorMsg;
	};

} //namespace Network

#endif // UV_EVENTLOOP_H
