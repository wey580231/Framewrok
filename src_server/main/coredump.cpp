#include "coredump.h"

#include <QCoreApplication>
#include <QDir>

#include <base/util/rutil.h>
#include <core\file\programfilepath.h>

namespace App {

#ifdef Q_OS_WIN
	/*!
	 * @brief 创建奔溃时dump文件
	 */
	inline void CreateMinDump(PEXCEPTION_POINTERS pep, LPCTSTR strFileName) {
		HANDLE hFile = CreateFile(strFileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE)) {
			MINIDUMP_EXCEPTION_INFORMATION mdel;
			mdel.ThreadId = GetCurrentThreadId();
			mdel.ExceptionPointers = pep;
			mdel.ClientPointers = NULL;

			::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, (pep != 0) ? &mdel : 0, NULL, 0);
			CloseHandle(hFile);
		}
	}

	LONG __stdcall DumpExceptionHandler(PEXCEPTION_POINTERS pExceptionPonter)
	{
		Core::ProgramFilePath path;
		QString t_fullPath = path.programPath + QDir::separator() + "/dump";

		Base::RUtil::createDir(t_fullPath);
		QString t_fullFilePath = t_fullPath + QDir::separator() + Base::RUtil::getTimeStamp("yyyyMMddhhmmss") + ".dmp";

		CreateMinDump(pExceptionPonter, t_fullFilePath.toStdWString().c_str());

		return EXCEPTION_EXECUTE_HANDLER;       //遇到错误直接关闭程序
	}

	void EnableRecordDump() {
		SetUnhandledExceptionFilter(DumpExceptionHandler);
	}

#endif

} //namespace App
