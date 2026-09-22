/******************************************************************************
 * File        : gpio.h
 * Project     : ChronosRTOS
 * Module      : GPIO
 * Description : GPIO driver interface.
 ******************************************************************************/

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f1xx.h"
#include <stdint.h>

/* =========================================================
 * GPIO Pin numbers
 * ========================================================= */

/* =========================================================
 * Enable / Disable
 * ========================================================= */
#define CHRONOS_GPIO_MODE_INPUT       0x0
#define ENABLE 1U
#define DISABLE 0U

/* =========================================================
 * GPIO Output Modes
 *
 * STM32F103 GPIO MODE field:
 *
 * 00 = Input
 * 01 = Output, max speed 10 MHz
 * 10 = Output, max speed 2 MHz
 * 11 = Output, max speed 50 MHz
 * ========================================================= */

#define GPIO_MODE_OUT_10MHz 0x1U
#define GPIO_MODE_OUT_2MHz 0x2U
#define GPIO_MODE_OUT_50MHz 0x3U

/* =========================================================
 * GPIO Input Configuration
 * ========================================================= */

#define GPIO_CNF_INPUT_ANALOG 0x0U
#define GPIO_CNF_INPUT_FLOATING 0x1U
#define GPIO_CNF_INPUT_PU_PD 0x2U

/* =========================================================
 * GPIO Output Configuration
 * ========================================================= */

#define GPIO_CNF_OUTPUT_PP 0x0U
#define GPIO_CNF_OUTPUT_OD 0x1U
#define GPIO_CNF_AF_OUTPUT_PP 0x2U
#define GPIO_CNF_AF_OUTPUT_OD 0x3U

/* =========================================================
 * GPIO Pin Configuration
 * ========================================================= */

typedef struct {
  uint8_t GPIO_PinNumber;
  uint8_t GPIO_Mode;
  uint8_t GPIO_CNF;

} GPIO_PinConfig_t;

/* =========================================================
 * GPIO Handle
 * ========================================================= */

typedef struct {
  GPIO_TypeDef *pGPIOx;
  GPIO_PinConfig_t GPIO_PinConfig;

} GPIO_Handle_t;

/* =========================================================
 * Generic GPIO Driver API
 * ========================================================= */

void GPIO_ClockControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi);

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

void GPIO_WritePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber, uint8_t Value);

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber);

void GPIO_DeInit(GPIO_TypeDef *pGPIOx);

/* =========================================================
 * ChronosRTOS PC13 LED API
 *
 * STM32F103C8T6 Blue Pill:
 *
 * PC13 LOW  -> LED ON
 * PC13 HIGH -> LED OFF
 * ========================================================= */

void GPIO_INIT(void);

void GPIO_ON(void);

void GPIO_OFF(void);

void GPIO_TOGGLE(void);

uint8_t GPIO_READ(void);

#endif /* GPIO_DRIVER_H */