#include<stdio.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<iostream>
#include<unistd.h>


#include<QtCore/QDebug>
#include<QJsonArray>
#include<QJsonObject>
#include<QJsonDocument>

#include<fpga.h>

#define LED_BASE_ADDR 0x41200000
#define SW_BASE_ADDR  0x41210000

static bool notifying = false;
static unsigned char cur_led = 0;
FPGA *fpga_instance = nullptr;

void FPGA::call_send_fpga_msg(QString msg) {
	emit send_fpga_msg(msg);
}


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

  munmap(lptr, 1*sizeof(char));
  close(memfd);
}

unsigned char read_gpio( ){
  int memfd;
  if ((memfd = open("/dev/mem", O_RDWR | O_DSYNC)) == -1){
    printf("Can't open /dev/mem\n");
    return -1;
  }

  void *ptr = mmap (0 , 1*sizeof(char), PROT_READ, MAP_SHARED, memfd, SW_BASE_ADDR);
  if(ptr == MAP_FAILED){
    printf("lfail");
    return -1;
  }

  
  unsigned char cur_val = *(unsigned char*)ptr;
  munmap(ptr, 1*sizeof(char));
  close(memfd);
  return cur_val;
}


static void loop_fn(){
  unsigned char led = 0;
	QJsonObject led_json, seg_json;
	QJsonArray led_val, seg_val;
	qDebug() << "monitor_thrd launch successfully";

	for(int i = 0; i < 8; i++) led_val.append(0);

	led_json["type"] = "LED";
	led_json["values"] = led_val;

  while(1){
    led = read_gpio();
    if (led ^ cur_led){
      cur_led = led;
      for (int i = 0; i < 8; i++){
        led_val[i] = (int)((cur_led >> i) & 1);
      }
      led_json["values"] = led_val;
      auto msg = QJsonDocument(led_json).toJson(QJsonDocument::Compact);
	    qDebug() << "led_json: " << msg;
      fpga_instance->call_send_fpga_msg(msg);
    }
  }
	return;
}

int FPGA::start_notify(){
	qInfo() << "start_notify receive notify_start";
	monitor_thrd = std::thread(loop_fn);
	return 0;
}



FPGA::FPGA(){
  fpga_instance = this;
	qInfo() << "FPGA instance init successfully\n";
}

FPGA::~FPGA(){
}
