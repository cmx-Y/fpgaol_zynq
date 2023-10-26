#include<wsserver.h>

void wsServer::test(){
	qInfo() << "wsServer test emit notify_start";
	int ret = emit notify_start();
	qDebug() << "Start Notify returned: " << ret;
}

wsServer::wsServer(){
	qInfo() << "wsServer instance create successfully";
}

wsServer::~wsServer(){

}


