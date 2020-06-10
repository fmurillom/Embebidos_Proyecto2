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
#include "sys/alt_stdio.h"
#include <stdio.h>


void io_button(void * context);
void io_button_setup();

void minutos();
void segundos();
void horas();
void counter();
void timer_setup();







volatile int edge_val = 0;

//Declaracion de Punteros para control del IRQ del PIO de botnones

volatile unsigned char * btn_irq = (unsigned char*)0x5008;
volatile unsigned char * btn_edge = (unsigned char*)0x500C;
volatile unsigned char * btn_data = (unsigned char*)0x5000;

//Declaracion de punteros para control del Timer
unsigned char * timer_status = (unsigned char*) 0x6000;
unsigned char * timer_control = (unsigned char*) 0x6004;
unsigned char * out = (unsigned char*) 0x3040;

//Variables globales para manejo de la hora
int segundosD = 0;
int segundosU = 0;
int minutosU = 9;
int minutosD = 5;
int horasU = 3;
int horasD = 2;

//Variables globales para manejo de la alarma
int alarmaMinU = 0;
int alarmaMinD = 0;
int alarmaHorU = 0;
int alarmaHorD = 0;


int mode = 0;
int alarm_stat = 0;

int main(void)
{ 

  *timer_status = 0;

  io_button_setup();

  timer_setup();


  /* Event loop never exits. */
  while (1){
	  if(mode == 1){
		  alt_printf("Set Alarm Mode\n");
		  alt_printf("%x%x : %x%x : %x%x\n",alarmaHorD, alarmaHorU, alarmaMinD, alarmaMinU ,0x0, 0x0);
	  }else if(mode == 2){
		  segundosD = 0;
		  segundosU = 0;
		  alt_printf("Set Time Mode\n");
		  alt_printf("%x%x : %x%x : %x%x\n",horasD, horasU, minutosD, minutosU ,segundosD, segundosU);
	  }
  }
  return 0;
}

//Inicializa el timer y llama a counter
void timer_setup(){
	* timer_control = 0x7;
	alt_ic_irq_enable(0,2);
	alt_ic_isr_register(0,2,counter,0,0x00);
}

//Actualiza el estado del reloj cada 20s
void counter(){
	*timer_status = 0;
	segundosD += 2;
	segundos();
	minutos();
	horas();
	if(mode == 0){
		alt_printf("%x%x : %x%x : %x%x\n",horasD, horasU, minutosD, minutosU ,segundosD, segundosU);
	}
	if(horasD == alarmaHorD && horasU == alarmaHorU && minutosD == alarmaMinD && minutosU == alarmaMinU && alarm_stat == 1){
		alt_printf("Beep Beep Beep Beep Beep\n");
	}
}

//Verifica minutos
void minutos(){
	if(minutosU == 10){
		minutosU = 0;
		minutosD += 1;
	}if(minutosD == 6){
		horasU += 1;
		minutosD = 0;
	}
}

//Verifica minutos
void minutosAl(){
	if(alarmaMinU == 10){
		alarmaMinU = 0;
		alarmaMinD += 1;
	}if(alarmaMinD == 6){
		alarmaHorU += 1;
		alarmaMinD = 0;
	}
}

//Verifica segundos
void segundos(){
	if(segundosD == 6){
		segundosD = 0;
		minutosU += 1;
	}
}



//Verifica horas
void horas(){
	if(horasU == 10){
		horasU = 0;
		horasD += 1;
	}if(horasD == 2 && horasU == 4){
		horasD = 0;
		horasU = 0;
	}
}

void horasAl(){
	if(alarmaHorU == 10){
		alarmaHorU = 0;
		alarmaHorD += 1;
	}if(alarmaHorD == 2 && horasU == 4){
		alarmaHorD = 0;
		alarmaHorU = 0;
	}
}


void io_button_setup(){
	* btn_irq = 15;
	* btn_edge = 0;

	void * edge_val_ptr;

	edge_val_ptr = (void *) edge_val;

	alt_ic_irq_enable(0,1);

	alt_ic_isr_register(0,
			1,
			io_button,
			edge_val_ptr,
			0x00);
}


void io_button(void * context){
	volatile int * edge_ptr;
	edge_ptr = (volatile int *) context;

	 if(*btn_data == 0x01){
	  	mode = mode + 1;
	  	if(mode == 3){
	  		mode = 0;
	  	}
	  }
	  else if(*btn_data == 0x02){ //Boton de Minutos
		  if(mode == 2){
			  minutosU += 1;
			  segundosU = 0;
			  segundosD = 0;
			  segundos();
			  minutos();
			  horas();
		  }
		  if(mode == 1){
			  alarmaMinU += 1;
			  minutosAl();
			  horasAl();
		  }
	  }
	  else if(*btn_data == 0x04){ //Boton de horas
		  if(mode == 2){
			  horasU += 1;
			  segundosU = 0;
			  segundosD = 0;
		  	  segundos();
		  	  minutos();
		  	  horas();
		  }
		  if(mode == 1){
			  alarmaHorU += 1;
			  minutosAl();
			  horasAl();
		  }
	  }
	  else if(*btn_data == 0x08){
		  if(alarm_stat == 0){
			  alt_putstr("Alarm set to On\n");
			  alarm_stat = 1;
		  }else{
			  alt_putstr("Alarm set to Off\n");
			  alarm_stat = 0;
		  }
	  }

	  *btn_edge = 0x0;
}
