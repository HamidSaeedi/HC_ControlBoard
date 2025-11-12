#ifndef COM_H
#define COM_H
#include "main.h"
#include "usart.h"
#define SLAVE_ADDR 0x05
#define MASTER_ADDR 0x01
#define START 0
#define ADDR_RESOLVED 1
#define DATA_RECEPTION 2
#define END_OF_DATA_FLAG 0x04
#define HEADER 'q'
#define TX_MODE 1
#define RX_MODE 0
#define COM_MODE_TX HAL_GPIO_WritePin(DERE_GPIO_Port, DERE_Pin, TX_MODE);
#define COM_MODE_RX HAL_GPIO_WritePin(DERE_GPIO_Port, DERE_Pin, RX_MODE);
#define RX_BUFFER_SIZE 32

struct COM_FLAG {
	uint8_t newdata:1; //newdata=0 no new data available in uart; newdata=1  new data available in uart
	uint8_t state:2; //state=0 start; state=1 ADDR_RESOLVED; state=2 DATA_RECEPTION;
	uint8_t newdatatransaction:1;
	uint8_t error:1;
	uint8_t reserved:4;

};

struct COM_DATA {
	uint8_t rec_buf[16];
	uint8_t buf[16];
	uint8_t index;
	uint8_t tx_buf[16];
	uint8_t rx_buffer[RX_BUFFER_SIZE];  // DMA Buffer

	uint16_t rx_size;
	uint32_t packet_lost_counter;
	uint32_t error_counter;
};
#endif

extern struct COM_FLAG com_flag;
extern struct COM_DATA com_data;


void i2s(unsigned long int num,char str[5]);
