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
* File Name		: uart.h
* Version 		: 1
* Device 		: RX6xx
* Description 	: header file for UART driver using RX6xx SCI peripheral.
*******************************************************************************/
/*******************************************************************************
* History       : DD.MM.YYYY     Version     Description
*                 14.09.2011     Ver. 1 
*                 17.02.2012                 comment revisions 
*******************************************************************************/
#ifndef UART_H // multiple inclusion guard
#define UART_H

/*******************************************************************************
Macro definitions
*******************************************************************************/
#define NEWLINE_CHAR    '\n'
#define RETURN_CHAR     '\r'

/*******************************************************************************
Exported global functions (to be accessed by other files)
*******************************************************************************/
void     sci_uart_init(void);
void     sci_put_char(uint8_t write_data);
uint8_t  sci_get_char(void);
void     sci_put_string(uint8_t *source_string);
uint32_t sci_read_count_get(void);
void     sci_tx_int_enable(void);
void     sci_rx_int_enable(void);
void     sci_tx_int_disable(void);
void     sci_rx_int_disable(void);

#endif