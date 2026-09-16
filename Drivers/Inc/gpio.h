/******************************************************************************
 * File        : gpio.h
 * Project     : ChronosRTOS
 * Module      : GPIO
 * Group       :
 * Description : GPIO driver interface.
 ******************************************************************************/

/*#ifndef GPIO_H
#define GPIO_H

void GPIO_INIT();

void GPIO_ON();

void GPIO_OFF();
void GPIO_TOGGLE();

#endif /* GPIO_H */

#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include "stm32f10x.h"
#include <stdint.h>


/* =========================================================
 * GPIO Pin numbers
 * ========================================================= */

#define GPIO_PIN_0      0
#define GPIO_PIN_1      1
#define GPIO_PIN_2      2
#define GPIO_PIN_3      3
#define GPIO_PIN_4      4
#define GPIO_PIN_5      5
#define GPIO_PIN_6      6
#define GPIO_PIN_7      7
#define GPIO_PIN_8      8
#define GPIO_PIN_9      9
#define GPIO_PIN_10     10
#define GPIO_PIN_11     11
#define GPIO_PIN_12     12
#define GPIO_PIN_13     13
#define GPIO_PIN_14     14
#define GPIO_PIN_15     15


/* =========================================================
 * Enable / Disable
 * ========================================================= */

#define ENABLE         1
#define DISABLE        0


/* =========================================================
 * GPIO Output Modes
 *
 * STM32F103 GPIO MODE field:
 * 00 = Input
 * 01 = Output, max speed 10 MHz
 * 10 = Output, max speed 2 MHz
 * 11 = Output, max speed 50 MHz
 * ========================================================= */

#define GPIO_MODE_INPUT       0x0
#define GPIO_MODE_OUT_10MHz   0x1
#define GPIO_MODE_OUT_2MHz    0x2
#define GPIO_MODE_OUT_50MHz   0x3


/* =========================================================
 * GPIO Input Configuration
 * ========================================================= */

#define GPIO_CNF_INPUT_ANALOG       0x00
#define GPIO_CNF_INPUT_FLOATING     0x01
#define GPIO_CNF_INPUT_PU_PD        0x02


/* =========================================================
 * GPIO Output Configuration
 * ========================================================= */

#define GPIO_CNF_OUTPUT_PP          0x00
#define GPIO_CNF_OUTPUT_OD          0x01
#define GPIO_CNF_AF_OUTPUT_PP       0x02
#define GPIO_CNF_AF_OUTPUT_OD       0x03


/* =========================================================
 * GPIO Clock Control
 *
 * APB2ENR:
 * GPIOA clock -> bit 2
 * GPIOB clock -> bit 3
 * GPIOC clock -> bit 4
 * ========================================================= */

#define GPIOA_Clk_En()   (RCC->APB2ENR |= (1U << 2))
#define GPIOB_Clk_En()   (RCC->APB2ENR |= (1U << 3))
#define GPIOC_Clk_En()   (RCC->APB2ENR |= (1U << 4))

#define GPIOA_Clk_Di()   (RCC->APB2ENR &= ~(1U << 2))
#define GPIOB_Clk_Di()   (RCC->APB2ENR &= ~(1U << 3))
#define GPIOC_Clk_Di()   (RCC->APB2ENR &= ~(1U << 4))


/* =========================================================
 * GPIO Pin Configuration Structure
 * ========================================================= */

typedef struct
{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_Mode;
    uint8_t GPIO_CNF;

} GPIO_PinConfig_t;


/* =========================================================
 * GPIO Handle Structure
 * ========================================================= */

typedef struct
{
    GPIO_TypeDef *pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;

} GPIO_Handle_t;


/* =========================================================
 * Generic GPIO Driver API
 * ========================================================= */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);

void GPIO_WritePin(GPIO_TypeDef *pGPIOx,
                   uint8_t PinNumber,
                   uint8_t Value);

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx,
                     uint8_t PinNumber);

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx,
                    uint8_t PinNumber);

void GPIO_DeInit(GPIO_TypeDef *pGPIOx);

void GPIO_ClockControl(GPIO_TypeDef *pGPIOx,
                       uint8_t EnorDi);


/* =========================================================
 * ChronosRTOS PC13 LED API
 *
 * Hardware:
 * STM32F103C8T6 Blue Pill
 * Onboard LED -> PC13
 *
 * LED is active-low:
 * PC13 LOW  -> LED ON
 * PC13 HIGH -> LED OFF
 * ========================================================= */

void GPIO_INIT(void);

void GPIO_ON(void);

void GPIO_OFF(void);

void GPIO_TOGGLE(void);

uint8_t GPIO_READ(void);


#endif /* GPIO_DRIVER_H */
