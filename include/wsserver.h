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

Q_SIGNALS:
	int notify_start();
};


#endif // WSSERVER_H
