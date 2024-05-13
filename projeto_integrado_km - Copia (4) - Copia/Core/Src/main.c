/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "st7735\st7735.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LIGA_LEDS HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3 + GPIO_PIN_4 + GPIO_PIN_5 + GPIO_PIN_6, 1)
#define LE_BOTAO_CIMA HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)
#define LE_BOTAO_DIREITO HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define LE_BOTAO_BAIXO HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
#define TAMANHO_TABULEIRO 3
#define JOGADOR1 'X'
#define JOGADOR2 'O'
#define VAZIO ' '
//#define INTERROGACAO '?'
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
static int tabuleiro[3][3];
int placar_jogador1 = 0;
int placar_jogador2 = 0;
int rodadas_jogador1 = 0;
int rodadas_jogador2 = 0;
static int cursor_coluna = 0;
static int cursor_linha = 0;
int jogador_atual = 1; // X É 1
static int posicao_atual = 0; // X É 1
//int x;
//int y;
//int n;
static int matriz_position = 0;

typedef struct {
	int x; // posicao display
	int y; // posicao display
	int jogador;
	int position; // posicao novetor
	char valor; // X, O, ?,...
} table_game_t;
table_game_t table_game[9];

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

int main(void);
void mover(void);
void  Definir_local(int cursor_linha, int cursor_coluna, int jogadorAtual);
void inicializar_tabuleiro(void);
//void exibir_tabuleiro(void);
void efetuar_jogada(int cursor_linha, int cursor_coluna);
void alternar_jogador(void);
void jogar_partida();
void jogar_melhor_de_tres(void);
void ChamaHashtag(void);
int verificar_vencedor(void);
void desenha_pela_coordenada(int a, int b, int jogador);
void Error_Handler(void);
void desenha_matriz(void);
void tabuleirio_init (void);
int aloca_proxima_posicao(int);
int aloca_posicao_anterior(int);
int confirma_posicao(int posicao);
void exibe_vencedor(void);



/* USER CODE BEGIN PFP */

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

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	/* USER CODE BEGIN 2 */

	ST7735_Init();
	ST7735_FillScreen(BLACK);






	//////////////////////////////////////////////////////////// AQUI ////////////////////////////////////////////

	/*if ( HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9) ==0)
	{
		LIGA_LEDS;
	}*/




	/* USER CODE END 2 */

	/* Infinite loop */

	tabuleirio_init();


	/* USER CODE BEGIN WHILE */
	while (1)
	{
		// Preenchendo tabuleiro
		desenha_matriz();

		mover();

		//   jogar_partida();
		//  jogar_melhor_de_tres();

		/* USER CODE END WHILE */

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
	hspi1.Init.Direction = SPI_DIRECTION_1LINE;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
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
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin|GPIO_PIN_3|GPIO_PIN_4
			|GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pin : ST7735_CS_Pin */
	GPIO_InitStruct.Pin = ST7735_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin PB3 PB4
                           PB5 PB6 */
	GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin|GPIO_PIN_3|GPIO_PIN_4
			|GPIO_PIN_5|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PA9 PA10 PA11 PA12 */
	GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

void mover(void)
{
	static int posicao_tabuleiro = 0;

	if (LE_BOTAO_DIREITO == 0)
	{

		posicao_tabuleiro = aloca_proxima_posicao(posicao_tabuleiro);
		HAL_Delay(150);

		if(posicao_tabuleiro > 8){

			posicao_tabuleiro = 0;
		}

		return;

	}
	else if (LE_BOTAO_BAIXO == 0) // MOVE CURSOR PARA ESQUERDA
	{
		posicao_tabuleiro = aloca_posicao_anterior(posicao_tabuleiro);
		HAL_Delay(150);

		if(posicao_tabuleiro < 0){

			posicao_tabuleiro = 8;
		}

		return;

	}

	else if (LE_BOTAO_CIMA == 0)
	{
		posicao_tabuleiro = confirma_posicao(posicao_tabuleiro);
		HAL_Delay(150);
	}
}

int aloca_proxima_posicao(int proximo){

	int i;

	for(i = proximo; i < 9; i++){

		if(table_game[i].valor == ' ') {

			table_game[i].valor = '?';
			break;
		}
		else if(table_game[i].valor == '?') {

			table_game[i].valor = ' ';
			i++;
			if (i < 9){

				if(table_game[i].valor == ' '){ // se a proxima posição estiver vazia

					table_game[i].valor = '?';
				}
			}
			else {
				i = 0;
			}
			break;
		}
		else{ // aposição é X ou O
			//i++;

		}
	}

	return i;
}

int aloca_posicao_anterior(int anterior){

	int i;

	for(i = anterior; i > -1; i--){

		if(table_game[i].valor == ' ') {

			table_game[i].valor = '?';
			break;
		}
		else if(table_game[i].valor == '?') {

			table_game[i].valor = ' ';
			i--;
			if (i > -1){

				if(table_game[i].valor == ' '){ // se a proxima posição estiver vazia

					table_game[i].valor = '?';
				}
			}
			else {
				i = 8;
			}
			break;
		}
	}

	return i;
}

int confirma_posicao(int posicao){


	if((table_game[posicao].valor == ' ') || (table_game[posicao].valor == '?')){

		if(jogador_atual == 1){

			table_game[posicao].valor = 'X';
		}
		else {

			table_game[posicao].valor = 'O';
		}

		posicao++;

		if(posicao >= 9){
			posicao = 0;
		}

		if (verificar_vencedor() != 0){
			// se achou vencedor, exibe resultado
			exibe_vencedor();

			//somar partida

			//delay

			// reinicia tabuleiro
		}

		alternar_jogador();



	}



	return posicao;
}



/*void exibir_tabuleiro()
{
	//ST7735_WriteString(100,15,"PLACAR", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(60,15,"Jogador 1:  \n",Font_7x10, WHITE, BLACK);
	ST7735_WriteString(60,15,"Jogador 2:  \n",Font_7x10, WHITE, BLACK);


	}*/

void tabuleirio_init (void){
	// Inicia tabuleiro
	for(int i = 0; i < 9; i++){

		table_game[i].jogador = 0;
		table_game[i].position = i;
		table_game[i].valor = ' ';

		if (table_game[i].position == 0)
		{
			table_game[i].x = 15; table_game[i].y = 2;

			table_game[i].valor = '?';
		}

		else if (table_game[i].position == 1)

		{
			table_game[i].x = 39  ; table_game[i].y = 2;
		}

		else if (table_game[i].position == 2 )

		{
			table_game[i].x = 67  ; table_game[i].y = 2;
		}

		else if (table_game[i].position == 3)

		{
			table_game[i].x = 15 ; table_game[i].y = 25;
		}
		else if (table_game[i].position == 4)

		{
			table_game[i].x = 39  ; table_game[i].y = 25;
		}

		else if (table_game[i].position == 5)
		{
			table_game[i].x = 67  ; table_game[i].y = 25;
		}

		else if (table_game[i].position == 6)
		{
			table_game[i].x = 15  ; table_game[i].y = 55;
		}

		else if (table_game[i].position == 7)
		{
			table_game[i].x = 39  ; table_game[i].y = 55;
		}

		else if (table_game[i].position == 8)
		{
			table_game[i].x = 67  ; table_game[i].y = 55;
		}
	}
}



void alternar_jogador()
{
	if (jogador_atual == 1)
	{
		jogador_atual = 2;
	}
	else
	{
		jogador_atual = 1;
	}
}



/*void jogar_partida()
{
	int jogadas = 0;
	while (1) {



		ST7735_WriteString(100,10,"Faça sua jogada.\n", Font_7x10, WHITE, BLACK);
		mover();
		efetuar_jogada( cursor_linha,  cursor_coluna);
		jogadas++;

		if (verificar_vencedor(n, jogador_atual) == 1)
		{
			if (jogador_atual == 1) {
				placar_jogador1++;
				rodadas_jogador1++;
				ST7735_WriteString(100,10,"Jogador X venceu esta rodada!", Font_7x10, WHITE, BLACK);
			} else {
				placar_jogador2++;
				rodadas_jogador2++;
				ST7735_WriteString(100,10,"Jogador O venceu esta rodada!", Font_7x10, WHITE, BLACK);
			}
			break;
		} else if (jogadas == 9)
		{
			ST7735_WriteString(100,10,"Empate!", Font_7x10, WHITE, BLACK);

			break;
		}

		alternar_jogador();
	}
}
*/
void jogar_melhor_de_tres()
{
	while (1) {
		jogar_partida();

		if (rodadas_jogador1 == 2) {
			ST7735_WriteString(100,10,"Jogador 1 venceu a partida!", Font_7x10, WHITE, BLACK);
			placar_jogador1 = 0;
			placar_jogador2 = 0;
			rodadas_jogador1 = 0;
			rodadas_jogador2 = 0;
			break;
		} else if (rodadas_jogador2 == 2) {
			ST7735_WriteString(100,10,"Jogador 2 venceu a partida!", Font_7x10, WHITE, BLACK);
			placar_jogador1 = 0;
			placar_jogador2 = 0;
			rodadas_jogador1 = 0;
			rodadas_jogador2 = 0;
			break;
		}
	}
}
void ChamaHashtag()
{

	ST7735_DrawLine(10, 50, 79, 50, WHITE);
	ST7735_DrawLine(10, 20, 79, 20, WHITE);
	ST7735_DrawLine(30, 70, 30, 5, BLUE);
	ST7735_DrawLine(60, 70, 60, 5, BLUE);
	HAL_Delay(0);
	ST7735_WriteString(100,15,"PLACAR", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(100,30,"j-1: ", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(100,60,"j-2: ", Font_7x10, WHITE, BLACK);


}

int verificar_vencedor(void)
{

	//if (table_game[0].valor == table_game[1].valor == table_game[2].valor) // verifica linha 1
	if ((table_game[0].valor == table_game[1].valor) && (table_game[0].valor == table_game[2].valor) &&
		(table_game[0].valor != ' ') && (table_game[0].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[3].valor == table_game[4].valor == table_game[5].valor) // verifica linha 2
	else if ((table_game[3].valor == table_game[4].valor) && (table_game[3].valor == table_game[5].valor)&&
			(table_game[3].valor != ' ') && (table_game[3].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[6].valor == table_game[7].valor == table_game[8].valor) // verifica linha 3
	else if ((table_game[6].valor == table_game[7].valor) && (table_game[6].valor == table_game[8].valor)&&
			(table_game[6].valor != ' ') && (table_game[6].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[0].valor == table_game[3].valor == table_game[6].valor) // verifica coluna 1
	else if ((table_game[0].valor == table_game[3].valor) && (table_game[0].valor == table_game[6].valor)&&
			(table_game[0].valor != ' ') && (table_game[0].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[1].valor == table_game[4].valor == table_game[7].valor) // verifica coluna 2
	else if ((table_game[1].valor == table_game[4].valor) && (table_game[1].valor == table_game[7].valor)&&
			(table_game[1].valor != ' ') && (table_game[1].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[2].valor == table_game[5].valor == table_game[8].valor) // verifica coluna 3
	else if ((table_game[2].valor == table_game[5].valor) && (table_game[2].valor == table_game[8].valor)&&
			(table_game[2].valor != ' ') && (table_game[2].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[0].valor == table_game[4].valor == table_game[8].valor) // verifica diagonal para direita
	else if ((table_game[0].valor == table_game[4].valor) && (table_game[0].valor == table_game[8].valor)&&
			(table_game[0].valor != ' ') && (table_game[0].valor != '?'))
	{
		return jogador_atual;
	}
	//else if (table_game[2].valor == table_game[4].valor == table_game[6].valor) // verifica diagonal para esquerda
	else if ((table_game[2].valor == table_game[4].valor) && (table_game[2].valor == table_game[6].valor)&&
			(table_game[2].valor != ' ') && (table_game[2].valor != '?'))
	{
		return jogador_atual;
	}

	return 0;
}

void exibe_vencedor(void){

	desenha_matriz();

	ST7735_DrawLine(10, 50, 79, 50, WHITE);
	ST7735_DrawLine(10, 20, 79, 20, WHITE);
	ST7735_DrawLine(30, 70, 30, 5, BLUE);
	ST7735_DrawLine(60, 70, 60, 5, BLUE);

	ST7735_WriteString(100,15,"      ", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(100,30,"      ", Font_7x10, WHITE, BLACK);
	ST7735_WriteString(100,60,"      ", Font_7x10, WHITE, BLACK);

	HAL_Delay(0);

	if(jogador_atual == 1){
		ST7735_WriteString(100,15,"X WIN", Font_7x10, GREEN, BLACK);
	} else
	{
		ST7735_WriteString(100,15,"O WIN", Font_7x10, GREEN, BLACK);
	}

	HAL_Delay(3000);

}

void desenha_matriz(){

	ChamaHashtag();

	for (int i = 0; i < 9; i++)
	{
		ST7735_WriteString(table_game[i].x, table_game[i].y,&table_game[i].valor, Font_11x18, WHITE, BLACK); // & no terceiro argumento estou passando o endereço da variavel como pede na função ST7735_WriteString
	}

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
