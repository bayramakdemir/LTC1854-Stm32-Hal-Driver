/*
 * ltc1854.c
 *
 *  Created on: 17 Ağu 2021
 *      Author: Bayram
 */

#include "ltc1854hal.h"

#if LTC_MODE == 2
uint8_t mode_2_param[9] = {
	0b10000000,
	0b11000000,
	0b10010000,
	0b11010000,
	0b10100000,
	0b11100000,
	0b10110000,
	0b11110000,
	0b11110000 //dummy
};
#endif

void MX_TIM_Init(TIM_HandleTypeDef *tim,TIM_TypeDef *instance);
void begin_conv();
void wait_for_conversion_end();
void get_all_channels(LTC1854 *dev);
void ltc_initialize(LTC1854 *dev, SPI_HandleTypeDef *spiHandle, TIM_HandleTypeDef *tim, TIM_TypeDef *instance);

uint8_t current_channel = 0;

void inline begin_conv(){
	HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin, GPIO_PIN_SET); // to start conversion,
	//conversion pin should set high at least 40ns
	//conversion start rising edge of conversion pin
	//typical conversion time 4 us max 5us
}
void inline wait_for_conversion_end(){
	while(!HAL_GPIO_ReadPin(BUSY_C_GPIO_Port, BUSY_C_Pin));
	HAL_GPIO_WritePin(CONVST_GPIO_Port, CONVST_Pin, GPIO_PIN_RESET); //convst pin must be low for next conversion period
}

/**
  * @brief gett all channel measurements
  * @param LTC1854 *dev : device
  * @retval None
  */

void get_all_channels(LTC1854 *dev){

	while(current_channel <= 8){
		__HAL_TIM_SET_COUNTER(dev->tim, 0);
		uint8_t RxData[2];
		uint16_t Tx_channel_data = 0;

#if LTC_MODE == 1
	Tx_channel_data = current_channel << 12 ;
#else
	Tx_channel_data = (uint16_t) (mode_2_param[current_channel] << 8);
#endif

		HAL_SPI_TransmitReceive(dev->spiHandle, (uint8_t *) &Tx_channel_data, RxData, 1, 5);
		//request adc data from channel "current_channel", current_channel = 0,1,2 ... 7,8

		begin_conv(); //start conversion
		wait_for_conversion_end(); // wait for conversion typical 4 us

#if LTC_MODE == 1
		HAL_GPIO_WritePin(RD_C_GPIO_Port, RD_C_Pin, GPIO_PIN_RESET); // in mode 1 to send result write 0 to rd pin
		//in mode 2 output always enabled which means rd pin should be 0 all times
#endif

		if(current_channel != 0)
			dev->data[current_channel - 1]= ( (float) ((RxData[1] << 4) | (RxData[0]>>4)) ) * 0.0048828125f;

		if(current_channel == 8)
		{
			current_channel = 0;
			break;
		}else{
			current_channel++;
		}
		uint32_t us_time =__HAL_TIM_GET_COUNTER(dev->tim);
		if(us_time < 10) // check measurement time to make sure never exceed 100ksample/second
		{
			__HAL_TIM_SET_COUNTER(dev->tim, 0);  // set the counter value a 0
			while (__HAL_TIM_GET_COUNTER(dev->tim) < 10 - us_time);  // wait 10us to reach the max 100 ksample per second
		}
	}

}
/**
  * @brief LTC Initialization Function
  * @param LTC1854 *dev : device
  * @param SPI_HandleTypeDef *spiHandle : Spi address of device
  * @param TIM_HandleTypeDef *tim : timer
  * @param TIM_TypeDef *instance: timer select
  * @retval None
  */

void ltc_initialize(LTC1854 *dev, SPI_HandleTypeDef *spiHandle, TIM_HandleTypeDef *tim){

	dev->spiHandle = spiHandle;
	dev->tim = tim;
	HAL_TIM_Base_Start(tim);

}


