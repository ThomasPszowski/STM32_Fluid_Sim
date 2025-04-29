#include "main.h"
I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);


#define FRAME_DIM 8
#define FRAME_SIZE FRAME_DIM * FRAME_DIM
uint16_t frame[FRAME_SIZE] = {
    0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, // y = 0
    0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117, // y = 1
    0x120, 0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, // y = 2
    0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x136, 0x137, // y = 3
    0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147, // y = 4
    0x150, 0x151, 0x152, 0x153, 0x154, 0x155, 0x156, 0x157, // y = 5
    0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167, // y = 6
    0x170, 0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177  // y = 7
};

int Heart_buffer[FRAME_DIM][FRAME_DIM] = {
	{0,1,1,0,0,1,1,0}, // y = 0
	{1,1,1,1,1,1,1,1}, // y = 1
	{1,1,1,1,1,1,1,1}, // y = 2
	{1,1,1,1,1,1,1,1}, // y = 3
	{0,1,1,1,1,1,1,0}, // y = 4
	{0,0,1,1,1,1,0,0}, // y = 5
	{0,0,0,1,1,0,0,0}, // y = 6
	{0,0,0,0,0,0,0,0}  // y = 7
};

int UP_buffer[FRAME_DIM][FRAME_DIM] = {
	{1,1,1,1,1,1,1,1},
	{0,1,1,1,1,1,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};
int DOWN_buffer[FRAME_DIM][FRAME_DIM] = {
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,1,1,1,1,0,0},
	{0,1,1,1,1,1,1,0},
	{1,1,1,1,1,1,1,1}
};
int LEFT_buffer[FRAME_DIM][FRAME_DIM] = {
	{0,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,1,1},
	{0,0,0,0,0,1,1,1},
	{0,0,0,0,1,1,1,1},
	{0,0,0,0,1,1,1,1},
	{0,0,0,0,0,1,1,1},
	{0,0,0,0,0,0,1,1},
	{0,0,0,0,0,0,0,1}
};
int RIGHT_buffer[FRAME_DIM][FRAME_DIM] = {
	{1,0,0,0,0,0,0,0},
	{1,1,0,0,0,0,0,0},
	{1,1,1,0,0,0,0,0},
	{1,1,1,1,0,0,0,0},
	{1,1,1,1,0,0,0,0},
	{1,1,1,0,0,0,0,0},
	{1,1,0,0,0,0,0,0},
	{1,0,0,0,0,0,0,0}
};



void DrawBuffer(int buffer[FRAME_DIM][FRAME_DIM]) {
	for (int i = 0; i < FRAME_DIM; i++) {
		for (int j = 0; j < FRAME_DIM; j++) {
			if (buffer[i][j]){
				frame[i * FRAME_DIM + j] &= ~0x200;
			}
			else {
				frame[i * FRAME_DIM + j] |= 0x200;
			}
		}
	}
}

void FillScreen() {
	for (int i = 0; i < FRAME_DIM; i++) {
		for (int j = 0; j < FRAME_DIM; j++) {
			frame[i * FRAME_DIM + j] &= ~0x200;
		}
	}
}

void EmptyScreen() {
	for (int i = 0; i < FRAME_DIM; i++) {
		for (int j = 0; j < FRAME_DIM; j++) {
			frame[i * FRAME_DIM + j] |= 0x200;
		}
	}
}

#define MPU6050_ADDR 0x68 << 1  // Przesunięcie dla HAL (7-bit + R/W bit)

void check_mpu() {
    if (HAL_I2C_IsDeviceReady(&hi2c1, MPU6050_ADDR, 3, 100) == HAL_OK) {
        // Urządzenie jest gotowe
    } else {
        // Brak odpowiedzi
    }
}

void MPU6050_Init() {
    uint8_t data = 0;
    HAL_I2C_Mem_Write(&hi2c1, MPU6050_ADDR, 0x6B, 1, &data, 1, 100); // Rejestr 0x6B = PWR_MGMT_1
}

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} AccelData_t;

AccelData_t accel;

AccelData_t MPU6050_Read_Accel() {
    uint8_t Rec_Data[6];
    AccelData_t accel;

    HAL_I2C_Mem_Read(&hi2c1, MPU6050_ADDR, 0x3B, 1, Rec_Data, 6, 100);

    accel.x = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
    accel.y = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
    accel.z = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

    return accel;
}

void TestAccel(){
	accel = MPU6050_Read_Accel();

	int abs_y = accel.y < 0 ? -accel.y : accel.y;
	int abs_z = accel.z < 0 ? -accel.z : accel.z;

	if (abs_y > abs_z){
		if (accel.y < 0){
			DrawBuffer(RIGHT_buffer);
		}
		else {
			DrawBuffer(LEFT_buffer);
		}
	}
	else {
		if (accel.z < 0){

			DrawBuffer(DOWN_buffer);
		}
		else {
			DrawBuffer(UP_buffer);
		}
	}
}

#include "SimC.h"

int main(void)
{


	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();



	// Włącz DMA i Timer
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// DMA1_Channel2 ← wyzwalany przez TIM2_UP
	DMA1_Channel2->CCR &= ~DMA_CCR_EN;  // wyłącz DMA przed konfiguracją

	DMA1_Channel2->CPAR = (uint32_t)&GPIOA->ODR;
	DMA1_Channel2->CMAR = (uint32_t)frame;
	DMA1_Channel2->CNDTR = FRAME_SIZE;

	DMA1_Channel2->CCR =
	    DMA_CCR_MINC |         // Memory increment
	    DMA_CCR_DIR  |         // Mem → Periph
	    DMA_CCR_CIRC |         // Cyklicznie
	    DMA_CCR_MSIZE_0 |      // MSIZE = 16-bit
	    DMA_CCR_PSIZE_0;       // PSIZE = 16-bit

	DMA1_Channel2->CCR |= DMA_CCR_EN;   // włącz DMA

	// Timer 2 – co 1 ms
	TIM2->PSC = 72 - 1;  // (72MHz / 7200 = 10kHz)
	TIM2->ARR = 10 - 1;    // co 1 ms
	TIM2->DIER |= TIM_DIER_UDE; // update event triggers DMA
	TIM2->CR1 |= TIM_CR1_CEN;   // start

	MX_I2C1_Init(); // wygenerowane przez CubeMX

	check_mpu();     // sprawdź czy odpowiada
	MPU6050_Init();  // wybudź MPU6050

	init_particles();

	while (1)
	{
		step_simulation();

	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 PA10 PA11
                           PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB10 PB11 PB12 PB13
                           PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
