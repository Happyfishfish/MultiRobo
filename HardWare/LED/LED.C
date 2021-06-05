#include "led.h"
/**************************************************************************
×÷Õß£ºÄ«±ÈË¹¿Æ¼¼
ÎÒµÄÌÔ±¦Ð¡µê£ºhttps://moebius.taobao.com/
**************************************************************************/ 

/**************************************************************************
º¯Êý¹¦ÄÜ£ºLED½Ó¿Ú³õÊ¼»¯
Èë¿Ú²ÎÊ
ý£ºÎÞ 
·µ»Ø  Öµ£ºÎÞ
**************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	//Ê¹ÄÜ¶Ë¿ÚÊ±ÖÓ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50M
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//¸ù¾ÝÉè¶¨²ÎÊý³õÊ¼»¯GPIOC
}
