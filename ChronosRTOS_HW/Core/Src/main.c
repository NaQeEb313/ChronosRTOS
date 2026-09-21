/******************************************************************************
 * File        : main.c
 * Project     : ChronosRTOS_HW
 * Description : ChronosRTOS hardware bootstrap and kernel startup.
 ******************************************************************************/

#include "main.h"

#include "gpio.h"
#include "kernel.h"
#include "task.h"
#include "uart.h"
#include "demo_tasks.h"

/* Private function prototypes ---------------------------------------------*/
void SystemClock_Config(void);

/* -------------------------------------------------------------------------- */
/* Main                                                                       */
/* -------------------------------------------------------------------------- */

int main(void) {
  USART_Handle_t usart1;

  /* Initialize HAL infrastructure */
  HAL_Init();

  /* Configure system clock
   *
   * HSE  = 8 MHz
   * PLL  = x9
   * SYSCLK = 72 MHz
   * APB1 = 36 MHz
   * APB2 = 72 MHz
   */
  SystemClock_Config();

  /* Initialize custom ChronosRTOS GPIO driver */
  GPIO_INIT();

  /* Configure custom USART1 driver */
  usart1.pUSARTx = USART1;

  usart1.USART_Config.USART_Mode = USART_MODE_TXRX;

  usart1.USART_Config.USART_Baud = USART_STD_BAUD_115200;

  usart1.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;

  usart1.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

  usart1.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;

  usart1.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;

  USART_Init(&usart1);

  /* Boot message */
  USART_SendString(USART1, "ChronosRTOS booted\r\n");

  /* Initialize ChronosRTOS kernel */
Kernel_Init();

USART_SendString(USART1, "Kernel initialized\r\n");

Task_Create(TASK_A, 1);
Task_Create(TASK_B, 5);
Task_Create(TASK_C, 3);
Task_Create(TASK_D, 2);
Task_Create(TASK_E, 4);

USART_SendString(USART1, "5 tasks created\r\n");

Kernel_Start();

  USART_SendString(USART1, "Kernel started\r\n");

  /* Main should eventually never execute application logic here.
   * For the current prototype, context switching has not been
   * integrated yet, so remain here.
   */
  while (1) {
  }
}

/* -------------------------------------------------------------------------- */
/* System Clock Configuration                                                 */
/* -------------------------------------------------------------------------- */

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Configure HSE and PLL */

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;

  RCC_OscInitStruct.HSEState = RCC_HSE_ON;

  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;

  RCC_OscInitStruct.HSIState = RCC_HSI_ON;

  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;

  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /* Configure CPU, AHB and APB clocks */

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;

  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/* -------------------------------------------------------------------------- */
/* Error Handler                                                              */
/* -------------------------------------------------------------------------- */

void Error_Handler(void) {
  __disable_irq();

  while (1) {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {
  (void)file;
  (void)line;
}

#endif /* USE_FULL_ASSERT */