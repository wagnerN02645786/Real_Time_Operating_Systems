/*
*********************************************************************************************************
*                                            EXAMPLE CODE
*
*                          (c) Copyright 2009-2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               Please feel free to use any application code labeled as 'EXAMPLE CODE' in
*               your application products.  Example code may be used as is, in whole or in
*               part, or may be used as a reference only.
*
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*
*               You can contact us at www.micrium.com.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                       uC/OS-III Training Lab 3
*
*                                            Renesas RX63N
*                                               on the
*                                              YRDKRX63N
*                                          Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : AA
*                 JM
*********************************************************************************************************
* Note(s)       : (1) Assumes the following versions (or more recent) of software modules are included in
*                     the project build :
*
*                     (a) uC/OS-III     V3.04.02
*                     (b) uC/LIB        V1.37.02
*                     (c) uC/CPU        V1.30.00
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  "app_cfg.h"
#include  "cpu_cfg.h"
#include  "bsp_cfg.h"




#include  <cpu_core.h>
#include  <os.h>
#include  <iodefine.h>

#include  "..\bsp\bsp.h"
#include  "..\bsp\bsp_misc.h"
#include  "..\bsp\bsp_int_vect_tbl.h"

#if       BSP_CFG_SER_EN       > 0u
#include  "..\bsp\bsp_ser.h"
#endif

#if       BSP_CFG_LED_EN       > 0u
#include  "..\bsp\bsp_led.h"
#endif

#if       BSP_CFG_GRAPH_LCD_EN > 0u
#include  "..\bsp\bsp_glcd.h"
#endif

#if       SPIN_LEDS_APP_EN     > 0u
#include  "spin_led.h"
#endif


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/
static  OS_SEM App_SemADC; 
static  OS_TCB AppTaskLCDTCB; 
static  CPU_STK AppTaskLCDStk[APP_CFG_TASK_LCD_STK_SIZE];


/*$PAGE*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  CPU_STK     AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];

static  OS_TCB      AppTaskStartTCB;


/*
*********************************************************************************************************
*                                            LOCAL MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void     AppTaskStart (void  *p_arg);
static  void     AppTaskLCD    (void *p_arg); 
CPU_ISR  App_ISRADC    (void);
void delayMS(int delay_time);


int data[96];
int pos=0;
int current=0;

/*$PAGE*/
/*
*********************************************************************************************************
*                                               main()
*
* Description : Entry point for C code.
*
* Arguments   : none.
*
* Returns     : none.
*
* Note(s)     : (1) It is assumed that your code will call main() once you have performed all necessary
*                   initialization.
*********************************************************************************************************
*/



void  main (void)
{
    OS_ERR  err;


    CPU_IntDis();                                               /* Disable all interrupts.                              */

    BSP_IntVectSet(27, (CPU_FNCT_VOID)OSCtxSwISR);              /* Setup kernel context switch                          */

    OSInit(&err);                                               /* Init uC/OS-III.                                      */

    
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"Startup Task",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_CFG_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE / 10u,
                 (CPU_STK_SIZE) APP_CFG_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (1) {
        ;
    }
}

static  void  AppTaskLCD (void *p_arg) 
{ 
  CPU_INT16U  adc_val; 
  CPU_CHAR    adc_str[7]; 
  OS_ERR      err;
  BSP_Ser_Init(115200);
  int freqF;
  int count;
//BSP_GraphLCD_String(3, "ADC Value:");
BSP_IntVectSet(VECT_S12AD0_S12ADI0, (CPU_FNCT_VOID)App_ISRADC); 
BSP_ADC_Init();

while (DEF_ON) { 
  
  
  int8_t char_61[] = {
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
//change(char_61);
BSP_GraphLCD_Clear ();
float minVolt=0;
float maxVolt=0;
int mincnt=17000;
int freq=0;
int freqc=0;
float amplitude;
uint8_t buf[13]; 
for(int i=0; i<12; i++)
	{
          int z=0;
        float adc_count;
       
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

while(z<8)
	{
          
         //OSSemPend(&App_SemADC, 0, OS_OPT_PEND_BLOCKING, 0, &err); 
//adc_val = S12AD.ADDR1; 
          
          
//Str_FmtNbr_Int32U(adc_val, 6, 10, ' ', DEF_NO, DEF_YES, adc_str); 
//BSP_GraphLCD_String(4, adc_str); 
          
          
          
          
       
          
          
           CPU_CHAR    adc_str[7];
      
   //Str_FmtNbr_Int32U(adc_val, 6, 10, ' ', DEF_NO, DEF_YES, adc_str); 
//BSP_GraphLCD_String(4, adc_str);
          
          
          int adcNEW=data[current];
		
                adc_count=data[current];
                current++;
                if(current==96) {current=0;}
                adc_count=adc_count/4095;
		float adccount=adc_count*3.3;
        
		if(adccount>maxVolt)
		{
			maxVolt=adccount;
		}
	//	if((adccount>=(((maxVolt-minVolt)/2)+minVolt-.1))&&(adccount<=(((maxVolt-minVolt)/2)+minVolt+.1))) freqc++;
	
		if((adccount>=(maxVolt-.03))&&(adccount<=maxVolt)) freqc=0;  else freqc=1;
		if(freqc==0) ;//wdt_feed_watchdog(); 
		if(freqc==1){
		 //watchdog start
		 //if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
		 
		}
		if(freqc==2){
		freqc=0;	
			//if(WDT.WDTSR.BIT.CNTVAL<mincnt)  mincnt=WDT.WDTSR.BIT.CNTVAL;
			 //wdt_feed_watchdog();
			
		}
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
                char adcnum[12];
               
                //BSP_Ser_WrStr("HI");
               int ADCNEW1=adcNEW & 0xFF;
               int ADCNEW2=adcNEW & 0xFF00;
               ADCNEW2=ADCNEW2/256;
                
                
                BSP_Ser_WrByte(ADCNEW2);
                //BSP_Ser_WrByte(ADCNEW1);
		z++;
                delayMS(15);
                BSP_Ser_WrByte(ADCNEW1);
               
        }
	int x1,y1;
BSP_GraphLCD_String1(char_1, i*8, 32);
BSP_GraphLCD_String1(char_2, i*8, 24);
BSP_GraphLCD_String1(char_3, i*8, 18);
BSP_GraphLCD_String1(char_4, i*8, 9);
               /* change(char_1);
        GlyphSetXY(BSP_GraphLCD, i*8, 32);
	GlyphChar(BSP_GraphLCD, '~');
	
		change(char_2);
	GlyphSetXY(BSP_GraphLCD, i*8, 24);
	GlyphChar(BSP_GraphLCD, '~');
	
		change(char_3);
	GlyphSetXY(BSP_GraphLCD, i*8, 18);
	GlyphChar(BSP_GraphLCD, '~');
	
		change(char_4);
	GlyphSetXY(BSP_GraphLCD, i*8, 9);
	GlyphChar(BSP_GraphLCD, '~');*/


        


	
        char str1[10];
	char Freq[12];
	amplitude=(maxVolt-minVolt)/2;
        Str_FmtNbr_Int32U(amplitude*1000, 6, 10, ' ', DEF_NO, DEF_YES, str1);
        //Str_FmtNbr_32(amplitude,0,0, ' ', DEF_YES,str1);
	//sprintf(amp,"%f",10.5);//amplitude);
	
	
	BSP_GraphLCD_String(0,"Mitch & Paul");
	BSP_GraphLCD_String(6,str1);
        
	//lcd_display1(LCD_LINE7,buf);
	
      
      
      
   if((freqF/count)-10>=freq  &&(freqF/count)+10<=freq);
   else{
   freqF=freqF+freq;
   count++;
   }
   char buff[12];
  // sprintf(buff, "FREQ=%5d\n\r",freqF/count);
   //Str_FmtNbr_Int32U(freq, 6, 10, ' ', DEF_NO, DEF_YES, str1);
   //BSP_GraphLCD_String(7, str1);
      
      
      
        }
      
      
      
      
      
      
      
      /////////////////////////////////////////////////////////////////////






  delayMS(2000);




} 
  
  
  
  
  
  
  
  
  
  
  
}



/*$PAGE*/
/*
*********************************************************************************************************
*                                           App_TaskStart()
*
* Description : Startup task example code.
*
* Arguments   : p_arg       Argument passed by 'OSTaskCreate()'.
*
* Returns     : none.
*
* Created by  : main().
*
* Notes       : (1) The ticker MUST be initialized AFTER multitasking has started.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT08U  i;
    OS_ERR      err;


   (void)&p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */

    CPU_Init();                                                 /* Initialize the uC/CPU services                       */
    
    OSSemCreate(&App_SemADC, "ADC Sem", 0, &err);
OSTaskCreate((OS_TCB     *)&AppTaskLCDTCB,
             (CPU_CHAR   *)"App Task LCD",
             (OS_TASK_PTR ) AppTaskLCD,
             (void       *) 0,
             (OS_PRIO     ) APP_CFG_TASK_LCD_PRIO,
             (CPU_STK    *)&AppTaskLCDStk[0],
             (CPU_STK_SIZE) APP_CFG_TASK_LCD_STK_SIZE/10u,
             (CPU_STK_SIZE) APP_CFG_TASK_LCD_STK_SIZE,
             (OS_MSG_QTY  ) 0u,
             (OS_TICK     ) 0u,
             (void       *) 0,
             (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
             (OS_ERR     *)&err);
    
    

#if (OS_CFG_STAT_TASK_EN > 0u)
    OSStatTaskCPUUsageInit(&err);                               /* Compute CPU capacity with no task running            */
#endif

    Mem_Init();

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif

#if BSP_CFG_SER_EN > 0u
    BSP_Ser_Init(115200);                                       /* Initialize serial communications                     */
#endif
    
#if (BSP_CFG_GRAPH_LCD_EN > 0u)
    BSP_GraphLCD_Clear();
#endif

    
    
    while (DEF_ON) {
        for (i = 4; i <= 15; i++) {
            BSP_LED_On(i);
            OSTimeDlyHMSM(0, 0, 0, 100,
                          OS_OPT_TIME_HMSM_STRICT,
                         &err);
        }      
        for (i = 15; i >= 4; i--) {
            BSP_LED_Off(i);
            OSTimeDlyHMSM(0, 0, 0, 100,
                          OS_OPT_TIME_HMSM_STRICT,
                         &err);
        }
    }
    
}

int first=0;

void delayMS(int delay_time)
{
	/* Used to pace delay for the LED */
	uint32_t led_counter;
	int i;
	for(i=0;i<delay_time; i++)
	{
		for (led_counter = 0; led_counter < 14000; led_counter++){}
	}
}

#if      __RENESAS__
#pragma  interrupt  OS_BSP_TickISR 
#endif
CPU_ISR  App_ISRADC (void) 
{ 
  OS_ERR  err;
OSIntEnter();
//OSSemPost(&App_SemADC, OS_OPT_POST_1, &err); 
int adc_val = S12AD.ADDR1;

//if((pos==96) && (current!=0)) {
//  if(first==0){ BSP_Ser_WrStr("HI1"); first++;}
  
  
  

//}
if((pos==96) && (current==0)) {
  
  first=0;
  pos=0;
  data[pos]=adc_val;
  pos++;
}

if(pos!=96){
data[pos]=adc_val;
pos++;
}
OSIntExit();
}


