/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <string.h>
#include <stdarg.h>
#include "ili9341.h"
#include "fonts.h"
#include "testimg.h"
#include "lvgl.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;


/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


void init() {
    ILI9341_Unselect();
    ILI9341_Init();
}

void loop() {
    if(HAL_SPI_DeInit(&hspi1) != HAL_OK) {
        return;
    }

    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;

    if(HAL_SPI_Init(&hspi1) != HAL_OK) {
        return;
    }

    // Check border
    ILI9341_FillScreen(ILI9341_BLACK);

    for(int x = 0; x < ILI9341_WIDTH; x++) {
        ILI9341_DrawPixel(x, 0, ILI9341_RED);
        ILI9341_DrawPixel(x, ILI9341_HEIGHT-1, ILI9341_RED);
    }

    for(int y = 0; y < ILI9341_HEIGHT; y++) {
        ILI9341_DrawPixel(0, y, ILI9341_RED);
        ILI9341_DrawPixel(ILI9341_WIDTH-1, y, ILI9341_RED);
    }

    HAL_Delay(3000);

    // Check fonts
    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, ILI9341_RED, ILI9341_BLACK);
    ILI9341_WriteString(0, 3*10, "Font_11x18, green, lorem ipsum dolor sit amet", Font_11x18, ILI9341_GREEN, ILI9341_BLACK);
    ILI9341_WriteString(0, 3*10+3*18, "Font_16x26, blue, lorem ipsum dolor sit amet", Font_16x26, ILI9341_BLUE, ILI9341_BLACK);

    HAL_Delay(1000);
    ILI9341_InvertColors(true);
    HAL_Delay(1000);
    ILI9341_InvertColors(false);

    HAL_Delay(5000);

    // Check colors
    ILI9341_FillScreen(ILI9341_WHITE);
    ILI9341_WriteString(0, 0, "WHITE", Font_11x18, ILI9341_BLACK, ILI9341_WHITE);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_BLUE);
    ILI9341_WriteString(0, 0, "BLUE", Font_11x18, ILI9341_BLACK, ILI9341_BLUE);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_RED);
    ILI9341_WriteString(0, 0, "RED", Font_11x18, ILI9341_BLACK, ILI9341_RED);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_GREEN);
    ILI9341_WriteString(0, 0, "GREEN", Font_11x18, ILI9341_BLACK, ILI9341_GREEN);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_CYAN);
    ILI9341_WriteString(0, 0, "CYAN", Font_11x18, ILI9341_BLACK, ILI9341_CYAN);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_MAGENTA);
    ILI9341_WriteString(0, 0, "MAGENTA", Font_11x18, ILI9341_BLACK, ILI9341_MAGENTA);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_YELLOW);
    ILI9341_WriteString(0, 0, "YELLOW", Font_11x18, ILI9341_BLACK, ILI9341_YELLOW);
    HAL_Delay(500);

    ILI9341_FillScreen(ILI9341_BLACK);
    ILI9341_WriteString(0, 0, "BLACK", Font_11x18, ILI9341_WHITE, ILI9341_BLACK);
    HAL_Delay(500);

    ILI9341_DrawImage((ILI9341_WIDTH - 240) / 2, (ILI9341_HEIGHT - 240) / 2, 240, 240, (const uint16_t*)test_img_240x240);

    HAL_Delay(3000);
}


void my_disp_flush(lv_disp_t * disp, const lv_area_t * area, lv_color_t * color_p)
  {

    /*It's a very slow but simple implementation.
     *`set_pixel` needs to be written by you to a set pixel on the screen*/
    // for(y = area->y1; y <= area->y2; y++) {
    //     for(x = area->x1; x <= area->x2; x++) {
    //         set_pixel(x, y, *color_p);
    //         color_p++;
    //     }
    // }
    uint16_t x = (uint16_t)area->x1;
    uint16_t w = (uint16_t)area->x2 - x + 1;
    uint16_t y = (uint16_t)area->y1;
    uint16_t h = (uint16_t)area->y2 - y + 1;
        /// 2024-02-04 convert data for transmit
    for(int i = 0; i < w*h; i++)
    {
        uint16_t temp = *((uint16_t*)color_p + i);
        *((uint16_t*)color_p + i) = ((temp & 0xFF00) >> 8) | ((temp & 0xFF) << 8);
    }
    ILI9341_DrawImage(x, y, w, h, (const uint16_t*)color_p);

    lv_disp_flush_ready(disp);         /* Indicate you are ready with the flushing*/
}



static void event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * label = lv_event_get_user_data(e);
  switch(code) {
  case LV_EVENT_PRESSED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_PRESSED");
    break;
  case LV_EVENT_CLICKED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_CLICKED");
    break;
  case LV_EVENT_LONG_PRESSED:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED");
    break;
  case LV_EVENT_LONG_PRESSED_REPEAT:
    lv_label_set_text(label, "The last button event:\nLV_EVENT_LONG_PRESSED_REPEAT");
    break;
  default:
    break;
  }
}



/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  init();
  lv_init();

  static lv_draw_buf_t draw_buf;
  static lv_color_t buf1[240 * 320 / 10];                        /*Declare a buffer for 1/10 screen size*/
  lv_draw_buf_init(&draw_buf, 320, 240, LV_COLOR_FORMAT_NATIVE, 20,
                             buf1, sizeof(buf1));  /*Initialize the display buffer.*/
  
  lv_display_t* disp_drv = lv_display_create(320, 240);        /*Basic initialization*/
  lv_display_set_flush_cb(disp_drv, my_disp_flush);    /*Set your driver function*/
  lv_display_set_draw_buffers(disp_drv, &draw_buf, NULL);
  //lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/

  lv_display_set_rotation(disp_drv, LV_DISPLAY_ROTATION_0); 

  // Some example here

  lv_obj_t * btn = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn, 100, 50);
  lv_obj_center(btn);
  lv_obj_t * btn_label = lv_label_create(btn);
  lv_label_set_text(btn_label, "Click me!");
  lv_obj_center(btn_label);
  lv_obj_t * info_label = lv_label_create(lv_scr_act());
  lv_label_set_text(info_label, "The last button event:\nNone");
  lv_obj_add_event(btn, event_cb, LV_EVENT_ALL, info_label);


  while (1)
  {
    /* USER CODE END WHILE */
	  //loop();
    HAL_Delay(5);
    lv_timer_handler();
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_10, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC7 */
  GPIO_InitStruct.Pin = GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PB4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
