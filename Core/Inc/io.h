#ifndef __IO_H__
#define __IO_H__
#include <stdint.h>

#define MAX_SETPOINT 4095
#define DC_OKAY_DELAY 2
#define PFC_DELAY 1
#define INRUSH_DELAY 1
#define FAN_DIS_TIMER 180
#define ADC_PARAMETER 5
#define ADC_SAMPLE_NUM 150
#define V25_VOLTAGE_MV 1430 //mV at 25C (typical for many families)
#define VREF_MV 3300
#define ADC_RESOLUTION 4095
#define TEMP_SLOPE 4.3f
#define REF_TEMP 25.0f

#define IO_TASK_DELAY 300
#define FAN_DISABLE_DELAY_MS 180000  //uint32_t It should be multiple of the   IO_TASK_DELAY
#define FAN_DISABLE_DELAY_COUNT FAN_DISABLE_DELAY_MS/IO_TASK_DELAY

#define TACHO_ERROR_TIME_MS 1000
#define TACHO_ERROR_COUNT TACHO_ERROR_TIME_MS/IO_TASK_DELAY

#define ADC_RIM_FREQ 72000000
#define ADC_TIM_PRE_SCALE
#define ADC_TIM_PERIODE_COUNT 10000

#define FAN1_EN HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, 1)
#define FAN2_EN HAL_GPIO_WritePin(FAN1_GPIO_Port, FAN1_Pin, 1)
#define FAN1_DIS HAL_GPIO_WritePin(FAN_GPIO_Port, FAN_Pin, 0)
#define FAN2_DIS HAL_GPIO_WritePin(FAN1_GPIO_Port, FAN1_Pin, 0)
#define PFC_EN HAL_GPIO_WritePin(PFC_EN_GPIO_Port, PFC_EN_Pin, 1)
#define PFC_DIS HAL_GPIO_WritePin(PFC_EN_GPIO_Port, PFC_EN_Pin, 0)
#define INRUSH_EN HAL_GPIO_WritePin(RELAY_INRUSH_GPIO_Port,RELAY_INRUSH_Pin,1)
#define INRUSH_DIS HAL_GPIO_WritePin(RELAY_INRUSH_GPIO_Port,RELAY_INRUSH_Pin,0)
#define HC_EN  HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 1)
#define HC_DIS HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, 0)
#define HC_LED_EN	HAL_GPIO_WritePin(ENABLE_LED_GPIO_Port,ENABLE_LED_Pin,1)
#define HC_LED_DIS	HAL_GPIO_WritePin(ENABLE_LED_GPIO_Port,ENABLE_LED_Pin,0)
struct IO_VAR
{
	uint16_t current_limit;
	uint16_t voltage_limit;
	uint16_t power_limit;
	uint16_t current_sp;
	uint16_t voltage_sp;
	uint16_t power_sp;
	uint32_t dc_ok_counter;
	uint16_t pfc_counter;
	uint16_t inrush_counter;
	uint16_t tacho1_counter;
	uint16_t tacho2_counter;
	uint16_t powerout;
	float iout;
	float vout;
	float pot;
	float cputemp;
	float vbat;
	uint16_t adcbuf[ADC_PARAMETER * ADC_SAMPLE_NUM];
	uint16_t fan_disable_counter;
	uint16_t tacho1_error_count;
	uint16_t tacho2_error_count;
};
struct IO_FLAG
{
	uint8_t enable_hc;
	uint8_t dc_okay:1;
	uint8_t fan:1;
	uint8_t pfc:1;
	uint8_t fan_timer:1;
	uint8_t tacho1:1;
	uint8_t tacho2:1;
	uint8_t inrush:1;
	uint8_t heatsink_overheat:1;
	uint8_t transformer_overheat:1;
};

#endif
extern struct IO_VAR iov;
extern struct IO_FLAG iof;
void io_function(void);
