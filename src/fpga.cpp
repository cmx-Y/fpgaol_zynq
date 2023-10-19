#include<stdio.h>
#include<fcntl.h>
#include<sys/mman.h>

#define LED_BASE_ADDR 0x41200000
#define SW_BASE_ADDR  0x41210000

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

int main(){
  int gpio;
  int level;
  while(1){
    scanf("%d", &gpio);
    scanf("%d", &level);
    write_gpio(gpio, level);
  }
  return 0;
}
