/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED_Animation.h"
#include "OLED_Driver.h"
#include "OLED_GFX.h"
#include "OLED_FFT.h"
#include "sys.h"
#include "sk6812.h"
#include "motion.h"
#include "stmflash.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

uint8_t Flag_Refrash = False;
uint8_t Flag_Blink = False;
uint8_t Flag_Sleep = False;
uint8_t Flag_Continue = False;
uint8_t Flag_Reception = True;
uint8_t OfflineCount = 6;
uint8_t SystemActive = True;
u8 datatemp[256] = {};
u16 RandomX=30,RandomY=30;

#define Radius 60
#define RadiusC 56
#define RadiusB 53
#define HourRadius 30
#define MinRadius 40
#define SecRadius 50
u16 ColorPointer[3];
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SLEEPTIME    20*60

OLED_GFX oled = OLED_GFX();
OLED_FFT fft = OLED_FFT();
OLED_Animation motion = OLED_Animation();
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

u16 Display_Mode = MODE_OFFLINE;
u16 Current_Mode = MODE_OFFLINE;
u16 Sleep_Mode;

#define BOOTBMP Logo_asus

int Count=0;
int FPS=0;

u8 Dataleng;
char DataDis[20];
char DataDisf[3];

u8 DataDisType=0;
u8 UnitDisType=0;
u8 HWDisType=0;
u8 Show_Float=False;

void CopyString(char *ch1,char *ch2)
{
	int i;
	for(i=0;i<cont_str(ch2);i++)
		ch1[i]=ch2[i];
}


void NORMAL_Mode_In(void)
{
	SetCurrent(POSNBAR,148) ;
	SetCurrent(POSNBAT,170) ;
	SetTarget(POSNRCT,25);
	SetTarget(POSNTOP,0);
	SetTarget(POSNBAR,103);
	SetTarget(POSNBAT,115);
	SetTarget(DAMPTYP,0);
}

void NORMAL_Mode_Out(void)
{
	SetTarget(POSNRCT,-73) ;
	SetTarget(POSNTOP,60) ;
	SetTarget(POSNBAR,148) ;
	SetTarget(POSNBAT,170) ;
	SetTarget(DAMPTYP,0);
	DampCustormPos(POSNBAT,0.1);
}


void TIME_Mode_In(void)
{
	SetCurrent(RDRadius, Radius*2);
	SetCurrent(RDRadiusC, RadiusC*2);
	SetCurrent(RDRadiusB, RadiusB*2);
	SetTarget(RDRadius, Radius);
	SetTarget(RDRadiusC, RadiusC);
	SetTarget(RDRadiusB, RadiusB);
	SetTarget(RDHourRadius, HourRadius);
	SetTarget(RDMinRadius, MinRadius);
	SetTarget(RDSecRadius, SecRadius);
	SetTarget(RDRuleDelaut, 0);
	if(Device_Msg.uarthour%12<6)
	{
		SetTarget(PNTTIME,22);
	}
	else
	{
		SetTarget(PNTTIME,22+60);
		if(pit[PNTTIME].target!=22+60)
			SetCurrent(PNTTIME,128+50);
	}
}

void TIME_Mode_Out(void)
{
	SetTarget(RDRadius, Radius*2);
	SetTarget(RDRadiusC, RadiusC*2);
	SetTarget(RDRadiusB, RadiusB*2);
	SetTarget(RDRuleDelaut, 180);
	if(Display_Mode != MODE_GAME)
	{
		SetTarget(RDHourRadius, 0);
		SetTarget(RDMinRadius, 0);
		SetTarget(RDSecRadius, 0);
		SetTarget(PNTHOUR,-90);
		SetTarget(PNTMIN,-90);
		SetTarget(PNTSEC,-90);
		if(pit[PNTTIME].current<60)
		{
			SetTarget(PNTTIME,-50);
		}
		else
		{
			SetTarget(PNTTIME,128+50);
		}
	}
	DampCustormPos(RDRadius,0.1);
}

void TIMESQ_Mode_In(void)
{
	SetCurrent(RDRadiusA, Radius);
	SetTarget(RDRadiusA, Radius/2);
	SetTarget(RDHourRadius, HourRadius);
	SetTarget(RDMinRadius, MinRadius);
	SetTarget(RDSecRadius, SecRadius);
	if(Device_Msg.uarthour%12<6)
	{
		SetTarget(PNTTIME,22);
	}
	else
	{
		SetTarget(PNTTIME,22+60);
		if(pit[PNTTIME].target!=22+60)
			SetCurrent(PNTTIME,128+50);
	}
}
	
void TIMESQ_Mode_Out(void)
{
	SetTarget(RDRadiusA, Radius);
	if(Display_Mode != MODE_DATE)
	{
		SetTarget(RDHourRadius, 0);
		SetTarget(RDMinRadius, 0);
		SetTarget(RDSecRadius, 0);
		SetTarget(PNTHOUR,-90);
		SetTarget(PNTMIN,-90);
		SetTarget(PNTSEC,-90);
		if(pit[PNTTIME].current<60)
		{
			SetTarget(PNTTIME,-50);
		}
		else
		{
			SetTarget(PNTTIME,128+50);
		}
	}
	DampCustormPos(RDRadiusA,0.1);
}

void MUSIC_Mode_In(void)
{
	int i;
	for(i=0;i<sizeof(fall_pot);i++)
	{
		fall_pot[i] = 128;
		flow_pot[i] = 128;
	}
	ClearFFT();
}

void MUSIC_Mode_Out(void)
{
//	int i;
//	for(i=0;i<sizeof(fall_pot);i++)
//	{
//		fall_pot[i] = 128;
//		flow_pot[i] = 128;
//	}
	ClearFFT();
}


void NORMAL_Mode_Run(void)
{ 
	switch(DataDisType)
	{
		case 3:
			if(Device_Msg.cpufan>0)
			{
				
				SetCurrent(POSNRCD,0+8) ;
				SetTarget(POSNRCD,0+8);
				SetTarget(CRTVANE,Device_Msg.cpufan);
				HWDisType = CU_CPU;
				Show_Float = False;
				sprintf(DataDis,"%04d",(u16)pit[CRTVANE].target);
				UnitDisType=UT_RPM;
				pit[CRTVADS].target = pit[CRTVANE].target/50/80*118;
				
			}
			else
			{
				DataDisType++;return;
//				sprintf(DataDis,"----");
//				UnitDisType=UT_RPM;
//				pit[CRTVANE].target = 0;
			}
			break;
		case 0:
			if(Device_Msg.cputemp>0)
			{
				SetTarget(POSNRCD,11+8)  ;
				pit[CRTVANE].target = Device_Msg.cputemp/10;
				HWDisType = CU_CPU;
				Show_Float = True;
				sprintf(DataDis,"%02d",(u16)pit[CRTVANE].target);
				sprintf(DataDisf,"%01d",Device_Msg.cputemp%10);
				UnitDisType=UT_DEG;
				
				if(pit[CRTVANE].target<=80)
					pit[CRTVADS].target = pit[CRTVANE].target/80*118;
				else
					pit[CRTVADS].target = 117;
				
			}
			else
			{
				DataDisType++;return;
			}
			break;
		case 1:
			if(Device_Msg.cpuclock>0)
			{
				SetCurrent(POSNRCD,0+8) ;
				SetTarget(POSNRCD,0+8)  ;
				pit[CRTVANE].target = Device_Msg.cpuclock;
				HWDisType = CU_CPU;
				Show_Float = False;
				sprintf(DataDis,"%04d",(u16)pit[CRTVANE].target);
				UnitDisType=UT_MHZ;
				pit[CRTVADS].target = pit[CRTVANE].target/50/80*118;
				
			}
			else
			{
				DataDisType++;return;
//				sprintf(DataDis,"----");
//				UnitDisType=UT_MHZ;
//				pit[CRTVANE].target = 0;
			}
			break;
		case 2:
			SetTarget(POSNRCD,11+8)  ;
			pit[CRTVANE].target = Device_Msg.cpuload/10;
			HWDisType = CU_CPU;
			Show_Float = True;
//			if(Device_Msg.cpuload>0)
			{
				sprintf(DataDis,"%02d",(u16)pit[CRTVANE].target);
				sprintf(DataDisf,"%01d",Device_Msg.cpuload%10);
				UnitDisType=UT_PREC;
				if(pit[CRTVANE].target<=80)
					pit[CRTVADS].target = pit[CRTVANE].target/80*118;
				else
					pit[CRTVADS].target = 117;
				
			}break;
		case 7:
			if(Device_Msg.gpufan>0)
			{
				SetCurrent(POSNRCD,0+8) ;
				SetTarget(POSNRCD,0+8) ;
				pit[CRTVANE].target = Device_Msg.gpufan;
				HWDisType = CU_GPU;
				Show_Float = False;
				sprintf(DataDis,"%04d",(u16)pit[CRTVANE].target);
				UnitDisType=UT_RPM;
				pit[CRTVADS].target = pit[CRTVANE].target/50/80*118;
				
			}
			else
			{
				DataDisType++;return;
//				sprintf(DataDis,"----");
//				UnitDisType=UT_RPM;
//				pit[CRTVANE].target = 0;
			}
			break;
		case 4:
			if(Device_Msg.gputemp>0)
			{
				SetTarget(POSNRCD,11+8) ;
				pit[CRTVANE].target = Device_Msg.gputemp/10;
				HWDisType = CU_GPU;
				Show_Float = True;
				sprintf(DataDis,"%02d",(u16)pit[CRTVANE].target);
				sprintf(DataDisf,"%01d",Device_Msg.gputemp%10);
				UnitDisType=UT_DEG;
				pit[CRTVADS].target = pit[CRTVANE].target/80*118;
				
			}
			else
			{
				DataDisType++;return;
			}
			break;
		case 5:
			if(Device_Msg.gpuclock>0)
			{
				SetCurrent(POSNRCD,0+8) ;
				SetTarget(POSNRCD,0+8) ;
				pit[CRTVANE].target = Device_Msg.gpuclock;
				HWDisType = CU_GPU;
				Show_Float = False;
				sprintf(DataDis,"%04d",(u16)pit[CRTVANE].target);
				UnitDisType=UT_MHZ;
				pit[CRTVADS].target = pit[CRTVANE].target/50/80*118;
				
			}
			else
			{
				DataDisType++;return;
//				sprintf(DataDis,"----");
//				UnitDisType=UT_MHZ;
//				pit[CRTVANE].target = 0;
			}
			break;
		case 6:
			SetTarget(POSNRCD,11+8);
			pit[CRTVANE].target = Device_Msg.gpuload/10;
			HWDisType = CU_GPU;
			Show_Float = True;
//			if(Device_Msg.gpuload>0)
			{
				sprintf(DataDis,"%02d",(u16)pit[CRTVANE].target);
				sprintf(DataDisf,"%01d",Device_Msg.gpuload%10);
				UnitDisType=UT_PREC;
				if(pit[CRTVANE].target<=80)
					pit[CRTVADS].target = pit[CRTVANE].target/80*118;
				else
					pit[CRTVADS].target = 117;
				
			}break;
		case 8:
			SetTarget(POSNRCD,11+8);
			pit[CRTVANE].target = Device_Msg.ramload/10;
			HWDisType = CU_RAM;
			Show_Float = True;
//			if(Device_Msg.gpuload>0)
			{
				sprintf(DataDis,"%02d",(u16)pit[CRTVANE].target);
				sprintf(DataDisf,"%01d",Device_Msg.ramload%10);
				UnitDisType=UT_PREC;
				if(pit[CRTVANE].target<=80)
					pit[CRTVADS].target = pit[CRTVANE].target/80*118;
				else
					pit[CRTVADS].target = 117;
				
			}break;
	}
}


void TIME_Mode_Run()
{
	pit[PNTHOUR].target = (Device_Msg.uarthour%12-3)*30+Device_Msg.uartminute/2;
	pit[PNTMIN].target = (Device_Msg.uartminute-15)*6+Device_Msg.uartsecond/2/5;
	pit[PNTSEC].target = (Device_Msg.uartsecond-15)*6;
	if(pit[PNTHOUR].target == -90)
		pit[PNTHOUR].target = 270;
	else if(pit[PNTHOUR].current == 270)
		pit[PNTHOUR].current = -90;
	if(pit[PNTMIN].target == -90)
		pit[PNTMIN].target = 270;
	else if(pit[PNTMIN].current == 270)
		pit[PNTMIN].current = -90;
	if(pit[PNTSEC].target == -90)
		pit[PNTSEC].target = 270;
	else if(pit[PNTSEC].current == 270)
	{
		pit[RDHourColor].target = oled.WheelPw(rand()%96);
		pit[RDMinColor].target = oled.WheelPw(rand()%96);
		pit[RDSecColor].target = oled.WheelP(rand()%96);
		
		pit[PNTSEC].current = -90;
	}

//	pit[PNTTIME].target = 52+(pit[RDHourRadius].current*cos((pit[PNTSEC].current+180)*PI/180));
//	pit[PNTTIMEY].target = 60+(pit[RDHourRadius].current*sin((pit[PNTSEC].current+180)*PI/180));
	if(Device_Msg.uarthour%12<6)
	{
		SetTarget(PNTTIME,22);
	}
	else
	{
		SetTarget(PNTTIME,22+60);
	}
}


void Mode_In(void)
{
	switch(Current_Mode)
	{
		case MODE_DATE:TIME_Mode_In();break;
		case MODE_NORMAL:NORMAL_Mode_In();break;
		case MODE_GAME:TIMESQ_Mode_In();break;
		case MODE_MUSIC:MUSIC_Mode_In();break;
	}
}


void Mode_Out(void)
{
	switch(Current_Mode)
	{
		case MODE_DATE:TIME_Mode_Out();break;
		case MODE_NORMAL:NORMAL_Mode_Out();break;
		case MODE_GAME:TIMESQ_Mode_Out();break;
		case MODE_MUSIC:MUSIC_Mode_Out();break;
	}
}

static u16 PulseRun = 4060 ,dir = True;
void TIMESQ_Mode();
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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM8_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  HAL_UART_Receive_DMA(&huart1,Uart_Recv1_Buf,Uart_Max_Length);
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_TIM_Base_Start_IT(&htim7);
	HAL_TIM_Base_Start_IT(&htim8);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
  oled.Device_Init();
	SK6812_Init();
	InitData();
	motion.OLED_AllMotion_Init();
	Recvcmd();
//	if(Display_Mode!=MODE_OFFLINE)
	{
		UsartCommand(0xA001,3);//获取命令
		UsartCommand(0xA002,3);//获取设备名
		UsartCommand(0xA003,3);//获取硬盘信息
	}
//	HAL_GPIO_TogglePin(GPIOB, SYS_LED_Pin);
//	HAL_Delay(100);
//	HAL_GPIO_TogglePin(GPIOB, SYS_LED_Pin);
//	HAL_Delay(100);
//	HAL_GPIO_TogglePin(GPIOB, SYS_LED_Pin);
//	HAL_Delay(500);
//	
	if(Display_Mode!=MODE_OFFLINE)
	{
		UsartPrint("Nubla is online\n");
		UsartCommand(0xA001,3);//获取命令
		OFFLINE_Mode();
	}
	else
	{
		printf("Nubla is online\r\n");
	}
	SetTarget(RDHourColor, oled.WheelPw(rand()%96));
	SetTarget(RDMinColor, oled.WheelPw(rand()%96));
	SetTarget(RDSecColor, oled.WheelP(rand()%96));
	
//	
//	Display_Mode = MODE_DATE;
//	Current_Mode = MODE_DATE;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		switch(Current_Mode)
		{
			case MODE_DEFALUT:NORMAL_Mode();break;
			case MODE_DATE:TIME_Mode();break;
			case MODE_NORMAL:NORMAL_Mode();break;
			case MODE_GAME:OFFLINE_Mode();break;
			case MODE_OFFLINE:OFFLINE_Mode();break;
//			case MODE_OFFLINE:SLEEP_Mode();break;
			case MODE_MUSIC:MUSIC_Mode();break;
			default:Current_Mode = Display_Mode;break;
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void OFFLINEUI_Init(void)
{
	switch(Device_Cmd.commandlogotheme)
	{
		case 0:oled.Display_FadeinAll(Logo_asus);break;
		case 1:oled.Display_FadeinAll(Logo_msi);break;
		case 2:oled.Display_FadeinAll(Logo_amd);break;
		case 3:oled.Display_FadeinAll(Logo_radeon);break;
		case 4:oled.Display_FadeinAll(Logo_nvidia);break;
		case 5:oled.Display_FadeinAll(Logo_custorm);break;
		default:oled.Display_FadeinAll(Logo_msi);break;
	}
}

void OFFLINEUI_Out(void)
{
	oled.Display_FadeoutAll();
}

void OFFLINE_Mode(void)
{
	OFFLINEUI_Init();
	if(!(Device_Cmd.commandset&0x8)&&OfflineCount>6)
	{
		oled.Clear_Screen();
	}
	while(Display_Mode == MODE_OFFLINE||Display_Mode == MODE_DEFALUT)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;
			if(Flag_Sleep)
			{
				Flag_Sleep = False;
				SLEEP_Mode();
			}
			if(Device_Cmd.commandset&0x8)
				switch(Device_Cmd.commandlogotheme)
				{
					case 0:oled.Display_bmp(Logo_asus);break;
					case 1:oled.Display_bmp(Logo_msi);break;
					case 2:oled.Display_bmp(Logo_amd);break;
					case 3:oled.Display_bmp(Logo_radeon);break;
					case 4:oled.Display_bmp(Logo_nvidia);break;
					case 5:oled.Display_bmp(Logo_custorm);break;
					default:oled.Display_bmp(Logo_msi);break;
				}
			else
			{
				oled.Clear_Screen();
				motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			}
			oled.Refrash_Screen();
		}
		HAL_Delay(1);
	}
	Current_Mode = Display_Mode;
	Mode_In();
	if(Display_Mode != MODE_GAME)
	{
		UsartCommand(0xA001,3);//获取命令
		UsartCommand(0xA002,3);//获取设备名
		UsartCommand(0xA003,3);//获取硬盘信息
		UsartPrint("Nubla is online\n");
	}
	oled.Display_FadeoutAll();
}

void NORMAL_Display(void)
{
	if(Device_Cmd.commandtoptheme<=6)
		oled.Display_bmp(pit[POSNTOP].current,pit[POSNRCT].current-25,128,25,Corn_Top[Device_Cmd.commandtoptheme]);
	else
		oled.Display_bmp(pit[POSNTOP].current,pit[POSNRCT].current-25,128,25,Corn_Top[5]);
		
	oled.Display_bbmp(0,pit[POSNRCT].current,128,73,BMP_DataBackGround);
	oled.Display_bbmp(89,pit[POSNRCT].current+69,35,5,Corn_BarUnit[UnitDisType],color_half);
	oled.OLED_SF22x40(pit[POSNRCD].current,11+pit[POSNRCT].current,DataDis);
	if(pit[DAMPTYP].match)
		oled.Display_bbmp(10,pit[POSNRCT].current+62,23,7,Corn_DataType[HWDisType],color_half);
	else
		oled.Display_bbmp(10,pit[POSNRCT].current+62,23,7,Corn_DataType[HWDisType]);
	oled.Display_bbmp(98,11+pit[POSNRCT].current,24,24,Corn_Unit[UnitDisType],color_half);
	if((Show_Float)&&pit[CRTVANE].target < 100)
	{
		oled.OLED_SF12x24(pit[POSNRCD].current+52,pit[POSNRCT].current+27,DataDisf,color_half);
		oled.Fill_Rect(pit[POSNRCD].current+46,pit[POSNRCT].current+45,2,4,color_half);
	}
	oled.Display_bbmp(0,pit[POSNBAT].current,128,5,Corn_Rule);
	oled.Display_bbmp(0,pit[POSNBAT].current+5,128,5,Corn_RuleIndex,color_half);
	oled.Fill_Rect(3+2,pit[POSNBAR].current+2,pit[CRTVADS].current,6,color_half);
	oled.Draw_Rect(3,pit[POSNBAR].current,121,10);
}

void NORMAL_Mode(void)
{
	if(Flag_Refrash)
	{
		Flag_Refrash = False;
		if(Flag_Sleep)
		{
			Flag_Sleep = False;
			SLEEP_Mode();
		}
		
		DampAutoPos();
		if(Display_Mode == Current_Mode)
		{
			NORMAL_Mode_Run();
		}
		else
		{
			Mode_Out();
			
			if(pit[POSNBAT].match)
			{
				Current_Mode = Display_Mode;
				Mode_In();
			}
		}
		oled.Clear_Screen();
		motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		NORMAL_Display();	
		oled.Refrash_Screen();
	}
	HAL_Delay(1);
}

void TIME_Display()
{
	int i;
	for(i=0;i<60;i++)
	{
		if(i%5==0)
			oled.Draw_Line((OCX+(pit[RDRadius].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadius].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCX+(pit[RDRadiusB].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadiusB].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),color_half);
		else
			oled.Draw_Line((OCX+(pit[RDRadius].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadius].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCX+(pit[RDRadiusC].current*cos((i*6+pit[RDRuleDelaut].current)*PI/180))),(OCY+(pit[RDRadiusC].current*sin((i*6+pit[RDRuleDelaut].current)*PI/180))),color_min);
	}
	oled.Draw_Circle(OCX,OCY,pit[RDRadius].current+2,color_half);
	oled.Draw_Circle(OCX,OCY,pit[RDRadius].current);

	
	oled.OLED_SNF6x8(pit[PNTTIME].current,60,Device_Str.vtime);
	
	oled.Draw_Line((OCX+(pit[RDHourRadius].current*cos(pit[PNTHOUR].current*PI/180))),(OCY+(pit[RDHourRadius].current*sin(pit[PNTHOUR].current*PI/180))),OCX,OCY,2,pit[RDHourColor].target);
	oled.Draw_Line((OCX+(pit[RDMinRadius].current*cos(pit[PNTMIN].current*PI/180))),(OCY+(pit[RDMinRadius].current*sin(pit[PNTMIN].current*PI/180))),OCX,OCY,pit[RDMinColor].target);
	oled.Draw_Line((OCX+(pit[RDSecRadius].current*cos(pit[PNTSEC].current*PI/180))),(OCY+(pit[RDSecRadius].current*sin(pit[PNTSEC].current*PI/180))),OCX,OCY,pit[RDSecColor].target);

	oled.Fill_Circle(OCX,OCY,3,0);
	oled.Draw_Circle(OCX,OCY,1);
	oled.Draw_Circle(OCX,OCY,3,color_half);
}

void TIME_Mode(void)
{
	if(Flag_Refrash)
	{
		Flag_Refrash = False;
		if(Flag_Sleep)
		{
			Flag_Sleep = False;
			SLEEP_Mode();
		}
		
		DampAutoPos();
		if(Display_Mode == Current_Mode)
		{
			TIME_Mode_Run();
		}
		else
		{
			Mode_Out();
			if(pit[RDRadius].match)
			{
				Current_Mode = Display_Mode;
				Mode_In();
			}
		}
		oled.Clear_Screen();
		motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		TIME_Display();
		oled.Refrash_Screen();
	}
	HAL_Delay(1);
}
	

#define RLV Radius/2

void TIMESQ_Display(void)
{
	oled.Draw_Circle(OCX+RLV,OCY+RLV,pit[RDRadiusA].current,1,color_now);
	oled.Draw_Circle(OCX+RLV,OCY+RLV,pit[RDRadiusA].current+2,1,color_half); //右下
	
	oled.Draw_Circle(OCX-RLV,OCY+RLV,pit[RDRadiusA].current,2,color_now);
	oled.Draw_Circle(OCX-RLV,OCY+RLV,(pit[RDRadiusA].current+2),2,color_half);	//左下
	
	oled.Draw_Circle(OCX-RLV,OCY-RLV,pit[RDRadiusA].current,4,color_now);
	oled.Draw_Circle(OCX-RLV,OCY-RLV,(pit[RDRadiusA].current+2),4,color_half); //左上
	
	oled.Draw_Circle(OCX+RLV,OCY-RLV,pit[RDRadiusA].current,3,color_now);
	oled.Draw_Circle(OCX+RLV,OCY-RLV,(pit[RDRadiusA].current+2),3,color_half); //右上
	
	oled.Draw_Line(OCX+RLV+pit[RDRadiusA].current,OCY+RLV,OCX+RLV+pit[RDRadiusA].current,OCY-RLV,color_now);//右侧引导线
	oled.Draw_Line(OCX+RLV+(pit[RDRadiusA].current+2),OCY+RLV,OCX+RLV+(pit[RDRadiusA].current+2),OCY-RLV,color_half);//右侧引导线
	
	oled.Draw_Line(OCX-RLV-pit[RDRadiusA].current,OCY+RLV,OCX-RLV-pit[RDRadiusA].current,OCY-RLV,color_now);//左侧引导线
	oled.Draw_Line(OCX-RLV-(pit[RDRadiusA].current+2),OCY+RLV,OCX-RLV-(pit[RDRadiusA].current+2),OCY-RLV,color_half);//左侧引导线

	oled.OLED_SNF6x8(pit[PNTTIME].current,60,Device_Str.vtime);
	oled.Draw_Line(pit[PNTTIME].current-1,60+9,pit[PNTTIME].current+30,60+9,color_half);
	
	oled.Draw_Line((OCX+(pit[RDHourRadius].current*cos(pit[PNTHOUR].current*PI/180))),(OCY+(pit[RDHourRadius].current*sin(pit[PNTHOUR].current*PI/180))),OCX,OCY,2,pit[RDHourColor].target);
	oled.Draw_Line((OCX+(pit[RDMinRadius].current*cos(pit[PNTMIN].current*PI/180))),(OCY+(pit[RDMinRadius].current*sin(pit[PNTMIN].current*PI/180))),OCX,OCY,pit[RDMinColor].target);
	oled.Draw_Line((OCX+(pit[RDSecRadius].current*cos(pit[PNTSEC].current*PI/180))),(OCY+(pit[RDSecRadius].current*sin(pit[PNTSEC].current*PI/180))),OCX,OCY,pit[RDSecColor].target);

	oled.Fill_Circle(OCX,OCY,3,0);
	oled.Draw_Circle(OCX,OCY,1);
	oled.Draw_Circle(OCX,OCY,3,color_half);
}

void TIMESQ_Mode(void)
{
	if(Flag_Refrash)
	{
		Flag_Refrash = False;
		if(Flag_Sleep)
		{
			Flag_Sleep = False;
			SLEEP_Mode();
		}
		
		DampAutoPos();
		if(Display_Mode == Current_Mode)
		{
			TIME_Mode_Run();
		}
		else
		{
			Mode_Out();
			if(pit[RDRadiusA].match)
			{
				Current_Mode = Display_Mode;
				Mode_In();
			}
		}
		oled.Clear_Screen();
		motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		TIMESQ_Display();
		oled.Refrash_Screen();
	}
	HAL_Delay(1);
}

void MUSIC_Display()	
{
	static u8 Currentstyle = 0;
	int i;
	if(Currentstyle!=Device_Cmd.commandstyle)
	{
		Currentstyle=Device_Cmd.commandstyle;
		
		for(i=0;i<sizeof(fall_pot);i++)
		{
			fall_pot[i] = 128;
			flow_pot[i] = 128;
		}
	}
//		TIM3->CCR2=4000-Device_Msg.leftvol*4000/(256);
	switch(Device_Cmd.commandstyle)
	{
		case 1:fft.Display_Style1();break;
		case 2:fft.Display_Style2();break;
		case 3:fft.Display_Style3();break;
		case 4:fft.Display_Style4();break;
		case 5:fft.Display_Style5();break;
		case 6:fft.Display_Style6();break;
		default:fft.Display_Style1();break;
	}
}

void MUSIC_Mode(void)
{
	static int OutCount = 25;
	if(Flag_Refrash)
	{
		Flag_Refrash = False;
		if(Flag_Sleep)
		{
			Flag_Sleep = False;
			SLEEP_Mode();
		}
		
		DampAutoPos();
		if(Display_Mode == Current_Mode)
		{
//			MUSIC_Mode_Run();
		}
		else
		{
			Mode_Out();
			if(OutCount--<=0)
			{
				OutCount = 25;
				Current_Mode = Display_Mode;
				Mode_In();
			}
		}
		oled.Clear_Screen();
		motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
		MUSIC_Display();
		oled.Refrash_Screen();
	}
	HAL_Delay(1);
}

void SLEEP_Mode(void)
{
	u16 i;
	for(i=0;i<300;)
	{
		if(Flag_Refrash)
		{
			Flag_Refrash = False;i++;
			oled.Clear_Screen();
			motion.OLED_AllMotion(Device_Cmd.commandmotion,Device_Cmd.commandspeed);
			oled.Refrash_Screen();
		}
	}
	oled.Clear_Screen();
	Current_Mode = Sleep_Mode;
	UsartCommand(0xA001,3);//获取命令
	UsartCommand(0xA002,3);//获取设备名
	UsartCommand(0xA003,3);//获取硬盘信息
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static u16 SleepCount = 0;
	static u16 ContinueCount = 0;
	static u16 fps = 0;
	static u16 Temp = 0;
	static u16 TimeRun = 0;
	if (htim->Instance == htim8.Instance)
	{
		SK6812_Wheel(0,Temp+=5);
		SK6812_Refrash();
		Flag_Refrash = True;
		if(PulseRun>4020)
			dir = True;
		else if(PulseRun<50)
			dir = False;
			
		if(dir)
			TIM3->CCR2=(PulseRun-=50);
		else
			TIM3->CCR2=(PulseRun+=50);
	}
	if (htim->Instance == htim6.Instance)
	{
		if(ContinueCount++>20)
		{
			ContinueCount = 0;
			HAL_TIM_Base_Stop_IT(&htim6);
		}
	}
	if (htim->Instance == htim7.Instance)
	{
		if(TimeRun++>40)
		{
			TimeRun = 0;
			DataDisType++;
			pit[DAMPTYP].current = 30;
			if(DataDisType>=8)
				DataDisType=0;
		}
		switch(Device_Cmd.commandrgbmode)
		{
			case 1:oled.Set_Wheelf(fps%96);break;
			case 2:oled.Set_Wheel(fps%96);break;
			case 3:oled.Set_Wheel(Device_Cmd.commandrgbcolor*96/256);break;
			default:oled.Set_Wheel(Device_Cmd.commandrgbcolor*96/256);break;
		}
		
		if(fps++%2)
		{
//		oled.Set_DampColor(rand()&0xffff);
//			printf("fps:%d\r\n",fps);
			FPS=0;
		}
		if(OfflineCount < 1)
			OfflineCount++;
		else if(OfflineCount == 2)
		{
			ClearFFT();
			OfflineCount++;
		}
		else if(OfflineCount == 8)
		{
			InitData();
			Display_Mode = MODE_OFFLINE;
			OfflineCount++;
//			Display_Mode = MODE_OFFLINE;
//			OfflineCount++;
		}
		else if(OfflineCount <10)
		{
			OfflineCount++;
		}
		
		if(Uart_Overflow1_Flag)
		{
//			HAL_GPIO_TogglePin(GPIOB, SYS_LED_Pin);
			Uart_Overflow1_Flag = False;
		}
		if(Uart_Overflow3_Flag)
		{
			Uart_Overflow3_Flag = False;
		}
		if(SleepCount++>SLEEPTIME*2)
		{
			if(Device_Cmd.commandset&0x4)
			{
				Flag_Sleep = True;
				Sleep_Mode = Current_Mode;
				SleepCount = 0;
			}
			else
			{
				SleepCount = 0;
			}
		}
		if(SaveFlag && SystemActive)
		{
			SaveFlag = False;
			Tranfcmd();
		}
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
