/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include <string.h>
#include "sys.h"
#include "main.h"
#include "motion.h"
#include <stdio.h>
#include "stmflash.h"
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
DMA_HandleTypeDef hdma_usart3_rx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}
/* USART3 init function */

void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_HIGH;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspInit 0 */

  /* USER CODE END USART3_MspInit 0 */
    /* USART3 clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 DMA Init */
    /* USART3_RX Init */
    hdma_usart3_rx.Instance = DMA1_Channel3;
    hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart3_rx.Init.Mode = DMA_NORMAL;
    hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart3_rx);

  /* USER CODE BEGIN USART3_MspInit 1 */

  /* USER CODE END USART3_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
  else if(uartHandle->Instance==USART3)
  {
  /* USER CODE BEGIN USART3_MspDeInit 0 */

  /* USER CODE END USART3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* USART3 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
int std::fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
	return 1;
}


uint8_t Nix2Num(uint8_t Input)
{
	switch(Input)
	{
		case 0x3f:return 0;
		case 0x06:return 1;
		case 0x5b:return 2;
		case 0x4f:return 3;
		case 0x66:return 4;
		case 0x6d:return 5;
		case 0x7d:return 6;
		case 0x07:return 7;
		case 0x7f:return 8;
		case 0x6f:return 9;
		default:return 0xff;
	}
}

CLOCK_MSG Clock_Msg;
DEVICE_CMD Device_Cmd;
DEVICE_MSG Device_Msg;
DEVICE_STR Device_Str;
const char *Week[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

void ConvertData(void)
{
	if(Device_Msg.cputemp)
	{
	sprintf(Device_Str.cputemp,"%02d",Device_Msg.cputemp/10);
	sprintf(Device_Str.cputempf,"%1d",Device_Msg.cputemp%10);
	}
	if(Device_Msg.cpuclock)
	sprintf(Device_Str.cpuclock,"%dMhz ",Device_Msg.cpuclock);
	sprintf(Device_Str.cpuload,"%02.1f%%  ",(double)Device_Msg.cpuload/10);
	if(Device_Msg.cpufan)
	sprintf(Device_Str.cpufan,"%dRpm ",Device_Msg.cpufan);
	
	if(Device_Msg.gputemp)
	sprintf(Device_Str.gputemp,"%02d.%1d$ ",Device_Msg.gputemp/10,Device_Msg.gputemp%10);
	if(Device_Msg.gpuclock)
	sprintf(Device_Str.gpuclock,"%dMhz ",Device_Msg.gpuclock);
	sprintf(Device_Str.gpuload,"%02.1f%%  ",(double)Device_Msg.gpuload/10);
	if(Device_Msg.gpufan)
	sprintf(Device_Str.gpufan,"%dRpm ",Device_Msg.gpufan);
	
	if(Device_Msg.maintemp)
	sprintf(Device_Str.maintemp,"%03.1f$ ",(double)Device_Msg.maintemp/10);
	if(Device_Msg.mainfan)
	sprintf(Device_Str.mainfan,"%dRpm ",Device_Msg.mainfan);
	
	sprintf(Device_Str.ramload,"%02.1f%% ",(double)Device_Msg.ramload/10);
	
	if(Device_Msg.ramusrdata)
	sprintf(Device_Str.ramusrdata,"%d",Device_Msg.ramusrdata);
	
	if(Device_Msg.cpufan)	
	sprintf(Device_Str.ncpufan,"%04d",Device_Msg.cpufan);
	if(Device_Msg.cpuclock)
	sprintf(Device_Str.ncpuclock,"%04d",Device_Msg.cpuclock);
	if(Device_Msg.gpufan)
	sprintf(Device_Str.ngpufan,"%04d",Device_Msg.gpufan);
	if(Device_Msg.gputemp)
	sprintf(Device_Str.ngputemp,"%02d.%1d",Device_Msg.gputemp/10,Device_Msg.gputemp%10);
	
	
	if(Device_Msg.cputemp)
		sprintf(Device_Str.vcputemp,"%03d#",Device_Msg.cputemp);	
	if(Device_Msg.gputemp)
		sprintf(Device_Str.vgputemp,"%03d#",Device_Msg.gputemp);	
	
		
	sprintf(Device_Str.vtime,"%02d:%02d",Device_Msg.uarthour,Device_Msg.uartminute);	
	sprintf(Device_Str.vhour,"%02d",Device_Msg.uarthour);	
	sprintf(Device_Str.vmin,"%02d",Device_Msg.uartminute);	
	sprintf(Device_Str.vsec,"%02d",Device_Msg.uartsecond);	
	sprintf(Device_Str.date,"%s. %02d-%02d",Week[Device_Msg.uartweek],Device_Msg.uartmonth,Device_Msg.uartday);	
}

void InitData(void)
{
	sprintf(Device_Str.cputemp,"--.-$ ");
	sprintf(Device_Str.cpuclock,"----Mhz ");
	sprintf(Device_Str.cpuload,"--.-%%  ");
	sprintf(Device_Str.cpufan,"----Rpm ");
	
	sprintf(Device_Str.gputemp,"--.-$ ");
	sprintf(Device_Str.gpuclock,"----Mhz ");
	sprintf(Device_Str.gpuload,"--.-%%  ");
	sprintf(Device_Str.gpufan,"----Rpm ");
	
	sprintf(Device_Str.maintemp,"--.-$ ");
	sprintf(Device_Str.mainfan,"----Rpm ");
	
	sprintf(Device_Str.ramload,"--.-%% ");
	sprintf(Device_Str.ramusrdata,"----");
	
	sprintf(Device_Str.ngputemp,"--.-");	
	sprintf(Device_Str.ncpuclock,"----");	
	sprintf(Device_Str.ngpufan,"----");	
	sprintf(Device_Str.ncpufan,"----");	
	
	
	sprintf(Device_Str.vtime,"--:--");
	sprintf(Device_Str.vhour,"--");	
	sprintf(Device_Str.vmin,"--");	
	sprintf(Device_Str.vsec,"--");	
	sprintf(Device_Str.date,"--Unkown--");	

	memset(&Device_Msg,0,sizeof(Device_Msg));
}

void AnalysisMiStaus(uint8_t Data)
{
//	printf("%x\r\n",Data);
	if(Data&8)
		Clock_Msg.tmsys = 0;
	else if(Data&0x20)
		Clock_Msg.tmsys = 1;
	if(Data&1)
		Clock_Msg.cation = True;
	else
		Clock_Msg.cation = False;
	if(Data&2)
		Clock_Msg.mute = True;
	else
		Clock_Msg.mute = False;
	if(Data&4)
		Clock_Msg.alarm = True;
	else
		Clock_Msg.alarm = False;
	if(Data&0x10)
		Clock_Msg.wifi = True;
	else 
		Clock_Msg.wifi = False;
}

void Judge_Mode()
{
//	static u8 normalcount=6,gamecount=6;
//	if(Device_Msg.gpuload>800)
//	{
//		gamecount ++;
//		if(gamecount>=10)
//		{
//			gamecount = 10;Display_Mode = MODE_GAME;
//			normalcount = 0;
//		}
//	}
//	else if(Device_Msg.gpuload<200)
//	{
//		normalcount ++;
//		if(normalcount>=10)
//		{
//			normalcount = 10;
//			gamecount = 0;
//		}
//	}
	Display_Mode = MODE_NORMAL;
}
uint8_t Uart_Recv1_Buf[Uart_Max_Length] = {0};
uint16_t Uart_Recv1_Length = 0;
uint8_t Uart_Overflow1_Flag = False;

char Device_Name[20] = "Nebula\0\0\0\0\0\0";

u8 ReponseID[40]={0xFF,0x55,'O','K'};

void AnalysisComputermsg(uint8_t *Buf)
{
	int i;
	if(Buf[0] == 0xFF &&Buf[1] == 0x55)
	{
		if(Buf[4] == 0x2)
		{
			switch (MAKEWORD(Buf[3], Buf[2]))
			{
			case CPU_Temp:
				Device_Msg.cputemp = MAKEWORD(Buf[6],Buf[5]);
				break;
			case CPU_Clock:
				Device_Msg.cpuclock = MAKEWORD(Buf[6],Buf[5]);
				break;
			case CPU_Load:
				Device_Msg.cpuload = MAKEWORD(Buf[6],Buf[5]);
				break;
			case CPU_Fan:
				Device_Msg.cpufan = MAKEWORD(Buf[6],Buf[5]);
				break;
			case CPU_POWER:
				Device_Msg.cpupower = MAKEWORD(Buf[6],Buf[5]);
				break;

			case GPU_Temp:
				Device_Msg.gputemp = MAKEWORD(Buf[6],Buf[5]);
				break;
			case GPU_Clock:
				Device_Msg.gpuclock = MAKEWORD(Buf[6],Buf[5]);
				break;
			case GPU_Load:
				Device_Msg.gpuload = MAKEWORD(Buf[6],Buf[5]);
				break;
			case GPU_Fan:
				Device_Msg.gpufan = MAKEWORD(Buf[6],Buf[5]);
				break;


			case Main_Temp:
				Device_Msg.maintemp = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Main_Fan:
				Device_Msg.mainfan = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Main_Vol:
				Device_Msg.mainvoltage = MAKEWORD(Buf[6],Buf[5]);
				break;

			case RAM_Load:
				Device_Msg.ramload = MAKEWORD(Buf[6],Buf[5]);
				break;
			case RAM_Data:
				Device_Msg.ramusrdata = MAKEWORD(Buf[6],Buf[5]);
				break;

			case HDD_Load:
				Device_Msg.hddload = MAKEWORD(Buf[6],Buf[5]);
				break;
			case HDD_Temp:
				Device_Msg.hddtemp = MAKEWORD(Buf[6],Buf[5]);
				break;

			case Uart_Year:
				Device_Msg.uartyear = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Month:
				Device_Msg.uartmonth = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Day:
				Device_Msg.uartday = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Week:
				Device_Msg.uartweek = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Hour:
				Device_Msg.uarthour = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Minute:
				Device_Msg.uartminute = MAKEWORD(Buf[6],Buf[5]);
				break;
			case Uart_Second:
				Device_Msg.uartsecond = MAKEWORD(Buf[6],Buf[5]);
				break;
			case End_Frame_ADDR:
				if(MAKEWORD(Buf[6],Buf[5]) == 0x5A5A)
				{
					ConvertData();		
					Uart_Overflow1_Flag = True;
					if(Device_Cmd.commandmode == 1) 
						Judge_Mode();
					else if(Display_Mode == MODE_OFFLINE)
						Display_Mode = MODE_DEFALUT;
				}break;
			case 0x3F3F:
				STMFLASH_Read(FLASH_NAME_ADDR,(u16*)Device_Name,20);
				ReponseID[2] = (Buf[5] & 0x5A) | (Buf[6] & 0xA5);
				ReponseID[3] = (Buf[5] ^ 0x57) | (Buf[6] & 0x57);
				ReponseID[4] = cont_str(Device_Name);
				for(i=0;i<ReponseID[4];i++)
					ReponseID[5+i] = Device_Name[i];
				HAL_UART_Transmit_IT(&huart1,ReponseID,ReponseID[4]+5);break;
			}
		}
	}
}

u8 SaveFlag = False;

void AnalysisCommand(uint8_t *Buf)
{
	int i;
	if(Buf[0] == 0xFF &&Buf[1] == 0x55)
	{
//		if(Buf[4] == 0x2)
		{
			switch (MAKEWORD(Buf[3], Buf[2]))
			{
				case Command_Mode:
//					if(Device_Cmd.commandmode != MAKEWORD(Buf[6],Buf[5]))
//					{
						Device_Cmd.commandmode = MAKEWORD(Buf[6],Buf[5]); 
						if(Device_Cmd.commandmode>1) Display_Mode = Device_Cmd.commandmode;
//					}
				break;
				case Command_Motion:
					if(Device_Cmd.commandmotion != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandmotion = MAKEWORD(Buf[6],Buf[5]);
						SaveFlag = True;
					}break;
				case Command_Style:
					if(Device_Cmd.commandstyle != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandstyle = MAKEWORD(Buf[6],Buf[5]);
//						SaveFlag = True;
					}break;
				case Command_Speed:
					if(Device_Cmd.commandspeed != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandspeed = MAKEWORD(Buf[6],Buf[5]);
//						SaveFlag = True;
					}break;
				case Command_Brightness:
					if(Device_Cmd.commandbrightness != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandbrightness = MAKEWORD(Buf[6],Buf[5]);
//						SaveFlag = True;
					}break;
				case Command_Set:
					if(Device_Cmd.commandset != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandset = MAKEWORD(Buf[6],Buf[5]);
						SaveFlag = True;
					}break;
				case Command_RGBMode:
					if(Device_Cmd.commandrgbmode != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandrgbmode = MAKEWORD(Buf[6],Buf[5]);
						SaveFlag = True;
					}break;
				case Command_RGBBrightness:
					if(Device_Cmd.commandrgbbrightness != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandrgbbrightness = MAKEWORD(Buf[6],Buf[5]);
						SaveFlag = True;
					}break;
				case Command_RGBColor:
					if(Device_Cmd.commandrgbcolor != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandrgbcolor = MAKEWORD(Buf[6],Buf[5]);
						SaveFlag = True;
					}break;
				case Command_TOPTHEME:
					if(Device_Cmd.commandtoptheme != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandtoptheme = MAKEWORD(Buf[6],Buf[5]);
						if(Device_Cmd.commandtoptheme<2)
							printf("Current top %d\r\n",Device_Cmd.commandtoptheme);
						else
							printf("Current top 1\r\n");
						SaveFlag = True;
					}break;
				case Command_LOGOTHEME:
					if(Device_Cmd.commandlogotheme != MAKEWORD(Buf[6],Buf[5]))
					{
						Device_Cmd.commandlogotheme = MAKEWORD(Buf[6],Buf[5]);
						if(Device_Cmd.commandlogotheme<6)
							printf("Current logo %d\r\n",Device_Cmd.commandlogotheme);
						else
							printf("Current logo 5\r\n");
						SaveFlag = True;
					}break;
				case Command_DEVICENAME:
					memset(&Device_Name,0,sizeof(Device_Name));
					
					for(i=0;i<Buf[4];i++) Device_Name[i] = Buf[i + 5];
					STMFLASH_Write(FLASH_NAME_ADDR,(u16*)Device_Name,20);
					break;
			}
		}
	}
}

void AnalysisComputername(uint8_t *Buf)
{
	u8 i;
	if(Buf[0] == 0xFF &&Buf[1] == 0x55)
	{
		if(Buf[4] > 0x2)
		{
			switch (MAKEWORD(Buf[3], Buf[2]))
			{
			case 0x101:for(i=0;i<Buf[4];i++) Device_Msg.cpuname[i] = Buf[i + 5];break;
			case 0x102:for(i=0;i<Buf[4];i++) Device_Msg.gpuname[i] = Buf[i + 5];break;
			case 0x103:for(i=0;i<Buf[4];i++) Device_Msg.mainname[i] = Buf[i + 5];break;
			}
		}
	}
}


void ClearFFT(void)
{
	int i;
	for(i=0;i<Uart_FFT_Length;i++) 
		Device_Msg.fft[i] = 0;
	
	Device_Msg.leftvol = 0;
	Device_Msg.rightvol = 0;
}


void AnalysisFFT(uint8_t *Buf)
{
	u16 i;
	if(Buf[0] == 0xFF &&Buf[1] == 0x55)
	{
		if(Buf[4] >= 0x2)
		{
			switch (MAKEWORD(Buf[3], Buf[2]))
			{
				case FFT_Data:for(i=0;i<Buf[4];i++) Device_Msg.fft[i] = Buf[i + 5];
				if(Device_Cmd.commandmode == 1) 
					Display_Mode = MODE_MUSIC;
				else if(Display_Mode == MODE_OFFLINE)
					Display_Mode = MODE_MUSIC;
					break;
				case Left_Vol:
					Device_Msg.leftvol = MAKEWORD(Buf[6],Buf[5]);break;
				case Right_Vol:
					Device_Msg.rightvol = MAKEWORD(Buf[6],Buf[5]);break;
			}
			Uart_Overflow1_Flag = True;
		}
	}
}

u8 Flash_SaveCMD[32];
void Tranfcmd(void)
{
	Flash_SaveCMD[0] = Device_Cmd.commandmode>>8;
	Flash_SaveCMD[1] = Device_Cmd.commandmode&0xff;
	
	Flash_SaveCMD[2] = Device_Cmd.commandmotion>>8;
	Flash_SaveCMD[3] = Device_Cmd.commandmotion&0xff;
	
	Flash_SaveCMD[4] = Device_Cmd.commandstyle>>8;
	Flash_SaveCMD[5] = Device_Cmd.commandstyle&0xff;
	
	Flash_SaveCMD[6] = Device_Cmd.commandspeed>>8; 
	Flash_SaveCMD[7] = Device_Cmd.commandspeed&0xff;
	
	Flash_SaveCMD[8] = Device_Cmd.commandbrightness>>8;
	Flash_SaveCMD[9] = Device_Cmd.commandbrightness&0xff;
	
	Flash_SaveCMD[10] = Device_Cmd.commandset>>8;
	Flash_SaveCMD[11] = Device_Cmd.commandset&0xff;
	
	Flash_SaveCMD[12] = Device_Cmd.commandrgbmode>>8; 
	Flash_SaveCMD[13] = Device_Cmd.commandrgbmode&0xff;
	
	Flash_SaveCMD[14] = Device_Cmd.commandrgbcolor>>8;
	Flash_SaveCMD[15] = Device_Cmd.commandrgbcolor&0xff;
	
	Flash_SaveCMD[16] = Device_Cmd.commandrgbbrightness>>8;
	Flash_SaveCMD[17] = Device_Cmd.commandrgbbrightness&0xff;
	
	Flash_SaveCMD[18] = Device_Cmd.commandtoptheme>>8;
	Flash_SaveCMD[19] = Device_Cmd.commandtoptheme&0xff;
	
	Flash_SaveCMD[20] = Device_Cmd.commandlogotheme>>8;
	Flash_SaveCMD[21] = Device_Cmd.commandlogotheme&0xff;
//	SPI_Flash_Write(Flash_SaveCMD,SAVE_FLASH,32);
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_SaveCMD,32);
}
#define VERIF 0xA5

void Recvcmd(void)
{
//	SPI_Flash_Read(Flash_SaveCMD,SAVE_FLASH,32);
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Flash_SaveCMD,32);
	Device_Cmd.commandmotion = MAKEWORD(Flash_SaveCMD[3],Flash_SaveCMD[2]);
	Device_Cmd.commandstyle = MAKEWORD(Flash_SaveCMD[5],Flash_SaveCMD[4]);
	Device_Cmd.commandspeed = MAKEWORD(Flash_SaveCMD[7],Flash_SaveCMD[6]);
	Device_Cmd.commandbrightness = MAKEWORD(Flash_SaveCMD[9],Flash_SaveCMD[8]);
	Device_Cmd.commandset = MAKEWORD(Flash_SaveCMD[11],Flash_SaveCMD[10]);

//	Device_Cmd.commandmode = MAKEWORD(Flash_SaveCMD[13],Flash_SaveCMD[12]);
	Device_Cmd.commandrgbcolor = MAKEWORD(Flash_SaveCMD[15],Flash_SaveCMD[14]);
	Device_Cmd.commandrgbbrightness = MAKEWORD(Flash_SaveCMD[17],Flash_SaveCMD[16]);
	Device_Cmd.commandtoptheme = MAKEWORD(Flash_SaveCMD[19],Flash_SaveCMD[18]);
	Device_Cmd.commandlogotheme = MAKEWORD(Flash_SaveCMD[21],Flash_SaveCMD[20]);
	if(Flash_SaveCMD[31]!=VERIF)
	{
		Flash_SaveCMD[31] = VERIF;
		STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_SaveCMD,32);
		STMFLASH_Write(FLASH_NAME_ADDR,(u16*)Device_Name,20);
	}
}

u8 Uart_Recv3_Data;
u8 Uart_Overflow3_Flag = False;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	static uint8_t Command,Data;
	static uint8_t Uart3_Busy = False;
	if(huart->Instance == USART3)   // 判断是由哪个串口触发的中断
	{
//		printf("Uart3:%d\r\n",Uart_Recv3_Data);
		if(!Uart3_Busy)
		{
			if((Uart_Recv3_Data&0x1F) == 0x8)
			{
				Command = Uart_Recv3_Data;
				Uart3_Busy = True;
			}
		}
		else
		{
			if(Command == 0x28)
			{
				if(!Uart_Recv3_Data)
					Clock_Msg.aiwork = True;
				else
				{
					Clock_Msg.aiwork = False;
					AnalysisMiStaus(Uart_Recv3_Data);
				}
			}
			else
			{
				Data = Nix2Num(Uart_Recv3_Data&0x7F);
				if(Data!=0x7F)
				switch(Command)
				{
					case 0x08:if(Data<2) {Clock_Msg.hour&=0xF;	Clock_Msg.hour|= Data<<4;}break;
					case 0x88:Clock_Msg.hour&=0xF0; Clock_Msg.hour|= Data;break;
					case 0x48:if(Data<6) {Clock_Msg.minute&=0xF;  Clock_Msg.minute |= Data<<4;}break;
					case 0xC8:Clock_Msg.minute&=0xF0; Clock_Msg.minute |= Data;break;
					
				}
			}
			Uart_Overflow3_Flag = True;
			Uart3_Busy = False;
			Command = 0;
		}
		HAL_UART_Receive_IT(&huart3,&Uart_Recv3_Data,1);      // 重新使能串口1接收中断
	}
}
u8 Uartdata[7]={0xFF,0x55,0,0,0x02,0,0};

void UsartCommand(u16 Addr,u16 Data)
{
	Uartdata[2] = (Addr>>8)&0xFF;
	Uartdata[3] = Addr&0xFF;
	
	Uartdata[5] = (Data>>8)&0xFF;
	Uartdata[6] = Data&0xFF;
	HAL_Delay(10);
	HAL_UART_Transmit(&huart1,Uartdata,7, 0xffff);
}

int cont_str(char *s)
{
	int i = 0;      
	while ( s[++i] != '\0')   ;
	return i;
}
u8 UartPrint[255]={0xFF,0x55,0xC0,0x01,0,0,0};
void UsartPrint(char *Data)
{
	u8 i,Length;
	Length = cont_str(Data) ;
	UartPrint[4] = Length;
	for(i=0;i<Length;i++)
		UartPrint[5+i] = Data[i];
	HAL_Delay(10);
	HAL_UART_Transmit(&huart1,UartPrint,Length+5, 0xffff);
}

u8 UartClear[]={0xFF,0x55,0xC0,0x02};
void UsartClear(void)
{
	HAL_UART_Transmit(&huart1,UartClear,5, 0xffff);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
