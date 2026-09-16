/******************************************************************************
 * File        : uart.h
 * Project     : ChronosRTOS
 * Module      : UART
 * Group       :
 * Description : UART driver interface.
 ******************************************************************************/

/*#ifndef UART_H
#define UART_H



#endif /* UART_H */

#ifndef UART_H
#define UART_H

#include <stm32f10x.h>
#include <stdint.h>

/*
 * USART operating modes
 */
#define USART_MODE_ONLY_TX    0
#define USART_MODE_ONLY_RX    1
#define USART_MODE_TXRX       2


/*
 * Standard baud rates
 */
#define USART_STD_BAUD_1200      1200
#define USART_STD_BAUD_2400      2400
#define USART_STD_BAUD_9600      9600
#define USART_STD_BAUD_19200     19200
#define USART_STD_BAUD_38400     38400
#define USART_STD_BAUD_57600     57600
#define USART_STD_BAUD_115200    115200


/*
 * Word length
 */
#define USART_WORDLEN_8BITS      0
#define USART_WORDLEN_9BITS      1


/*
 * Parity
 */
#define USART_PARITY_DISABLE     0
#define USART_PARITY_EN_EVEN     1
#define USART_PARITY_EN_ODD      2


/*
 * Stop bits
 */
#define USART_STOPBITS_1         0
#define USART_STOPBITS_0_5       1
#define USART_STOPBITS_2         2
#define USART_STOPBITS_1_5       3


/*
 * Hardware flow control
 */
#define USART_HW_FLOW_CTRL_NONE     0
#define USART_HW_FLOW_CTRL_CTS      1
#define USART_HW_FLOW_CTRL_RTS      2
#define USART_HW_FLOW_CTRL_CTS_RTS  3


/*
 * USART configuration structure
 */
typedef struct
{
    uint8_t  USART_Mode;
    uint32_t USART_Baud;
    uint8_t  USART_NoOfStopBits;
    uint8_t  USART_WordLength;
    uint8_t  USART_ParityControl;
    uint8_t  USART_HWFlowControl;

} USART_Config_t;


/*
 * USART handle
 */
typedef struct
{
    USART_TypeDef *pUSARTx;
    USART_Config_t USART_Config;

} USART_Handle_t;


/*
 * Peripheral clock control
 */
void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnorDi);


/*
 * USART initialization
 */
void USART_Init(USART_Handle_t *pUSARTHandle);


/*
 * USART de-initialization
 */
void USART_DeInit(USART_TypeDef *pUSARTx);


/*
 * Polling transmission/reception
 */
void USART_SendData(USART_TypeDef *pUSARTx,
                    uint8_t *pTxBuffer,
                    uint32_t Len);

void USART_ReceiveData(USART_TypeDef *pUSARTx,
                       uint8_t *pRxBuffer,
                       uint32_t Len);


/*
 * Simple character/string functions
 */
void USART_SendChar(USART_TypeDef *pUSARTx, char data);

void USART_SendString(USART_TypeDef *pUSARTx, char *str);

uint8_t USART_ReceiveChar(USART_TypeDef *pUSARTx);


/*
 * Baud-rate configuration
 */
void USART_SetBaudRate(USART_TypeDef *pUSARTx,
                       uint32_t BaudRate);

#endif
