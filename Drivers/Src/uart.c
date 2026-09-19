/******************************************************************************
 * File        : uart.c
 * Project     : ChronosRTOS
 * Module      : UART
 * Group       :
 * Description : UART driver implementation.
 ******************************************************************************/

#include "uart.h"

/*
 * =========================================================
 * USART Peripheral Clock Control
 * =========================================================
 */

void USART_PeriClockControl(USART_TypeDef *pUSARTx, uint8_t EnorDi) {
  if (EnorDi == 1U) {
    if (pUSARTx == USART1) {
      /* APB2ENR bit 14 */
      RCC->APB2ENR |= (1U << 14);
    } else if (pUSARTx == USART2) {
      /* APB1ENR bit 17 */
      RCC->APB1ENR |= (1U << 17);
    } else if (pUSARTx == USART3) {
      /* APB1ENR bit 18 */
      RCC->APB1ENR |= (1U << 18);
    }
  } else {
    if (pUSARTx == USART1) {
      RCC->APB2ENR &= ~(1U << 14);
    } else if (pUSARTx == USART2) {
      RCC->APB1ENR &= ~(1U << 17);
    } else if (pUSARTx == USART3) {
      RCC->APB1ENR &= ~(1U << 18);
    }
  }
}

/*
 * =========================================================
 * USART1 GPIO Initialization
 *
 * USART1:
 * PA9  -> TX
 * PA10 -> RX
 * =========================================================
 */

static void USART1_GPIO_Init(void) {
  /*
   * Enable GPIOA clock.
   *
   * APB2ENR bit 2 = IOPAEN
   */
  RCC->APB2ENR |= (1U << 2);

  /*
   * PA9 -> USART1_TX
   *
   * PA9 is in CRH.
   * PA9 uses bits [7:4].
   *
   * 1011:
   *
   * MODE = 11 -> Output 50 MHz
   * CNF  = 10 -> Alternate function push-pull
   */
  GPIOA->CRH &= ~(0xFU << 4);
  GPIOA->CRH |= (0xBU << 4);

  /*
   * PA10 -> USART1_RX
   *
   * PA10 is in CRH.
   * PA10 uses bits [11:8].
   *
   * 0100:
   *
   * MODE = 00 -> Input
   * CNF  = 01 -> Floating input
   */
  GPIOA->CRH &= ~(0xFU << 8);
  GPIOA->CRH |= (0x4U << 8);
}

/*
 * =========================================================
 * USART Baud Rate Configuration
 * =========================================================
 *
 * Current ChronosRTOS clock configuration:
 *
 * SYSCLK = 72 MHz
 * APB2   = 72 MHz
 * APB1   = 36 MHz
 *
 * USART1 -> APB2
 * USART2 -> APB1
 * USART3 -> APB1
 *
 * Oversampling by 16 is used.
 * =========================================================
 */

void USART_SetBaudRate(USART_TypeDef *pUSARTx, uint32_t BaudRate) {
  uint32_t PCLK;
  uint32_t USARTDIV_MUL100;
  uint32_t Mantissa;
  uint32_t Fraction;

  /*
   * Determine peripheral clock.
   */
  if (pUSARTx == USART1) {
    PCLK = 72000000U;
  } else {
    PCLK = 36000000U;
  }

  /*
   * USARTDIV = PCLK / (16 * BaudRate)
   *
   * Calculate USARTDIV * 100 so that
   * the fractional portion is preserved.
   */
  USARTDIV_MUL100 = (PCLK * 100U) / (16U * BaudRate);

  /*
   * Integer portion.
   */
  Mantissa = USARTDIV_MUL100 / 100U;

  /*
   * Fractional portion.
   *
   * BRR has a 4-bit fraction field.
   */
  Fraction = ((USARTDIV_MUL100 % 100U) * 16U + 50U) / 100U;

  /*
   * Fraction rounding may produce 16.
   *
   * In that case carry into the mantissa.
   */
  if (Fraction >= 16U) {
    Mantissa++;
    Fraction = 0U;
  }

  /*
   * USART_BRR:
   *
   * Bits [15:4] -> Mantissa
   * Bits [3:0]  -> Fraction
   */
  pUSARTx->BRR = (Mantissa << 4) | Fraction;
}

/*
 * =========================================================
 * USART Initialization
 * =========================================================
 */

void USART_Init(USART_Handle_t *pUSARTHandle) {
  uint32_t tempreg = 0U;

  /*
   * -----------------------------------------------------
   * 1. Enable USART peripheral clock
   * -----------------------------------------------------
   */

  USART_PeriClockControl(pUSARTHandle->pUSARTx, 1U);

  /*
   * -----------------------------------------------------
   * 2. Configure GPIO
   * -----------------------------------------------------
   */

  if (pUSARTHandle->pUSARTx == USART1) {
    USART1_GPIO_Init();
  }

  /*
   * -----------------------------------------------------
   * 3. Configure CR1
   * -----------------------------------------------------
   *
   * CR1:
   *
   * Bit 12 -> M
   * Bit 10 -> PCE
   * Bit 9  -> PS
   * Bit 3  -> TE
   * Bit 2  -> RE
   *
   * UE is enabled later.
   * -----------------------------------------------------
   */

  /*
   * Word length
   *
   * 0 -> 8 bits
   * 1 -> 9 bits
   */
  tempreg |= ((uint32_t)pUSARTHandle->USART_Config.USART_WordLength << 12);

  /*
   * Parity.
   */

  if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN) {
    /*
     * PCE = 1
     * PS  = 0
     */
    tempreg |= (1U << 10);
  } else if (pUSARTHandle->USART_Config.USART_ParityControl ==
             USART_PARITY_EN_ODD) {
    /*
     * PCE = 1
     * PS  = 1
     */
    tempreg |= (1U << 10);
    tempreg |= (1U << 9);
  }

  /*
   * Transmitter enable.
   */

  if ((pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX) ||

      (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)) {
    tempreg |= (1U << 3);
  }

  /*
   * Receiver enable.
   */

  if ((pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX) ||

      (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX)) {
    tempreg |= (1U << 2);
  }

  /*
   * Write CR1.
   *
   * UE remains 0 for now.
   */
  pUSARTHandle->pUSARTx->CR1 = tempreg;

  /*
   * -----------------------------------------------------
   * 4. Configure CR2
   * -----------------------------------------------------
   *
   * STOP bits:
   *
   * CR2 bits [13:12]
   * -----------------------------------------------------
   */

  tempreg = 0U;

  tempreg |= ((uint32_t)pUSARTHandle->USART_Config.USART_NoOfStopBits << 12);

  pUSARTHandle->pUSARTx->CR2 = tempreg;

  /*
   * -----------------------------------------------------
   * 5. Configure CR3
   * -----------------------------------------------------
   *
   * CTS -> bit 9
   * RTS -> bit 8
   * -----------------------------------------------------
   */

  tempreg = 0U;

  if (pUSARTHandle->USART_Config.USART_HWFlowControl ==
      USART_HW_FLOW_CTRL_CTS) {
    tempreg |= (1U << 9);
  } else if (pUSARTHandle->USART_Config.USART_HWFlowControl ==
             USART_HW_FLOW_CTRL_RTS) {
    tempreg |= (1U << 8);
  } else if (pUSARTHandle->USART_Config.USART_HWFlowControl ==
             USART_HW_FLOW_CTRL_CTS_RTS) {
    tempreg |= (1U << 9);
    tempreg |= (1U << 8);
  }

  pUSARTHandle->pUSARTx->CR3 = tempreg;

  /*
   * -----------------------------------------------------
   * 6. Configure baud rate
   * -----------------------------------------------------
   */

  USART_SetBaudRate(pUSARTHandle->pUSARTx,
                    pUSARTHandle->USART_Config.USART_Baud);

  /*
   * -----------------------------------------------------
   * 7. Enable USART
   * -----------------------------------------------------
   *
   * CR1 bit 13 = UE
   * -----------------------------------------------------
   */

  pUSARTHandle->pUSARTx->CR1 |= (1U << 13);
}

/*
 * =========================================================
 * Send One Character
 * =========================================================
 */

void USART_SendChar(USART_TypeDef *pUSARTx, char data) {
  /*
   * Wait until transmit data register is empty.
   *
   * SR bit 7 = TXE
   */
  while (!(pUSARTx->SR & (1U << 7))) {
  }

  /*
   * Write character to data register.
   */
  pUSARTx->DR = (uint8_t)data;

  /*
   * Wait until complete frame has been transmitted.
   *
   * SR bit 6 = TC
   */
  while (!(pUSARTx->SR & (1U << 6))) {
  }
}

/*
 * =========================================================
 * Send String
 * =========================================================
 */

void USART_SendString(USART_TypeDef *pUSARTx, char *str) {
  while (*str != '\0') {
    USART_SendChar(pUSARTx, *str);

    str++;
  }
}

/*
 * =========================================================
 * Receive One Character
 * =========================================================
 */

uint8_t USART_ReceiveChar(USART_TypeDef *pUSARTx) {
  /*
   * Wait until receive data is available.
   *
   * SR bit 5 = RXNE
   */
  while (!(pUSARTx->SR & (1U << 5))) {
  }

  /*
   * Read received data.
   */
  return (uint8_t)pUSARTx->DR;
}

/*
 * =========================================================
 * Send Multiple Bytes
 * =========================================================
 */

void USART_SendData(USART_TypeDef *pUSARTx, uint8_t *pTxBuffer, uint32_t Len) {
  uint8_t data;

  while (Len > 0U) {
    /*
     * Wait until transmit data register is empty.
     */
    while (!(pUSARTx->SR & (1U << 7))) {
    }

    /*
     * Get byte from buffer.
     */
    data = *pTxBuffer;

    /*
     * Send byte.
     */
    pUSARTx->DR = data;

    /*
     * Move to next byte.
     */
    pTxBuffer++;

    Len--;
  }

  /*
   * Wait until final byte has completely
   * left the USART.
   */
  while (!(pUSARTx->SR & (1U << 6))) {
  }
}

/*
 * =========================================================
 * Receive Multiple Bytes
 * =========================================================
 */

void USART_ReceiveData(USART_TypeDef *pUSARTx, uint8_t *pRxBuffer,
                       uint32_t Len) {
  while (Len > 0U) {
    /*
     * Wait for received byte.
     */
    while (!(pUSARTx->SR & (1U << 5))) {
    }

    /*
     * Read received byte.
     */
    *pRxBuffer = (uint8_t)pUSARTx->DR;

    /*
     * Move to next buffer position.
     */
    pRxBuffer++;

    Len--;
  }
}

/*
 * =========================================================
 * USART De-Initialization
 * =========================================================
 */

void USART_DeInit(USART_TypeDef *pUSARTx) {
  if (pUSARTx == USART1) {
    /*
     * APB2RSTR bit 14
     */
    RCC->APB2RSTR |= (1U << 14);
    RCC->APB2RSTR &= ~(1U << 14);
  } else if (pUSARTx == USART2) {
    /*
     * APB1RSTR bit 17
     */
    RCC->APB1RSTR |= (1U << 17);
    RCC->APB1RSTR &= ~(1U << 17);
  } else if (pUSARTx == USART3) {
    /*
     * APB1RSTR bit 18
     */
    RCC->APB1RSTR |= (1U << 18);
    RCC->APB1RSTR &= ~(1U << 18);
  }
}