/******************************************************************************
 * File        : uart.c
 * Project     : ChronosRTOS
 * Module      : UART
 * Group       :
 * Description : UART driver implementation.
 ******************************************************************************/

#include "uart.h"


/*
 * Enable / Disable USART peripheral clock
 */
void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnorDi)
{
    if (EnorDi == 1)
    {
        if (pUSARTx == USART1)
        {
            RCC->APB2ENR |= (1U << 14);
        }

        else if (pUSARTx == USART2)
        {
            RCC->APB1ENR |= (1U << 17);
        }

        else if (pUSARTx == USART3)
        {
            RCC->APB1ENR |= (1U << 18);
        }
    }

    else
    {
        if (pUSARTx == USART1)
        {
            RCC->APB2ENR &= ~(1U << 14);
        }

        else if (pUSARTx == USART2)
        {
            RCC->APB1ENR &= ~(1U << 17);
        }

        else if (pUSARTx == USART3)
        {
            RCC->APB1ENR &= ~(1U << 18);
        }
    }
}


/*
 * Configure USART GPIO pins
 *
 * USART1:
 * PA9  -> TX
 * PA10 -> RX
 */
static void USART1_GPIO_Init(void)
{
    /*
     * Enable GPIOA clock
     *
     * RCC_APB2ENR bit 2 = IOPAEN
     */
    RCC->APB2ENR |= (1U << 2);


    /*
     * PA9 configuration
     *
     * PA9 is in CRH.
     *
     * PA9 uses CRH bits [7:4].
     *
     * 1011 = Alternate Function Push-Pull, 50 MHz
     */
    GPIOA->CRH &= ~(0xFU << 4);
    GPIOA->CRH |=  (0xBU << 4);


    /*
     * PA10 configuration
     *
     * PA10 is in CRH.
     *
     * PA10 uses CRH bits [11:8].
     *
     * 0100 = Floating input
     */
    GPIOA->CRH &= ~(0xFU << 8);
    GPIOA->CRH |=  (0x4U << 8);
}


/*
 * Configure baud rate
 */
void USART_SetBaudRate(USART_TypeDef *pUSARTx,
                       uint32_t BaudRate)
{
    uint32_t PCLK;
    uint32_t USARTDIV;
    uint32_t Mantissa;
    uint32_t Fraction;


    /*
     * USART1 is connected to APB2.
     * USART2 and USART3 are connected to APB1.
     *
     * Assuming the standard STM32F103 clock setup:
     *
     * SYSCLK = 72 MHz
     * APB2   = 72 MHz
     * APB1   = 36 MHz
     */
    if (pUSARTx == USART1)
    {
        PCLK = 72000000U;
    }
    else
    {
        PCLK = 36000000U;
    }


    /*
     * USARTDIV = PCLK / (16 × BaudRate)
     *
     * USART oversampling by 16 is being used.
     */
    USARTDIV = (PCLK + (8U * BaudRate))
             / (16U * BaudRate);


    /*
     * The above gives USARTDIV in 16.4 fixed-point form.
     *
     * Mantissa = USARTDIV / 16
     * Fraction = USARTDIV % 16
     */
    Mantissa = USARTDIV / 16U;
    Fraction = USARTDIV % 16U;


    /*
     * Write BRR
     */
    pUSARTx->BRR = (Mantissa << 4) | Fraction;
}


/*
 * USART initialization
 */
void USART_Init(USART_Handle_t *pUSARTHandle)
{
    uint32_t tempreg = 0;


    /*
     * ------------------------------------------------
     * 1. Enable USART peripheral clock
     * ------------------------------------------------
     */
    USART_PeriClockControl(
        pUSARTHandle->pUSARTx,
        1
    );


    /*
     * ------------------------------------------------
     * 2. Configure GPIO pins
     * ------------------------------------------------
     *
     * USART1:
     * PA9  -> TX
     * PA10 -> RX
     */
    if (pUSARTHandle->pUSARTx == USART1)
    {
        USART1_GPIO_Init();
    }


    /*
     * ------------------------------------------------
     * 3. Configure CR1
     * ------------------------------------------------
     */


    /*
     * Word length
     *
     * CR1 bit 12 = M
     *
     * 0 -> 8 data bits
     * 1 -> 9 data bits
     */
    tempreg |=
        (pUSARTHandle->USART_Config.USART_WordLength << 12);


    /*
     * Parity configuration
     */
    if (pUSARTHandle->USART_Config.USART_ParityControl
        == USART_PARITY_EN_EVEN)
    {
        /*
         * PCE = 1
         * PS  = 0
         */
        tempreg |= (1U << 10);
    }

    else if (pUSARTHandle->USART_Config.USART_ParityControl
             == USART_PARITY_EN_ODD)
    {
        /*
         * PCE = 1
         * PS = 1
         */
        tempreg |= (1U << 10);
        tempreg |= (1U << 9);
    }


    /*
     * Transmitter enable
     *
     * CR1 bit 3 = TE
     */
    if ((pUSARTHandle->USART_Config.USART_Mode
         == USART_MODE_ONLY_TX) ||
        (pUSARTHandle->USART_Config.USART_Mode
         == USART_MODE_TXRX))
    {
        tempreg |= (1U << 3);
    }


    /*
     * Receiver enable
     *
     * CR1 bit 2 = RE
     */
    if ((pUSARTHandle->USART_Config.USART_Mode
         == USART_MODE_ONLY_RX) ||
        (pUSARTHandle->USART_Config.USART_Mode
         == USART_MODE_TXRX))
    {
        tempreg |= (1U << 2);
    }


    /*
     * Write CR1
     *
     * UE is NOT set yet.
     */
    pUSARTHandle->pUSARTx->CR1 = tempreg;


    /*
     * ------------------------------------------------
     * 4. Configure CR2
     * ------------------------------------------------
     *
     * STOP bits are CR2 bits [13:12]
     */
    tempreg = 0;

    tempreg |=
        (pUSARTHandle->USART_Config.USART_NoOfStopBits << 12);

    pUSARTHandle->pUSARTx->CR2 = tempreg;


    /*
     * ------------------------------------------------
     * 5. Configure CR3
     * ------------------------------------------------
     */
    tempreg = 0;


    /*
     * CTS = bit 9
     * RTS = bit 8
     */
    if (pUSARTHandle->USART_Config.USART_HWFlowControl
        == USART_HW_FLOW_CTRL_CTS)
    {
        tempreg |= (1U << 9);
    }

    else if (pUSARTHandle->USART_Config.USART_HWFlowControl
             == USART_HW_FLOW_CTRL_RTS)
    {
        tempreg |= (1U << 8);
    }

    else if (pUSARTHandle->USART_Config.USART_HWFlowControl
             == USART_HW_FLOW_CTRL_CTS_RTS)
    {
        tempreg |= (1U << 9);
        tempreg |= (1U << 8);
    }

    pUSARTHandle->pUSARTx->CR3 = tempreg;


    /*
     * ------------------------------------------------
     * 6. Configure baud rate
     * ------------------------------------------------
     */
    USART_SetBaudRate(
        pUSARTHandle->pUSARTx,
        pUSARTHandle->USART_Config.USART_Baud
    );


    /*
     * ------------------------------------------------
     * 7. Finally enable USART
     * ------------------------------------------------
     *
     * CR1 bit 13 = UE
     */
    pUSARTHandle->pUSARTx->CR1 |= (1U << 13);
}


/*
 * Send one character
 */
void USART_SendChar(USART_TypeDef *pUSARTx,
                    char data)
{
    /*
     * Wait until TX data register is empty.
     *
     * SR bit 7 = TXE
     */
    while (!(pUSARTx->SR & (1U << 7)))
    {
    }


    /*
     * Put character into data register.
     */
    pUSARTx->DR = (uint8_t)data;


    /*
     * Wait until complete frame has been transmitted.
     *
     * SR bit 6 = TC
     */
    while (!(pUSARTx->SR & (1U << 6)))
    {
    }
}


/*
 * Send a string
 */
void USART_SendString(USART_TypeDef *pUSARTx,
                      char *str)
{
    while (*str != '\0')
    {
        USART_SendChar(pUSARTx, *str);

        str++;
    }
}


/*
 * Receive one character
 */
uint8_t USART_ReceiveChar(USART_TypeDef *pUSARTx)
{
    /*
     * Wait until data is received.
     *
     * SR bit 5 = RXNE
     */
    while (!(pUSARTx->SR & (1U << 5)))
    {
    }


    /*
     * Read received data
     */
    return (uint8_t)pUSARTx->DR;
}


/*
 * Send multiple bytes
 */
void USART_SendData(USART_TypeDef *pUSARTx,
                    uint8_t *pTxBuffer,
                    uint32_t Len)
{
    uint8_t data;


    while (Len > 0)
    {
        /*
         * Wait until transmit data register is empty.
         */
        while (!(pUSARTx->SR & (1U << 7)))
        {
        }


        /*
         * Read the byte to transmit.
         */
        data = *pTxBuffer;


        /*
         * Send data.
         */
        pUSARTx->DR = data;


        /*
         * Move to next byte.
         */
        pTxBuffer++;

        Len--;
    }


    /*
     * Wait until complete transmission.
     */
    while (!(pUSARTx->SR & (1U << 6)))
    {
    }
}


/*
 * Receive multiple bytes
 */
void USART_ReceiveData(USART_TypeDef *pUSARTx,
                       uint8_t *pRxBuffer,
                       uint32_t Len)
{
    while (Len > 0)
    {
        /*
         * Wait until receive data is available.
         */
        while (!(pUSARTx->SR & (1U << 5)))
        {
        }


        /*
         * Read received byte.
         */
        *pRxBuffer =
            (uint8_t)pUSARTx->DR;


        /*
         * Move to next buffer location.
         */
        pRxBuffer++;

        Len--;
    }
}


/*
 * USART de-initialization
 */
void USART_DeInit(USART_TypeDef *pUSARTx)
{
    if (pUSARTx == USART1)
    {
        /*
         * APB2RSTR bit 14
         */
        RCC->APB2RSTR |= (1U << 14);
        RCC->APB2RSTR &= ~(1U << 14);
    }

    else if (pUSARTx == USART2)
    {
        /*
         * APB1RSTR bit 17
         */
        RCC->APB1RSTR |= (1U << 17);
        RCC->APB1RSTR &= ~(1U << 17);
    }

    else if (pUSARTx == USART3)
    {
        /*
         * APB1RSTR bit 18
         */
        RCC->APB1RSTR |= (1U << 18);
        RCC->APB1RSTR &= ~(1U << 18);
    }
}
