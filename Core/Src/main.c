/* USER CODE BEGIN Header */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mpu6050.h"
#include "SimC.h"
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
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define FRAME_DIM 16
#define FRAME_SIZE FRAME_DIM * FRAME_DIM
uint16_t frame[FRAME_SIZE] = {
    0x100, 0x101, 0x102, 0x103, 0x104, 0x105, 0x106, 0x107, 0x108, 0x109, 0x10A, 0x10B, 0x10C, 0x10D, 0x10E, 0x10F,
    0x110, 0x111, 0x112, 0x113, 0x114, 0x115, 0x116, 0x117, 0x118, 0x119, 0x11A, 0x11B, 0x11C, 0x11D, 0x11E, 0x11F,
    0x120, 0x121, 0x122, 0x123, 0x124, 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A, 0x12B, 0x12C, 0x12D, 0x12E, 0x12F,
    0x130, 0x131, 0x132, 0x133, 0x134, 0x135, 0x136, 0x137, 0x138, 0x139, 0x13A, 0x13B, 0x13C, 0x13D, 0x13E, 0x13F,
    0x140, 0x141, 0x142, 0x143, 0x144, 0x145, 0x146, 0x147, 0x148, 0x149, 0x14A, 0x14B, 0x14C, 0x14D, 0x14E, 0x14F,
    0x150, 0x151, 0x152, 0x153, 0x154, 0x155, 0x156, 0x157,	0x158, 0x159, 0x15A, 0x15B, 0x15C, 0x15D, 0x15E, 0x15F,
    0x160, 0x161, 0x162, 0x163, 0x164, 0x165, 0x166, 0x167,	0x168, 0x169, 0x16A, 0x16B, 0x16C, 0x16D, 0x16E, 0x16F,
    0x170, 0x171, 0x172, 0x173, 0x174, 0x175, 0x176, 0x177, 0x178, 0x179, 0x17A, 0x17B, 0x17C, 0x17D, 0x17E, 0x17F,
	0x180, 0x181, 0x182, 0x183, 0x184, 0x185, 0x186, 0x187,	0x188, 0x189, 0x18A, 0x18B, 0x18C, 0x18D, 0x18E, 0x18F,
	0x190, 0x191, 0x192, 0x193, 0x194, 0x195, 0x196, 0x197,	0x198, 0x199, 0x19A, 0x19B, 0x19C, 0x19D, 0x19E, 0x19F,
	0x1A0, 0x1A1, 0x1A2, 0x1A3, 0x1A4, 0x1A5, 0x1A6, 0x1A7,	0x1A8, 0x1A9, 0x1AA, 0x1AB, 0x1AC, 0x1AD, 0x1AE, 0x1AF,
	0x1B0, 0x1B1, 0x1B2, 0x1B3, 0x1B4, 0x1B5, 0x1B6, 0x1B7,	0x1B8, 0x1B9, 0x1BA, 0x1BB, 0x1BC, 0x1BD, 0x1BE, 0x1BF,
	0x1C0, 0x1C1, 0x1C2, 0x1C3, 0x1C4, 0x1C5, 0x1C6, 0x1C7,	0x1C8, 0x1C9, 0x1CA, 0x1CB, 0x1CC, 0x1CD, 0x1CE, 0x1CF,
	0x1D0, 0x1D1, 0x1D2, 0x1D3, 0x1D4, 0x1D5, 0x1D6, 0x1D7,	0x1D8, 0x1D9, 0x1DA, 0x1DB, 0x1DC, 0x1DD, 0x1DE, 0x1DF,
	0x1E0, 0x1E1, 0x1E2, 0x1E3, 0x1E4, 0x1E5, 0x1E6, 0x1E7,	0x1E8, 0x1E9, 0x1EA, 0x1EB, 0x1EC, 0x1ED, 0x1EE, 0x1EF,
	0x1F0, 0x1F1, 0x1F2, 0x1F3, 0x1F4, 0x1F5, 0x1F6, 0x1F7,	0x1F8, 0x1F9, 0x1FA, 0x1FB, 0x1FC, 0x1FD, 0x1FE, 0x1FF
};

char Heart_buffer[FRAME_DIM][FRAME_DIM] = {
	{0,1,1,0,0,1,1,0}, // y = 0
	{1,1,1,1,1,1,1,1}, // y = 1
	{1,1,1,1,1,1,1,1}, // y = 2
	{1,1,1,1,1,1,1,1}, // y = 3
	{0,1,1,1,1,1,1,0}, // y = 4
	{0,0,1,1,1,1,0,0}, // y = 5
	{0,0,0,1,1,0,0,0}, // y = 6
	{0,0,0,0,0,0,0,0}  // y = 7
};

char general_buffer[FRAME_DIM][FRAME_DIM] = {
	{1,1,1,1,1,1,1,1},
	{0,1,1,1,1,1,1,0},
	{0,0,1,1,1,1,0,0},
	{0,0,0,1,1,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0}
};


void DrawBuffer(char buffer[FRAME_DIM][FRAME_DIM]) {
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

void DrawBufferDim() {
	for (int i = 0; i < OUT_BUFFER_DIM; i++) {
		for (int j = 0; j < OUT_BUFFER_DIM; j++) {
			if (out_buffer[i][j]){
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

int x = 0;
int y = 0;
int ON = 1;

void SetPixel(int x, int y, int ON) {
	if (ON) {
		frame[y * FRAME_DIM + x] &= ~0x200; // Set bit to 1
	} else {
		frame[y * FRAME_DIM + x] |= 0x200; // Set bit to 0
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_15)
    {
        x = (GPIOB->IDR & 0x00F0) >> 4;
        y = (GPIOB->IDR & 0x0F00) >> 8;
        ON = GPIOB->IDR & 0x1000;
        SetPixel(x, y, ON);
    }
	// set A15 to 0
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
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

    // init mpu
  	MPU6050_Data mpuData;

  	if (MPU6050_Init() == 0) {

  	} else {

  	}

  	init_particles();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  	while (1)
	{
		  MPU6050_Read_Accel(&mpuData);
					  //printf("ACC: X=%d Y=%d Z=%d\r\n", mpu.Accel_X, mpu.Accel_Y, mpu.Accel_Z);
		  Vec2 gravity;
		  gravity.x = -mpuData.Accel_Y;
		  gravity.y = mpuData.Accel_Z;
		  set_gravity_direction(gravity);


		  step_simulation();
		  generate_output();
		  DrawBufferDim();

	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3
                           PA4 PA5 PA6 PA7
                           PA8 PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

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
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
