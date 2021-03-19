#pragma once

#include <QObject>

class FileProcessCenter : public QObject
{
	Q_OBJECT

public:
	FileProcessCenter(QObject *parent = nullptr);
	~FileProcessCenter();
};
