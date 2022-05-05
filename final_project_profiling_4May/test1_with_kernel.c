 
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <sys/mman.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/version.h>
#include <math.h>    
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <termios.h>
#include <sys/time.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)

#define PL_SIZE 4096UL
#define PL_MASK (PL_SIZE - 1)

#include "common_functions.h"

#define START_ADDRESS 0xA0000000

#undef DEBUG
#define DEBUG

#define ONE_BIT_MASK(_bit)    (0x00000001 << (_bit))

 
int main(void)
{ 
	dh = open("/dev/mem", O_RDWR | O_SYNC); // Open /dev/mem which represents the whole physical memory
	if(dh == -1) {
		printf("Unable to open /dev/mem\n");
	}
	
    uint32_t* acc_virtual_address  = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, dh, START_ADDRESS); // Memory map AXI Lite register block
			  ps_reg 			   = mmap(NULL, PL_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, dh, 0xFD1A0000 &~PL_MASK);		//Block for PS clock related register
			  pl_reg 			   = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, dh, 0xFF5E0000 & ~PL_MASK);		//Block for PL clock related register	
    int i,j;
    struct timeval start_timestamp;
	
	change_ps_freq(1);
	change_pl_freq(1);
	
	pm_call(0xA0000000,0x10,0x33221100,0, acc_virtual_address);
	pm_call(0xA0000000,0x14,0x77665544,0, acc_virtual_address);
	pm_call(0xA0000000,0x18,0xbbaa9988,0, acc_virtual_address);
	pm_call(0xA0000000,0x1C,0xffeeddcc,0, acc_virtual_address);
	pm_call(0XA0000000,0x00,0x1, 0, acc_virtual_address);
	int value = 0;
	while(value!=1) {
    value = dm_call_ret(0XA0000000,0x1,0, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = (value << 1) && 1;
	}
	
	//Mix Column Values
	value = dm_call_ret(0XA0000000,0x20,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x24,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x28,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x2C,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);


    return 0;
}







