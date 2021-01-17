#include "rringbuffer.h"

namespace Base {

RFixedRingBuffer::RFixedRingBuffer(int growth):bufferSize(growth),readPointer(0),writePointer(0)
{
    buffer.resize(bufferSize);
}

RFixedRingBuffer::~RFixedRingBuffer()
{
    buffer.resize(0);
}

bool RFixedRingBuffer::isEmpty()
{
    if(!isValid())
        return false;
    return readPointer == writePointer;
}

bool RFixedRingBuffer::isFull()
{
    if(!isValid())
        return false;
    return (writePointer - readPointer) == bufferSize;
}

/*!
 * @brief 向缓冲区内写入指定长度数据
 * @details 1.若缓冲区满，直接返回；
 *          2.若缓冲区剩余空间小于可写入空间，只写入部分;
 *          3.若写入的数据长度超过tail，将数据剩余部分从head拷贝;
 *          4.若不满足以上各种，数据直接在当前位置往后写
 * @param[in]  data 待写入数据启动位置
 * @param[in] length 待写入长度
 */
void RFixedRingBuffer::append(const char *data, int length)
{
    //[0]
    if(data == NULL || length <= 0)
        return;

    //[1]
    if(!isValid() || isFull())
        return;

    //[2]
    int writeBytes = qMin(emptySize(),length);

    int pos = writePointer % bufferSize;
    //[3]
    if(pos + writeBytes > bufferSize){
        int tailWriteLen = bufferSize - pos;
        ::memcpy(buffer.data() + pos,data,tailWriteLen);
        ::memcpy(buffer.data(),data + tailWriteLen,writeBytes - tailWriteLen);
    }else{
        ::memcpy(buffer.data() + pos,data,writeBytes);
    }
    writePointer += writeBytes;
}

/*!
 * @brief 从缓冲区读取指定长度数据
 * @details 1.若缓冲区空，直接返回0
 *          2.若缓冲区可读空间小于需要读取的空间，只返回可读空间;
 *          3.若不满足以上，直接读取
 * @param[in] data 待读取的缓冲区
 * @param[in] maxLen 读取长度
 * @return 最大读取长度
 */
int RFixedRingBuffer::read(char *data, int maxLen)
{
    if(!isValid())
        return -1;
    int readLen = readData(data,maxLen);
    readPointer += readLen;
    return readLen;
}

int RFixedRingBuffer::read(QByteArray &data, int maxLen)
{
    if(!isValid())
        return -1;

    int readLen = qMin(dataSize(),maxLen);
    data.resize(readLen);
    return read(data.data(),readLen);
}

/*!
* @brief 从当前位置跳过指定长度数据再读取
* @attention 此操作影响读指针
* @param[in] skipLen 跳过的数据长度
*/
void RFixedRingBuffer::skipRead(quint64 skipLen)
{
    if(skipLen <= 0)
        return;

    if(skipLen > dataSize())
        skipLen = dataSize();

    readPointer += skipLen;
}

/*!
 * @brief 预读取到一定数据
 * @attention 预读取时，读指针不移动
 * @param[in] data 待读取的缓冲区
 * @param[in] maxLen 读取长度
 * @return 最大读取长度
 */
int RFixedRingBuffer::preRead(char *data, int maxLen)
{
    return readData(data,maxLen);
}

/*!
 * @brief 在当前位置的基础上，偏移offset的长度，读取数据
 * @param[in] offset 位置偏移量
 * @param[in] data 待读取的缓冲区
 * @param[in] maxLen 读取长度
 * @return 最大读取长度
 */
int RFixedRingBuffer::preRead(int offset, char *data, int maxLen)
{
    if(!isValid())
        return -1;
    readPointer += offset;
    int readLen = readData(data,maxLen);
    readPointer -= offset;
    return readLen;
}

/*!
 * @brief 将缓冲区数据内容重置，后期认可访问
 * @details 读、写指针归0
 * @warning 调用后，原来的数据访问指针丢失，无法再次获取
 */
void RFixedRingBuffer::reset()
{
    readPointer = 0;
    writePointer = 0;
}

/*!
 * @brief 清空缓冲区存储空间
 * @details 清空后，不可再次写入
 */
void RFixedRingBuffer::clear()
{
    readPointer = -1;
    writePointer = -1;
    bufferSize = -1;
    buffer.clear();
}

/*!
 * @brief 回滚读指针
 * @param[in]  size 回滚指定长度数据
 */
void RFixedRingBuffer::rollBackReadPointer(int size)
{

}

int RFixedRingBuffer::readData(char *data, int maxLen)
{
    if(!isValid() || data == NULL || maxLen <= 0)
        return 0;
    //[0]
    if(isEmpty())
        return 0;
    //[1]
    int readLen = qMin(dataSize(),maxLen);
    int pos = readPointer % bufferSize;
    if(pos + readLen > bufferSize){
        int tailReadLen = bufferSize - pos;
        ::memcpy(data,buffer.data() + pos,tailReadLen);
        ::memcpy(data + tailReadLen,buffer.data(),readLen - tailReadLen);
    }
    else{
        memcpy(data,buffer.data() + pos,readLen);
    }
    return readLen;
}

} //namespace Base{
