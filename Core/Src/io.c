#include "io.h"
#include "Communication.h"
#include "systeminit.h"
struct IO_VAR iov;
struct IO_FLAG iof;

void io_function(void)
{
	iov.current_limit = ((com_data.buf[0]-48)*1000) + ((com_data.buf[1]-48)*100) + ((com_data.buf[2]-48)*10) + ((com_data.buf[3]-48)*1);
	iov.voltage_limit = ((com_data.buf[4]-48)*1000) + ((com_data.buf[5]-48)*100) + ((com_data.buf[6]-48)*10) + ((com_data.buf[7]-48)*1);
	iov.power_limit = ((com_data.buf[8]-48)*1000) + ((com_data.buf[9]-48)*100) + ((com_data.buf[10]-48)*10) + ((com_data.buf[11]-48)*1);
	iof.enable_hc = (com_data.buf[12]-48);
	if(iov.current_limit>MAX_SETPOINT)
		iov.current_limit=MAX_SETPOINT;
	if(iov.voltage_limit>MAX_SETPOINT)
	{
		iov.voltage_limit=MAX_SETPOINT;
	}
	if(iov.power_limit>MAX_SETPOINT)
	{
		iov.power_limit=MAX_SETPOINT;
	}

	iov.current_sp = iov.current_limit;
	iov.voltage_sp = iov.voltage_limit;
	iov.power_sp = iov.power_limit;


	if(HAL_GPIO_ReadPin(DC_OK_GPIO_Port, DC_OK_Pin) == 0)
	{
		iov.dc_ok_counter++;
		if(iov.dc_ok_counter>=DC_OKAY_DELAY)
		{
			iof.dc_okay=1;
			iov.dc_ok_counter=DC_OKAY_DELAY;
			HAL_GPIO_WritePin(DC_OKAY_ERROR_LED_GPIO_Port, DC_OKAY_ERROR_LED_Pin, 0);
		}
	}
	else
	{
		iov.dc_ok_counter=0;
		iof.dc_okay=0;
		iov.current_sp=0;
		iov.voltage_sp=0;
		HAL_GPIO_WritePin(DC_OKAY_ERROR_LED_GPIO_Port, DC_OKAY_ERROR_LED_Pin, 1);
	}

	if((com_flag.error==0) & (iof.enable_hc==1) & (iof.transformer_overheat==0) & (iof.heatsink_overheat==0))
	{
		FAN1_EN;
		FAN2_EN;


		iof.fan=1;
		if(iov.pfc_counter<PFC_DELAY)
		{
			iov.pfc_counter++;
		}
		else
		{
			iof.pfc=1;
			iov.pfc_counter=PFC_DELAY;
			PFC_EN;
		}

		iof.fan_timer=0;
		iov.fan_disable_counter=0;
		if( (iof.tacho1==0) & (iof.tacho2==0) & (iof.dc_okay==1) & (iof.pfc==1) )
		{
			if(iov.inrush_counter<INRUSH_DELAY)
			{
				iov.inrush_counter++;
			}
			else
			{
				iov.inrush_counter=INRUSH_DELAY;
				INRUSH_EN;
				iof.inrush=1;
			}
			if( (iof.inrush==1) & (iov.current_sp>0) & (iov.voltage_sp>0) )
			{
				HC_EN;
				HC_LED_EN;
			}
			else
			{
				iov.current_sp=0;
				iov.voltage_sp=0;
			}

		}
		else
		{
			if(iov.dc_ok_counter>=DC_OKAY_DELAY)//why if is on delay not the flag
			{
				PFC_DIS;
			}
			INRUSH_DIS;
			HC_DIS;
			HC_LED_DIS;
			iov.voltage_sp=0;
			iov.current_sp=0;
		}
	}
	else
	{
		PFC_DIS;
		INRUSH_DIS;
		HC_DIS;
		HC_DIS;
		iof.fan_timer=0;
		iov.inrush_counter=0;
		iof.inrush=0;
		iov.pfc_counter=0;
		iof.pfc=0;
		if(iov.fan_disable_counter>FAN_DISABLE_DELAY_COUNT)
		{
			FAN1_DIS;
			FAN2_DIS;
			iof.fan=0;
			iov.fan_disable_counter=0;
		}
		else
		{
			iov.fan_disable_counter++;
		}
		iov.voltage_sp=0;
		iov.current_sp=0;
	}
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, (uint32_t)(iov.voltage_sp));
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_2, DAC_ALIGN_12B_R, (uint32_t)(iov.current_sp));
	iov.powerout = (iov.vout * iov.iout);

	if(HAL_GPIO_ReadPin(HEAT_SINK_GPIO_Port, HEAT_SINK_Pin)==1)
	{
		iof.heatsink_overheat = 1;
	}
	else
	{
		iof.heatsink_overheat = 0;
	}
	if(HAL_GPIO_ReadPin(HEAT_SINK2_GPIO_Port, HEAT_SINK2_Pin)==1)
	{
		iof.transformer_overheat = 1;
	}
	else
	{
		iof.transformer_overheat = 0;
	}

	iov.tacho1_error_count++;
	iov.tacho2_error_count++;
	if(iov.tacho1_error_count>TACHO_ERROR_COUNT)
	{
		iov.tacho1_error_count=0;
		if(iov.tacho1_counter<50 && iof.enable_hc)
	    {

			iof.tacho1=1;
			iov.tacho1_counter=0;
		}
		else if(iov.tacho1_counter>=50){
			iof.tacho1=0;
			iov.tacho1_counter=0;
		}



	}
	if(iov.tacho2_error_count>TACHO_ERROR_COUNT)
	{
		iov.tacho2_error_count=0;
		if(iov.tacho2_counter<50 && iof.enable_hc)
	    {

			iof.tacho2=1;
			iov.tacho2_counter=0;
		}
		else if(iov.tacho2_counter>=50){
			iof.tacho2=0;
			iov.tacho2_counter=0;
		}

	}
}









