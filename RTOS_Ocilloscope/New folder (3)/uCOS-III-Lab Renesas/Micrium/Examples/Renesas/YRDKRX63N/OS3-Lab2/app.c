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
*                                       uC/OS-III Training Lab 2
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

#include  "..\bsp\bsp.h"
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


/*$PAGE*/
/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  CPU_STK  AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  CPU_STK  AppTaskStart2Stk[512];

static  OS_TCB   AppTaskStartTCB;
static  OS_TCB   AppTaskStart2TCB;

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

static  void  AppTaskStart (void  *p_arg);
static  void  AppTask_Start2 (void  *p_arg);

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
                 (OS_PRIO     ) 6,
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
       OSTaskCreate((OS_TCB     *)&AppTaskStart2TCB,                /* Create the start task                                */
                 (CPU_CHAR   *)"Another Task",
                 (OS_TASK_PTR ) AppTask_Start2,
                 (void       *) 0,
                 (OS_PRIO     ) 5,
                 (CPU_STK    *)&AppTaskStart2Stk[512],
                 (CPU_STK_SIZE)0,
                 (CPU_STK_SIZE) 128,
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

    BSP_GraphLCD_String(2,"RTOS LAB2");
    BSP_GraphLCD_String(3,"Mitch & Paul");
    BSP_GraphLCD_String(4,"Not Task 2");
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
static  void  AppTask_Start2 (void *p_arg)
{
    CPU_INT08U  i;
    OS_ERR      err;


   (void)&p_arg;

    BSP_Init();                                                 /* Initialize BSP functions                             */

    CPU_Init();                                                 /* Initialize the uC/CPU services                       */

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
    while(1){
BSP_GraphLCD_Clear();
BSP_GraphLCD_String(2,"uC/OS-III");
BSP_GraphLCD_String(3,"on the");
BSP_GraphLCD_String(4,"RX MCU");
    OSTimeDlyHMSM(0, 0, 5, 0,
                          OS_OPT_TIME_HMSM_STRICT,
                         &err);

    BSP_GraphLCD_Clear();
BSP_GraphLCD_String(2,"Micrium");
BSP_GraphLCD_String(3,"Hands-On");
BSP_GraphLCD_String(4,"Training");
    OSTimeDlyHMSM(0, 0, 5, 0,
                          OS_OPT_TIME_HMSM_STRICT,
                         &err);
    }
}
