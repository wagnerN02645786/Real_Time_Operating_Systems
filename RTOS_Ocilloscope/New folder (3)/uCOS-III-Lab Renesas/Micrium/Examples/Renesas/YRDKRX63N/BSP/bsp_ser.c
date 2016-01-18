/*
*********************************************************************************************************
*                                         BOARD SUPPORT PACKAGE
*
*                            (c) Copyright 2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved. Protected by international copyright laws.
*
*               BSP is provided in source form to registered licensees ONLY.  It is
*               illegal to distribute this source code to any third party unless you receive
*               written permission by an authorized Micrium representative.  Knowledge of
*               the source code may NOT be used to develop a similar product.
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
*                                         BOARD SUPPORT PACKAGE
*
*                                              Renesas RX63N
*                                                on the
*                                        YRDKRX63N Evaluation Board
*
* Filename      : bsp_ser.c
* Version       : V1.00
* Programmer(s) : AA
*                 JM
*********************************************************************************************************
*/

#include  <bsp_cfg.h>

#if  BSP_CFG_SER_EN > 0u
#include  <cpu.h>
#include  <cpu_core.h>
#include  <lib_def.h>
#include  <bsp_os.h>
#include  <bsp_sys.h>
#include  <bsp_int_vect_tbl.h>
#include  <bsp_ser.h>

#include  <stdio.h>
#include  <stdarg.h>

#include  <iodefine.h>


int vsnprintf(char *str, size_t str_m, const char *fmt, va_list ap);


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


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

static  BSP_OS_SEM   BSP_SerTxWait;
static  BSP_OS_SEM   BSP_SerRxWait;
static  BSP_OS_SEM   BSP_SerLock;
static  CPU_INT08U   BSP_SerRxData;


/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void        BSP_Ser_WrByteUnlocked  (CPU_INT08U  c);
static  CPU_INT08U  BSP_Ser_RdByteUnlocked  (void);
static  CPU_INT16U  BSP_Ser_GetBRR          (CPU_INT08U  bclk,
                                             CPU_INT32U  baud_rate);


/*
*********************************************************************************************************
*********************************************************************************************************
*                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           BSP_Ser_Init()
*
* Description : Initialize serial module for communication.
*
* Argument(s) : baud_rate   Baud rate of RS-232.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_Init (CPU_INT32U  baud_rate)
{
    CPU_INT08U  baud_clk;
    CPU_INT16U  brr;
    CPU_REG32   i;


                                                                /* Initialize OS Objects.                               */
    BSP_OS_SemCreate(&BSP_SerTxWait, 0, "Serial Tx Wait");
    BSP_OS_SemCreate(&BSP_SerRxWait, 0, "Serial Rx Wait");
    BSP_OS_SemCreate(&BSP_SerLock,   1, "Serial Lock");


    SYSTEM.PRCR.WORD        = 0xA50B;                           /* Protect off.                                         */
    MPC.PWPR.BIT.B0WI       =      0;                           /* Unlock protection register.                          */
    MPC.PWPR.BIT.PFSWE      =      1;                           /* Unlock MPC registers.                                */

    SYSTEM.MSTPCRA.BIT.ACSE =      0;                           /* clear ACSE Bit (All-Module Clock Stop Mode Enable).  */

    MSTP(SCI2)              =      0;                           /* Cancel stop state of SCI2 Periph to en wr to it.     */

    SYSTEM.PRCR.WORD        = 0xA500;                           /* Protect on.                                          */

    SCI2.SCR.BYTE           =   0x00;                           /* Clear bits TIE, RIE, RE, and TEIE in SCR to 0 ...    */
                                                                /* ... Set CKE to internal.                             */


                                                                /* Set up the UART I/O port and pins.                   */
    MPC.P50PFS.BYTE         =   0x4A;                           /* P50 is TxD2.                                         */
    MPC.P52PFS.BYTE         =   0x4A;                           /* P52 is RxD2.                                         */

    PORT5.PDR.BIT.B0        =      1;                           /* TxD2 is output.                                      */
    PORT5.PDR.BIT.B2        =      0;                           /* RxD2 is input.                                       */

    PORT5.PMR.BIT.B0        =      1;                           /* TxD2 is peripheral.                                  */
    PORT5.PMR.BIT.B2        =      1;                           /* RxD2 is peripheral.                                  */


    IR(SCI2, RXI2)          =      0;                           /* Clear any pending ISR.                               */
    IR(SCI2, TXI2)          =      0;
    IR(SCI2, TEI2)          =      0;

    IPR(SCI2, RXI2)         =      BSP_CFG_SER_IPL;             /* Set interrupt priority.                              */
    IPR(SCI2, TXI2)         =      BSP_CFG_SER_IPL;
    IPR(SCI2, TEI2)         =      BSP_CFG_SER_IPL;

    IEN(SCI2, RXI2)         =      1;                           /* Enable interrupt source.                             */
    IEN(SCI2, TXI2)         =      1;
    IEN(SCI2, TEI2)         =      1;


    BSP_IntVectSet(VECT_SCI2_RXI2, (CPU_FNCT_VOID)BSP_Ser_ISR_Rx_Handler);
    BSP_IntVectSet(VECT_SCI2_TXI2, (CPU_FNCT_VOID)BSP_Ser_ISR_Tx_Handler);



                                                                /* Set data transfer format in Serial Mode Register.    */
                                                                /*  -Asynchronous Mode.                                 */
                                                                /*  -8 bits.                                            */
                                                                /*  -no parity.                                         */
                                                                /*  -1 stop bit.                                        */
                                                                /*  -PCLK clock (n = 0).                                */
    SCI2.SMR.BYTE      = 0x00;

    SCI2.SCMR.BIT.SMIF =    0;                                  /* Set to 0 for serial communications interface mode.   */

    baud_clk           =    0;                                  /* Algorithm for finding BRR value                      */
    do {                                                        /* N = BRR (0-255), B = bit rate, n = baud clock        */
        brr = BSP_Ser_GetBRR(baud_clk, baud_rate);              /* N = Pclk / (64 * 2^(2 * n - 1) * B) - 1              */
        if (brr > 255) {
            baud_clk++;
        }
    } while (brr > 255);

    SCI2.SMR.BIT.CKS = baud_clk;
    SCI2.BRR         = brr;

    for (i = 0; i < 10000; i++) {                               /* Wait for at least 1 bit time                         */
        ;
    }
                                                                /* Enable RXI and TXI interrupts in SCI peripheral.     */
    SCI2.SCR.BYTE |= 0x30;
}


/*
*********************************************************************************************************
*                                          BSP_Ser_GetBRR()
*
* Description   : Calculate BRR for the settings chosen.
*
* Argument      : bclk          Baud rate generator clock source (0-3)
*
*                 baud_rate     Baud rate.
*
* Returns       : Calculated BRR.
*********************************************************************************************************
*/

static  CPU_INT16U  BSP_Ser_GetBRR (CPU_INT08U  bclk,
                                    CPU_INT32U  baud_rate)
{
    CPU_INT16U  mult;
    CPU_INT16U  val;

                                                                /* Switch for (64 * 2^(2 * baud_clk - 1)) values.       */
    switch (bclk) {
        case 0:
             mult =   32;
             break;

        case 1:
             mult =  128;
             break;

        case 2:
             mult =  512;
             break;

        case 3:
        default:
             mult = 2048;
             break;
    }


    val = BSP_SysPerClkFreqGet() / (mult * baud_rate) - 1;

    return (val);
}


/*
*********************************************************************************************************
*                                         BSP_Ser_ISR_Rx_Handler()
*
* Description : Serial ISR
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if      __RENESAS__
#pragma  interrupt  BSP_Ser_ISR_Rx_Handler
#endif

CPU_ISR  BSP_Ser_ISR_Rx_Handler (void)
{
    OSIntEnter();                                               /* Notify uC/OS-III or uC/OS-II of ISR entry            */
    CPU_INT_GLOBAL_EN();                                        /* Reenable global interrupts                           */

    BSP_SerRxData  = SCI2.RDR;                                  /* Read one byte from the receive data register.        */
    IR(SCI2, RXI2) = 0;                                         /* Clear the SCI2 receive interrupt.                    */
    BSP_OS_SemPost(&BSP_SerRxWait);                             /* Post to the semaphore.                               */

    OSIntExit();                                                /* Notify uC/OS-III or uC/OS-II of ISR exit             */
}


/*
*********************************************************************************************************
*                                         BSP_Ser_ISR_Tx_Handler()
*
* Description : Serial ISR
*
* Argument(s) : none
*
* Return(s)   : none.
*
* Caller(s)   : This is an ISR.
*
* Note(s)     : none.
*********************************************************************************************************
*/

#if      __RENESAS__
#pragma  interrupt  BSP_Ser_ISR_Tx_Handler
#endif

CPU_ISR  BSP_Ser_ISR_Tx_Handler (void)
{
    OSIntEnter();                                               /* Notify uC/OS-III or uCOS-II of ISR entry             */
    CPU_INT_GLOBAL_EN();                                        /* Reenable global interrupts                           */

//    if (SCI2.SSR.BIT.TEND) {                                  /* Check the Transmission Complete bit.                 */
    SCI2.SSR.BIT.TEND;
    SCI2.SSR.BIT.TEND = 0;
    BSP_OS_SemPost(&BSP_SerTxWait);                             /* Post to the semaphore                                */
//    }

    OSIntExit();                                                /* Notify uC/OS-III or uC/OS-II of ISR exit             */
}


/*
*********************************************************************************************************
*                                                BSP_Ser_Printf()
*
* Description : Formatted output to the serial port.
*               This function reads a string from a serial port. This call blocks until a
*               character appears at the port and the last character is a Carriage
*               Return (0x0D).
*
* Argument(s) : Format string following the C format convention.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_Printf (CPU_CHAR *format, ...)
{
    static  CPU_CHAR  buffer[80 + 1];
            va_list   vArgs;


    va_start(vArgs, format);
    vsprintf((char *)buffer, (char const *)format, vArgs);
    va_end(vArgs);

    BSP_Ser_WrStr((CPU_CHAR*) buffer);
}


/*
*********************************************************************************************************
*                                                BSP_Ser_RdByte()
*
* Description : Receive a single byte.
*
* Argument(s) : none.
*
* Return(s)   : The received byte
*
* Caller(s)   : Application
*
* Note(s)     : (1) This functions blocks until a data is received.
*
*               (2) It can not be called from an ISR.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Ser_RdByte (void)
{
    CPU_INT08U  rx_byte;


    BSP_OS_SemWait(&BSP_SerLock, 0);                            /* Obtain access to the serial interface              */

    rx_byte = BSP_Ser_RdByteUnlocked();

    BSP_OS_SemPost(&BSP_SerLock);                               /* Release access to the serial interface             */

    return (rx_byte);
}


/*
*********************************************************************************************************
*                                       BSP_Ser_RdByteUnlocked()
*
* Description : Receive a single byte.
*
* Argument(s) : none.
*
* Return(s)   : The received byte
*
* Caller(s)   : BSP_Ser_RdByte()
*               BSP_Ser_RdStr()
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT08U  BSP_Ser_RdByteUnlocked (void)
{
    CPU_INT08U   rx_byte;


    IEN(SCI2, RXI2)  = 1;                                       /* Enable the Receive Data Register Full interrupt.     */
    SCI2.SCR.BIT.RIE = 1;                                       /* Enable Receive Interrupts                            */

    BSP_OS_SemWait(&BSP_SerRxWait, 0);                          /* Wait until a data is received                        */

    IEN(SCI2, RXI2)  = 0;                                       /* Disable the Receive Data Register Full interrupt.    */
    SCI2.SCR.BIT.RIE = 0;                                       /* Disable Receive Interrupts                           */

    rx_byte = BSP_SerRxData;                                    /* Read the data form the temporal register.            */

    return (rx_byte);
}


/*
*********************************************************************************************************
*                                                BSP_Ser_RdStr()
*
* Description : This function reads a string from a UART.
*
* Argument(s) : p_str      A pointer to a buffer at which the string can be stored.
*
*               len         The size of the string that will be read.
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_RdStr (CPU_CHAR    *p_str,
                     CPU_INT16U   len)
{
    CPU_CHAR     rx_data;
    CPU_CHAR     rx_buf_ix;
    CPU_BOOLEAN  err;


    rx_buf_ix = 0;
    p_str[0]  = 0;

    err       = BSP_OS_SemWait(&BSP_SerLock, 0);                /* Obtain access to the serial interface              */

    if (err != DEF_OK ) {
        return;
    }

    while (DEF_TRUE) {
        rx_data = BSP_Ser_RdByteUnlocked();

        if ((rx_data == ASCII_CHAR_CARRIAGE_RETURN) ||          /* Is it '\r' or '\n' character  ?                    */
            (rx_data == ASCII_CHAR_LINE_FEED      )) {

            BSP_Ser_WrByteUnlocked((CPU_INT08U)ASCII_CHAR_LINE_FEED);
            BSP_Ser_WrByteUnlocked((CPU_INT08U)ASCII_CHAR_CARRIAGE_RETURN);

            p_str[rx_buf_ix] = 0;                              /* set the null character at the end of the string     */
            break;                                             /* exit the loop                                       */
        }

        if (rx_data == ASCII_CHAR_BACKSPACE) {                 /* Is backspace character                              */
            if (rx_buf_ix > 0) {
                BSP_Ser_WrByteUnlocked((CPU_INT08U)ASCII_CHAR_BACKSPACE);
                BSP_Ser_WrByteUnlocked((CPU_INT08U)ASCII_CHAR_SPACE);
                BSP_Ser_WrByteUnlocked((CPU_INT08U)ASCII_CHAR_BACKSPACE);

                rx_buf_ix--;                                   /* Decrement the index                                 */
                p_str[rx_buf_ix] = 0;
            }
        }

        if (ASCII_IsPrint(rx_data)) {                           /* Is it a printable character ... ?                  */
            BSP_Ser_WrByteUnlocked((CPU_INT08U)rx_data);        /* Echo-back                                          */
            p_str[rx_buf_ix] = rx_data;                         /* Save the received character in the buffer          */
            rx_buf_ix++;                                        /* Increment the buffer index                         */
            if (rx_buf_ix >= len) {
               rx_buf_ix = len;
            }
        }
    }

    BSP_OS_SemPost(&BSP_SerLock);                               /* Release access to the serial interface            */
}


/*
*********************************************************************************************************
*                                          BSP_Ser_WrByteUnlocked()
*
* Description : Writes a single byte to a serial port.
*
* Argument(s) : c    The character to output.
*
* Return(s)   : none.
*
* Caller(s)   : BSP_Ser_WrByte()
*               BSP_Ser_WrByteUnlocked()
*
* Note(s)     : (1) This function blocks until room is available in the UART for the byte to be sent.
*********************************************************************************************************
*/

void  BSP_Ser_WrByteUnlocked (CPU_INT08U c)
{
                                                                /* Enable the Transmission Complete interrupt.          */
    IEN(SCI2, TEI2)  = 1;
    SCI2.SCR.BIT.TIE = 1;                                       /* Enable Transmit Interrupts                           */

    SCI2.TDR         = c;                                       /* Place character to be transmitted into register      */

    BSP_OS_SemWait(&BSP_SerTxWait, 0);

    IEN(SCI2, TEI2)  = 0;
    SCI2.SCR.BIT.TIE = 0;                                       /* Disable Transmit Interrupts                          */
}


/*
*********************************************************************************************************
*                                      ProbeRS232_TxIntDis()
*
* Description : Disable Tx interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_TxIntDis (void)
{
    do {
        SCI2.SCR.BIT.TIE = 0;                                   /* Disable tx int.                                      */
    } while (SCI2.SCR.BIT.TIE > 0);

    while (SCI2.SSR.BIT.TEND == 0) {                            /* Read transmission end flag until set.                */
        ;
    }
}


/*
*********************************************************************************************************
*                                      BSP_Ser_TxIntEn()
*
* Description : Enable Tx interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*********************************************************************************************************
*/

void  BSP_Ser_TxIntEn (void)
{
    SCI2.SCR.BIT.TIE = 1;                                       /* Enable tx int.                                       */
}


/*
*********************************************************************************************************
*                                                BSP_Ser_WrByte()
*
* Description : Writes a single byte to a serial port.
*
* Argument(s) : tx_byte     The character to output.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_WrByte(CPU_INT08U  c)
{
    BSP_OS_SemWait(&BSP_SerLock, 0);                            /* Obtain access to the serial interface              */

    BSP_Ser_WrByteUnlocked(c);

    BSP_OS_SemPost(&BSP_SerLock);                               /* Release access to the serial interface             */
}


/*
*********************************************************************************************************
*                                                BSP_Ser_WrStr()
*
* Description : Transmits a string.
*
* Argument(s) : p_str      Pointer to the string that will be transmitted
*
* Caller(s)   : Application
*
* Return(s)   : none.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Ser_WrStr (CPU_CHAR  *p_str)
{
    CPU_BOOLEAN  err;


    err = BSP_OS_SemWait(&BSP_SerLock, 0);                      /* Obtain access to the serial interface              */

    if (err != DEF_OK ) {
        return;
    }

    while ((*p_str) != (CPU_CHAR )0) {

        if (*p_str == ASCII_CHAR_LINE_FEED) {
            BSP_Ser_WrByteUnlocked(ASCII_CHAR_CARRIAGE_RETURN);
            BSP_Ser_WrByteUnlocked(ASCII_CHAR_LINE_FEED);
            p_str++;
        } else {
            BSP_Ser_WrByteUnlocked(*p_str++);
        }
    }

    BSP_OS_SemPost(&BSP_SerLock);                               /* Release access to the serial interface             */
}
#endif
