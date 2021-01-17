#include "id.h"

#include "iostream"
#include "string.h"

namespace Base {

	Id::Id() :str(NULL)
	{

	}

	Id::Id(const char * name) : str(NULL)
	{
		if (name && *name)
		{
			size_t len = strlen(name) + 1;
			str = new char[len];
			memset(str, 0, len);
			strncpy(str, name, strlen(name));
		}
	}

	Id::Id(const Id &source)
	{
		if (source.str)
		{
			size_t len = strlen(source.str) + 1;
			str = new char[len];
			memset(str, 0, len);
			strncpy(str, source.str, strlen(source.str));
		}
	}

	Id::~Id()
	{
		if (str)
		{
			delete[] str;
			str = NULL;
		}
	}

	const char *Id::data()
	{
		return str;
	}

	Id& Id::operator= (const char * name)
	{
		if (name)
		{
			if (this->str)
			{
				delete[]  this->str;
			}

			size_t len = strlen(name) + 1;

			this->str = new char[len];
			memset(this->str, 0, len);

			strncpy(this->str, name, strlen(name));
		}

		return *this;
	}

	Id& Id::operator= (const Id& id)
	{
		if (this != &id)
		{
			delete[] this->str;

			size_t len = strlen(id.str) + 1;

			this->str = new char[len];
			memset(this->str, 0, len);

			strncpy(this->str, id.str, strlen(id.str));
		}

		return *this;
	}

	bool Id::operator==(Id id)const
	{
		return this->str && id.str && strcmp(this->str, id.str) == 0;
	}

	bool Id::operator==(const char * name)const
	{
		return this->str && name && strcmp(this->str, name) == 0;
	}

	bool Id::operator!=(Id id)const
	{
		if (this->str && id.str && strcmp(this->str, id.str) == 0)
		{
			return false;
		}

		return true;
	}

	bool Id::operator!=(const char * name)const
	{
		if (this->str && name && strcmp(this->str, name) == 0)
		{
			return false;
		}

		return true;
	}

	bool Id::operator<(Id id)const
	{
		if (this->str && id.str && strcmp(this->str, id.str) < 0)
		{
			return true;
		}

		return false;
	}

	bool Id::operator<(const char * name)const
	{
		if (this->str && name && strcmp(this->str, name) < 0)
		{
			return true;
		}

		return false;
	}

	Id &Id::operator+(const char *name)
	{
		if (name)
		{
			size_t t_strLen = strlen(this->str);
			size_t t_concatLen = strlen(name);
			size_t len = t_strLen + t_concatLen + 1;

			char * t_tmp = new char[len];
			memset(t_tmp, 0, len);

			strncpy(t_tmp, this->str, t_strLen);
			strncpy(t_tmp + t_strLen, name, t_concatLen);

			if (this->str)
				delete[]  this->str;

			this->str = t_tmp;
		}

		return *this;
	}

	Id &Id::operator+(const Id & id)
	{
		if (id.str)
		{
			size_t t_strLen = strlen(this->str);
			size_t t_concatLen = strlen(id.str);
			size_t len = t_strLen + t_concatLen + 1;

			char * t_tmp = new char[len];
			memset(t_tmp, 0, len);

			strncpy(t_tmp, this->str, t_strLen);
			strncpy(t_tmp + t_strLen, id.str, t_concatLen);

			if (this->str)
				delete[]  this->str;

			this->str = t_tmp;
		}

		return *this;
	}

	QString Id::toString()
	{
		return QString(str);
	}
} //namespace Base
