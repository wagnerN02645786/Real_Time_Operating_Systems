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
/*******************************************************************************
* File Name     : adc_oneshot_demo_main.c
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
* Description   : This sample demonstrates use of the 12-bit A/D converter (S12ADC).
*                 The S12ADC is set up for one-shot reading of channel AN002.
*                 AN002 is connected to the potentiometer VR1 on the YRDKRX63N board.  
*                 Each time SW1 is pressed the S12ADC is started and the ADC reading
*                 is recorded and displayed on the LCD.
* Operation     : 1. Build and download the sample code to the YRDK.
*
*                 2. Click 'Reset Go' to start the software.
*         
*                 3. The debug LCD will show the name of the sample along with
*                    instructions directing you to adjust pot VR1 and then 
*                    press SW1.
*
*                 4. The current ADC value, in decimal format, and the voltage that
*                    represents are displayed.
*                      
*******************************************************************************/         
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 15.02.2012     1.00        First release
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <machine.h>
#include "platform.h"
#include "s12adc.h"
//#include "..\lcd.h"
//#include "..\r_glyph\src\glyph\fonts\font_8x8.c"

/*******************************************************************************
Prototypes for local functions
*******************************************************************************/
void display_results (uint16_t adc_counts);
void delayMS1(int delay_time);


/*******************************************************************************
* Function name: main
* Description  : Main program function. Initializes the peripherals used in the
*                demo and executes a loop that reads the ADC and updates the 
*                display once for each press of switch 1. 
* Arguments    : none
* Return value : none
*******************************************************************************/
void main(void)
{
    /* ADC reading */
    uint16_t adc_counts;
    
    uint32_t i; /* Common loop count variable. */
       
    /* Initialize LCD */
    lcd_initialize();
   
	
    /* Clear LCD */
    lcd_clear();
	int freq=0;
	int count=0;
	while(1){
 /*
	uint8_t char_60[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x08, 0x1C, 0x36, 0x63, 0x41, 0x00, 0x00, 
};
	uint8_t char_61[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x36, 0x63, 0x41, 0x00, 0x00, 
};*/
	//for(int i=0; i<6;i++)
	{
		uint8_t char_60[] = {
    0x08, 0x08, // width=8, height=8
    0xFF, 0xFF, 0x00, 0xFF, 0xFF, 0x00, 0xFF, 0xFF, 
};
	}
	//change(char_60);
	/*
	uint8_t char_61[10] =GLYPH_FONT_8_BY_8[126];
	
	Font8x8_char_126[0] = char_60[0];
	Font8x8_char_126[1] = char_60[1];
	Font8x8_char_126[2] = char_60[2];
	Font8x8_char_126[3] = char_60[3];
	Font8x8_char_126[4] = char_60[4];
	Font8x8_char_126[5] = char_60[5];
	Font8x8_char_126[6] = char_60[6];
	Font8x8_char_126[7] = char_60[7];
	Font8x8_char_126[8] = char_60[8];
	Font8x8_char_126[9] = char_60[9];*/
	
	
    /* Display message on LCD */
	
   int temp=lcd_display2(LCD_LINE2, "~");
   if((freq/count)-10>=temp  &&(freq/count)+10<=temp);
   else{
   freq=freq+temp;
   count++;
   }
   char buf[20];
   sprintf((char *)buf, "FREQ=%5d",freq/count);
   
   lcd_display1(LCD_LINE7, buf);
   delayMS1(150);
   
   
    //lcd_display(LCD_LINE2, " YRDKRX63N  ");
    //lcd_display(LCD_LINE3, "   S12ADC   ");
    //lcd_display(LCD_LINE4, "  ONE SHOT  ");
    //lcd_display(LCD_LINE5, " ADJUST POT,");
    //lcd_display(LCD_LINE6, " PRESS SW1  ");
	

	



	

	
	}
    /* Setup the 12-bit A/D converter */
    //S12ADC_init();

    /* 
        STDOUT is routed through the virtual console window tunneled through the 
        JTAG debugger. Open the console window in HEW to see the output 
    */
    printf("This is the debug console\r\n");
    
    

} /* End function main() */


/*******************************************************************************
* Function name: display_results
* Description  : Converts ADC counts to volts value and shows both on the LCD
* Arguments    : uint16_t adc_counts: current reading from ADC
* Return value : none
*******************************************************************************/
void display_results (uint16_t adc_counts)
{
    /* For converting the reading to volts */
    float adc_volts;
    /* String to display */
    char result_string[20];
    
    /* Convert to a string */
    sprintf(result_string, "Reading=%4d", adc_counts);

    /* Update the display */
    lcd_display(LCD_LINE7, (const uint8_t *)result_string);

    /* Convert S12ADC counts to volts and display */    
    adc_volts = ((float) adc_counts) / MAX_COUNTS * (VREFH - VREFL);

    /* Convert to a string */
    sprintf(result_string, "Volts  =%4.2f", adc_volts);

    /* Update the display */
    lcd_display(LCD_LINE8, (const uint8_t *)result_string);
} /* End function display_results() */


void delayMS1(int delay_time)
{
	/* Used to pace delay for the LED */
	uint32_t led_counter;
	int i;
	for(i=0;i<delay_time; i++)
	{
		for (led_counter = 0; led_counter < 40000; led_counter++){}
	}
}

