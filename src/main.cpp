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
#include "httpserver.h"

#include<exception>

int main(int argc, char *argv[]){

	qInstallMessageHandler(outputMessage);

	QCoreApplication app(argc, argv);


	qInfo() << "FPGAOL_ZYNQ compiled at" << __TIME__ << ", " << __DATE__;
	app.setApplicationName("fpgaol_zynq");
	app.setOrganizationName("fpgaol_developer");

	new httpServer(8080, &app);

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
		ws_server = new wsServer(8090, true, &app);
	}
	catch(const std::exception& e){
		qWarning() << (e.what());
		return 255;
	}

	QObject::connect(ws_server, &wsServer::notify_start,
					fpga, &FPGA::start_notify);

	QObject::connect(fpga, &FPGA::send_fpga_msg,
			ws_server, &wsServer::sendFPGAMessage);

	QObject::connect(ws_server, &wsServer::gpio_write,
			fpga, &FPGA::write_gpio);



	qInfo("Application has started");
	app.exec();
	qInfo("Application has stopped");
}
