/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/******************************************************************************
* File Name     : main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : Empty application project
*                 This application provides a handy framework to serve as a
*                 generic basis for any application. The MCU startup procedures
*                 are provided, as well as some basic means for I/O. The LCD is 
*                 initialized, board switches and their interrupt ISR stubs are
*                 defined, and a simple loop blinks an LED to indicate that the
*                 board is operating.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 22.09.2011     1.00        First release              
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "r_switches.h"
#include "s12adc.h"
#include "uart.h"
/*******************************************************************************
* Prototypes for local functions
*******************************************************************************/

/******************************************************************************
* Function name: main
* Description  : Main program function
* Arguments    : none
* Return value : none
******************************************************************************/
//Global variables
int count;
int Forward;
int Backword;
int Enable;
int LastDir;
int inloop=0;

void trainMove(void);
void delayMS(int delay_time);
void main(void)
{    
	//uint16_t adc_count;
	float adc_count;
    /* Initialize LCD */
    lcd_initialize();
	S12ADC_init();
    //UART initialization
	sci_uart_init();
	sci_tx_int_enable();
	sci_rx_int_enable();
	
	
    /* Clear LCD */
    lcd_clear();
    
    /* Display message on LCD */
    //Message will be on Position 1 and it will be stopped
    lcd_display(LCD_LINE1, " Mitch and  ");
	lcd_display(LCD_LINE2, "    Paul    ");
	lcd_display(LCD_LINE3, "Train Track ");
	lcd_display(LCD_LINE4, "  STOPPED   ");
	lcd_display(LCD_LINE5, "       _oo_");
	lcd_display(LCD_LINE6, " Pos1 |[][]|");
	lcd_display(LCD_LINE7, "      |O  O|");
	lcd_display(LCD_LINE8, "      |____|");

    /* 
        STDOUT is routed through the virtual console window tunneled through the JTAG debugger.
        Open the console window in HEW to see the output 
    */
    printf("This is the debug console\r\n");
    
    /* The three pushbuttons on the YRDK board are tied to interrupt lines, set them up here */
    R_SWITCHES_Init();
    
    //Intitalize all global variables
	count=0;
    Forward=0;
	Backword=0;
	Enable=0;
	
	//Character to input from putty
	char new_char;
	
	//Initialize Local variables
	int toggle=0;
	
	//Initialize all LED's starting at position 1
	LED4=LED_ON;
	LED5=LED_ON;
	LED6=LED_ON;
	LED7=LED_ON;
	LED8=LED_OFF;
	LED9=LED_OFF;
	LED10=LED_OFF;
	LED11=LED_OFF;
	LED12=LED_OFF;
	LED13=LED_OFF;
	LED14=LED_OFF;
	LED15=LED_OFF;
	
	while (1)
    {
		S12ADC_start();
		while(false==S12ADC_conversion_complete())
		{}
		new_char= sci_get_char();
		
		//Through the serial communication
		if(new_char =='F' ||new_char=='f')	//If inputted 'F' or 'f' move Forward
		{
			lcd_display(LCD_LINE4, "  FORWARD ");
			Forward=1;
			Enable=1;
			Backword=0;
			sci_put_string("Remote: Forward\n\r");
		}
		if(new_char =='S' ||new_char=='s')	//If inputted 'S' or 's' stop movement
		{
			lcd_display(LCD_LINE4, "  STOPPED   ");
			//Forward=0;
			Enable=0;
			//Backward=0;
			sci_put_string("Remote: Stop\n\r");

		}
		if(new_char =='R' ||new_char=='r')	//If inputted 'R' or 'r' move in reverse
		{
			lcd_display(LCD_LINE4, "  REVERSE  ");
			Forward=0;
			Enable=1;
			Backword=1;
			sci_put_string("Remote: Reverse\n\r");
		}
		
		
		
		adc_count= S12ADC_read();
		
		adc_count=adc_count/4095;
		int adccount=adc_count*100;
		char result[20];
		sprintf(result, "sp%3d%|____|", adccount);
		lcd_display(LCD_LINE8, (const uint8_t *) result);
	//	lcd_display(LCD_LINE8, adc_count);
		if(new_char=='D'||new_char=='d')	//If inputted 'D' or 'd' diplay the current speed
		{
			char speed[20];
			sprintf(speed,"Current Speed: %3d \n\r",adccount);
			sci_put_string(speed);
		}
		
		if(adc_count<=.01 & Enable==1)
		{
			lcd_display(LCD_LINE4, "  STOPPED   ");
			//Forward=0;
			Enable=0;
			//Backward=0;
			sci_put_string("Remote: Stop\n\r");
			while(adc_count<=0.05)
			{
				inloop=1;
				S12ADC_start();
				while(false==S12ADC_conversion_complete())
				{}
				adc_count= S12ADC_read();
				adc_count=adc_count/4095;
				
			}
			if(LastDir==1)
			{
				lcd_display(LCD_LINE4, "  FORWARD ");
				Forward=1;
				Enable=1;
				Backword=0;
				sci_put_string("Forward\n\r");
			}
			else if(LastDir==-1)
			{
				lcd_display(LCD_LINE4, "  REVERSE  ");
				Forward=0;
				Enable=1;
				Backword=1;
				sci_put_string("Reverse\n\r");
			}
			
			inloop=0;
		}
		else if(adc_count<=.25)
		{
			delayMS(1000);
		}
		else if(adc_count<=.5)
		{
			delayMS(500);
		}
		else if(adc_count<=.75)
		{
			delayMS(100);
		}		
		else if(adc_count<=1.0)
		{
			delayMS(10);
		}
		
		
		if(Enable==1)
		{
			trainMove();
			if(toggle==1 && Backword==1)
			{
				lcd_display(LCD_LINE5, "       _**_");
				lcd_display(LCD_LINE7, "      |*  *|");
				toggle=0;
			}
			else if(toggle==0 && Backword==1)
			{
				lcd_display(LCD_LINE5, "       _oo_");
				lcd_display(LCD_LINE7, "      |O  O|");
				toggle=1;
			}
			if(Forward==1)
			{
				lcd_display(LCD_LINE5, "       _oo_");
				lcd_display(LCD_LINE7, "      |*  *|");
			}
		}
		else
		{
			lcd_display(LCD_LINE5, "       _oo_");
			lcd_display(LCD_LINE7, "      |O  O|");
		}
		
		if(Forward==1 && Enable==1)	{count++; 	if(count==12){count=0;}}
		if(Backword==1 && Enable==1){count--;	if(count==-1){count=11;}}
    }
}
//

//Train Move funtion controls the LED on off motion
void trainMove(void)
{
	switch(count)
	{
	case 0:
		LED15=LED_OFF;
		LED4=LED_ON;
		LED5=LED_ON;
		LED6=LED_ON;
		LED7=LED_ON;
		LED8=LED_OFF;
		lcd_display(LCD_LINE6, " Pos1 |[][]|");
		break;
	case 1:
		LED4=LED_OFF;
		LED5=LED_ON;
		LED6=LED_ON;
		LED7=LED_ON;
		LED8=LED_ON;
		LED9=LED_OFF;
		lcd_display(LCD_LINE6, " Pos2 |[][]|");
		break;
	case 2:
		LED5=LED_OFF;
		LED6=LED_ON;
		LED7=LED_ON;
		LED8=LED_ON;
		LED9=LED_ON;
		LED10=LED_OFF;
		lcd_display(LCD_LINE6, " Pos3 |[][]|");
		break;
	case 3:
		LED6=LED_OFF;
		LED7=LED_ON;
		LED8=LED_ON;
		LED9=LED_ON;
		LED10=LED_ON;
		LED11=LED_OFF;
		lcd_display(LCD_LINE6, " Pos4 |[][]|");
		break;
	case 4:
		LED7=LED_OFF;
		LED8=LED_ON;
		LED9=LED_ON;
		LED10=LED_ON;
		LED11=LED_ON;
		LED12=LED_OFF;
		lcd_display(LCD_LINE6, " Pos5 |[][]|");
		break;
	case 5:
		LED8=LED_OFF;
		LED9=LED_ON;
		LED10=LED_ON;
		LED11=LED_ON;
		LED12=LED_ON;
		LED13=LED_OFF;
		lcd_display(LCD_LINE6, " Pos6 |[][]|");
		break;
	case 6:
		LED9=LED_OFF;
		LED10=LED_ON;
		LED11=LED_ON;
		LED12=LED_ON;
		LED13=LED_ON;
		LED14=LED_OFF;
		lcd_display(LCD_LINE6, " Pos7 |[][]|");
		break;
	case 7:
		LED10=LED_OFF;
		LED11=LED_ON;
		LED12=LED_ON;
		LED13=LED_ON;
		LED14=LED_ON;
		LED15=LED_OFF;
		lcd_display(LCD_LINE6, " Pos8 |[][]|");
		break;
	case 8:
		LED11=LED_OFF;
		LED12=LED_ON;
		LED13=LED_ON;
		LED14=LED_ON;
		LED15=LED_ON;
		LED4=LED_OFF;
		lcd_display(LCD_LINE6, " Pos9 |[][]|");
		break;
	case 9:
		LED12=LED_OFF;
		LED13=LED_ON;
		LED14=LED_ON;
		LED15=LED_ON;
		LED4=LED_ON;
		LED5=LED_OFF;
		lcd_display(LCD_LINE6, " Pos10|[][]|");
		break;
	case 10:
		LED13=LED_OFF;
		LED14=LED_ON;
		LED15=LED_ON;
		LED4=LED_ON;
		LED5=LED_ON;
		LED6=LED_OFF;
		lcd_display(LCD_LINE6, " Pos11|[][]|");
		break;
	case 11:
		LED14=LED_OFF;
		LED15=LED_ON;
		LED4=LED_ON;
		LED5=LED_ON;
		LED6=LED_ON;
		LED7=LED_OFF;
		lcd_display(LCD_LINE6, " Pos12|[][]|");
		break;
	default:
		break;
	}
}

//Delay in milli seconds about contant 0.5seconds
void delayMS(int delay_time)
{
	/* Used to pace delay for the LED */
	uint32_t led_counter;
	int i;
	for(i=0;i<delay_time; i++)
	{
		for (led_counter = 0; led_counter < 40000; led_counter++){}
	}
}

//Switch 1 is an interrupt that will control forward motion
void sw1_callback(void)
{
	
	Forward=1;
	Enable=1;
	Backword=0;
	if(inloop==0){
	sci_put_string("Forward\n\r");
	lcd_display(LCD_LINE4, "  FORWARD ");}
	LastDir=1;
}

//Switch 2 is an interrupt that controls stop motion
void sw2_callback(void)
{
	lcd_display(LCD_LINE4, "  STOPPED   ");
	//Forward=0;
	Enable=0;
	//Backward=0;
	sci_put_string("Stop\n\r");
	LastDir=0;

} 

//Switch 3 is an interrupt that controls the reverse motion
void sw3_callback(void)
{
	
	Forward=0;
	Enable=1;
	Backword=1;
	if(inloop==0){
	sci_put_string("Reverse\n\r");
	lcd_display(LCD_LINE4, "  REVERSE  ");}
	LastDir=-1;
}

