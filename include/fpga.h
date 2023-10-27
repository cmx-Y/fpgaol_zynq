#ifndef FPGA_H
#define FPGA_H

#include<QtCore/QObject>
#include<QString>

#include<thread>

class FPGA : public QObject
{
	Q_OBJECT

private:
	std::thread monitor_thrd;
public:
	FPGA();
	~FPGA();
	void call_send_fpga_msg(QString msg);

public Q_SLOTS:
	int start_notify();
	int write_gpio(int gpio, int level);

signals:
	void send_fpga_msg(QString msg);
};


#endif /*FPGA_H*/
