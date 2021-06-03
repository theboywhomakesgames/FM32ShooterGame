/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2020 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"
#include "stm32f3xx.h"
#include "stm32f3xx_it.h"

/* USER CODE BEGIN 0 */
#include <LiquidCrystal.h>
#include <stdlib.h>
#include <string.h>
extern void printAt(int r, int c, char str[]);
extern void writeAt(int r, int c, int charIndex);
extern UART_HandleTypeDef huart1;

int r, c, lr, lc;
int timerCount = 0;
int timerDivider = 20;
char map[20][4];
char lmap[20][4];
int randdd = 0;
int reverseDifficulity = 50;
int speed = 0, speedChangeBase = 20, speedChange = 20, speedAv, score;
int paused = 1, isInMenu = 1, menuPointerIndex = 0, mustPrint = 1, isInAbout = 0, isInSaveMenu = 0;
int fuelAmount = 1, fuelDeathCount = 0, isPlaneOff = 0;
int updateCounter = 0, lvl = 0;
char data[200],name[10];
int time;


int getRand(ADC_HandleTypeDef hadc2){
	HAL_ADC_Start(&hadc2);
	return HAL_ADC_GetValue(&hadc2)%100;
}

int getADCValue(ADC_HandleTypeDef hadc){
	HAL_ADC_Start(&hadc);
	int adcNum = HAL_ADC_GetValue(&hadc);
	//char data[20];
	//sprintf(data, "%d", adcNum);
	//printAt(0, 0, data);
	return adcNum;
}

void saveMenu(){
	if(!isInMenu){
		paused = 1;
		isInMenu = 1;
		isInSaveMenu = 1;
		mustPrint = 1;
	}
}

void encode(){
	//char data[200];
	int p = 0;
	for(int i = 0; i < 4 ;i++){
		for(int j = 0; j < 20; j++){
			data[p] = map[i][j];
			p++;
		}
	data[p++] = '%';
	}
	// map

	int i = 0;
	while(i < strlen(name)){
		data[p] = name[i];
		i++;
		p++;
	}
	data[p++] = '%';
	// name
    char buf[20];

    sprintf(buf,"%d",fuelAmount);
    strcat(data,buf);
    p+=strlen(buf);
	data[p++] = '%';
	// fuelamount

	sprintf(buf,"%d",lvl);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// level

	sprintf(buf,"%d",speed);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// speed


	sprintf(buf,"%d",60-updateCounter);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// remaining length of level

	sprintf(buf,"%d",c);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	sprintf(buf,"%d",r);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// plane position

    sprintf(buf,"%d",time);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// time

	sprintf(buf,"%d",score);
	strcat(data,buf);
	p+=strlen(buf);
	data[p++] = '%';
	// score

}

void decode(){
    int p=0,q=0;
    char buf[20];
    for(int i=0;i<4;i++){
        for(int j=0;j<20;j++){
            map[i][j]=data[p++];
        }
        p++;
    }

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    strcpy(name,buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    fuelAmount=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    lvl=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    speed=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    updateCounter = 60 - atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    c=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    r=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    time=atoi(buf);

    while(data[p]!='%'){
        buf[q]=data[p];
        p++;
        q++;
    }
    p++;
    q=0;
    score=atoi(buf);

}

void save(){
		char data[256] = "";
		for(int i = 0; i < 20; i++){
			for(int j = 0; j < 4; j++){
				data[i*4 + j] = map[i][j];
				if(data[i*4 + j] == ' '){
					data[i*4 + j] = '-';
				}
			}
		}
		HAL_UART_Transmit(&huart1, data, sizeof(data), 100);	
}

void load(){
	
}

void resetGame(){	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 20; j++){
				map[j][i] = ' ';
				lmap[j][i] = ' ';
			}
		}
		clear();
		c = 0;
		r = 2;
		speedChange = speedChangeBase;
		fuelAmount = 1;
		isPlaneOff = 0;
		fuelDeathCount = 0;
}

void genNewLine(ADC_HandleTypeDef hadc2){
	int number_ = 0;
	for(int i = 0; i < 4; i++){
		char cc = ' ';
		int randNum = getRand(hadc2);
		randNum%=reverseDifficulity;
		
		//number_ *= 10;
		//number_ += randNum;
		
		switch(randNum){
			case 0: cc = 'm'; break;
			case 1: cc = 'b'; break;
			case 2: cc = 'f'; break;
			case 33: cc = 'f'; break;
			case 38: cc = 'f'; break;
			case 41: cc = 'f'; break;
			case 47: cc = 'f'; break;
		}
		map[19][i] = cc;
		
		//char data[20];
		//sprintf(data, "%d", number_);
		//printAt(0, 0, data);
	}
}

void showFuel(){
	
	
	
	if(fuelAmount>0) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, 0);
	
	if(fuelAmount>1) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
	
	if(fuelAmount>2) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, 0);
	
	if(fuelAmount>3) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
	
	if(fuelAmount>4) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, 0);
	
	if(fuelAmount>5) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
	
	if(fuelAmount>6) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, 0);
	
	if(fuelAmount>7) HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 1);
	else HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, 0);
}

void slowDown(){
	speedChange = speedChangeBase;
	fuelAmount --;
	if(fuelAmount <= 0){
		fuelAmount = 0;
		isPlaneOff = 1;
	}
}

void lose(){
	paused = 1;
	clear();
	printAt(2, 3, "Game Over");
}
	
void fuelUp(){
	fuelAmount ++;
	if(fuelAmount > 8) fuelAmount = 8;
	showFuel();
}

void checkMap(){
	switch(map[c][r]){
		case 'm':
			lose();
			map[c][r] = ' ';
			break;
		case 'b':
			slowDown();
			map[c][r] = ' ';
			break;
		case 'f':
			fuelUp();
			map[c][r] = ' ';
			break;
	}
}

void printSpaceShip(){
	if(!paused){
		checkMap();
		if(lr != r || lc != c)
			printAt(lr, lc, " ");
		writeAt(r, c, 0);
	}
}

void printMapTest(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 20; j++){
			char data[20];
			sprintf(data, "%c", map[j][i]);
			printAt(i, j, data);
		}
	}
}

void printMap(){
	if(!paused){
		int characterIndex = -1;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 20; j++){
				if(i == r && j == c) continue;
				switch(lmap[j][i]){
					case 'm':
						characterIndex = 1;
						break;
					case 'b':
						characterIndex = 3;
						break;
					case 'f':
						characterIndex = 2;
						break;
					default :
						characterIndex = -1;
						break;
				}
				if(characterIndex != -1)
					printAt(i, j, " ");
			}
		}	
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 20; j++){
				if(i == r && j == c) continue;
				switch(map[j][i]){
					case 'm':
						characterIndex = 1;
						break;
					case 'b':
						characterIndex = 3;
						break;
					case 'f':
						characterIndex = 2;
						break;
					default :
						characterIndex = -1;
						break;
				}
				if(characterIndex != -1)
					writeAt(i, j, characterIndex);
			}
		}
	}
}

void scrollMap(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 20; j++){
			lmap[j][i] = map[j][i];
		}
	}
	for(int k = 0; k < 19; k++){
		for(int i = 0; i < 4; i++){
			map[k][i] = map[k+1][i];
		}	
	}
}

void saveLastPos(){
	lc = c;
	lr = r;
}

void goLeft(){
	if(!isInMenu){
		saveLastPos();
		r--;
		if(r > 3) r = 3;
		if(r < 0) r = 0;
		printSpaceShip();
	}
}

void goRight(){
	if(!isInMenu){
		saveLastPos();
		r++;
		if(r > 3) r = 3;
		if(r < 0) r = 0;
		printSpaceShip();
	}
}

void goUp(){
	if(!isInMenu){
		saveLastPos();
		c++;
		if(c > 19) c = 19;
		if(c < 0) c = 0;
		printSpaceShip();
	}
	else{
		printAt(menuPointerIndex, 0, " ");
		menuPointerIndex--;
		if(menuPointerIndex < 0) menuPointerIndex = 2;
		menuPointerIndex %= 3;
		mustPrint = 1;
	}
}

void goDown(){	
	if(!isInMenu){
		saveLastPos();
		c--;
		if(c > 19) c = 19;
		if(c < 0) c = 0;
		printSpaceShip();
	}
	else{
		printAt(menuPointerIndex, 0, " ");
		menuPointerIndex++;
		menuPointerIndex %= 3;
		mustPrint = 1;
	}
}

void showSpeed(){
	int a[10],n,i;
	speed = 100 - timerDivider;
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
	
	// 0
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, 0);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 1);
	
	HAL_Delay(1);
	
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
	// 1
	n = speed%10;
	speed /=10;
	for(i=0;i<10;i++){
		a[i] = 0;
	}
	for(i=0;n>0;i++)    
	{    
		a[i]=n%2;
		n=n/2;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, a[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, a[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, a[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, a[3]);
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0);
	
	HAL_Delay(1);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
	// 2
	n = speed%10;
	speed /=10;
	for(i=0;i<10;i++){
		a[i] = 0;
	}
	for(i=0;n>0;i++)    
	{    
		a[i]=n%2;
		n=n/2;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, a[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, a[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, a[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, a[3]);
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0);
	
	HAL_Delay(1);
	
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
	// 3
	n = speed%10;
	speed /=10;
	for(i=0;i<10;i++){
		a[i] = 0;
	}
	for(i=0;n>0;i++)    
	{    
		a[i]=n%2;
		n=n/2;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, a[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, a[1]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, a[2]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, a[3]);
	
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_6, 0);
}

void shoot() {
	
}
/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/******************************************************************************/
/*            Cortex-M4 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles EXTI line0 interrupt.
*/
void EXTI0_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI0_IRQn 0 */
	
  /* USER CODE END EXTI0_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
  /* USER CODE BEGIN EXTI0_IRQn 1 */
	if(paused){
		if(isInMenu){
			if(isInAbout){
				isInAbout = 0;
				mustPrint = 1;
			}
			else
			{
				switch(menuPointerIndex){
					case 0:
						isInMenu = 0;
						resetGame();
						break;
					case 1:
						break;
					case 2:
						isInAbout = 1;
						mustPrint = 1;
						break;
				}
			}
		}
		else{
			resetGame();
			paused = 0;
		}
	}
	else{
		if(isPlaneOff && fuelAmount > 0){
			isPlaneOff = 0;
			fuelDeathCount = 0;
			speedChange = speedChangeBase;
		}
	}
	while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)){};
  /* USER CODE END EXTI0_IRQn 1 */
}

/**
* @brief This function handles EXTI line1 interrupt.
*/
void EXTI1_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI1_IRQn 0 */

  /* USER CODE END EXTI1_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
  /* USER CODE BEGIN EXTI1_IRQn 1 */
	for(int i = 0; i < 4; i++){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		
		if(i == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)){
				// 1
			}
		}
		else if(i == 1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)){
				// 4
				goLeft();
			}
		}
		else if(i == 2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)){
				// 7
			}
		}
		else if(i == 3){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)){
				// *
			}
		}
		HAL_Delay(10);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1));
  /* USER CODE END EXTI1_IRQn 1 */
}

/**
* @brief This function handles EXTI line2 and Touch Sense controller.
*/
void EXTI2_TSC_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI2_TSC_IRQn 0 */

  /* USER CODE END EXTI2_TSC_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
  /* USER CODE BEGIN EXTI2_TSC_IRQn 1 */
	for(int i = 0; i < 4; i++){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		
		if(i == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
				// 2
				goUp();
			}
		}
		else if(i == 1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
				// 5
				shoot();
			}
		}
		else if(i == 2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
				// 8
				goDown();
			}
		}
		else if(i == 3){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)){
				// 0
			}
		}
		HAL_Delay(10);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2));
  /* USER CODE END EXTI2_TSC_IRQn 1 */
}

/**
* @brief This function handles EXTI line3 interrupt.
*/
void EXTI3_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI3_IRQn 0 */

  /* USER CODE END EXTI3_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);
  /* USER CODE BEGIN EXTI3_IRQn 1 */
	for(int i = 0; i < 4; i++){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		
		if(i == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
				// 3
			}
		}
		else if(i == 1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
				// 6
				goRight();
			}
		}
		else if(i == 2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
				// 9
			}
		}
		else if(i == 3){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)){
				// #
			}
		}
		HAL_Delay(10);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3));
  /* USER CODE END EXTI3_IRQn 1 */
}

/**
* @brief This function handles ADC1 and ADC2 interrupts.
*/
void ADC1_2_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_2_IRQn 0 */

  /* USER CODE END ADC1_2_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc1);
  HAL_ADC_IRQHandler(&hadc2);
  /* USER CODE BEGIN ADC1_2_IRQn 1 */
  /* USER CODE END ADC1_2_IRQn 1 */
}

/**
* @brief This function handles EXTI line[9:5] interrupts.
*/
void EXTI9_5_IRQHandler(void)
{
  /* USER CODE BEGIN EXTI9_5_IRQn 0 */

  /* USER CODE END EXTI9_5_IRQn 0 */
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
  /* USER CODE BEGIN EXTI9_5_IRQn 1 */
	for(int i = 0; i < 4; i++){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		
		if(i == 0){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)){
				// A
			}
		}
		else if(i == 1){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)){
				// B
			}
		}
		else if(i == 2){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)){
				// C
			}
		}
		else if(i == 3){
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
			if(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7)){
				// D
				saveMenu();
			}
		}
		HAL_Delay(10);
	}
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	while(HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_7));
  /* USER CODE END EXTI9_5_IRQn 1 */
}

/**
* @brief This function handles TIM2 global interrupt.
*/
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */
	if(!paused){
		if(timerCount++ % timerDivider == 0){
			if(updateCounter++ % 60 == 0){
				updateCounter = 1;
				reverseDifficulity -= 5;
				lvl++;
				speedAv /= 60;
				score += speedAv;
				speedAv = 0;
				if(lvl == 5){ 
					clear();
					printAt(0,0,"You won");
					paused = 1;
				}
				fuelAmount--;
				int fa = fuelAmount;
				resetGame();
				fuelAmount = fa;
			}
			if(isPlaneOff){
				fuelDeathCount++;
				if(fuelDeathCount > 5) lose();
			}
			scrollMap();
			genNewLine(hadc2);
			printMap();
			printSpaceShip();
			showFuel();
			timerCount = 1;
			timerDivider = (100 - lvl*10) + speedChange - getADCValue(hadc1) + fuelDeathCount * 10;
			speedAv += 200 - timerDivider;
			if(speedChange > 0){
				speedChange -= 5;
				if(speedChange < 0) speedChange = 0;
			}
			//checkMap();
			//printMap();
			//scrollMap();
			c %= 20;
		}
	}
	else{
		if(isInMenu && mustPrint){
			clear();
			if(isInAbout){
				printAt(0, 0, "@sinar77 @mamatosen on github");
			}
			else{
				if(isInSaveMenu){
					save();
					isInSaveMenu = 0;
					isInMenu = 0;
					paused = 0;
				}
				else{
					printAt(0, 1, "New Game");
					printAt(1, 1, "Load Game");
					printAt(2, 1, "About");
					writeAt(menuPointerIndex, 0, 0);
				}
			}
			mustPrint = 0;
		}
	}
  /* USER CODE END TIM2_IRQn 1 */
}

/**
* @brief This function handles TIM3 global interrupt.
*/
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */

  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */
	showSpeed();
  /* USER CODE END TIM3_IRQn 1 */
}

/**
* @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
*/
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */
	
  /* USER CODE END USART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
