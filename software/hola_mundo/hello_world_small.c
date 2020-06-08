/* 
 * "Small Hello World" example. 
 * 
 * This example prints 'Hello from Nios II' to the STDOUT stream. It runs on
 * the Nios II 'standard', 'full_featured', 'fast', and 'low_cost' example 
 * designs. It requires a STDOUT  device in your system's hardware. 
 *
 * The purpose of this example is to demonstrate the smallest possible Hello 
 * World application, using the Nios II HAL library.  The memory footprint
 * of this hosted application is ~332 bytes by default using the standard 
 * reference design.  For a more fully featured Hello World application
 * example, see the example titled "Hello World".
 *
 * The memory footprint of this example has been reduced by making the
 * following changes to the normal "Hello World" example.
 * Check in the Nios II Software Developers Manual for a more complete 
 * description.
 * 
 * In the SW Application project (small_hello_world):
 *
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 * In System Library project (small_hello_world_syslib):
 *  - In the C/C++ Build page
 * 
 *    - Set the Optimization Level to -Os
 * 
 *    - Define the preprocessor option ALT_NO_INSTRUCTION_EMULATION 
 *      This removes software exception handling, which means that you cannot 
 *      run code compiled for Nios II cpu with a hardware multiplier on a core 
 *      without a the multiply unit. Check the Nios II Software Developers 
 *      Manual for more details.
 *
 *  - In the System Library page:
 *    - Set Periodic system timer and Timestamp timer to none
 *      This prevents the automatic inclusion of the timer driver.
 *
 *    - Set Max file descriptors to 4
 *      This reduces the size of the file handle pool.
 *
 *    - Check Main function does not exit
 *    - Uncheck Clean exit (flush buffers)
 *      This removes the unneeded call to exit when main returns, since it
 *      won't.
 *
 *    - Check Don't use C++
 *      This builds without the C++ support code.
 *
 *    - Check Small C library
 *      This uses a reduced functionality C library, which lacks  
 *      support for buffering, file IO, floating point and getch(), etc. 
 *      Check the Nios II Software Developers Manual for a complete list.
 *
 *    - Check Reduced device drivers
 *      This uses reduced functionality drivers if they're available. For the
 *      standard design this means you get polled UART and JTAG UART drivers,
 *      no support for the LCD driver and you lose the ability to program 
 *      CFI compliant flash devices.
 *
 *    - Check Access device drivers directly
 *      This bypasses the device file system to access device drivers directly.
 *      This eliminates the space required for the device file system services.
 *      It also provides a HAL version of libc services that access the drivers
 *      directly, further reducing space. Only a limited number of libc
 *      functions are available in this configuration.
 *
 *    - Use ALT versions of stdio routines:
 *
 *           Function                  Description
 *        ===============  =====================================
 *        alt_printf       Only supports %s, %x, and %c ( < 1 Kbyte)
 *        alt_putstr       Smaller overhead than puts with direct drivers
 *                         Note this function doesn't add a newline.
 *        alt_putchar      Smaller overhead than putchar with direct drivers
 *        alt_getchar      Smaller overhead than getchar with direct drivers
 *
 */

#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_irq.h"
#include "alt_types.h"
#include <stdio.h>


void io_button(void * context);

void io_button_op(void * context);



void io_button_setup();
void io_button_setup2();



volatile int edge_val = 0;

volatile unsigned char * reg_mode = (unsigned char*)0x5010;

int mode = 0;


int main(void)
{ 
  alt_putstr("Hello from Nios II!\n");

  io_button_setup();
  io_button_setup2();




  /* Event loop never exits. */
  while (1){
	  if(mode == 1){
		  alt_putstr("Set Alarm Mode\n");
	  }else if(mode == 2){
		  alt_putstr("Set Time Mode\n");
	  }

  }
  return 0;
}

void io_button_setup(){
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BTN_MODE_BASE, 0xf);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_MODE_BASE, 0x00);

	void * edge_val_ptr;

	edge_val_ptr = (void *) &edge_val;

	alt_ic_isr_register(BTN_MODE_IRQ_INTERRUPT_CONTROLLER_ID,
			BTN_MODE_IRQ,
			io_button,
			edge_val_ptr,
			0x00);
}

void io_button_setup2(){
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BTN_OP_BASE, 0xf);

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_OP_BASE, 0x00);

	void * edge_val_ptr;

	edge_val_ptr = (void *) &edge_val;

	alt_ic_isr_register(BTN_OP_IRQ_INTERRUPT_CONTROLLER_ID,
			BTN_OP_IRQ,
			io_button_op,
			edge_val_ptr,
			0x00);
}

void io_button(void * context){
	volatile int * edge_ptr;
	edge_ptr = (volatile int *) context;

	*edge_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BTN_MODE_BASE);

	  if(edge_val & 0x01){
	  	alt_putstr("Interruption Mode\n");
	  	mode = mode + 1;
	  	if(mode == 3){
	  		mode = 0;
	  	}
	  }
	  else if(edge_val & 0x02){
	  	  	alt_putstr("Interruption Min\n");
	  }

	 * reg_mode = 0x0;


	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_MODE_BASE, 0);
}

void io_button_op(void * context){
	volatile int * edge_ptr;
	edge_ptr = (volatile int *) context;

	*edge_ptr = IORD_ALTERA_AVALON_PIO_EDGE_CAP(BTN_OP_BASE);

	  if(edge_val & 0x01){
	  	alt_putstr("Interruption Hour\n");
	  }
	  else if(edge_val & 0x02){
	  	  	alt_putstr("Interruption Alarm\n");
	  }

	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BTN_OP_BASE, 0);
}
