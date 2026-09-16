/******************************************************************************
 * File        : gpio.c
 * Project     : ChronosRTOS
 * Module      : GPIO
 * Group       :
 * Description : GPIO driver implementation.
 ******************************************************************************/
/*#include "gpio.h"

void GPIO_INIT() {}

void GPIO_OFF() {}

void GPIO_ON() {}
void GPIO_TOGGLE() {}*/


#include "gpio_drivers.h"
#include "stm32f10x.h"


/* =========================================================
 * Generic GPIO Clock Control
 * ========================================================= */

void GPIO_ClockControl(GPIO_TypeDef *pGPIOx, uint8_t EnorDi)
{
    if (EnorDi == ENABLE)
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_Clk_En();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_Clk_En();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_Clk_En();
        }
    }
    else
    {
        if (pGPIOx == GPIOA)
        {
            GPIOA_Clk_Di();
        }
        else if (pGPIOx == GPIOB)
        {
            GPIOB_Clk_Di();
        }
        else if (pGPIOx == GPIOC)
        {
            GPIOC_Clk_Di();
        }
    }
}


/* =========================================================
 * Generic GPIO Initialization
 *
 * Supports GPIOA, GPIOB and GPIOC.
 *
 * STM32F103:
 * Pins 0-7  -> CRL
 * Pins 8-15 -> CRH
 *
 * Each pin uses 4 configuration bits:
 *
 * [CNF1 CNF0 MODE1 MODE0]
 * ========================================================= */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
    uint32_t temp = 0;

    uint8_t pin  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
    uint8_t mode = pGPIOHandle->GPIO_PinConfig.GPIO_Mode;
    uint8_t cnf  = pGPIOHandle->GPIO_PinConfig.GPIO_CNF;

    volatile uint32_t *configReg;

    uint8_t pos;


    /* Select CRL for pins 0-7 and CRH for pins 8-15 */

    if (pin < 8)
    {
        configReg = &pGPIOHandle->pGPIOx->CRL;

        pos = pin * 4;
    }
    else
    {
        configReg = &pGPIOHandle->pGPIOx->CRH;

        pos = (pin - 8) * 4;
    }


    /* =====================================================
     * Configure INPUT mode
     * ===================================================== */

    if (mode == GPIO_MODE_INPUT)
    {
        /*
         * MODE = 00 for input
         */

        if (cnf == GPIO_CNF_INPUT_ANALOG)
        {
            /* CNF = 00, MODE = 00 */
            temp = 0x0;
        }

        else if (cnf == GPIO_CNF_INPUT_FLOATING)
        {
            /* CNF = 01, MODE = 00 */
            temp = (1U << 2);
        }

        else if (cnf == GPIO_CNF_INPUT_PU_PD)
        {
            /* CNF = 10, MODE = 00 */
            temp = (2U << 2);
        }

        else
        {
            temp = 0x0;
        }
    }


    /* =====================================================
     * Configure OUTPUT mode
     * ===================================================== */

    else
    {
        if (cnf == GPIO_CNF_OUTPUT_PP)
        {
            /* CNF = 00 */
            temp = mode;
        }

        else if (cnf == GPIO_CNF_OUTPUT_OD)
        {
            /* CNF = 01 */
            temp = (1U << 2) | mode;
        }

        else if (cnf == GPIO_CNF_AF_OUTPUT_PP)
        {
            /* CNF = 10 */
            temp = (2U << 2) | mode;
        }

        else if (cnf == GPIO_CNF_AF_OUTPUT_OD)
        {
            /* CNF = 11 */
            temp = (3U << 2) | mode;
        }

        else
        {
            temp = 0x0;
        }
    }


    /* =====================================================
     * Clear the existing 4-bit configuration
     * for this GPIO pin.
     * ===================================================== */

    *configReg &= ~(0xFU << pos);


    /* =====================================================
     * Write the new configuration
     * ===================================================== */

    *configReg |= (temp << pos);
}


/* =========================================================
 * Generic GPIO Read
 * ========================================================= */

uint8_t GPIO_ReadPin(GPIO_TypeDef *pGPIOx, uint8_t PinNumber)
{
    if ((pGPIOx->IDR & (1U << PinNumber)) != 0U)
    {
        return 1U;
    }

    return 0U;
}


/* =========================================================
 * Generic GPIO Write
 * ========================================================= */

void GPIO_WritePin(GPIO_TypeDef *pGPIOx,
                   uint8_t PinNumber,
                   uint8_t Value)
{
    if (Value == 1U)
    {
        pGPIOx->ODR |= (1U << PinNumber);
    }
    else
    {
        pGPIOx->ODR &= ~(1U << PinNumber);
    }
}


/* =========================================================
 * Generic GPIO De-Initialization
 * ========================================================= */

void GPIO_DeInit(GPIO_TypeDef *pGPIOx)
{
    if (pGPIOx == GPIOA)
    {
        RCC->APB2RSTR |= (1U << 2);
        RCC->APB2RSTR &= ~(1U << 2);
    }

    else if (pGPIOx == GPIOB)
    {
        RCC->APB2RSTR |= (1U << 3);
        RCC->APB2RSTR &= ~(1U << 3);
    }

    else if (pGPIOx == GPIOC)
    {
        RCC->APB2RSTR |= (1U << 4);
        RCC->APB2RSTR &= ~(1U << 4);
    }
}


/* =========================================================
 * Generic GPIO Toggle
 * ========================================================= */

void GPIO_TogglePin(GPIO_TypeDef *pGPIOx,
                    uint8_t PinNumber)
{
    pGPIOx->ODR ^= (1U << PinNumber);
}


/* =========================================================
 * ChronosRTOS LED Driver
 *
 * Hardware:
 * STM32F103C8T6 Blue Pill
 * Onboard LED -> PC13
 *
 * PC13 LED is ACTIVE-LOW:
 *
 * PC13 = 0 -> LED ON
 * PC13 = 1 -> LED OFF
 * ========================================================= */


/* =========================================================
 * GPIO_INIT()
 *
 * 1. Enable GPIOC clock
 * 2. Configure PC13 as:
 *      - General purpose output
 *      - Push-pull
 *      - 2 MHz
 * 3. Set PC13 HIGH so LED starts OFF
 * ========================================================= */

void GPIO_INIT(void)
{
    /*
     * RCC->APB2ENR bit 4 = IOPCEN
     *
     * Enable GPIOC peripheral clock.
     */
    RCC->APB2ENR |= (1U << 4);


    /*
     * GPIOC->CRH controls PC8-PC15.
     *
     * PC13 uses bits [23:20].
     *
     * MODE13 = 10
     *     Output mode, maximum speed 2 MHz
     *
     * CNF13 = 00
     *     General-purpose push-pull
     *
     * Therefore:
     *
     *     CNF MODE = 0010
     *              = 0x2
     */

    /* Clear PC13 configuration bits [23:20]. */
    GPIOC->CRH &= ~(0xFU << 20);

    /* Configure PC13 as 2 MHz push-pull output. */
    GPIOC->CRH |= (0x2U << 20);


    /*
     * PC13 HIGH -> LED OFF.
     *
     * BSRR bit 13 sets PC13 HIGH.
     */
    GPIOC->BSRR = (1U << 13);
}


/* =========================================================
 * GPIO_ON()
 *
 * Active-low LED:
 *
 * PC13 LOW -> LED ON
 *
 * BRR bit 13 resets PC13 to LOW.
 * ========================================================= */

void GPIO_ON(void)
{
    GPIOC->BRR = (1U << 13);
}


/* =========================================================
 * GPIO_OFF()
 *
 * Active-low LED:
 *
 * PC13 HIGH -> LED OFF
 *
 * BSRR bit 13 sets PC13 HIGH.
 * ========================================================= */

void GPIO_OFF(void)
{
    GPIOC->BSRR = (1U << 13);
}


/* =========================================================
 * GPIO_TOGGLE()
 *
 * Invert the current PC13 output state.
 * ========================================================= */

void GPIO_TOGGLE(void)
{
    GPIOC->ODR ^= (1U << 13);
}


/* =========================================================
 * GPIO_READ()
 *
 * Read the physical logic level on PC13.
 *
 * Returns:
 *     0 -> PC13 LOW
 *     1 -> PC13 HIGH
 *
 * Remember:
 *     0 -> LED ON
 *     1 -> LED OFF
 * ========================================================= */

uint8_t GPIO_READ(void)
{
    return (uint8_t)((GPIOC->IDR >> 13) & 0x01U);
}
