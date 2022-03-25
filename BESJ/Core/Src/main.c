/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "string.h"
#include <stdlib.h>
#include <sys/time.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define X_ARRAY 0
#define Y_ARRAY 1
#define X_Y_ARRAY 2
#define MUSHROOM_X_MIN_BOUND 0
#define MUSHROOM_X_MAX_BOUND 31
#define MUSHROOM_Y_MIN_BOUND 2
#define MUSHROOM_Y_MAX_BOUND 19
#define Y_MAX 20
#define X_MAX 32
#define MUSHROOM_MAX 30
#define POWER_UP_MAX 3
#define BULLETS_MAX 100
#define PLAYER_X_MIN_BOUND 0
#define	PLAYER1_START_POSITION 9
#define	PLAYER2_START_POSITION 17
#define PLAYER_X_MAX_BOUND 31
#define CENTIPEDE_MAXLENGTH 6
#define LASER 0
#define BOOST 1
#define POWER 2
//GROEN IS LINKS, GEEL IS RECHTS (MET STICKER)
//GROEN IS BOVEN, GEEL IS ONDER (ZONDER STICKER

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

//GRID
uint8_t GRID[32][20];
//SCORE
int score;
//PLAYERS
int player1_x_position,		//PLAYER 1
	player1_left_state, 	player1_left_state_prev,
	player1_right_state, 	player1_right_state_prev,
	player1_select_state,	player1_select_state_prev,
	player1_shoot_state, 	player1_shoot_state_prev;
int player2_x_position,		//PLAYER 2
	player2_left_state, 	player2_left_state_prev,
	player2_right_state, 	player2_right_state_prev,
	player2_select_state,	player2_select_state_prev,
	player2_shoot_state, 	player2_shoot_state_prev;

//BULLETS
int player1_bullet_positions[BULLETS_MAX][X_Y_ARRAY];
int player1_bullets_active;
int player2_bullet_positions[BULLETS_MAX][X_Y_ARRAY];
int player2_bullets_active;

//Centipedes
int centipede1_Position[CENTIPEDE_MAXLENGTH][X_Y_ARRAY];
int centipede2_Position[CENTIPEDE_MAXLENGTH][X_Y_ARRAY];

//MUSHROOMS
int mushroomFarm[MUSHROOM_MAX][X_Y_ARRAY];
int mushroomPowerUP[POWER_UP_MAX][3];
int mushroom_active;
int mushroomPowerUP_active;
int mushroomTimer;
//test
char uart_buf[50];
int uart_buf_len;
uint16_t timer_val, timer_val2;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM16_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
//PLAYER1
int Player1_move(void);
int Player1_shoot(void);
int Player1_select(void);
void Player1_position(int);
void Player1_newBullets(void);
//PLAYER2
int Player2_move(void);
int Player2_shoot(void);
int Player2_select(void);
void Player2_position(int);
void Player2_newBullets(void);
//BULLETS
void bullets_delete_player1(int);
void bullets_delete_player2(int);
void bullets_move(void);
void bullets_position_max(void);
//Centipede1
void Centipede1_INIT(void); 			//TODO
void Centipede1_move(void); 			//TODO
void Centipede1_position(void); 		//TODO
//Centipede2
void Centipede2_INIT(void); 			//TODO
void Centipede2_move(void); 			//TODO
void Centipede2_position(void); 		//TODO
//MUSHROOM
void powerUpSpawn(void);				//TODO
void deletePowerUp(int);				//TODO
void mushroomDestroyed(int);
void mushroomFarm_INIT(int);
void mushroomSpawn(void);
int checkMushroomFarm(int,int);
//INTERACTIONS
void mushroomShot(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  struct timeval t1;
  gettimeofday(&t1, NULL);
  srand(t1.tv_usec * t1.tv_sec);

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM16_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //INIT VARIABLES
  player1_x_position = PLAYER1_START_POSITION;
  player2_x_position = PLAYER2_START_POSITION;
  HAL_TIM_Base_Start(&htim16);
  //INIT FUCNTIONS
  //powerUp_INIT();
  mushroom_active = 0;
  mushroomPowerUP_active = 0;
  mushroomFarm_INIT(20);
  int found = 0;
  int found2 = 0;
  int found3 = 0;
  player1_bullets_active = 0;
  int move = 0;
  int shoot = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  bullets_position_max();
	  if((rand() % 20) < 1)
	  {
		  powerUpSpawn();
	  }
	  else
	  {
		  mushroomSpawn();
	  }


		mushroomShot();
		if(move == 0){
			move = Player1_move();
		}
		if(Player1_shoot())
		{
			shoot = 1;
		}
		if (__HAL_TIM_GET_COUNTER(&htim16) - timer_val2 >= 2000) //runt elke 1khz
		{
			timer_val2 = __HAL_TIM_GET_COUNTER(&htim16);
			Player1_position(move);
			if(shoot)
			{
				shoot = 0;
				Player1_newBullets();
			}
			bullets_move();
			move = 0;
		}
		if (__HAL_TIM_GET_COUNTER(&htim16) - timer_val >= 10000) //runt elke 1khz
		{
			//Player1_position(Player1_move());
			//uart_buf_len = sprintf(uart_buf, "%d : %d\r\n", mushroomFarm[k][0],mushroomFarm[k][1]);
			//HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
			timer_val = __HAL_TIM_GET_COUNTER(&htim16);


			for(int j = 0; j<Y_MAX; j++)
			{
				for(int k = 0; k<X_MAX; k++)
				{

					for(int i = 0; i<player1_bullets_active; i++)
					{
						if(player1_bullet_positions[i][Y_ARRAY] == j && player1_bullet_positions[i][X_ARRAY] == k)
						{
							found = 1;

						}
					}
					for(int i = 0; i<mushroom_active; i++)
					{
						if(mushroomFarm[i][Y_ARRAY] == j && mushroomFarm[i][X_ARRAY] == k)
						{
							found2 = 1;
						}
						if(mushroomPowerUP[i][Y_ARRAY] == j && mushroomPowerUP[i][X_ARRAY] == k && i<mushroomPowerUP_active)
						{
							found3 = 1;
						}
					}
					if(found)
					{
						uart_buf_len = sprintf(uart_buf, "|");
					}
					else if(found2 == 1 && found3 == 0)
					{
						uart_buf_len = sprintf(uart_buf, "0");
					}
					else if (found3)
					{
						uart_buf_len = sprintf(uart_buf, "X");
					}
					else if(player1_x_position == k && j == 0)
					{
						uart_buf_len = sprintf(uart_buf, "V");
					}
					else
					{
						uart_buf_len = sprintf(uart_buf, " ");
					}
					HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
					found = 0;
					found2 = 0;
					found3 = 0;
				}
				uart_buf_len = sprintf(uart_buf, "\r\n");
				HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
			}
			uart_buf_len = sprintf(uart_buf, "______________%d_________%d_______%d_____\r\n", mushroomPowerUP_active, player1_bullets_active, score);
			HAL_UART_Transmit(&huart2, (uint8_t *)uart_buf, uart_buf_len, 100);
		}

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM16 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM16_Init(void)
{

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 4799;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 460800;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_HalfDuplex_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 38400;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Player2_left_Pin Player2_Select_Pin */
  GPIO_InitStruct.Pin = Player2_left_Pin|Player2_Select_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Player1_Select_Pin Player1_Left_Pin Player1_Shoot_Pin Player1_Right_Pin
                           Player2_Shoot_Pin */
  GPIO_InitStruct.Pin = Player1_Select_Pin|Player1_Left_Pin|Player1_Shoot_Pin|Player1_Right_Pin
                          |Player2_Shoot_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : Player2_Right_Pin */
  GPIO_InitStruct.Pin = Player2_Right_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Player2_Right_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

//Mushroom shot by player TODO untested
void mushroomShot(void)
{
	for(int i = 0; i < BULLETS_MAX ; i++)
	{
		for(int j = 0; j < MUSHROOM_MAX; j++)
		{
			if(player1_bullet_positions[i][X_ARRAY] == mushroomFarm[j][X_ARRAY] && player1_bullet_positions[i][Y_ARRAY]+1 == mushroomFarm[j][Y_ARRAY])
			{
				bullets_delete_player1(i);
				mushroomDestroyed(j);
				score += 10;
			}
		}
	}
	for(int i = 0; i < BULLETS_MAX ; i++)
	{
		for(int j = 0; j < MUSHROOM_MAX; j++)
		{
			if(player2_bullet_positions[i][X_ARRAY] == mushroomFarm[j][X_ARRAY] && player2_bullet_positions[i][Y_ARRAY]+1 == mushroomFarm[j][Y_ARRAY])
			{
				bullets_delete_player2(i);
				mushroomDestroyed(j);
				score += 10;
			}
		}
	}
}

void bullets_delete_player1(int pos)
{
    for (int j = pos; j < player1_bullets_active-1; j++)
    {
    	player1_bullet_positions[j][X_ARRAY] = player1_bullet_positions[j+1][X_ARRAY];
    	player1_bullet_positions[j][Y_ARRAY] = player1_bullet_positions[j+1][Y_ARRAY];
    }
    player1_bullets_active--;
}
void bullets_delete_player2(int pos)
{
    for (int j = pos; j < player2_bullets_active-1; j++)
    {
    	player2_bullet_positions[j][X_ARRAY] = player2_bullet_positions[j+1][X_ARRAY];
    	player2_bullet_positions[j][Y_ARRAY] = player2_bullet_positions[j+1][Y_ARRAY];
    }
    player2_bullets_active--;
}
void bullets_position_max(void)
{
	for(int i = 0; i < player1_bullets_active; i++)
	{
		if(player1_bullet_positions[i][Y_ARRAY] > Y_MAX)
		{
			bullets_delete_player1(i);
		}
	}
	for(int i = 0; i < player2_bullets_active; i++)
	{
		if(player2_bullet_positions[i][Y_ARRAY] > Y_MAX)
		{
			bullets_delete_player2(i);
		}
	}
}
//upwards movement from bullets
void bullets_move(void)
{
	for(int i = 0; i < player1_bullets_active; i++)
	{
		player1_bullet_positions[i][Y_ARRAY]++;
	}
	for(int j = 0; j < player2_bullets_active; j++)
	{
		player2_bullet_positions[j][Y_ARRAY]++;
	}
}
// power up TODO untested
void powerUpSpawn(void)
{
	int new_mushroom_x = 0;
	int new_mushroom_y = 0;
	int powerUP = 0;
	if(mushroomPowerUP_active < POWER_UP_MAX)
	{
		do {
			new_mushroom_x = rand() % MUSHROOM_X_MAX_BOUND;
			new_mushroom_y = (rand() % (MUSHROOM_Y_MAX_BOUND-3)) + 3;
		}
		while(checkMushroomFarm(new_mushroom_x,new_mushroom_y));
		powerUP = rand() % 3;
		mushroomFarm[mushroom_active][X_ARRAY] = new_mushroom_x;
		mushroomFarm[mushroom_active][Y_ARRAY] = new_mushroom_y;
		mushroomPowerUP[mushroomPowerUP_active][X_ARRAY] = new_mushroom_x;
		mushroomPowerUP[mushroomPowerUP_active][Y_ARRAY] = new_mushroom_y;
		mushroomPowerUP[mushroomPowerUP_active][3] = powerUP;
		mushroom_active++;
		mushroomPowerUP_active++;
	}
}
void deletePowerUp(int pos)
{
	for (int i = pos; i < mushroomPowerUP_active-1; i++)
	{
		mushroomPowerUP[i][X_ARRAY] = mushroomPowerUP[i+1][X_ARRAY];
		mushroomPowerUP[i][Y_ARRAY] = mushroomPowerUP[i+1][Y_ARRAY];
		mushroomPowerUP[i][3] = mushroomPowerUP[i+1][3];
	}
	mushroomPowerUP_active--;
}
//delete a single  mushroom  TODO untested
void mushroomDestroyed(int pos)
{
	for(int i = 0; i < mushroomPowerUP_active; i++)
	{
		if(mushroomFarm[pos][X_ARRAY] == mushroomPowerUP[i][X_ARRAY] && mushroomFarm[pos][Y_ARRAY] == mushroomPowerUP[i][Y_ARRAY])
		{
			//addPowerUP(mushroomPowerUP[i][3]); TODO
			deletePowerUp(i);
		}
	}
	for (int i = pos; i < mushroom_active-1; i++)
	{
		mushroomFarm[i][X_ARRAY] = mushroomFarm[i+1][X_ARRAY];
		mushroomFarm[i][Y_ARRAY] = mushroomFarm[i+1][Y_ARRAY];
	}
	mushroom_active--;
}
//Spawn a single mushroom
void mushroomSpawn(void)
{
	int new_mushroom_x = 0;
	int new_mushroom_y = 0;
	if(mushroom_active < MUSHROOM_MAX)
	{
		do {
			new_mushroom_x = rand() % MUSHROOM_X_MAX_BOUND;
			new_mushroom_y = (rand() % (MUSHROOM_Y_MAX_BOUND-3)) + 3;
		}
		while(checkMushroomFarm(new_mushroom_x,new_mushroom_y));
		mushroomFarm[mushroom_active][X_ARRAY] = new_mushroom_x;
		mushroomFarm[mushroom_active][Y_ARRAY] = new_mushroom_y;
		mushroom_active++;
	}
}
//Check if position has a mushroom
int checkMushroomFarm(int new_mushroom_x,int new_mushroom_y)
{
	for(int i = 0; i < mushroom_active; i++)
	{
		if(new_mushroom_x == mushroomFarm[i][X_ARRAY])
		{
			if(new_mushroom_y == mushroomFarm[i][Y_ARRAY])
			{
				return 1;
			}
		}
	}
	return 0;
}
//Spawn mushrooms at the start of a level
void mushroomFarm_INIT(int amount_new_mushrooms)
{
	int new_mushroom_x = 0;
	int new_mushroom_y = 0;
	for(int i = 0; i < amount_new_mushrooms; i++)
	{
		do {
			new_mushroom_x = rand() % MUSHROOM_X_MAX_BOUND;
			new_mushroom_y = (rand() % (MUSHROOM_Y_MAX_BOUND-3)) + 3;
		}
		while(checkMushroomFarm(new_mushroom_x,new_mushroom_y));
		mushroomFarm[mushroom_active][X_ARRAY] = new_mushroom_x;
		mushroomFarm[mushroom_active][Y_ARRAY] = new_mushroom_y;
		mushroom_active++;
	}
}
//player1 add bullet
void Player1_newBullets(void)
{
	player1_bullet_positions[player1_bullets_active][X_ARRAY] = player1_x_position;
	player1_bullet_positions[player1_bullets_active][Y_ARRAY] = 1;
	player1_bullets_active++;
}
//player1 position changes position of sprite
void Player1_position(int movement)
{
	int next_position = player1_x_position + movement;
	if(PLAYER_X_MIN_BOUND <= next_position && next_position <= PLAYER_X_MAX_BOUND)
	{
		player1_x_position = next_position;
	}
}
//Player 1 move returns either 0 for no movement, -1 for left and 1 for right;
int Player1_move(void)
{
	player1_left_state = HAL_GPIO_ReadPin(GPIOB, Player1_Left_Pin);
	if(player1_left_state != player1_left_state_prev && player1_left_state)
	{
		player1_left_state_prev = player1_left_state;
		return -1;
	}
	player1_left_state_prev = 0;
	player1_right_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5);
	if(player1_right_state != player1_right_state_prev && player1_right_state)
	{
		player1_right_state_prev = player1_right_state;
		return 1;
	}
	player1_right_state_prev = 0;
	return 0;
}
//Player 1 shoot returns either 0 or 1;
int Player1_shoot(void)
{
	player1_shoot_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4);
	if(player1_shoot_state != player1_shoot_state_prev && player1_shoot_state)
	{
		player1_shoot_state_prev = player1_shoot_state;
		return 1;
	}
	else if(player1_shoot_state == 0)
	{
		player1_shoot_state_prev = 0;
	}
	return 0;
}
//Player 1 select returns either 0 or 1;
int Player1_select(void)
{
	player1_select_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10);
	if(player1_select_state != player1_select_state_prev && player1_select_state)
	{
		player1_select_state_prev = player1_select_state;
		return 1;
	}
	else if (player1_select_state == 0)
	{
		player1_select_state_prev = 0;
	}
	return 0;
}
//player2 add bullet
void Player2_newBullets(void)
{
	player2_bullet_positions[player2_bullets_active][X_ARRAY] = player2_x_position;
	player2_bullet_positions[player2_bullets_active][Y_ARRAY] = 1;
	player2_bullets_active++;
}
//player2 position changes position of sprite
void Player2_position(int movement)
{
	int next_position = player2_x_position + movement;
	if(PLAYER_X_MIN_BOUND <= next_position && next_position <= PLAYER_X_MAX_BOUND)
	{
		player2_x_position = next_position;
	}
}
//Player 2 move returns either 0 for no movement, -1 for left and 1 for right;
int Player2_move(void)
{
	player2_left_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0);
	if(player2_left_state != player2_left_state_prev && player2_left_state)
	{
		player2_left_state_prev = player2_left_state;
		return -1;
	}
	player2_left_state_prev = 0;
	player2_right_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
	if(player2_right_state != player2_right_state_prev && player2_right_state)
	{
		player2_right_state_prev = player2_right_state;
		return 1;
	}
	player2_right_state_prev = 0;
	return 0;
}
//Player 2 shoot returns either 0 or 1;
int Player2_shoot(void)
{
	player2_shoot_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6);
	if(player2_shoot_state != player2_shoot_state_prev && player2_shoot_state)
	{
		player2_shoot_state_prev = player2_shoot_state;
		return 1;
	}
	else if(player2_shoot_state == 0)
	{
		player2_shoot_state_prev = 0;
	}
	return 0;
}
//Player 2 select returns either 0 or 1;
int Player2_select(void)
{
	player2_select_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7);
	if(player2_select_state != player2_select_state_prev && player2_select_state)
	{
		player2_select_state_prev = player2_select_state;
		return 1;
	}
	else if (player2_select_state == 0)
	{
		player2_select_state_prev = 0;
	}
	return 0;
}
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
