/*!
 *  @brief     异步缓冲队列
 *  @details   为了简化线程间使用队列进行数据同步处理，使得生产者和消费者模型更加的简单。将同步中所涉及的mutex、condition_variable以及queue
 *             三者封装成阻塞队列
 *  @author    wey
 *  @version   1.0
 *  @date      2019.03.26
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RBLOCKINGQUQUE_H
#define RBLOCKINGQUQUE_H

#include <mutex>
#include <list>
#include <condition_variable>
#include <assert.h>

namespace Base {

	/*!
	 *  @brief 元素执行优先级
	 *  @details 默认新增加元素加入事件队尾，若有紧急事件则通过设置HighPriority，可在对头插入事件
	 */
	enum ElementPriority {
		NormalPriority,         /*!< 默认，普通优先级，加入队尾 */
		HighPriority            /*!< 高优先级，插入对头*/
	};

	template<class T>
	class RBlockingQueque
	{
	public:
		explicit RBlockingQueque() {}

		RBlockingQueque(RBlockingQueque & t_queue) = delete;
		RBlockingQueque & operator=(const RBlockingQueque & t_queue) = delete;

		/*!
		 * @brief 添加单个元素
		 * @attention 添加元素后自动通知在等待此队列不为空的线程
		 * @param[in] t 新添加元素
		 * @param[in] priority 元素执行优先级， @see ElementPriority
		 */
		void put(const T & t, ElementPriority priority = NormalPriority) {
			{
				std::unique_lock<std::mutex> ul(m_mutex);
				if (priority == NormalPriority)
					m_queue.push_back(t);
				else if (priority == HighPriority)
					m_queue.push_front(t);
			}
			m_cv.notify_one();
		}

		/*!
		 * @brief 批量添加元素
		 * @attention 添加元素后自动通知在等待此队列不为空的线程
		 * @param[in] tlist 新添加元素集合
		 */
		void put(const std::list<T> & tlist, ElementPriority priority = NormalPriority) {
			{
				std::unique_lock<std::mutex> ul(m_mutex);
				std::list<T>::const_iterator tIter = tlist.cbegin();
				while (tIter != tlist.cend()) {
					if (priority == NormalPriority)
						m_queue.push_back(*tIter);
					else if (priority == HighPriority)
						m_queue.push_front(*tIter);
					tIter++;
				}
			}
			m_cv.notify_one();
		}

		/*!
		 * @brief 获取单个元素
		 * @details 若队列元素为空且waitData=true，则调用线程会阻塞，直至有新元素加入被唤醒
		 *          若waitData=false，若队列有值则返回最顶端元素
		 * @attention 此调用在waitData=true时会导致调用线程阻塞
		 * @param[in] tlist 新添加元素集合
		 */
		T take(bool waitData = false) {
			std::unique_lock<std::mutex> ul(m_mutex);
			if (waitData) {
				while (m_queue.empty()) {
					m_cv.wait(ul);
				}
			}

			//BUG 20190326 可能线程阻塞在这里，外部直接调用wait()时会触发此问题
			assert(!m_queue.empty());

			T t_front(m_queue.front());
			m_queue.pop_front();

			return t_front;
		}

		std::list<T> takeAll(bool waitData = false) {
			std::unique_lock<std::mutex> ul(m_mutex);

			if (waitData) {
				while (m_queue.empty()) {
					m_cv.wait(ul);
				}
			}

			std::list<T> t_list;

			while (!m_queue.empty()) {
				t_list.push_back(std::move(m_queue.front()));
				m_queue.pop_front();
			}

			return t_list;
		}

		/*!
		 * @brief 最多获取指定数量的元素
		 * @details 指定waitData=true时，若队列元素数量小于指定获取条数
		 * @param[in] mostNum 最多获取的元素数量
		 * @param[in] waitData 是否等待，默认为false
		 */
		std::list<T> takeMost(const size_t mostNum, bool waitData = false) {
			std::unique_lock<std::mutex> ul(m_mutex);

			if (waitData) {
				while (m_queue.empty() || m_queue.size() < mostNum) {
					m_cv.wait(ul);
				}
			}

			std::list<T> t_list;

			int loop = 0;
			while (loop < mostNum && !m_queue.empty()) {
				t_list.push_back(m_queue.front());
				m_queue.pop_front();
				loop++;
			}

			return t_list;
		}

		size_t size() {
			std::unique_lock<std::mutex> ul(m_mutex);
			return m_queue.size();
		}

		bool isEmpty() {
			std::unique_lock<std::mutex> ul(m_mutex);
			return m_queue.size() == 0;
		}

		void clear() {
			std::unique_lock<std::mutex> ul(m_mutex);
			std::queue<T> t_tmp;
			m_queue.swap(t_tmp);
		}

		void wait() {
			std::unique_lock<std::mutex> ul(m_mutex);
			m_cv.wait(ul);
		}

		void wakeUpConsumer(bool t_wakeupAll = false) {
			if (t_wakeupAll)
				m_cv.notify_all();
			else
				m_cv.notify_one();
		}

	private:
		std::list<T> m_queue;
		std::mutex m_mutex;
		std::condition_variable m_cv;

	};

} //namespace Base

#endif // RBLOCKINGQUQUE_H
