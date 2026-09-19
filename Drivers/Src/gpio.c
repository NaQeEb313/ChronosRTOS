/******************************************************************************
 * File        : gpio.c
 * Project     : ChronosRTOS
 * Module      : GPIO
 * Description : GPIO driver implementation.
 ******************************************************************************/

#include "gpio.h"

/* =========================================================
 * GPIO Clock Control
 *
 * GPIOA -> APB2ENR bit 2
 * GPIOB -> APB2ENR bit 3
 * GPIOC -> APB2ENR bit 4
 * ========================================================= */

void GPIO_ClockControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi) {
  if (EnorDi == ENABLE) {
    if (pGPIOx == GPIOA) {
      RCC->APB2ENR |= (1U << 2);
    } else if (pGPIOx == GPIOB) {
      RCC->APB2ENR |= (1U << 3);
    } else if (pGPIOx == GPIOC) {
      RCC->APB2ENR |= (1U << 4);
    }
  } else {
    if (pGPIOx == GPIOA) {
      RCC->APB2ENR &= ~(1U << 2);
    } else if (pGPIOx == GPIOB) {
      RCC->APB2ENR &= ~(1U << 3);
    } else if (pGPIOx == GPIOC) {
      RCC->APB2ENR &= ~(1U << 4);
    }
  }
}

/* =========================================================
 * Generic GPIO Initialization
 *
 * STM32F103:
 *
 * Pins 0-7  -> CRL
 * Pins 8-15 -> CRH
 *
 * Each pin uses 4 configuration bits:
 *
 * CNF[1:0] + MODE[1:0]
 * ========================================================= */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle) {
  uint32_t config = 0U;

  uint8_t pin = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
  uint8_t mode = pGPIOHandle->GPIO_PinConfig.GPIO_Mode;
  uint8_t cnf = pGPIOHandle->GPIO_PinConfig.GPIO_CNF;

  volatile uint32_t *configReg;
  uint8_t position;

  /* Enable peripheral clock before accessing GPIO registers. */
  GPIO_ClockControl(pGPIOHandle->pGPIOx, ENABLE);

  /* Select CRL or CRH. */
  if (pin < 8U) {
    configReg = &pGPIOHandle->pGPIOx->CRL;
    position = pin * 4U;
  } else {
    configReg = &pGPIOHandle->pGPIOx->CRH;
    position = (pin - 8U) * 4U;
  }

  /* =====================================================
   * INPUT MODE
   * ===================================================== */

  if (mode == GPIO_MODE_INPUT) {

    /* MODE = 00 */

    if (cnf == GPIO_CNF_INPUT_ANALOG) {
      /* CNF = 00, MODE = 00 */
      config = 0x0U;

    } else if (cnf == GPIO_CNF_INPUT_FLOATING) {
      /* CNF = 01, MODE = 00 */
      config = (1U << 2);

    } else if (cnf == GPIO_CNF_INPUT_PU_PD) {
      /* CNF = 10, MODE = 00 */
      config = (2U << 2);

    } else {
      config = 0x0U;
    }
  }

  /* =====================================================
   * OUTPUT MODE
   * ===================================================== */

  else {

    if (cnf == GPIO_CNF_OUTPUT_PP) {
      /* CNF = 00 */
      config = mode;

    } else if (cnf == GPIO_CNF_OUTPUT_OD) {
      /* CNF = 01 */
      config = (1U << 2) | mode;

    } else if (cnf == GPIO_CNF_AF_OUTPUT_PP) {
      /* CNF = 10 */
      config = (2U << 2) | mode;

    } else if (cnf == GPIO_CNF_AF_OUTPUT_OD) {
      /* CNF = 11 */
      config = (3U << 2) | mode;

    } else {
      config = 0x0U;
    }
  }

  /* Clear existing 4-bit configuration. */
  *configReg &= ~(0xFU << position);

  /* Write new configuration. */
  *configReg |= (config << position);
}

/* =========================================================
 * GPIO Read
 * ========================================================= */

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber) {
  return (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x01U);
}

/* =========================================================
 * GPIO Write
 * ========================================================= */

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t Value) {
  if (Value == 1U) {
    pGPIOx->BSRR = (1U << PinNumber);
  } else {
    pGPIOx->BRR = (1U << PinNumber);
  }
}

/* =========================================================
 * GPIO Toggle
 * ========================================================= */

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber) {
  pGPIOx->ODR ^= (1U << PinNumber);
}

/* =========================================================
 * GPIO De-Initialization
 * ========================================================= */

void GPIO_DeInit(GPIO_TypeDef *pGPIOx) {
  if (pGPIOx == GPIOA) {
    RCC->APB2RSTR |= (1U << 2);
    RCC->APB2RSTR &= ~(1U << 2);

  } else if (pGPIOx == GPIOB) {
    RCC->APB2RSTR |= (1U << 3);
    RCC->APB2RSTR &= ~(1U << 3);

  } else if (pGPIOx == GPIOC) {
    RCC->APB2RSTR |= (1U << 4);
    RCC->APB2RSTR &= ~(1U << 4);
  }
}

/* =========================================================
 * ChronosRTOS PC13 LED Driver
 *
 * STM32F103C8T6 Blue Pill:
 *
 * PC13 LOW  -> LED ON
 * PC13 HIGH -> LED OFF
 * ========================================================= */

/* =========================================================
 * GPIO_INIT
 *
 * Configure PC13 as:
 *
 * General-purpose output
 * Push-pull
 * 2 MHz
 *
 * LED starts OFF.
 * ========================================================= */

void GPIO_INIT(void) {
  GPIO_Handle_t GPIOC_Handle;

  GPIOC_Handle.pGPIOx = GPIOC;

  /*
   * GPIO_PinNumber represents the actual pin number,
   * not the STM32 HAL bit-mask.
   */
  GPIOC_Handle.GPIO_PinConfig.GPIO_PinNumber = 13;

  GPIOC_Handle.GPIO_PinConfig.GPIO_Mode = GPIO_MODE_OUT_2MHz;

  GPIOC_Handle.GPIO_PinConfig.GPIO_CNF = GPIO_CNF_OUTPUT_PP;

  GPIO_Init(&GPIOC_Handle);

  /* LED OFF: PC13 HIGH */
  GPIOC->BSRR = (1U << 13);
}

/* =========================================================
 * GPIO_ON
 *
 * PC13 LOW -> LED ON
 * ========================================================= */

void GPIO_ON(void) { GPIOC->BRR = (1U << 13); }

/* =========================================================
 * GPIO_OFF
 *
 * PC13 HIGH -> LED OFF
 * ========================================================= */

void GPIO_OFF(void) { GPIOC->BSRR = (1U << 13); }

/* =========================================================
 * GPIO_TOGGLE
 * ========================================================= */

void GPIO_TOGGLE(void) { GPIOC->ODR ^= (1U << 13); }

/* =========================================================
 * GPIO_READ
 *
 * Returns:
 *
 * 0 -> PC13 LOW
 * 1 -> PC13 HIGH
 * ========================================================= */

uint8_t GPIO_READ(void) { return (uint8_t)((GPIOC->IDR >> 13) & 0x01U); }