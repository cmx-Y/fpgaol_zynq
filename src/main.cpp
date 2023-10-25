#include<QCoreApplication>
#include<QObject>
#include<QtCore/QDebug>
#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QString>

#include<log.h>
#include<fpga.h>
#include<wsserver.h>

#include<exception>

int main(int argc, char *argv[]){

	qInstallMessageHandler(outputMessage);

	QCoreApplication app(argc, argv);


	qInfo() << "FPGAOL_ZYNQ compiled at" << __TIME__ << ", " << __DATE__;
	app.setApplicationName("fpgaol_zynq");
	app.setOrganizationName("fpgaol_developer");

	FPGA *fpga;
	try{
		fpga = new FPGA();
	}
	catch(const std::exception& e){
		qWarning() << (e.what());
		return 255;
	}

	wsServer *ws_server;
	try{
		ws_server = new wsServer();
	}
	catch(const std::exception& e){
		qWarning() << (e.what());
		return 255;
	}

	bool ok= QObject::connect(ws_server, &wsServer::notify_start,
					fpga, &FPGA::start_notify);
	qDebug() << "connect result: " << ok;

	qInfo("Application has started");
	app.exec();
	ws_server->test();
	qInfo("Application has stopped");
}
