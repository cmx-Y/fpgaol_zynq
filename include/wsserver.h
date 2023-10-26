#ifndef WSSERVER_H
#define WSSERVER_H

#include<QtCore/QObject>
#include<QtCore/QDebug>

class wsServer : public QObject
{
	Q_OBJECT
public:
	wsServer();
	~wsServer();
	void test();

signals:
	int notify_start();
};


#endif // WSSERVER_H
