#include<stdio.h>
#include<fcntl.h>
#include<sys/mman.h>


#include<QtCore/QDebug>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>

#include<fpga.h>

#define LED_BASE_ADDR 0x41200000
#define SW_BASE_ADDR  0x41210000

static bool notifying = false;

int write_gpio(int gpio, int level){
  int memfd;
  if ((memfd = open("/dev/mem", O_RDWR | O_DSYNC)) == -1){
    printf("Can't open /dev/mem\n");
    return -1;
  }

  void *lptr = mmap (0 , 1*sizeof(char), PROT_READ|PROT_WRITE, MAP_SHARED, memfd, LED_BASE_ADDR);
  if(lptr == MAP_FAILED){
    printf("lfail");
    return -1;
  }

  unsigned char curr_val = *(unsigned char*)lptr;
  unsigned char offset = 1 << gpio;
  if(level == 1){
    *(unsigned char*)lptr = curr_val | offset;
  }
  else{
    offset = offset ^ 0xff;
    *(unsigned char*)lptr = curr_val & offset;
  }
}

static void loop_fn(){
	QJsonObject led_json, seg_json;
	QJsonArray led_val, seg_val;
	qInfo() << "monitor_thrd launch successfully";

	for(int i = 0; i < 4; i++) led_val.append(0);

	led_json["type"] = "LED";
	led_json["values"] = led_val;

	auto msg = QJsonDocument(led_json).toJson(QJsonDocument::Compact);
	qDebug() << "led_json: " << msg;
	return;
}

int FPGA::start_notify(){
	qInfo() << "start_notify receive notify_start";
	monitor_thrd = std::thread(loop_fn);
	return 114514;
}

FPGA::FPGA(){
	qInfo() << "FPGA instance init successfully\n";
}

FPGA::~FPGA(){
}
