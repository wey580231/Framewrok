/*!
 *  @brief     DUMP文件注册钩子函数
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2020.08.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef COREDUMP_H
#define COREDUMP_H

#include <QObject>

#ifdef Q_OS_WIN
#include <Windows.h>
#include <DbgHelp.h>
#include <ShellAPI.h>
#include <ShlObj.h>
#pragma comment(lib,"DbgHelp")
#endif

namespace App {

#ifdef Q_OS_WIN
	extern LONG __stdcall DumpExceptionHandler(PEXCEPTION_POINTERS pExceptionPonter);
	extern void EnableRecordDump();
#endif

}

#endif // COREDUMP_H
