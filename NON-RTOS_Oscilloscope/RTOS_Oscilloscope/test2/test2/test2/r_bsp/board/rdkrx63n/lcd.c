/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : lcd.c
* Version      : 1.0 
* Device(s)    : RX
* H/W Platform : YRDKRX63N
* Description  : LCD Module utility functions. Written for RDK Okaya graphic LCD.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Processor-specific details */
#include <machine.h>
/* Standard string manipulation & formatting functions */
#include <stdio.h>
#include <string.h>
/* Defines standard variable types used in this function */
#include <stdint.h>
/* Board includes. */
#include "platform.h"
/* Following header file provides function prototypes for LCD controlling functions & macro defines */
#include "lcd.h"
/* Graphics library support */
#include "glyph.h"
/* RSPI package. */
#include "r_rspi_rx600.h"
#include "s12adc.h"
#include "wdt.h"

void delayMS(int delay_time);
void lcd_display1(uint8_t position, const uint8_t * string);
/***********************************************************************************************************************
Private global variables and functions
***********************************************************************************************************************/

T_glyphHandle lcd_handle;


/***********************************************************************************************************************
* Function name : lcd_initialize
* Description   : Initializes the LCD display. 
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void lcd_initialize(void)
{
    /* Initialize RSPI channel */
    R_RSPI_Init(RSPI_CHANNEL_0);    
    
    if (GlyphOpen(&lcd_handle, 0) == GLYPH_ERROR_NONE)
    {
        /* use Glyph full access direct functions */
        GlyphNormalScreen(lcd_handle);
        GlyphSetFont(lcd_handle, GLYPH_FONT_8_BY_8);
        GlyphClearScreen(lcd_handle);
    }
	
}

/***********************************************************************************************************************
* Function name : lcd_clear
* Description   : Clears the LCD
* Arguments     : none
* Return Value  : none
***********************************************************************************************************************/
void lcd_clear(void)
{
    GlyphClearScreen(lcd_handle);
}

/***********************************************************************************************************************
* Function name : lcd_display
* Description   : This function controls the LCD writes.
*                 The display supports 8 lines with up to 12 characters per line. Use the defines LCD_LINE1 to 
*                 LCD_LINE8 to specfify the starting position.
*                 For example, to start at the 4th position on line 1:
*                     lcd_display(LCD_LINE1 + 4, "Hello")
* Arguments     : position - 
*                     Line number of display
*                 string - 
*                     Pointer to null terminated string
* Return Value  : none
***********************************************************************************************************************/
void lcd_display1(uint8_t position, const uint8_t * string)
{
    uint8_t y = position - (position % 8);
    uint8_t xOffset = (position % 8)<<3;

    /* Draw text lines, 8 pixels high, 96 pixels wide */
    /* Clear the rectangle of this line */
    GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
    GlyphSetXY(lcd_handle, xOffset, y);
    GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));

}

void lcd_display(uint8_t position, const uint8_t * string)
{
   // uint8_t y = position - (position % 8);
    //uint8_t xOffset = (position % 8)<<3;

    /* Draw text lines, 8 pixels high, 96 pixels wide */
    /* Clear the rectangle of this line */
    //GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
    //GlyphSetXY(lcd_handle, xOffset, y);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	//GlyphChar(lcd_handle, (uint8_t *)string[0]);
	
		uint8_t char_61[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

		uint8_t char_60[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
		uint8_t char_62[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
};
for(int i=0; i<9;i++)
{
	if(i<7)
	{
		char_61[i+2]=0x80;
		char_60[i+2]=0x01;
		char_62[i+2]=0x00;
	}
	else
	{
		char_61[i+2]=0xFF;
		char_60[i+2]=0xFF;
		char_62[i+2]=0xFF;
	}
}

		change(char_61);
//		change2(char_60);
	
	// position=0;//lin1
	
	uint8_t y = position - (position % 8);
    uint8_t xOffset = (position % 8)<<3;
	
  GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
/*    for(int i=0;i<9;i++)
	{
		if(i==0 ||i==3||i==7)
		{
			change(60);
		}
		if(i==1 ||i==4||i==8)
		{
			change(62);
		}
		if(i==3 ||i==5||i==9)
		{
			change(61);
		}
		*/
//	GlyphSetXY(lcd_handle, xOffset, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
//	GlyphChar(lcd_handle, '~');
//	change(char_62);
	
//	GlyphSetXY(lcd_handle, xOffset, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
//	GlyphChar(lcd_handle, '~');
	/*
	change(char_60);
	GlyphSetXY(lcd_handle, 9, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_62);
	GlyphSetXY(lcd_handle, 9, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');

	change(char_62);
	GlyphSetXY(lcd_handle, 18, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_61);
	GlyphSetXY(lcd_handle, 18, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_60);
	GlyphSetXY(lcd_handle, 27, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_62);
	GlyphSetXY(lcd_handle, 27, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');

	change(char_62);
	GlyphSetXY(lcd_handle, 36, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_61);
	GlyphSetXY(lcd_handle, 36, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	*/
float minVolt=0;
float maxVolt=0;
int mincnt=17000;
int freq=0;
int freqc=0;
float amplitude;
uint8_t buf[13]; 
	wdt_init();
	for(int i=0; i<12; i++)
	{
	int z=0;
	float adc_count;
	S12ADC_init();
		uint8_t char_1[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

		uint8_t char_2[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
		uint8_t char_3[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
		uint8_t char_4[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

//variables


	while(z<8)
	{
		delayMS(1);
	
		S12ADC_start();
		while(false==S12ADC_conversion_complete())
		{}
		
		adc_count= S12ADC_read();
		
		
		adc_count=adc_count/4095;
		float adccount=adc_count*3.3;
		
		if(adccount>maxVolt)
		{
			maxVolt=adccount;
		}
	//	if((adccount>=(((maxVolt-minVolt)/2)+minVolt-.1))&&(adccount<=(((maxVolt-minVolt)/2)+minVolt+.1))) freqc++;
	
		if((adccount>=(maxVolt-.01))&&(adccount<=maxVolt)) freqc=0;  else freqc=1;
		if(freqc==0) wdt_feed_watchdog(); 
		if(freqc==1){
		 //watchdog start
		 if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
		 
		}
		if(freqc==2){
		freqc=0;	
			if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
			wdt_feed_watchdog(); 
			
		}
	/*	if(freqc==0) wdt_feed_watchdog();
		if(freqc==1) ;
		if(freqc==2)
		if(WDT.WDTSR.BIT.CNTVAL<mincnt){  mincnt=WDT.WDTSR.BIT.CNTVAL; freqc=0;}*/
		freq=(1000/(((16383-mincnt)*2796*2)/16383));
		
		sprintf((char *)buf, "FREQ=%5d",freq);
		
		
		
		if(adccount<minVolt)
		{
			minVolt=adccount;
		}
		
		
		
		if(adccount<=.825)
		{
			
				if(adccount<=.103125)
				{
					char_1[z+2]=0x80;
				}
				if((adccount>.103125)&&(adccount<=.20625))
				{
					char_1[z+2]=0x40;
				}
				if((adccount>.20625)&&(adccount<=.309375))
				{
					char_1[z+2]=0x20;
				}
				if((adccount>.309375)&&(adccount<=.4125))
				{
					char_1[z+2]=0x10;
				}
				if((adccount>.4125)&&(adccount<=.515625))
				{
					char_1[z+2]=0x08;
				}
				if((adccount>.515625)&&(adccount<=.61875))
				{
					char_1[z+2]=0x04;
				}
				if((adccount>.61875)&&(adccount<=.721875))
				{
					char_1[z+2]=0x02;
				}
				if((adccount>.721875)&&(adccount<=.825))
				{
					char_1[z+2]=0x01;
				}
		}
		if((adccount>.825)&&(adccount<=1.65))
		{
			
				if((adccount>.825)&&(adccount<=.928125))
				{
					char_2[z+2]=0x80;
				}
				if((adccount>.928125)&&(adccount<=1.03125))
				{
					char_2[z+2]=0x40;
				}
				if((adccount>1.03125)&&(adccount<=1.134375))
				{
					char_2[z+2]=0x20;
				}
				if((adccount>1.134375)&&(adccount<=1.2375))
				{
					char_2[z+2]=0x10;
				}
				if((adccount>1.2375)&&(adccount<=1.340625))
				{
					char_2[z+2]=0x08;
				}
				if((adccount>1.340625)&&(adccount<=1.44375))
				{
					char_2[z+2]=0x04;
				}
				if((adccount>1.44375)&&(adccount<=1.546875))
				{
					char_2[z+2]=0x02;
				}
				if((adccount>1.546875)&&(adccount<=1.65))
				{
					char_2[z+2]=0x01;
				}
		}
		if((adccount>1.65)&&(adccount<=2.475))
		{
			
				if((adccount>1.65)&&(adccount<=1.753125))
				{
					char_3[z+2]=0x80;
				}
				if((adccount>1.753125)&&(adccount<=1.85625))
				{
					char_3[z+2]=0x40;
				}
				if((adccount>1.85625)&&(adccount<=1.959375))
				{
					char_3[z+2]=0x20;
				}
				if((adccount>1.959375)&&(adccount<=2.0625))
				{
					char_3[z+2]=0x10;
				}
				if((adccount>2.0625)&&(adccount<=2.165625))
				{
					char_3[z+2]=0x08;
				}
				if((adccount>2.165625)&&(adccount<=2.26875))
				{
					char_3[z+2]=0x04;
				}
				if((adccount>2.26875)&&(adccount<=2.371875))
				{
					char_3[z+2]=0x02;
				}
				if((adccount>2.371875)&&(adccount<=2.475))
				{
					char_3[z+2]=0x01;
				}
		}
		if((adccount>2.475)&&(adccount<=3.3))
		{
			
				if((adccount>2.475)&&(adccount<=2.578125))
				{
					char_4[z+2]=0x80;
				}
				if((adccount>2.578125)&&(adccount<=2.68125))
				{
					char_4[z+2]=0x40;
				}
				if((adccount>2.68125)&&(adccount<=2.784375))
				{
					char_4[z+2]=0x20;
				}
				if((adccount>2.784375)&&(adccount<=2.8875))
				{
					char_4[z+2]=0x10;
				}
				if((adccount>2.8875)&&(adccount<=2.990625))
				{
					char_4[z+2]=0x08;
				}
				if((adccount>2.990625)&&(adccount<=3.09375))
				{
					char_4[z+2]=0x04;
				}
				if((adccount>3.09375)&&(adccount<=3.196875))
				{
					char_4[z+2]=0x02;
				}
				if((adccount>3.196875)&&(adccount<=3.3))
				{
					char_4[z+2]=0x01;
				}
		}
		z++;
		
	}
	int x1,y1;
	change(char_1);
	GlyphSetXY(lcd_handle, i*8, 32);
	GlyphChar(lcd_handle, '~');
	
		change(char_2);
	GlyphSetXY(lcd_handle, i*8, 24);
	GlyphChar(lcd_handle, '~');
	
		change(char_3);
	GlyphSetXY(lcd_handle, i*8, 18);
	GlyphChar(lcd_handle, '~');
	
		change(char_4);
	GlyphSetXY(lcd_handle, i*8, 9);
	GlyphChar(lcd_handle, '~');
	

	
    //lcd_display1(LCD_LINE7,Freq);

	
	
	}
	char amp[20];
	char Freq[20];
	amplitude=(maxVolt-minVolt)/2;
	sprintf(amp,"Amp: %4f",amplitude);
	
	
	lcd_display1(LCD_LINE1,"Mitch & Paul");
	lcd_display1(LCD_LINE6,amp);
	//lcd_display1(LCD_LINE7,buf);

	

}
 
 void delayMS(int delay_time)
{
	/* Used to pace delay for the LED */
	uint32_t led_counter;
	int i;
	for(i=0;i<delay_time; i++)
	{
		for (led_counter = 0; led_counter < 14000; led_counter++){}//6000
	}
}





int lcd_display2(uint8_t position, const uint8_t * string)
{
   // uint8_t y = position - (position % 8);
    //uint8_t xOffset = (position % 8)<<3;

    /* Draw text lines, 8 pixels high, 96 pixels wide */
    /* Clear the rectangle of this line */
    //GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
    //GlyphSetXY(lcd_handle, xOffset, y);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	//GlyphChar(lcd_handle, (uint8_t *)string[0]);
	
		uint8_t char_61[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

		uint8_t char_60[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
		uint8_t char_62[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
};
for(int i=0; i<9;i++)
{
	if(i<7)
	{
		char_61[i+2]=0x80;
		char_60[i+2]=0x01;
		char_62[i+2]=0x00;
	}
	else
	{
		char_61[i+2]=0xFF;
		char_60[i+2]=0xFF;
		char_62[i+2]=0xFF;
	}
}

		change(char_61);
//		change2(char_60);
	
	// position=0;//lin1
	
	uint8_t y = position - (position % 8);
    uint8_t xOffset = (position % 8)<<3;
	
  GlyphEraseBlock(lcd_handle, xOffset, y, (95 - xOffset), y+7);
/*    for(int i=0;i<9;i++)
	{
		if(i==0 ||i==3||i==7)
		{
			change(60);
		}
		if(i==1 ||i==4||i==8)
		{
			change(62);
		}
		if(i==3 ||i==5||i==9)
		{
			change(61);
		}
		*/
//	GlyphSetXY(lcd_handle, xOffset, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
//	GlyphChar(lcd_handle, '~');
//	change(char_62);
	
//	GlyphSetXY(lcd_handle, xOffset, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
//	GlyphChar(lcd_handle, '~');
	/*
	change(char_60);
	GlyphSetXY(lcd_handle, 9, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_62);
	GlyphSetXY(lcd_handle, 9, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');

	change(char_62);
	GlyphSetXY(lcd_handle, 18, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_61);
	GlyphSetXY(lcd_handle, 18, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_60);
	GlyphSetXY(lcd_handle, 27, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_62);
	GlyphSetXY(lcd_handle, 27, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');

	change(char_62);
	GlyphSetXY(lcd_handle, 36, 9);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	change(char_61);
	GlyphSetXY(lcd_handle, 36, 18);
    //GlyphString(lcd_handle, (uint8_t *)string, strlen((void *)string));
	GlyphChar(lcd_handle, '~');
	*/
float minVolt=0;
float maxVolt=0;
int mincnt=17000;
int freq=0;
int freqc=0;
float amplitude;
uint8_t buf[13]; 
	wdt_init();
	for(int i=0; i<12; i++)
	{
	int z=0;
	float adc_count;
	S12ADC_init();
		uint8_t char_1[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

		uint8_t char_2[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};
		uint8_t char_3[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
		uint8_t char_4[] = {
    0x08, 0x08, // width=8, height=8
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

//variables


	while(z<8)
	{
		delayMS(1);
	
		S12ADC_start();
		while(false==S12ADC_conversion_complete())
		{}
		
		adc_count= S12ADC_read();
		
		
		adc_count=adc_count/4095;
		float adccount=adc_count*3.3;
		
		if(adccount>maxVolt)
		{
			maxVolt=adccount;
		}
	//	if((adccount>=(((maxVolt-minVolt)/2)+minVolt-.1))&&(adccount<=(((maxVolt-minVolt)/2)+minVolt+.1))) freqc++;
	
		if((adccount>=(maxVolt-.03))&&(adccount<=maxVolt)) freqc=0;  else freqc=1;
		if(freqc==0) wdt_feed_watchdog(); 
		if(freqc==1){
		 //watchdog start
		 if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
		 
		}
		if(freqc==2){
		freqc=0;	
			if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
			wdt_feed_watchdog(); 
			
		}
	/*	if(freqc==0) wdt_feed_watchdog();
		if(freqc==1) ;
		if(freqc==2)
		if(WDT.WDTSR.BIT.CNTVAL<mincnt){  mincnt=WDT.WDTSR.BIT.CNTVAL; freqc=0;}*/
		freq=(1000/(((16383-mincnt)*2796)/16383));
		
		sprintf((char *)buf, "FREQ=%5d",freq);
		
		
		
		if(adccount<minVolt)
		{
			minVolt=adccount;
		}
		
		
		
		if(adccount<=.825)
		{
			
				if(adccount<=.103125)
				{
					char_1[z+2]=0x80;
				}
				if((adccount>.103125)&&(adccount<=.20625))
				{
					char_1[z+2]=0x40;
				}
				if((adccount>.20625)&&(adccount<=.309375))
				{
					char_1[z+2]=0x20;
				}
				if((adccount>.309375)&&(adccount<=.4125))
				{
					char_1[z+2]=0x10;
				}
				if((adccount>.4125)&&(adccount<=.515625))
				{
					char_1[z+2]=0x08;
				}
				if((adccount>.515625)&&(adccount<=.61875))
				{
					char_1[z+2]=0x04;
				}
				if((adccount>.61875)&&(adccount<=.721875))
				{
					char_1[z+2]=0x02;
				}
				if((adccount>.721875)&&(adccount<=.825))
				{
					char_1[z+2]=0x01;
				}
		}
		if((adccount>.825)&&(adccount<=1.65))
		{
			
				if((adccount>.825)&&(adccount<=.928125))
				{
					char_2[z+2]=0x80;
				}
				if((adccount>.928125)&&(adccount<=1.03125))
				{
					char_2[z+2]=0x40;
				}
				if((adccount>1.03125)&&(adccount<=1.134375))
				{
					char_2[z+2]=0x20;
				}
				if((adccount>1.134375)&&(adccount<=1.2375))
				{
					char_2[z+2]=0x10;
				}
				if((adccount>1.2375)&&(adccount<=1.340625))
				{
					char_2[z+2]=0x08;
				}
				if((adccount>1.340625)&&(adccount<=1.44375))
				{
					char_2[z+2]=0x04;
				}
				if((adccount>1.44375)&&(adccount<=1.546875))
				{
					char_2[z+2]=0x02;
				}
				if((adccount>1.546875)&&(adccount<=1.65))
				{
					char_2[z+2]=0x01;
				}
		}
		if((adccount>1.65)&&(adccount<=2.475))
		{
			
				if((adccount>1.65)&&(adccount<=1.753125))
				{
					char_3[z+2]=0x80;
				}
				if((adccount>1.753125)&&(adccount<=1.85625))
				{
					char_3[z+2]=0x40;
				}
				if((adccount>1.85625)&&(adccount<=1.959375))
				{
					char_3[z+2]=0x20;
				}
				if((adccount>1.959375)&&(adccount<=2.0625))
				{
					char_3[z+2]=0x10;
				}
				if((adccount>2.0625)&&(adccount<=2.165625))
				{
					char_3[z+2]=0x08;
				}
				if((adccount>2.165625)&&(adccount<=2.26875))
				{
					char_3[z+2]=0x04;
				}
				if((adccount>2.26875)&&(adccount<=2.371875))
				{
					char_3[z+2]=0x02;
				}
				if((adccount>2.371875)&&(adccount<=2.475))
				{
					char_3[z+2]=0x01;
				}
		}
		if((adccount>2.475)&&(adccount<=3.3))
		{
			
				if((adccount>2.475)&&(adccount<=2.578125))
				{
					char_4[z+2]=0x80;
				}
				if((adccount>2.578125)&&(adccount<=2.68125))
				{
					char_4[z+2]=0x40;
				}
				if((adccount>2.68125)&&(adccount<=2.784375))
				{
					char_4[z+2]=0x20;
				}
				if((adccount>2.784375)&&(adccount<=2.8875))
				{
					char_4[z+2]=0x10;
				}
				if((adccount>2.8875)&&(adccount<=2.990625))
				{
					char_4[z+2]=0x08;
				}
				if((adccount>2.990625)&&(adccount<=3.09375))
				{
					char_4[z+2]=0x04;
				}
				if((adccount>3.09375)&&(adccount<=3.196875))
				{
					char_4[z+2]=0x02;
				}
				if((adccount>3.196875)&&(adccount<=3.3))
				{
					char_4[z+2]=0x01;
				}
		}
		z++;
		
	}
	int x1,y1;
	change(char_1);
	GlyphSetXY(lcd_handle, i*8, 32);
	GlyphChar(lcd_handle, '~');
	
		change(char_2);
	GlyphSetXY(lcd_handle, i*8, 24);
	GlyphChar(lcd_handle, '~');
	
		change(char_3);
	GlyphSetXY(lcd_handle, i*8, 18);
	GlyphChar(lcd_handle, '~');
	
		change(char_4);
	GlyphSetXY(lcd_handle, i*8, 9);
	GlyphChar(lcd_handle, '~');
	

	
    //lcd_display1(LCD_LINE7,Freq);

	
	
	}
	char amp[20];
	char Freq[20];
	amplitude=(maxVolt-minVolt)/2;
	sprintf(amp,"Amp: %4f",amplitude);
	
	
	lcd_display1(LCD_LINE1,"Mitch & Paul");
	lcd_display1(LCD_LINE6,amp);
	//lcd_display1(LCD_LINE7,buf);
	return freq;
	

}