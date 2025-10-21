/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Communication.h"
#include "io.h"
#include "systeminit.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void comunication_function(void const * argument);
void io_task(void const * argument);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
osThreadId communicationTaskHandle;
osThreadId ioTaskHandle;
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(communicationTask,comunication_function,osPriorityNormal,0,128);
  communicationTaskHandle = osThreadCreate(osThread(communicationTask),NULL);

  osThreadDef(ioTask,io_task,osPriorityNormal,0,128);
  ioTaskHandle = osThreadCreate(osThread(ioTask),NULL);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(100);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void comunication_function(void const * argument)
{
	//HAL_UART_Receive_IT(&huart2,(uint8_t*)uart_buf, 1);
	while(1)
	{
		com_data_parser();
		osDelay(150);
	}
}

void io_task(void const * argument)
{
	while(1)
	{
		io_function();
		osDelay(300);
		HAL_GPIO_TogglePin(PING_GPIO_Port, PING_Pin);
	}

}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	uint32_t sumV=0,sumI=0,sumPot=0,sumT=0,sumVbat=0;
	float temp_V_mv=0;
	uint16_t i=0;
	HAL_TIM_Base_Stop(&htim3);
	for(i=0;i<(ADC_PARAMETER * ADC_SAMPLE_NUM);i+=ADC_PARAMETER)
	{
		sumV   += iov.adcbuf[i];
		sumI   += iov.adcbuf[i+1];
		sumPot += iov.adcbuf[i+2];
		sumT   += iov.adcbuf[i+3];
		sumVbat+= iov.adcbuf[i+4];
	}
	iov.vout  = sumV   / (float) ADC_SAMPLE_NUM;
	iov.iout  = sumI   / (float) ADC_SAMPLE_NUM;
	iov.pot   = sumPot / (float) ADC_SAMPLE_NUM;
	temp_V_mv = sumT   / (float) ADC_SAMPLE_NUM;
	temp_V_mv = (temp_V_mv*VREF_MV) / ADC_RESOLUTION;
	iov.cputemp  = ((V25_VOLTAGE_MV-temp_V_mv)/TEMP_SLOPE)+REF_TEMP;
	iov.vbat  = sumVbat/ (float) ADC_SAMPLE_NUM;
	iov.vbat = (iov.vbat * (VREF_MV/1000))/ADC_RESOLUTION;
	//HAL_ADC_Start_DMA(&hadc1,iov.adcbuf, ADC_PARAMETER * ADC_SAMPLE_NUM);
	HAL_TIM_Base_Start(&htim3);

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin==GPIO_PIN_6)
	{
		iov.tacho1_counter++;
		if(iov.tacho1_counter>=50)
		{
			iof.tacho1=0;
			iov.tacho1_counter=0;
		}

	}
	if(GPIO_Pin==GPIO_PIN_13)
	{
		iov.tacho2_counter++;
		if(iov.tacho2_counter>=50)
		{
			iof.tacho2=0;
			iov.tacho2_counter=0;
		}

	}
}





/* USER CODE END Application */

