#include<QCoreApplication>
#include<QObject>
#include<QtCore/QDebug>
#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QString>

#include<log.h>
#include<fpga.h>

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

	qInfo("Application has started");
	app.exec();
	qInfo("Application has stopped");
}
