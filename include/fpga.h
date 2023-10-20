#ifndef FPGA_H
#define FPGA_H

#include<QtCore/QObject>

#include<thread>

class FPGA : public QObject
{
	Q_OBJECT

private:
	std::thread monitor_thrd;
public:
	FPGA(){
		qInfo("create FPGA successfully");
	};
	~FPGA(){};
};


#endif /*FPGA_H*/
