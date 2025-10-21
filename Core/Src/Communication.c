#include "Communication.h"
#include "io.h"
struct COM_FLAG com_flag;
struct COM_DATA com_data;
uint8_t mycount=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
   com_flag.newdata=1;
   mycount++;
   if(mycount>29)
	   mycount=0;
   HAL_UART_Receive_IT(&huart2,(uint8_t*)&com_data.rec_buf, 16);
}
void com_data_parser(void)
{
	char Vout_char[5];
	char Iout_char[5];
	uint8_t i=0;
	if(com_flag.newdata==1)
	{
		/*
		com_flag.newdata=0;
		switch (com_flag.state)
		{
			case START:
				if(com_data.rec_buf==SLAVE_ADDR)
				{
					com_flag.state = ADDR_RESOLVED;
				}
			break;
			case ADDR_RESOLVED:
				if(com_data.rec_buf==HEADER)
				{
					com_flag.state = DATA_RECEPTION;
					com_data.index = 0;
				}
				else
				{
					com_flag.state = START;
				}
			break;
			case DATA_RECEPTION:
				if(com_data.rec_buf!=END_OF_DATA_FLAG)
				{
					com_data.buf[com_data.index] = com_data.rec_buf;
					com_data.index++;
				}
				else if(com_data.rec_buf==END_OF_DATA_FLAG)
				{
					COM_MODE_TX
					//check the sizeof it should be problem replace it with 16 .
					HAL_UART_Transmit(&huart2,(uint8_t*) com_data.tx_buf,16, HAL_MAX_DELAY);
					com_flag.state = START;
					com_flag.newdatatransaction=1;
					COM_MODE_RX
					com_flag.error=0;
				}
			break;
		}
		*/
		com_flag.newdata=0;
		com_data.packet_lost_counter=0;
		if( (com_data.rec_buf[0]==SLAVE_ADDR) && com_data.rec_buf[1]==HEADER )
		{
			for(i=2;i<16;i++)
			{
				com_data.buf[i-2] = com_data.rec_buf[i];
			}
			COM_MODE_TX
			//check the sizeof it should be problem replace it with 16 .
			HAL_UART_Transmit(&huart2,(uint8_t*) com_data.tx_buf,16, HAL_MAX_DELAY);
			com_flag.state = START;
			com_flag.newdatatransaction=1;
			COM_MODE_RX
			com_flag.error=0;
			memset(com_data.rec_buf,0,16);
		}
		HAL_GPIO_TogglePin(COM_LED_GPIO_Port, COM_LED_Pin);




	}
	else
	{
		com_data.packet_lost_counter++;

		if(com_data.packet_lost_counter>=10)
		{
			com_flag.error=1;
			com_data.error_counter++;
			com_data.packet_lost_counter=0;
			HAL_UART_Receive_IT(&huart2,(uint8_t*)&com_data.rec_buf, 16);
			HAL_GPIO_WritePin(COM_LED_GPIO_Port, COM_LED_Pin, 1);
		}
	}

	//0x06,0x71
	com_data.tx_buf[0]=0x06;
	com_data.tx_buf[1]=0x71;
	i2s(iov.iout, Iout_char);
	com_data.tx_buf[2] = Iout_char[0];
	com_data.tx_buf[3] = Iout_char[1];
	com_data.tx_buf[4] = Iout_char[2];
	com_data.tx_buf[5] = Iout_char[3];

	i2s(iov.vout, Vout_char);
	com_data.tx_buf[6] = Vout_char[0];
	com_data.tx_buf[7] = Vout_char[1];
	com_data.tx_buf[8] = Vout_char[2];
	com_data.tx_buf[9] = Vout_char[3];

	com_data.tx_buf[10] = iof.dc_okay + '0';
	com_data.tx_buf[11]	= iof.transformer_overheat + '0';
	com_data.tx_buf[12] = iof.heatsink_overheat + '0';
	com_data.tx_buf[13] = iof.tacho1 + '0';
	com_data.tx_buf[14] = iof.tacho2 + '0';
	com_data.tx_buf[15] = 0x30;


	/*
	 * 	i2s(Vout,Read_Voltage);
		sent_data[6]=Read_Voltage[0];
		sent_data[7]=Read_Voltage[1];
		sent_data[8]=Read_Voltage[2];
		sent_data[9]=Read_Voltage[3];
	 */

	/*
	 * 	i2s(Iout,Read_Current);
		sent_data[2]=Read_Current[0];
		sent_data[3]=Read_Current[1];
		sent_data[4]=Read_Current[2];
		sent_data[5]=Read_Current[3];
		//-------------------------------------------------------//
		sent_data[10]=DC_OK+'0';
		sent_data[11]=Trans_Overheat+'0';
		sent_data[12]=Heatsink_Overheat+'0';
		sent_data[13]=tacho1_Disable+'0';
		sent_data[14]=tacho2_Disable+'0';
	 */
}


void i2s(unsigned long int num,char str[5]){
    str[0] = (num/1000)%10 + '0';
    str[1] = (num/100)%10 + '0';
    str[2] = (num/10)%10 + '0';
    str[3] = (num/1)%10 + '0';
    str[4] = '0';
}


