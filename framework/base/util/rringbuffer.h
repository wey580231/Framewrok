/*!
 *  @brief     自定义环形缓冲区
 *  @details   环形缓冲区用于取代服务器端网络数据频繁的new/delete，目前已知有两种实现方式：
 *             1.预先分配：程序启动时即预先分配一定数量的空间，程序运行过程中尺寸不变，只移动读写指针；
 *             2.动态分配：程序初始时不分配空间，待有数据到来时动态的根据数据的大小创建数据块，并将数据库保存至链表。链表的集合有一定的上限;
 *                       在达到最大长度后，再从头覆盖写入。
 *                  head                        tail
 *                  |_____________________________|
 *                __|            bufferSize       |__
 *               |  |_____________________________|  |
 *               |                                   |
 *               |---------------<<------------------|
 *
 *  @author    wey
 *  @version   1.0
 *  @date      2019.01.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RRINGBUFFER_H
#define RRINGBUFFER_H

#include <QSharedPointer>
#include "../base_global.h"

#define K_BYTES(n) (n * 1024)
#define M_BYTES(n) (n * 1024*1024)

namespace Base {

	/*!
	 *  @brief 固定大小的环形缓冲区
	 */
	class BASESHARED_EXPORT RFixedRingBuffer
	{
	public:
		RFixedRingBuffer(int growth = M_BYTES(10));
		~RFixedRingBuffer();

		bool isValid() { return readPointer >= 0 && writePointer >= 0 && bufferSize > 0; }
		bool isEmpty();
		bool isFull();

		int dataSize() { return writePointer - readPointer; }
		int emptySize() { return bufferSize - dataSize(); }

		/*!< 读写处理 */
		int append(const char * data, int length);
		int read(char * data, int maxLen);
		int read(QByteArray & data, int maxLen);
		void skipRead(quint64 skipLen);

		/*!< 预读取 */
		int preRead(char * data, int maxLen);
		int preRead(int offset, char *data, int maxLen);

		void reset();
		void clear();

		quint64 getReadPos() { return readPointer; }
		quint64 getWritePos() { return writePointer; }

		void rollBackReadPointer(int size);

	private:
		int readData(char * data, int maxLen);

	private:
		quint64 readPointer;         /*!< 写指针，与每次读的长度累加 */
		quint64 writePointer;        /*!< 写指针，与每次写的长度累加 */
		quint64 bufferSize;          /*!< 缓冲区大小 */
		QByteArray buffer;           /*!< 数据缓冲区 */
	};

} //namespace Base

#endif // RRINGBUFFER_H
