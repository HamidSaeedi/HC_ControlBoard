#include "systeminit.h"
#include "io.h"

void sysinit_function(void)
{
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_1);
	HAL_DAC_Start(&hdac1, DAC_CHANNEL_2);

	COM_MODE_RX
	HAL_UART_Receive_IT(&huart2,(uint8_t*)&com_data.rec_buf, 16);

	HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED);


	HAL_ADC_Start_DMA(&hadc1,iov.adcbuf,(uint32_t) (ADC_PARAMETER * ADC_SAMPLE_NUM));
	HAL_TIM_Base_Start(&htim3);
	iov.tacho1_counter=50;
	iov.tacho2_counter=50;


}
