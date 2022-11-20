/*
 * ltc1854hal.h
 *
 *  Created on: 17 Ağu 2021
 *      Author: Bayram
 */

#include "stdint.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"

#ifndef SRC_LTC1854HAL_H_
#define SRC_LTC1854HAL_H_

#define BUSY_C_Pin 				GPIO_PIN_5
#define BUSY_C_GPIO_Port 		GPIOC
#define CONVST_Pin 				GPIO_PIN_0
#define CONVST_GPIO_Port 		GPIOB
#define RD_C_Pin				GPIO_PIN_1
#define RD_C_GPIO_Port 			GPIOB
#define LTC_MODE 				2

typedef struct {
	SPI_HandleTypeDef *spiHandle;
	TIM_HandleTypeDef *tim;
	float data[8];

} LTC1854;

void get_all_channels(LTC1854 *dev);
void ltc_initialize(LTC1854 *dev, SPI_HandleTypeDef *spiHandle, TIM_HandleTypeDef *tim, TIM_TypeDef *instance);

#endif /* SRC_LTC1854HAL_H_ */
