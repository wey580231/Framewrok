﻿/*!
 *  @brief     控件ID
 *  @details   封装了控件的身份信息，用于比较操作
 *  @file      id.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.16
 *  @warning
 *  @copyright NanJing RenGu.
 */

#ifndef ID_H
#define ID_H

#include <QString>

#include "../base_global.h"

namespace Base {

	class BASESHARED_EXPORT Id
	{
	public:
		Id();

		Id(const char * name);
		Id(const Id & source);
		~Id();

		const char * data();

		bool isValid() { return str; }

		Id& operator= (const char * name);
		Id& operator= (const Id&);

		bool operator== (Id id)const;
		bool operator== (const char * name)const;

		bool operator!= (Id id)const;
		bool operator!= (const char * name)const;

		bool operator< (Id id)const;
		bool operator< (const char * name)const;

		Id& operator+ (const char * name);
		Id& operator+ (const Id &id);

		QString toString();

	private:
		char * str;
	};

} //namespace Base

#endif // ID_H
