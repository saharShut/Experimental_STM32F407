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



static lv_anim_timeline_t * anim_timeline = NULL;
static lv_obj_t * obj1 = NULL;
static lv_obj_t * obj2 = NULL;
static lv_obj_t * obj3 = NULL;
static const lv_coord_t obj_width = 90;
static const lv_coord_t obj_height = 70;

static void set_width(void * var, int32_t v)
{
  lv_obj_set_width((lv_obj_t *)var, v);
}

static void set_height(void * var, int32_t v)
{
  lv_obj_set_height((lv_obj_t *)var, v);
}

static void anim_timeline_create(void)
{
  /* obj1 */
  lv_anim_t a1;
  lv_anim_init(&a1);
  lv_anim_set_var(&a1, obj1);
  lv_anim_set_values(&a1, 0, obj_width);
  lv_anim_set_early_apply(&a1, false);
  lv_anim_set_exec_cb(&a1, (lv_anim_exec_xcb_t)set_width);
  lv_anim_set_path_cb(&a1, lv_anim_path_overshoot);
  lv_anim_set_time(&a1, 300);
  lv_anim_t a2;
  lv_anim_init(&a2);
  lv_anim_set_var(&a2, obj1);
  lv_anim_set_values(&a2, 0, obj_height);
  lv_anim_set_early_apply(&a2, false);
  lv_anim_set_exec_cb(&a2, (lv_anim_exec_xcb_t)set_height);
  lv_anim_set_path_cb(&a2, lv_anim_path_ease_out);
  lv_anim_set_time(&a2, 300);
  /* obj2 */
  lv_anim_t a3;
  lv_anim_init(&a3);
  lv_anim_set_var(&a3, obj2);
  lv_anim_set_values(&a3, 0, obj_width);
  lv_anim_set_early_apply(&a3, false);
  lv_anim_set_exec_cb(&a3, (lv_anim_exec_xcb_t)set_width);
  lv_anim_set_path_cb(&a3, lv_anim_path_overshoot);
  lv_anim_set_time(&a3, 300);
  lv_anim_t a4;
  lv_anim_init(&a4);
  lv_anim_set_var(&a4, obj2);
  lv_anim_set_values(&a4, 0, obj_height);
  lv_anim_set_early_apply(&a4, false);
  lv_anim_set_exec_cb(&a4, (lv_anim_exec_xcb_t)set_height);
  lv_anim_set_path_cb(&a4, lv_anim_path_ease_out);
  lv_anim_set_time(&a4, 300);
  /* obj3 */
  lv_anim_t a5;
  lv_anim_init(&a5);
  lv_anim_set_var(&a5, obj3);
  lv_anim_set_values(&a5, 0, obj_width);
  lv_anim_set_early_apply(&a5, false);
  lv_anim_set_exec_cb(&a5, (lv_anim_exec_xcb_t)set_width);
  lv_anim_set_path_cb(&a5, lv_anim_path_overshoot);
  lv_anim_set_time(&a5, 300);
  lv_anim_t a6;
  lv_anim_init(&a6);
  lv_anim_set_var(&a6, obj3);
  lv_anim_set_values(&a6, 0, obj_height);
  lv_anim_set_early_apply(&a6, false);
  lv_anim_set_exec_cb(&a6, (lv_anim_exec_xcb_t)set_height);
  lv_anim_set_path_cb(&a6, lv_anim_path_ease_out);
  lv_anim_set_time(&a6, 300);
  /* Create anim timeline */
  anim_timeline = lv_anim_timeline_create();
  lv_anim_timeline_add(anim_timeline, 0, &a1);
  lv_anim_timeline_add(anim_timeline, 0, &a2);
  lv_anim_timeline_add(anim_timeline, 200, &a3);
  lv_anim_timeline_add(anim_timeline, 200, &a4);
  lv_anim_timeline_add(anim_timeline, 400, &a5);
  lv_anim_timeline_add(anim_timeline, 400, &a6);
}

static void btn_start_event_handler(lv_event_t * e)
{
  lv_obj_t * btn = lv_event_get_target(e);
  if(!anim_timeline) {
    anim_timeline_create();
  }
  bool reverse = lv_obj_has_state(btn, LV_STATE_CHECKED);
  lv_anim_timeline_set_reverse(anim_timeline, reverse);
  lv_anim_timeline_start(anim_timeline);
}

static void btn_del_event_handler(lv_event_t * e)
{
  LV_UNUSED(e);
  if(anim_timeline) 
  {
    lv_anim_timeline_delete(anim_timeline);
    anim_timeline = NULL;
  }
}

static void btn_stop_event_handler(lv_event_t * e)
{
  LV_UNUSED(e);
  if(anim_timeline) 
  {
    lv_anim_timeline_pause(anim_timeline);
  }
}

static void slider_prg_event_handler(lv_event_t * e)
{
  lv_obj_t * slider = lv_event_get_target(e);
  if(!anim_timeline) 
  {
    anim_timeline_create();
  }
  int32_t progress = lv_slider_get_value(slider);
  lv_anim_timeline_set_progress(anim_timeline, progress);
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

  lv_obj_t * par = lv_scr_act();
  lv_obj_set_flex_flow(par, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(par, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  /* create btn_start */
  lv_obj_t * btn_start = lv_btn_create(par);
  lv_obj_add_event_cb(btn_start, btn_start_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_flag(btn_start, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_add_flag(btn_start, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_align(btn_start, LV_ALIGN_TOP_MID, -100, 20);
  lv_obj_t * label_start = lv_label_create(btn_start);
  lv_label_set_text(label_start, "Start");
  lv_obj_center(label_start);
  /* create btn_del */
  lv_obj_t * btn_del = lv_btn_create(par);
  lv_obj_add_event_cb(btn_del, btn_del_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(btn_del, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_align(btn_del, LV_ALIGN_TOP_MID, 0, 20);
  lv_obj_t * label_del = lv_label_create(btn_del);
  lv_label_set_text(label_del, "Delete");
  lv_obj_center(label_del);
  /* create btn_stop */
  lv_obj_t * btn_stop = lv_btn_create(par);
  lv_obj_add_event_cb(btn_stop, btn_stop_event_handler, LV_EVENT_CLICKED, NULL);
  lv_obj_add_flag(btn_stop, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_align(btn_stop, LV_ALIGN_TOP_MID, 100, 20);
  lv_obj_t * label_stop = lv_label_create(btn_stop);
  lv_label_set_text(label_stop, "Stop");
  lv_obj_center(label_stop);
  /* create slider_prg */
  lv_obj_t * slider_prg = lv_slider_create(par);
  lv_obj_add_event_cb(slider_prg, slider_prg_event_handler, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_flag(slider_prg, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_align(slider_prg, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_slider_set_range(slider_prg, 0, 65535);
  /* create 3 objects */
  obj1 = lv_obj_create(par);
  lv_obj_set_size(obj1, obj_width, obj_height);
  obj2 = lv_obj_create(par);
  lv_obj_set_size(obj2, obj_width, obj_height);
  obj3 = lv_obj_create(par);
  lv_obj_set_size(obj3, obj_width, obj_height);

  if(!anim_timeline) {
    anim_timeline_create();
  }
  bool reverse = false;
  lv_anim_timeline_set_reverse(anim_timeline, reverse);
  lv_anim_timeline_start(anim_timeline);

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
