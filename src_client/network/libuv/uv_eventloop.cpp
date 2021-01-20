#include "uv_eventloop.h"

#include <algorithm>

#include <base\util\rutil.h>

namespace Network {

	Uv_EventLoop::Uv_EventLoop() :m_startProgress(START_INIT), m_bForceQuit(false), m_bClosed(true), m_timerIdArray({ 0 })
	{
		int ret;
		do {
			ret = uv_loop_init(&m_eventLoop);
			if (ret) {
				break;
			}

			ret = uv_async_init(&m_eventLoop, &m_handleClose, closeAll);
			if (ret) {
				break;
			}

			m_handleClose.data = this;

		} while (0);

		if (ret) {
			m_errorMsg = getLastUvError(ret);
		}
	}

	bool Uv_EventLoop::startLoop(uv_run_mode status)
	{
		m_runMode = status;

		int ret = uv_thread_create(&m_serverThread, loopThread, this);
		if (ret != 0) {
			m_errorMsg = getLastUvError(ret);
			return false;
		}

		int t_busyLoop = 0;
		while (m_startProgress == START_INIT) {
			msleep(100);
			if (++t_busyLoop > 20) {
				m_startProgress = START_ERROR;
				break;
			}
		}

		return m_startProgress == START_SUCCESS;
	}

	void Uv_EventLoop::stopLoop()
	{
		if (m_bClosed)
			return;

		m_bForceQuit = true;
		uv_async_send(&m_handleClose);
	}

	/*!
	 * @brief 在当前时间之后指定时刻执行一次
	 * @param[in] millSeconds 指定时刻
	 * @param[in] cb 超时回调
	 * @return -1:创建失败
	 *         >0:定时器Id
	 */
	int Uv_EventLoop::runAt(int milliSeconds, const TimerCallback &cb)
	{
		Check_Return(milliSeconds <= 0 || cb == nullptr, -1);

		return createInnerTimer(cb, milliSeconds, 0);
	}


	/*!
	 * @brief 开启周期运行定时器
	 * @param[in] millSeconds 重复周期
	 * @param[in] cb 超时回调
	 * @return -1:创建失败
	 *         >0:定时器Id
	 */
	int Uv_EventLoop::runEvery(int milliSeconds, const TimerCallback &cb)
	{
		Check_Return(milliSeconds <= 0 || cb == nullptr, -1);

		return createInnerTimer(cb, milliSeconds, milliSeconds);
	}

	void Uv_EventLoop::cancelTimer(int timerId)
	{
		if (timerId >= 0) {
			if (timerId >= 0 && timerId < m_timerIdArray.size()) {

				if (m_timerIdArray[timerId] == 1) {
					auto result = std::find_if(m_timers.begin(), m_timers.end(), [&timerId](const TimerHandle * hdl) {
						return hdl->m_timerId == timerId;
					});

					if (result != m_timers.end()) {
						if (!uv_is_closing((uv_handle_t*)(*result)->timer))
							uv_close((uv_handle_t*)(*result)->timer, timercloseCB);
					}
				}
			}
		}
	}

	uv_loop_t *Uv_EventLoop::eventLoop()
	{
		return &m_eventLoop;
	}

	void Uv_EventLoop::loopThread(void *arg)
	{
		Uv_EventLoop * loop = static_cast<Uv_EventLoop*>(arg);
		if (loop) {
			loop->runLoop();
		}
	}

	void Uv_EventLoop::closeAll(uv_async_t* handle)
	{
		Uv_EventLoop * loop = static_cast<Uv_EventLoop*>(handle->data);
		if (loop->m_bForceQuit) {
			loop->closing();
		}
	}

	void Uv_EventLoop::walkCB(uv_handle_t *handle, void *arg)
	{
		if (!uv_is_closing(handle)) {
			uv_close(handle, handleCB);
		}
	}

	void Uv_EventLoop::handleCB(uv_handle_t *handle)
	{
		cout << "close handle" << handle << endl;
	}

	void Uv_EventLoop::timeroutCB(uv_timer_t *handle)
	{
		TimerHandle * hdl = static_cast<TimerHandle*>(handle->data);
		if (hdl && hdl->callBack) {
			hdl->callBack();

			if (hdl->m_bSingleShot) {
				int ret = uv_timer_stop(handle);
				if (ret == 0) {
					uv_close((uv_handle_t*)handle, timercloseCB);
				}
			}
		}
	}

	void Uv_EventLoop::timercloseCB(uv_handle_t *handle)
	{
		TimerHandle * hdl = static_cast<TimerHandle*>(handle->data);
		if (hdl) {
			if (hdl->m_timerId >= 0 && hdl->m_timerId < hdl->loop->m_timerIdArray.size()) {
				hdl->loop->m_timerIdArray[hdl->m_timerId] = 0;
			}

			hdl->loop->m_timers.remove(hdl);
			delete hdl;

			delete (uv_timer_t*)(handle);
		}
	}

	void Uv_EventLoop::runLoop()
	{
		m_bForceQuit = false;
		m_bClosed = false;
		int ret = uv_run(&m_eventLoop, m_runMode);
		m_bClosed = true;

		if (ret) {
			m_errorMsg = getLastUvError(ret);
		}
	}

	void Uv_EventLoop::closing()
	{
		if (m_bClosed)
			return;

		uv_walk(&m_eventLoop, walkCB, this);
	}

	/*!
	 * @brief 注册定时器
	 * @param[in] cb 超时回调
	 * @param[in] timeout 超时时间
	 * @param[in] repeat 重复周期
	 * @return -1:创建失败
	 *         >0:定时器句柄
	 */
	int Uv_EventLoop::createInnerTimer(const TimerCallback &cb, int timeout, int repeat)
	{
		uv_timer_t * timer = new uv_timer_t;
		int ret = uv_timer_init(&m_eventLoop, timer);
		if (ret) {
			delete timer;
			return -1;
		}

		TimerHandle * handle = new TimerHandle();
		handle->callBack = cb;
		handle->loop = this;
		handle->m_timerId = allocateTimerId();
		handle->timer = timer;

		if (repeat > 0)
			handle->m_bSingleShot = false;

		timer->data = handle;

		ret = uv_timer_start(timer, timeroutCB, timeout, repeat);
		if (ret) {
			delete handle;
			delete timer;
			return -1;
		}

		m_timers.push_back(handle);

		return handle->m_timerId;
	}

	/*!
	 * @brief 分配定时器Id
	 * @attention 定时器ID要具有唯一性，单一个eventloop中也不可能出现上千个定时器。所以其范围不必过大
	 * @return
	 */
	int Uv_EventLoop::allocateTimerId()
	{
		for (int i = 0; i < m_timerIdArray.size(); i++) {
			if (m_timerIdArray[i] == 0) {
				m_timerIdArray[i] = 1;
				return i;
			}
		}
	}

} //namespace Network