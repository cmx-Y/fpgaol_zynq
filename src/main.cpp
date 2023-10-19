#include<QCoreApplication>
#include<QObject>
#include<QtCore/QDebug>

#include<QMutex>
#include<QDateTime>
#include<QFile>
#include<QString>

#include<log.h>

#include<iostream>

int main(int argc, char *argv[]){

  qInstallMessageHandler(outputMessage);

  QCoreApplication app(argc, argv);

  qDebug("Debug log test");
  qWarning("Warning log test");
  qCritical("Critical log test");
  //qFatal("Fatal log test");


  qInfo() << "FPGAOL_ZYNQ compiled at" << __TIME__ << ", " << __DATE__;
  app.setApplicationName("fpgaol_zynq");
  app.setOrganizationName("fpgaol_developer");

  qInfo("Application has started");
  app.exec();
  qInfo("Application has stopped");
}
