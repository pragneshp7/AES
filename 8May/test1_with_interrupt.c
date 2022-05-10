 
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

//Code for interrupt handling
/* Device path name for the Interrupt device */
#define GPIO_DEV_PATH    "/dev/gpio_int"

#define GPIO_DR_NUM     0x0             // Pin Number
#define GPIO_DR         0xA0030004      // Interrupt register
#define GPIO_LED_NUM    0x1
#define GPIO_LED        0xA0030010      // LED register

/* File descriptor for Interrupt device */
int gpio_dev_fd  = -1;

/* -------------------------------------------------------------------------------
 *      Counter of number of times sigio_signal_handler() has been executed
 */
 
volatile int sigio_signal_count = 0;

/* -------------------------------------------------------------------------------
 *      Flag to indicate that a SIGIO signal has been processed
 */
static volatile sig_atomic_t sigio_signal_processed = 0;
 
/* -------------------------------------------------------------------------------
 *      Time stamp set in the last sigio_signal_handler() invocation:
 */
struct timeval sigio_signal_timestamp;

void sigio_signal_handler(int signo);
 
int main(void)
{ 
	dh = open("/dev/mem", O_RDWR | O_SYNC); // Open /dev/mem which represents the whole physical memory
	if(dh == -1) {
		printf("Unable to open /dev/mem\n");
	}
	
	acc_virtual_address  = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, dh, START_ADDRESS); // Memory map AXI Lite register block
	ps_reg 			   = mmap(NULL, PL_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, dh, 0xFD1A0000 &~PL_MASK);		//Block for PS clock related register
	pl_reg 			   = mmap(NULL, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, dh, 0xFF5E0000 & ~PL_MASK);		//Block for PL clock related register	
	volatile int rc;
    int i,j;
    struct timeval start_timestamp;
	
	/* Register signal handler for SIGIO signal */

    struct sigaction sig_action; 
    
    memset(&sig_action, 0, sizeof sig_action);
    sig_action.sa_handler = sigio_signal_handler;

    /* --------------------------------------------------------------------------
     *      Block all signals while our signal handler is executing:
     */
    (void)sigfillset(&sig_action.sa_mask);

    rc = sigaction(SIGIO, &sig_action, NULL);

    if (rc == -1) {
        perror("sigaction() failed");
        return -1;
    }

    /* -------------------------------------------------------------------------
     *      Open the device file
     */ 
        
    gpio_dev_fd = open(GPIO_DEV_PATH, O_RDWR);

     if(gpio_dev_fd == -1)    {
        perror("open() of " GPIO_DEV_PATH " failed");
        return -1;
    }    

    /* -------------------------------------------------------------------------
     * Set our process to receive SIGIO signals from the GPIO device:
     */
     
    rc = fcntl(gpio_dev_fd, F_SETOWN, getpid());
    
    if (rc == -1) {
        perror("fcntl() SETOWN failed\n");
        return -1;
    } 

    /* -------------------------------------------------------------------------
     * Enable reception of SIGIO signals for the gpio_dev_fd descriptor
     */
     
    int fd_flags = fcntl(gpio_dev_fd, F_GETFL);
        rc = fcntl(gpio_dev_fd, F_SETFL, fd_flags | O_ASYNC);

    if (rc == -1) {
        perror("fcntl() SETFL failed\n");
        return -1;
    } 

    /* -------------------------------------------------------------------------
     * Take interrupt latency measurements in a loop:
     */

    //rc = gpio_set_pin(GPIO_DR, GPIO_DR_NUM, 0);     // Clear output pin
	
    /*if (rc != 0) { 
        perror("gpio_set_pin() failed");
    return -1;
    }
	*/

    sigset_t signal_mask, signal_mask_old, signal_mask_most;
	//
	
	change_ps_freq(1);
	change_pl_freq(1);
	
	sigio_signal_processed = 0;	//Resetting the signal processed flag
	/* ---------------------------------------------------------------------
         * NOTE: This next section of code must be excuted each cycle to prevent
         * a race condition between the SIGIO signal handler and sigsuspend()
     */
         
    (void)sigfillset(&signal_mask);
    (void)sigfillset(&signal_mask_most);
    (void)sigdelset(&signal_mask_most, SIGIO);
    (void)sigprocmask(SIG_SETMASK,&signal_mask, &signal_mask_old);  
	
	pm_call(0XA0000000,0x20,0x0, 0, acc_virtual_address);
	pm_call(0xA0000000,0x00,0x33221100,0, acc_virtual_address);
	pm_call(0xA0000000,0x04,0x77665544,0, acc_virtual_address);
	pm_call(0xA0000000,0x08,0xbbaa9988,0, acc_virtual_address);
	pm_call(0xA0000000,0x0C,0xffeeddcc,0, acc_virtual_address);
	pm_call(0XA0000000,0x20,0x1, 0, acc_virtual_address);
	int value = 0;
	/*while(value!=1) {
    value = dm_call_ret(0XA0000000,0x1,0, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = (value << 1) && 1;
	}*/
	
	while(sigio_signal_processed!=1){
		printf("Value of sigio signal processing is %d\n",sigio_signal_processed);
	}
	
	(void)sigprocmask(SIG_SETMASK, &signal_mask_old, NULL);
	
	//Mix Column Values
	value = dm_call_ret(0XA0000000,0x10,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x14,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x18,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	value = dm_call_ret(0XA0000000,0x1C,0x1, acc_virtual_address);
	printf("Value returned is %x\n",value);
	
	(void)close(gpio_dev_fd);

    return 0;
}

/* -----------------------------------------------------------------------------
 * SIGIO signal handler
 */
 
void
sigio_signal_handler(int signo)
{
    volatile int rc1;

    assert(signo == SIGIO);   // Confirm correct signal #
    sigio_signal_count ++;

    //printf("sigio_signal_handler called (signo=%d)\n", signo);
	//printf("Updating sigio_signal_count to %d\n",sigio_signal_count);

    /* -------------------------------------------------------------------------
     * Set global flag
     */
     
    sigio_signal_processed = 1; 
    
    /* -------------------------------------------------------------------------
     * Take end timestamp for interrupt latency measurement
     */
    (void)gettimeofday(&sigio_signal_timestamp, NULL); 


}

/*
int gpio_set_pin(unsigned int target_addr, unsigned int pin_number, unsigned int bit_val)
{
    unsigned int reg_data;

    int fd = open("/dev/mem", O_RDWR|O_SYNC);
    
    if(fd == -1)
    {
        printf("Unable to open /dev/mem.  Ensure it exists (major=1, minor=1)\n");
        return -1;
    }    

    volatile unsigned int *regs, *address ;
    
    regs = (unsigned int *)mmap(NULL, 
                                MAP_SIZE, 
                                PROT_READ|PROT_WRITE, 
                                MAP_SHARED, 
                                fd, 
                                target_addr & ~MAP_MASK);
    
    address = regs + (((target_addr) & MAP_MASK)>>2);

#ifdef DEBUG1
    printf("REGS           = 0x%.8x\n", regs);    
    printf("Target Address = 0x%.8x\n", target_addr);
    printf("Address        = 0x%.8x\n", address);                    // display address value    
    //printf("Mask           = 0x%.8x\n", ONE_BIT_MASK(pin_number));  // Display mask value
#endif 
   
    reg_data = *address;
    
    if (bit_val == 0) {
        
        
        
        reg_data &= ~ONE_BIT_MASK(pin_number);
        *address = reg_data;
    } else {
        
        
                
        reg_data |= ONE_BIT_MASK(pin_number);
        *address = reg_data;
    }
    
    int temp = close(fd);
    if(temp == -1)
    {
        printf("Unable to close /dev/mem.  Ensure it exists (major=1, minor=1)\n");
        return -1;
    }    

    munmap(NULL, MAP_SIZE);        
    return 0;
    

    
}   
*/