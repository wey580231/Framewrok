#include "uv_headfile.h"

#include <iostream>

/*!
 * @brief 分配指定大小的数据块，用于拷贝发送数据
 * @attention 一次性分配长度<BUFFER_SIZE时，只分配对应大小的长度；若分配的长度>BUFFER_SIZE时，只分配BUFFER_SIZE长度数据
 * @param[in] dataSize 需分配的数据块长度
 * @return 数据请求块
 */
WriteSegment* allocWriteSegment(int fullDataSize)
{
	WriteSegment * param = (WriteSegment*)malloc(sizeof(WriteSegment));

	int realAllocSize = fullDataSize > BUFFER_SIZE ? BUFFER_SIZE : fullDataSize;
	param->deleteable = false;
	param->dataBuff.base = (char*)malloc(realAllocSize);
	param->dataBuff.len = realAllocSize;
	param->dataLen = realAllocSize;
	return param;
}

void freeWriteSegment(WriteSegment* param)
{
	free(param->dataBuff.base);
	free(param);
}

string getLastUvError(int erroCode)
{
	if (0 == erroCode)
		return "";

	std::string err;

	auto tmpChar = uv_err_name(erroCode);
	if (tmpChar) {
		err = tmpChar;
		err += ":";
	}
	else {
		err = "unknown system erroCode " + std::to_string((long long)erroCode);
		err += ":";
	}

	tmpChar = uv_strerror(erroCode);

	if (tmpChar) {
		err += tmpChar;
	}
	return std::move(err);
}

void msleep(int millsecond)
{
#if OS_WIN
	Sleep(millsecond);
#elif OS_LINUX
	usleep(millsecond * 1000);
#endif
}
