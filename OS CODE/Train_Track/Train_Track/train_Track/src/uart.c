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
* File Name		: uart.c
* Version 		: 1
* Device 		: RX63N
* H/W Platform	: YRDKRX63N specific
* Description 	: Example UART driver using RX63N SCI peripheral.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*           09.14.2011     Ver. 1      first 
*           22.02.2012                 ported to RDKRX63N.            
*******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <machine.h>

#include "platform.h"
#include "uart.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define TX_BUF_LEN 128
#define RX_BUF_LEN 128
#define BAUDRATE   115200 /* example other choices: 9600, 19200, 38400 */

#define SCI2_INT_LVL 3

/******************************************************************************
Private global variables and functions
******************************************************************************/
volatile uint8_t g_tx_buffer[TX_BUF_LEN];
volatile uint8_t g_rx_buffer[RX_BUF_LEN];

volatile uint32_t g_tx_count     = 0;
volatile uint32_t g_tx_in_index  = 0;
volatile uint32_t g_tx_out_index = 0;

/* Keeps track of un-processed data count remaining in read buffer. */
volatile uint32_t g_rx_count     = 0;
/* Read buffer position at which incoming data from UART will be stored. */ 
volatile uint32_t g_rx_in_index  = 0; 
/* Read buffer position from which data has been processed. */
volatile uint32_t g_rx_out_index = 0;
/* A flag that indicates TX buffer status. */
volatile bool g_uart_tx_ready    = false; 

/******************************************************************************
* Function Name	: sci_uart_init
* Description	: Initialize SCI2 as asynchronous UART
* Arguments     : none
* Return value	: none
*******************************************************************************/
void sci_uart_init(void)
{
	uint16_t i; /* for delay counter */


    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA50B; /* Protect off */
    #endif
    
	/* clear ACSE Bit (All-Module Clock Stop Mode Enable) */	
	SYSTEM.MSTPCRA.BIT.ACSE = 0;  
	  
	/* Cancel stop state of SCI2 Peripheral to enable writing to it*/	
    MSTP(SCI2) = 0;	
    
    #ifdef PLATFORM_BOARD_RDKRX63N
	SYSTEM.PRCR.WORD = 0xA500; /* Protect on  */
    #endif     
    
    /* Clear bits TIE, RIE, RE, and TEIE in SCR to 0. Set CKE to internal. */
	SCI2.SCR.BYTE = 0x00;

    /* Set up the UART I/O port and pins. */
    MPC.P50PFS.BYTE  = 0x4A; /* P50 is TxD2 */
    MPC.P52PFS.BYTE  = 0x4A; /* P52 is RxD2 */
	
    PORT5.PDR.BIT.B0 = 1;    /* TxD2 is output. */
    PORT5.PDR.BIT.B2 = 0;    /* RxD2 is input. */
	
    PORT5.PMR.BIT.B0 = 1;    /* TxD2 is peripheral. */
    PORT5.PMR.BIT.B2 = 1;    /* RxD2 is peripheral. */

	/* Set data transfer format in Serial Mode Register (SMR)*/ 
	/*  -Asynchronous Mode`
	    -8 bits
	    -no parity
	    -1 stop bit
	    -PCLK clock (n = 0) */
	SCI2.SMR.BYTE = 0x00;
    
    SCI2.SCMR.BIT.SMIF = 0; /* Set to 0 for serial communications interface mode. */
    
	/* Set bit rate register.
       Example: set to 115200 baud
	        N = (PCLK Frequency) / (64 * 2^(2*n - 1) * Bit Rate) - 1
	        N = (48,000,000)     / (64 * 2^(2*0 - 1) * 115200)   - 1
	        N = 12 */
	SCI2.BRR = 48000000 / ((64/2) * BAUDRATE) - 1; /* SCI2.BRR = 12; 12 is for 115200 */

	/* Wait at least one bit interval */
    /* TODO: check this for sufficient delay */
	for (i = 0; i < 200; i++) /* assume minimum of 2 instructions at 98MHz? */
	{
	}
	    
    /* Enable RXI and TXI interrupts in SCI peripheral */
	SCI2.SCR.BIT.RIE  = 1;   /* Set Receive Interrupt (RX buffer full) enable. */
	SCI2.SCR.BIT.TIE  = 1;   /* Set Transmit Interrupt (TX data register empty) enable. */
    SCI2.SCR.BIT.TEIE = 1;   /* Set Transmit End interrupt enable */
	
    /* Clear IR bits for TIE and RIE */
    IR(SCI2, RXI2) = 0;
    IR(SCI2, TXI2) = 0;
    
    /* RXI and TXI interrupts in ICU. */
    /* Designer option--handle ICU interrupt enables at application level */
    /* IEN(SCI2, RXI2) = 1; */
    /* IEN(SCI2, TXI2) = 1; */
    
	/* Set interrupt prio for SCI. */
	IPR(SCI2, TXI2) = SCI2_INT_LVL;
	IPR(SCI2, RXI2) = SCI2_INT_LVL;

	/* Enable Tx/Rx */
    /* Note: A 1 can only be written to these bits when TE = 0 and RE = 0. */
    /* After setting TE or RE to 1, only 0 can be written in TE and RE.    */
    /* So to enable both, write them simultaneously.                       */
	SCI2.SCR.BYTE |= 0x30;
    
    /* Initialize buffered I/O variables.  */
    g_tx_out_index = 0; 
    g_tx_in_index  = 0; 
    g_tx_count     = 0;
    g_rx_out_index = 0; 
    g_rx_in_index  = 0; 
    g_rx_count     = 0;   
     	
} /* End of function sci_uart_init() */


/******************************************************************************
* Function Name	: sci_put_char
* Description	: Writes a byte to the serial port. Uses buffered output for  
*               : low latency. If there is room in the buffer the data gets  
*               : copied in and the function returns immediately. The accumulated  
*               : data in the buffer will be written out by the TX interrupt  
*               : handler. If the buffer is full then this function blocks until  
*               : space is available. Adjust baud rate and/or buffer size as 
*               : required by the application to minimize waiting time.
*
* Arguments     : write_data -
*                     The byte to be written
* Return value	: None
*******************************************************************************/
void sci_put_char(uint8_t write_data)
{
    /* Enable transmit interrupts in ICU */
    IEN(SCI2, TXI2) = 1;    /* Expands to: ICU.IER1A.BIT.IEN7 = 1; */
    
    /* Wait for room in the write buffer. Normally the buffer would be large enough */
    /* that this does not block. Adjust buffer size or baudrate if the buffer gets  */
    /* full too frequently.                                                         */   
    while (g_tx_count >= TX_BUF_LEN) /* TX ISR will make room as it sends out data */ 
    {
    }
   
    /* Disable transmit interrupts to avoid access conflict of globals */
    IEN(SCI2, TXI2) = 0;   

    /* No data being sent right now so put this character right into the UART */
    /* and enable TX interrupts                                               */
    if (g_uart_tx_ready)
    {
        /* TODO: Peripheral interrupt enables already enabled in init function. */
        /*       Do interrupts need to be re-enabled with every write? */
        SCI2.SCR.BIT.TEIE = 1;          /* Enable transmit complete IRQ in peripheral. */  
        SCI2.SCR.BIT.TIE  = 1;          /* Enable transmit IRQ in peripheral. */

        SCI2.TDR          = write_data;
        g_uart_tx_ready   = false;      /* Clear ready flag to indicate busy status. */
    }
    else      /* Characters already being sent, queue this one in buffer. */
    {    
        g_tx_buffer[g_tx_in_index] = write_data; /* Put character in outbound buffer */     
        g_tx_in_index++;
      
        /* Treat transmit buffer as circular buffer */ 
        if (g_tx_in_index >= TX_BUF_LEN)
        {
            g_tx_in_index  = 0; /* Wrap back around to start of buffer */
        }
        g_tx_count++;
    }

    IEN(SCI2, TXI2) = 1; /* Re-enable TX buffer empty interrupt. */
    IEN(SCI2, TEI2) = 1; /* Transmit complete interrupt. */
    
} /* End of function sci_put_char() */


/******************************************************************************
* Function Name	: sci_get_char
* Description	: Transfers a byte from receive buffer that has been filled by the 
*               : serial port receive interrupt handler. 
* Arguments     : None
* Return value	: read_data -
*                     the byte read
*******************************************************************************/
uint8_t sci_get_char(void)
{
    uint8_t read_data = 0;

    if (g_rx_count > 0)
    {
        /* Disable RX interrupts to avoid access conflict of globals. */ 
        while (IEN(SCI2, RXI2) != 0)    /* Loop if necessary to handle latency */
        {
            IEN(SCI2, RXI2) = 0;              
        }         
        
        read_data = g_rx_buffer[g_rx_out_index]; 
        g_rx_out_index++;
    
        /* Treat read buffer as circular */
        if(g_rx_out_index >= RX_BUF_LEN) 
        {
	        g_rx_out_index = 0;     /* Reached end of buffer. Wrap back to start */
        }
   
        g_rx_count-- ;  
        IEN(SCI2, RXI2) = 1;        /* Re-enable RX interrupts */                     
    }
    
    return (read_data);
    
} /* End of function sci_get_char() */


/******************************************************************************
* Function Name	: sci_put_string
* Description	: Outputs a null terminated string from the serial port
* Arguments     : source_string -
*                     Pointer to the null terminated string to be sent
* Return value	: None
*******************************************************************************/
void sci_put_string(uint8_t *source_string)
{
    do
    {
        sci_put_char(*source_string);
        source_string++;
    } while (0 != *source_string);
   
    sci_put_char(*source_string);         /* write the null terminator */
} /* End of function sci_put_string() */


/******************************************************************************
* Function Name	: sci_read_count_get
* Description	: Gets the current count of unprocessed data in the read buffer.
* Arguments     : None
* Return value	: The byte count.
*******************************************************************************/
uint32_t sci_read_count_get(void)
{
    uint32_t count;
    
    /* Disable RX interrupts to avoid access conflict to globals. */
    IEN(SCI2, RXI2) = 0; 
    
    if(IEN(SCI2, RXI2))
    {
        /* Dummy read to handle I/O write latency. */         
    } 
    
    count           = g_rx_count;
    IEN(SCI2, RXI2) = 1;         /* Re-enable RX interrupts */
    return count;
} /* End of function sci_read_count_get() */


/******************************************************************************
* Function Name	: sci_tx_int_enable
* Description	: Enables handling of TXI interrupts from SCI channel 2.
* Arguments     : None
* Return value	: None
*******************************************************************************/
void sci_tx_int_enable(void)
{
    IEN(SCI2, TXI2) = 1;
} /* End of function sci_tx_int_enable() */


/******************************************************************************
* Function Name	: sci_rx_int_enable
* Description	: Enables handling of RXI interrupts from SCI channel 2.
* Arguments     : None
* Return value	: None
*******************************************************************************/
void sci_rx_int_enable(void)
{
    IEN(SCI2, RXI2) = 1;
} /* End of function sci_rx_int_enable() */


/******************************************************************************
* Function Name	: sci_tx_int_disable
* Description	: Disables handling of TXI interrupts from SCI channel 2.
* Arguments     : None
* Return value	: None
*******************************************************************************/
void sci_tx_int_disable(void)
{
    IEN(SCI2, TXI2) = 0;   

    if(IEN(SCI2, TXI2)) /* Wait for pipelining. See manual about writing I/O registers. */
    {
    }        
} /* End of function sci_tx_int_disable() */


/******************************************************************************
* Function Name	: sci_rx_int_disable
* Description	: Disables handling of RXI interrupts from SCI channel 2.
* Arguments     : None
* Return value	: None
*******************************************************************************/
void sci_rx_int_disable(void)
{
    IEN(SCI2, RXI2) = 0;
	
    if(IEN(SCI2, RXI2)) /* Wait for pipelining. See manual about writing I/O registers. */
    {
    }      
} /* End of function sci_rx_int_disable() */

/******************************************************************************
* Function Name	: SCI2_TXI2_isr
* Description	: SCI2 transmit data buffer empty interrupt service routine
* Arguments     : none
* Return value	: none
*******************************************************************************/
#pragma interrupt SCI2_TXI2_isr(vect = VECT_SCI2_TXI2, enable)
static void SCI2_TXI2_isr(void)
{
    if (g_tx_count)
    {
        g_uart_tx_ready = false;       /* Clear ready flag to indicate busy status. */
        SCI2.TDR        = g_tx_buffer[g_tx_out_index];     
        g_tx_out_index++;
               
        /* Treat transmit buffer as circular buffer */
        if (g_tx_out_index >= TX_BUF_LEN) 
        {
            g_tx_out_index  = 0;
        }
        
        g_tx_count--;   /* Decrement the count of number of bytes to be written. */
    }
    else    /* Nothing to transmit. */
    {       
        SCI2.SCR.BIT.TIE = 0;    /* TODO: is this necessary? */
        IEN(SCI2, TXI2)  = 0;    /* Disable transmit interrupts in ICU. */
	    g_uart_tx_ready  = true; /* Flag that the TX buffer is available. */
    }
           
} /* End of SCI2_TXI2_isr */


/******************************************************************************
* Function Name	: SCI2_RXI2_isr
* Description	: SCI2 receive data buffer full interrupt service routine
* Arguments     : none
* Return value	: none
*******************************************************************************/
#pragma interrupt SCI2_RXI2_isr(vect = VECT_SCI2_RXI2, enable)
static void SCI2_RXI2_isr(void)
{
    uint8_t read_byte;

    read_byte = SCI2.RDR;      /* Read a byte from the UART data register. */
	
    while(SCI2.SSR.BIT.ORER)   /* Check for overrun error. */
	{
		SCI2.SSR.BIT.ORER = 0; /* Clear overrun error. Repeats until condition cleared. */
                               /* TODO user: Error could be reported here if desired. */
	}
    
    if (g_rx_in_index < RX_BUF_LEN)
    {
        g_rx_buffer[g_rx_in_index] = read_byte;             
    }
    else     /* Treat read buffer as a circular buffer. Point back to beginning if full. */
    {
        g_rx_in_index = 0;
        g_rx_buffer[g_rx_in_index] = read_byte;             
    }
    
    g_rx_in_index++;    
    g_rx_count++;    /* Increment unprocessed data count. Gets decremented when processed. */
    
} /* End of SCI2_RXI2_isr */


/******************************************************************************
* Function Name	: SCI2_TEI2_isr
* Description	: SCI2 Transmit Completed interrupt service routine
* Arguments     : none
* Return value	: none
*******************************************************************************/
#pragma interrupt SCI2_TEI2_isr(vect = VECT(SCI2, TEI2))
static void SCI2_TEI2_isr(void)
{ 
    SCI2.SCR.BIT.TEIE = 0;          /* TODO: does this need to be done here? */
    IEN(SCI2, TEI2)   = 0;          /* Disable transmit ended interrupts in ICU. */
    g_uart_tx_ready   = true;       /* Flag that the TX buffer is available */
    
} /* End of SCI2_TEI2_isr */


/* end of file uart.c */

