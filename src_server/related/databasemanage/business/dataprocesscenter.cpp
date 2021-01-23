#include "dataprocesscenter.h"

namespace Related {

	DataProcessCenter::DataProcessCenter(QObject *parent)
		: QObject(parent)
	{
	}

	DataProcessCenter::~DataProcessCenter()
	{
	}

	Datastruct::UserLoginResponse DataProcessCenter::processUserLogin(int clientId, const Datastruct::UserLoginRequest & request)
	{
		Datastruct::UserLoginResponse response;



		return response;
	}

} //namespace Related 