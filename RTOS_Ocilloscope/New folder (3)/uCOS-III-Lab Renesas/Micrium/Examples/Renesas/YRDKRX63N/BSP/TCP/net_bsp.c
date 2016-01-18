/*
*********************************************************************************************************
*                                             uC/TCP-IP V3
*                                      The Embedded TCP/IP Suite
*
*                          (c) Copyright 2003-2014; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*
*               uC/TCP-IP is provided in source form to registered licensees ONLY.  It is
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
*                            NETWORK BOARD SUPPORT PACKAGE (BSP) FUNCTIONS
*
*                                         Renesas YRDKRX63N
*
* Filename      : net_bsp.c
* Version       : V3.00.00
* Programmer(s) : ITJ
*                 FGK
*                 PC
*********************************************************************************************************
* Note(s)       : (1) Assumes the following versions (or more recent) of software modules are included in
*                     the project build :
*
*                     (a) uC/TCP-IP V2.10
*                     (b) uC/OS-III V3.00.0
*                     (c) uC/LIB    V1.27
*                     (d) uC/CPU    V1.20
*
*                 (2) To provide the required Board Support Package functionality, insert the appropriate
*                     board-specific code to perform the stated actions wherever '$$$$' comments are found.
*
*                     #### This note MAY be entirely removed for specific board support packages.
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/

#define    NET_BSP_MODULE
#include  "../../../../../Software/uC-TCPIP/Source/net.h"
#include  "../../../../../Software/uC-TCPIP/IF/net_if_ether.h"
#include  "../bsp.h"
#include  "../bsp_int_vect_tbl.h"
#include  "net_bsp.h"
#include  <os.h>
#include  <iodefine.h>



/*$PAGE*/
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
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                  NETWORK DEVICE INTERFACE NUMBERS
*
* Note(s) : (1) (a) Each network device maps to a unique network interface number.
*
*               (b) Instances of network devices' interface number SHOULD be named using the following
*                   convention :
*
*                       <Board><Device>[Number]_IF_Nbr
*
*                           where
*                                   <Board>         Development board name
*                                   <Device>        Network device name (or type)
*                                   [Number]        Network device number for each specific instance
*                                                       of device (optional if the development board
*                                                       does NOT support multiple instances of the
*                                                       specific device)
*
*                   For example, the network device interface number variable for the #2 MACB Ethernet
*                   controller on an Atmel AT91SAM92xx should be named 'AT91SAM92xx_MACB_2_IF_Nbr'.
*
*               (c) Network device interface number variables SHOULD be initialized to 'NET_IF_NBR_NONE'.
*********************************************************************************************************
*/

static  NET_IF_NBR  RX63N_IF_Nbr = NET_IF_NBR_NONE;


/*$PAGE*/
/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                               NETWORK DEVICE BSP FUNCTION PROTOTYPES
*
* Note(s) : (1) Device driver BSP functions may be arbitrarily named.  However, it is recommended that
*               device BSP functions be named using the suggested names/conventions provided below.
*
*               (a) (1) Network device BSP functions SHOULD be named using the following convention :
*
*                           NetDev_[Device]<Function>[Number]()
*
*                               where
*                                   (A) [Device]        Network device name or type (optional if the
*                                                           development board does NOT support multiple
*                                                           devices)
*                                   (B) <Function>      Network device BSP function
*                                   (C) [Number]        Network device number for each specific instance
*                                                           of device (optional if the development board
*                                                           does NOT support multiple instances of the
*                                                           specific device)
*
*                       For example, the NetDev_CfgClk() function for the #2 MACB Ethernet controller
*                       on an Atmel AT91SAM92xx should be named NetDev_MACB_CfgClk2().
*
*                   (2) BSP-level device ISR handlers SHOULD be named using the following convention :
*
*                           NetDev_[Device]ISR_Handler[Type][Number]()
*
*                               where
*                                   (A) [Device]        Network device name or type (optional if the
*                                                           development board does NOT support multiple
*                                                           devices)
*                                   (B) [Type]          Network device interrupt type (optional if
*                                                           interrupt type is generic or unknown)
*                                   (C) [Number]        Network device number for each specific instance
*                                                           of device (optional if the development board
*                                                           does NOT support multiple instances of the
*                                                           specific device)
*
*               (b) All BSP function prototypes SHOULD be located within the development board's network
*                   BSP C source file ('net_bsp.c') & be declared as static functions to prevent name
*                   clashes with other network protocol suite BSP functions/files.
*********************************************************************************************************
*/

                                                                /* -- GENERIC NETWORK DEVICE BSP FUNCTION PROTOTYPES -- */
static  void        NetDev_CfgClk            (NET_IF   *pif,
                                              NET_ERR  *perr);

static  void        NetDev_CfgIntCtrl        (NET_IF   *pif,
                                              NET_ERR  *perr);

static  void        NetDev_CfgGPIO           (NET_IF   *pif,
                                              NET_ERR  *perr);

static  CPU_INT32U  NetDev_ClkFreqGet        (NET_IF   *pif,
                                              NET_ERR  *perr);


/*$PAGE*/
/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    NETWORK DEVICE BSP INTERFACE
*
* Note(s) : (1) Device board-support package (BSP) interface structures are used by the device driver to
*               call specific devices' BSP functions via function pointer instead of by name.  This enables
*               the network protocol suite to compile & operate with multiple instances of multiple devices
*               & drivers.
*
*           (2) In most cases, the BSP interface structure provided below SHOULD suffice for most devices'
*               BSP functions exactly as is with the exception that BSP interface structures' names MUST be
*               unique & SHOULD clearly identify the development board, device name, & possibly the specific
*               device number (if the development board supports multiple instances of any given device).
*
*               (a) BSP interface structures SHOULD be named using the following convention :
*
*                       NetDev_BSP_<Board><Device>[Number]{}
*
*                           where
*                               (1) <Board>         Development board name
*                               (2) <Device>        Network device name (or type)
*                               (3) [Number]        Network device number for each specific instance
*                                                       of device (optional if the development board
*                                                       does NOT support multiple instances of the
*                                                       specific device)
*
*                   For example, the BSP interface structure for the #2 MACB Ethernet controller on
*                   an Atmel AT91SAM92xx should be named NetDev_BSP_AT91SAM92xx_MACB_2{}.
*
*               (b) The BSP interface structure MUST also be externally declared in the development
*                   board's network BSP header file ('net_bsp.h') with the exact same name & type.
*********************************************************************************************************
*/
                                                                            /* Board-/device-specific BSP fnct ptrs :   */
const  NET_DEV_BSP_ETHER  NetDev_BSP_YRDK_RX63N = { NetDev_CfgClk,          /*   Cfg clk(s)                             */
                                                    NetDev_CfgIntCtrl,      /*   Cfg int ctrl(s)                        */
                                                    NetDev_CfgGPIO,         /*   Cfg GPIO                               */
                                                    NetDev_ClkFreqGet       /*   Get clk freq                           */
                                                  };


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                                   NETWORK DEVICE DRIVER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        NetDev_ISR_Handler()
*
* Description : BSP-level ISR handler(s) for device interrupts.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU &/or device interrupts.
*
* Note(s)     : (1) (a) Each device interrupt, or set of device interrupts, MUST be handled by a
*                       unique BSP-level ISR handler which maps each specific device interrupt to
*                       its corresponding network interface ISR handler.
*
*                   (b) BSP-level device ISR handlers SHOULD be named using the following convention :
*
*                           NetDev_[Device]ISR_Handler[Type][Number]()
*
*                               where
*                                   (1) [Device]        Network device name or type (optional if the
*                                                           development board does NOT support multiple
*                                                           devices)
*                                   (2) [Type]          Network device interrupt type (optional if
*                                                           interrupt type is generic or unknown)
*                                   (3) [Number]        Network device number for each specific instance
*                                                           of device (optional if the development board
*                                                           does NOT support multiple instances of the
*                                                           specific device)
*
*                       See also 'NETWORK DEVICE BSP FUNCTION PROTOTYPES  Note #2a2'.
*********************************************************************************************************
*/

#if      __RENESAS__
#pragma  interrupt  NetDev_ISR
#endif

CPU_ISR  NetDev_ISR (void)
{
    NET_IF_NBR        if_nbr;
    NET_DEV_ISR_TYPE  type;
    NET_ERR           err;


    OSIntEnter();                                               /* Notify uC/OS-III or uC/OS-II of ISR entry            */
    CPU_INT_GLOBAL_EN();                                        /* Reenable global interrupts                           */

    if_nbr = RX63N_IF_Nbr;                                      /* See Note #1b3.                                       */
    type   = NET_DEV_ISR_TYPE_UNKNOWN;                          /* See Note #1b2.                                       */

    NetIF_ISR_Handler(if_nbr, type, &err);
    OSIntExit();                                                /* Notify uC/OS-III or uC/OS-II of ISR exit             */
}


/*$PAGE*/

/*
*********************************************************************************************************
*                                           NetDev_CfgClk()
*
* Description : Configure clocks for the specified interface/device.
*
* Argument(s) : pif         Pointer to network interface to configure.
*               ---         Argument validated in NetDev_Init().
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device clock(s)     successfully configured.
*                               NET_DEV_ERR_FAULT               Device clock(s) NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_CfgClk (NET_IF   *pif,
                             NET_ERR  *perr)
{
   (void)&pif;                                                  /* Prevent 'variable unused' compiler warning.          */

    MSTP(EDMAC) = 0;

   *perr        = NET_DEV_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         NetDev_CfgIntCtrl()
*
* Description : Configure interrupts &/or interrupt controller for the specified interface/device.
*
* Argument(s) : pif         Pointer to network interface to configure.
*               ---         Argument validated in NetDev_Init().
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device interrupt(s)     successfully configured.
*                               NET_DEV_ERR_FAULT               Device interrupt(s) NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_CfgIntCtrl (NET_IF   *pif,
                                 NET_ERR  *perr)
{
    RX63N_IF_Nbr     = pif->Nbr;                                /* Cfg this device's BSP instance with specific IF nbr. */

    BSP_IntVectSet(VECT_DMAC_DMAC0I, (CPU_FNCT_VOID)NetDev_ISR);
    BSP_IntVectSet(VECT_ETHER_EINT,  (CPU_FNCT_VOID)NetDev_ISR);

    IR(ETHER, EINT)  = 0;                                       /* Clear any pending ISR.                               */
    IPR(ETHER, EINT) = 4;                                       /* Set interrupt priority.                              */
    IEN(ETHER, EINT) = 1;                                       /* Enable interrupt source.                             */

   *perr             = NET_DEV_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                          NetDev_CfgGPIO()
*
* Description : Configure general-purpose I/O (GPIO) for the specified interface/device.
*
* Argument(s) : pif         Pointer to network interface to configure.
*               ---         Argument validated in NetDev_Init().
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device GPIO     successfully configured.
*                               NET_DEV_ERR_FAULT               Device GPIO NOT successfully configured.
*
* Return(s)   : none.
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  NetDev_CfgGPIO (NET_IF   *pif,
                              NET_ERR  *perr)
{
    NET_PHY_CFG_ETHER  *pphy_cfg;


#if (NET_VERSION >= 21200)                                      /* Obtain pointer to Phy cfg.                           */
    pphy_cfg = (NET_PHY_CFG_ETHER *)pif->Ext_Cfg;
#else
    pphy_cfg = (NET_PHY_CFG_ETHER *)pif->Phy_Cfg;
#endif

    if (pphy_cfg == (NET_PHY_CFG_ETHER *)0) {
       *perr = NET_DEV_ERR_FAULT;
        return;
    }


    SYSTEM.PRCR.WORD = 0xA50B;                                  /* Protect off.                                         */
    MPC.PWPR.BIT.B0WI = 0 ;                                     /* Unlock protection register.                          */
    MPC.PWPR.BIT.PFSWE = 1 ;                                    /* Unlock MPC registers.                                */


    switch (pphy_cfg->BusMode) {
        case NET_PHY_BUS_MODE_MII:
             MPC.PFENET.BIT.PHYMODE = 0x01;                     /* PHY Mode is MII.                                     */
            *perr = NET_DEV_ERR_FAULT;
             break;


        case NET_PHY_BUS_MODE_RMII:
             MPC.PFENET.BIT.PHYMODE = 0x00;                     /* PHY Mode is RMII.                                    */


                                                                /* Port A - Ethernet MDIO                               */
             PORTA.PODR.BIT.B3      = DEF_OFF;                  /* PA3 is Low output.                                   */
             PORTA.PODR.BIT.B4      = DEF_OFF;                  /* PA4 is Low output.                                   */
             PORTA.PODR.BIT.B5      = DEF_OFF;                  /* PA5 is Low output.                                   */

             PORTA.PMR.BIT.B3       = DEF_OFF;                  /* PA3 is GPIO for now .                                */
             PORTA.PMR.BIT.B4       = DEF_OFF;                  /* PA4 is GPIO for now .                                */
             PORTA.PMR.BIT.B5       = DEF_OFF;                  /* PA5 is GPIO for now .                                */

             MPC.PA3PFS.BIT.PSEL    = 0x11 ;                    /* PA3 is ET_MDIO.                                      */
             MPC.PA3PFS.BIT.ISEL    = DEF_OFF;                  /* PA3 is not used as IRQn input pin.                   */

             MPC.PA4PFS.BIT.PSEL    = 0x11 ;                    /* PA4 is ET_MDC.                                       */
             MPC.PA4PFS.BIT.ISEL    = DEF_OFF;                  /* PA4 is not used as IRQn input pin.                   */

             MPC.PA5PFS.BIT.PSEL    = 0x11 ;                    /* PA5 is ET_LINKSTA.                                   */

             PORTA.PMR.BIT.B3       = DEF_ON;                   /* PA3 is used by Ethernet peripheral.                  */
             PORTA.PMR.BIT.B4       = DEF_ON;                   /* PA4 is used by Ethernet peripheral.                  */
             PORTA.PMR.BIT.B5       = DEF_ON;                   /* PA5 is used by Ethernet peripheral.                  */

             PORTA.PDR.BIT.B3       = DEF_ON;                   /* PA3 is output.                                       */
             PORTA.PDR.BIT.B4       = DEF_ON;                   /* PA4 is output.                                       */
             PORTA.PDR.BIT.B5       = DEF_ON;                   /* PA5 is output.                                       */


                                                                /* Port B - Ethernet signals.                           */
             PORTB.PODR.BIT.B0      = DEF_OFF;                  /* PB0 is Low output.                                   */
             PORTB.PODR.BIT.B1      = DEF_OFF;                  /* PB1 is Low output.                                   */
             PORTB.PODR.BIT.B2      = DEF_OFF;                  /* PB2 is Low output.                                   */
             PORTB.PODR.BIT.B3      = DEF_OFF;                  /* PB3 is Low output.                                   */
             PORTB.PODR.BIT.B4      = DEF_OFF;                  /* PB4 is Low output.                                   */
             PORTB.PODR.BIT.B5      = DEF_OFF;                  /* PB5 is Low output.                                   */
             PORTB.PODR.BIT.B6      = DEF_OFF;                  /* PB6 is Low output.                                   */
             PORTB.PODR.BIT.B7      = DEF_OFF;                  /* PB7 is Low output.                                   */

             PORTB.PMR.BIT.B0       = DEF_OFF;                  /* PB0 is GPIO for now.                                  */
             PORTB.PMR.BIT.B1       = DEF_OFF;                  /* PB1 is GPIO for now.                                  */
             PORTB.PMR.BIT.B2       = DEF_OFF;                  /* PB2 is GPIO for now.                                  */
             PORTB.PMR.BIT.B3       = DEF_OFF;                  /* PB3 is GPIO for now.                                  */
             PORTB.PMR.BIT.B4       = DEF_OFF;                  /* PB4 is GPIO for now.                                  */
             PORTB.PMR.BIT.B5       = DEF_OFF;                  /* PB5 is GPIO for now.                                  */
             PORTB.PMR.BIT.B6       = DEF_OFF;                  /* PB6 is GPIO for now.                                  */
             PORTB.PMR.BIT.B7       = DEF_OFF;                  /* PB7 is GPIO for now.                                  */


             MPC.PB0PFS.BIT.PSEL    = 0x12 ;                    /* PB0 is RMII_RXD1.                                    */
             MPC.PB0PFS.BIT.ISEL    = DEF_OFF;                  /* PB0 is not used as IRQn input pin.                   */

             MPC.PB1PFS.BIT.PSEL    = 0x12 ;                    /* PB1 is RMII_RXD0.                                    */
             MPC.PB1PFS.BIT.ISEL    = DEF_OFF;                  /* PB1 is not used as IRQn input pin.                   */

             MPC.PB2PFS.BIT.PSEL    = 0x12 ;                    /* PB2 is REF50CK.                                      */
             MPC.PB3PFS.BIT.PSEL    = 0x12 ;                    /* PB3 is RMI_RX_ERR.                                   */
             MPC.PB4PFS.BIT.PSEL    = 0x12 ;                    /* PB4 is RMII_TXD_EN.                                  */
             MPC.PB5PFS.BIT.PSEL    = 0x12 ;                    /* PB5 is RMII_TXD0.                                    */
             MPC.PB6PFS.BIT.PSEL    = 0x12 ;                    /* PB6 is RMII_TXD1.                                    */
             MPC.PB7PFS.BIT.PSEL    = 0x12 ;                    /* PB7 is RMII_CRS_DV .                                 */

             PORTB.PMR.BIT.B0       = DEF_ON;                   /* PB0 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B1       = DEF_ON;                   /* PB1 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B2       = DEF_ON;                   /* PB2 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B3       = DEF_ON;                   /* PB3 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B4       = DEF_ON;                   /* PB4 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B5       = DEF_ON;                   /* PB5 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B6       = DEF_ON;                   /* PB6 is used by Ethernet peripheral.                  */
             PORTB.PMR.BIT.B7       = DEF_ON;                   /* PB7 is used by Ethernet peripheral.                  */

             PORTB.PDR.BIT.B0       = DEF_OFF ;                 /* PB0=0 Set RMII_RXD1   input                            */
             PORTB.PDR.BIT.B1       = DEF_OFF ;                 /* PB0=0 Set RMII_RXD0   input                            */
             PORTB.PDR.BIT.B2       = DEF_OFF ;                 /* PB0=0 Set REF50CK     input                            */
             PORTB.PDR.BIT.B3       = DEF_OFF ;                 /* PB0=0 Set RMI_RX_ERR  input                            */
             PORTB.PDR.BIT.B7       = DEF_OFF ;                 /* PB0=0 Set RMII_CRS_DV input                            */

             break;
    }

   *perr = NET_DEV_ERR_NONE;
}


/*$PAGE*/
/*
*********************************************************************************************************
*                                         NetDev_ClkFreqGet()
*
* Description : Get device clock frequency.
*
* Argument(s) : pif         Pointer to network interface to get clock frequency.
*               ---         Argument validated in NetDev_Init().
*
*               perr        Pointer to variable that will receive the return error code from this function :
*
*                               NET_DEV_ERR_NONE                Device clock frequency     successfully
*                                                                   returned.
*                               NET_DEV_ERR_FAULT               Device clock frequency NOT successfully
*                                                                   returned.
*
* Return(s)   : Device clock frequency (in Hz).
*
* Caller(s)   : NetDev_Init().
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  CPU_INT32U  NetDev_ClkFreqGet (NET_IF   *pif,
                                       NET_ERR  *perr)
{
    CPU_INT32U  clk_freq;


   (void)&pif;                                                  /* Prevent 'variable unused' compiler warning.          */

    clk_freq = 0;
   *perr     = NET_DEV_ERR_NONE;

    return (clk_freq);
}


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                            TRANSMISSION CONTROL PROTOCOL LAYER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                        NetTCP_InitTxSeqNbr()
*
* Description : Initialize the TCP Transmit Initial Sequence Counter, 'NetTCP_TxSeqNbrCtr'.
*
*               (1) Possible initialization methods include :
*
*                   (a) Time-based initialization is one preferred method since it more appropriately
*                       provides a pseudo-random initial sequence number.
*                   (b) Hardware-generated random number initialization is NOT a preferred method since it
*                       tends to produce a discrete set of pseudo-random initial sequence numbers--often
*                       the same initial sequence number.
*                   (c) Hard-coded initial sequence number is NOT a preferred method since it is NOT random.
*
*                   See also 'net_tcp.h  NET_TCP_TX_GET_SEQ_NBR()  Note #1'.
*
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : NetTCP_Init().
*
*               This function is an INTERNAL network protocol suite function & SHOULD NOT be called by
*               application function(s).
*
* Note(s)     : none.
*********************************************************************************************************
*/

#ifdef  NET_TCP_MODULE_PRESENT
void  NetTCP_InitTxSeqNbr (void)
{
    /* $$$$ Insert code to initialize TCP Transmit Initial Sequence Counter (see Note #1).  */

    NetTCP_TxSeqNbrCtr = NET_TCP_SEQ_NBR_NONE;
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*********************************************************************************************************
*                               USER DATAGRAM PROTOCOL LAYER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      NetUDP_RxAppDataHandler()
*
* Description : Application-specific UDP connection handler function.
*
* Argument(s) : pbuf        Pointer to network buffer that received UDP datagram.
*               ----        Argument checked in NetUDP_Rx().
*
*               src_addr    Received UDP datagram's source      IP  address.
*
*               src_port    Received UDP datagram's source      UDP port.
*
*               dest_addr   Received UDP datagram's destination IP  address.
*
*               dest_port   Received UDP datagram's destination UDP port.
*
*               perr        Pointer to variable that will receive the return error code from this function
*                               (see Note #1b) :
*
*                               NET_APP_ERR_NONE                UDP datagram successfully received to application
*                                                                   connection(s).
*                               NET_ERR_RX                      Receive error; packet discarded.
*                               NET_ERR_RX_DEST                 Invalid destination; no application connection
*                                                                   available for received packet.
*
* Return(s)   : none.
*
* Caller(s)   : NetUDP_RxPktDemuxAppData().
*$PAGE*
* Note(s)     : (1) Application-specific UDP connection handler function is fully responsible for properly
*                   receiving, demultiplexing, & handling all UDP receive packets to the appropriate
*                   application connections.
*
*                   (a) (1) If the application-specific UDP connection handler function successfully
*                           demultiplexes UDP receive packets to an application connection, the handler
*                           function MUST at some point call NetUDP_RxAppData() to deframe the application
*                           data from the packet buffer(s) into an application array as well as copy any
*                           received IP options into an application IP options buffer.
*
*                           (A) The application-specific connection handler function may service the
*                               application data immediately within the handler function, in which case
*                               the application data SHOULD be serviced as quickly as possible since the
*                               network's global lock remains acquired for the full duration of the
*                               network receive.  Thus, no other network receives or transmits can occur
*                               while the application-specific handler function executes.
*
*                           (B) The application-specific connection handler function may delay servicing
*                               the application data by some other application-specific data servicing
*                               function(s), in which case the servicing function(s) MUST :
*
*                               (1) Acquire the network's global lock PRIOR to calling NetUDP_RxAppData()
*                               (2) Release the network's global lock AFTER    calling NetUDP_RxAppData()
*
*                               See 'net_udp.c  NetUDP_RxAppData()  Note #2'.
*
*                       (2) (A) (1) If NetUDP_RxAppDataHandler()          successfully demultiplexes the UDP
*                                   packets, it SHOULD eventually call NetUDP_RxAppData() to deframe the UDP
*                                   packet data.  If NetUDP_RxAppData() successfully deframes the UDP packet
*                                   application data :
*
*                                   (a) NetUDP_RxAppDataHandler() SHOULD return NET_APP_ERR_NONE, regardless
*                                       of whether the application handled the application data; ...
*                                   (b) but MUST NOT call NetUDP_RxPktFree() to free the UDP packet network
*                                       buffer(s) since NetUDP_RxAppData() frees the network buffer(s) [see
*                                       'NetUDP_RxAppData()  Note #1f'].
*
*                               (2) If NetUDP_RxAppDataHandler() does NOT successfully demultiplex   the UDP
*                                   packets :
*
*                                   (a) NetUDP_RxAppDataHandler() SHOULD return NET_ERR_RX_DEST, ...
*                                   (b) but must NOT free or discard the UDP packet network buffer(s) since
*                                       NetUDP_Rx() discards the network buffer(s).
*
*                               (3) If NetUDP_RxAppDataHandler() or NetUDP_RxAppData() fails for any other
*                                   reason(s) :
*
*                                   (a) NetUDP_RxAppDataHandler() SHOULD return NET_ERR_RX, ...
*                                   (b) but MUST call NetUDP_RxPktDiscard() to discard the UDP packet network
*                                       buffer(s).
*
*                           (B) FAILURE to appropriately call NetUDP_RxAppData() or NetUDP_RxPktDiscard() MAY
*                               result in lost network buffer(s).
*
*                   (b) Application-specific UDP connection handler function MUST return one of the following
*                       error codes ONLY [see 'Argument(s) : perr'] :
*
*                       (1) NET_APP_ERR_NONE  for     received UDP packets     destined to an available
*                               application connection & successfully deframed (see Note #1a2A1a)
*
*                       (2) NET_ERR_RX_DEST   for any received UDP packets NOT destined to an available
*                               application connection                         (see Note #1a2A2a)
*
*                       (3) NET_ERR_RX        for any other receive error      (see Note #1a2A3a)
*********************************************************************************************************
*/

#if ((NET_UDP_CFG_APP_API_SEL == NET_UDP_APP_API_SEL_APP     ) || \
     (NET_UDP_CFG_APP_API_SEL == NET_UDP_APP_API_SEL_SOCK_APP))
void  NetUDP_RxAppDataHandler (NET_BUF           *pbuf,
                               NET_IP_ADDR        src_addr,
                               NET_UDP_PORT_NBR   src_port,
                               NET_IP_ADDR        dest_addr,
                               NET_UDP_PORT_NBR   dest_port,
                               NET_ERR           *perr)
{
    /* $$$$ Insert code to demultiplex & handle UDP packets (see Note #1).  */

   *perr = NET_ERR_RX_DEST;
}
#endif

