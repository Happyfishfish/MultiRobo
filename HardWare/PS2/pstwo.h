#include "stm32f10x.h"
#define SEARCH_Ml_PIN         GPIO_Pin_1
#define SEARCH_Ml_GPIO        GPIOC
#define SEARCH_Ml_IO          GPIO_ReadInputDataBit(SEARCH_Ml_GPIO, SEARCH_Ml_PIN)

#define SEARCH_Mr_PIN         GPIO_Pin_3
#define SEARCH_Mr_GPIO        GPIOC
#define SEARCH_Mr_IO          GPIO_ReadInputDataBit(SEARCH_Mr_GPIO, SEARCH_Mr_PIN)

#define SEARCH_R_PIN         GPIO_Pin_4
#define SEARCH_R_GPIO        GPIOA
#define SEARCH_R_IO          GPIO_ReadInputDataBit(SEARCH_R_GPIO, SEARCH_R_PIN)

#define SEARCH_L_PIN         GPIO_Pin_2
#define SEARCH_L_GPIO        GPIOC
#define SEARCH_L_IO          GPIO_ReadInputDataBit(SEARCH_L_GPIO, SEARCH_L_PIN)
#define BLACK_AREA 1
#define WHITE_AREA 0




