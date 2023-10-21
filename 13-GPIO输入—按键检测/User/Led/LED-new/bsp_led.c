#include "bsp.h"

#if (LED_IS_LOW_LEVEL_ACTIVE == 1)
static const GPIO_Info_t s_tLedList[LED_NUM] = {
	{LED_RED_PORT, LED_RED_PIN, LOW_LEVEL},
	{LED_GREEN_PORT, LED_GREEN_PIN, LOW_LEVEL},
	{LED_BLUE_PORT, LED_BLUE_PIN, LOW_LEVEL},
};
#else
static const GPIO_Info_t s_tLedList[LED_NUM] = {
	{LED_RED_PORT, LED_RED_PIN, HIGH_LEVEL},
	{LED_GREEN_PORT, LED_GREEN_PIN, HIGH_LEVEL},
	{LED_BLUE_PORT, LED_BLUE_PIN, HIGH_LEVEL},
};

#endif //(LED_IS_LOW_LEVEL_ACTIVE == 1)

static LedBlink_t s_tLedBlinkInfo[LED_NUM];



void Bsp_LedInit(void)
{
	uint8_t i;
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	EN_LED_GPIO_CLK();

	for (i = 0; i <LED_NUM; i++)
	{
		HAL_GPIO_WritePin(s_tLedList[i].GPIOx, s_tLedList[i].GPIO_PIN_x, (GPIO_PinState)!s_tLedList[i].ActiveLevel);
	}

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	for (i = 0; i <LED_NUM; i++)
	{
		GPIO_InitStruct.Pin = s_tLedList[i].GPIO_PIN_x;
		HAL_GPIO_Init(s_tLedList[i].GPIOx, &GPIO_InitStruct);
	}
}

void Bsp_LedOn(LedID_e _eLedName)
{
#if (LED_MULTI_COLOR == 1)
	for (uint8_t i = 0; i < LED_NUM; i++)
	{
		if (i == _eLedName)
		{
			continue;
		}
		if (Bsp_LedGet((LedID_e)i) == LED_ON)
		{
			Bsp_LedOff((LedID_e)i);
		}
	}
#endif
	HAL_GPIO_WritePin(s_tLedList[_eLedName].GPIOx, s_tLedList[_eLedName].GPIO_PIN_x, (GPIO_PinState)s_tLedList[_eLedName].ActiveLevel);
}

void Bsp_LedOff(LedID_e _eLedName)
{
	HAL_GPIO_WritePin(s_tLedList[_eLedName].GPIOx, s_tLedList[_eLedName].GPIO_PIN_x, (GPIO_PinState)!s_tLedList[_eLedName].ActiveLevel);
}

//判断是否灯已经点亮
uint8_t Bsp_LedGet(LedID_e _eLedName)
{
	//读取是高电平
	if ( (s_tLedList[_eLedName].GPIOx->IDR & s_tLedList[_eLedName].GPIO_PIN_x) != LOW_LEVEL)
	{
		if (s_tLedList[_eLedName].ActiveLevel != LOW_LEVEL)
		{
			return LED_ON;
		}
		else
		{
			return LED_OFF;
		}
	}
	else
	{
		if (s_tLedList[_eLedName].ActiveLevel != LOW_LEVEL)
		{
			return LED_OFF;
		}
		else
		{
			return LED_ON;
		}
	}
}

void Bsp_LedToggle(LedID_e _eLedName)
{
	if (Bsp_LedGet(_eLedName) == LED_ON)
	{
		Bsp_LedOff(_eLedName);
	}
	else
	{
		Bsp_LedOn(_eLedName);
	}
}




/**
  * @brief  按照快慢闪烁
  * @param  _eLedName: 哪个led
  * @param  _eMode: 操作模式，快慢
  * @param  _ucTimes: 闪烁次数，0代表循环，其他数字为次数
  * @retval None
  note : 如果是循环模式记得使用Bsp_LedBlinkOff(); 关闭
  */
void Bsp_LedBlink(LedID_e _eLedName, LedOprate_e _eMode, uint8_t _ucTimes)
{
#if (LED_MULTI_COLOR == 1)
	for (uint8_t i = 0; i < LED_NUM; i++)
	{
		if (i == _eLedName)
		{
			continue;
		}
		if (s_tLedBlinkInfo[i].OnReload != 0 && s_tLedBlinkInfo[i].OffReload != 0)
		{
			Bsp_LedBlinkOff((LedID_e)i);
		}
	}
#endif //(LED_MULTI_COLOR == 1)
	Bsp_LedOff(_eLedName);
	if (s_tLedBlinkInfo[_eLedName].OnReload != 0 && s_tLedBlinkInfo[_eLedName].OffReload != 0)
	{
		Bsp_LedBlinkOff(_eLedName);
	}

	DISABLE_INT();

	if (_ucTimes == 0)
	{
		s_tLedBlinkInfo[_eLedName].Mode = LED_BLINK_CYCLIC;
		s_tLedBlinkInfo[_eLedName].Times = 0;
	}
	else
	{
		s_tLedBlinkInfo[_eLedName].Mode = LED_BLINK_TIMES;
		s_tLedBlinkInfo[_eLedName].Times = _ucTimes;
	}
	s_tLedBlinkInfo[_eLedName].State = LED_OFF;
	switch (_eMode)
	{
		case LED_BLINK_SLOW:
			s_tLedBlinkInfo[_eLedName].OnReload = LED_ON_LONG;
			s_tLedBlinkInfo[_eLedName].OffReload = LED_OFF_LONG;
			break;
		case LED_BLINK_GENERAL:
			s_tLedBlinkInfo[_eLedName].OnReload = LED_ON_MEDIUM;
			s_tLedBlinkInfo[_eLedName].OffReload = LED_OFF_MEDIUM;
			break;
		case LED_BLINK_FAST:
			s_tLedBlinkInfo[_eLedName].OnReload = LED_OFF_SHORT;
			s_tLedBlinkInfo[_eLedName].OffReload = LED_OFF_SHORT;
			break;
		default :
			break;
	}
	s_tLedBlinkInfo[_eLedName].OnCnt = 0;
	s_tLedBlinkInfo[_eLedName].OffCnt = 0;
	ENABLE_INT();
}

void Bsp_LedBlinkOff(LedID_e _eLedName)
{
	DISABLE_INT();
	s_tLedBlinkInfo[_eLedName].Mode = LED_BLINK_TIMES;
	s_tLedBlinkInfo[_eLedName].Times = 0;
	s_tLedBlinkInfo[_eLedName].State = LED_OFF;
	s_tLedBlinkInfo[_eLedName].OnCnt = 0;
	s_tLedBlinkInfo[_eLedName].OffCnt = 0;
	s_tLedBlinkInfo[_eLedName].OnReload = 0;
	s_tLedBlinkInfo[_eLedName].OffReload = 0;
	ENABLE_INT();
	Bsp_LedOff(_eLedName);
}


//放到100ms轮询
void Bsp_LedLoop100ms(void)
{
	uint8_t i;
	for (i = 0; i < LED_NUM; i++)
	{
		if (s_tLedBlinkInfo[i].OnReload == 0 && s_tLedBlinkInfo[i].OffReload == 0)
		{
			continue;
		}
		if (s_tLedBlinkInfo[i].Mode == LED_BLINK_CYCLIC && s_tLedBlinkInfo[i].State == LED_OFF && s_tLedBlinkInfo[i].OffCnt == 0)
		{
			s_tLedBlinkInfo[i].State = LED_ON;
			Bsp_LedOn((LedID_e)i);
			s_tLedBlinkInfo[i].OnCnt = s_tLedBlinkInfo[i].OnReload - 1;
			s_tLedBlinkInfo[i].OffCnt = s_tLedBlinkInfo[i].OffReload;
		}
		else if (s_tLedBlinkInfo[i].Mode == LED_BLINK_TIMES && s_tLedBlinkInfo[i].State == LED_OFF && s_tLedBlinkInfo[i].OffCnt == 0)
		{
			if (s_tLedBlinkInfo[i].Times == 0)
			{
				Bsp_LedBlinkOff((LedID_e)i);
			}
			else
			{
				s_tLedBlinkInfo[i].State = LED_ON;
				Bsp_LedOn((LedID_e)i);
				s_tLedBlinkInfo[i].OnCnt = s_tLedBlinkInfo[i].OnReload - 1;
				s_tLedBlinkInfo[i].OffCnt = s_tLedBlinkInfo[i].OffReload;
				s_tLedBlinkInfo[i].Times--;
			}
		}
		else
		{
			if (s_tLedBlinkInfo[i].State == LED_ON && s_tLedBlinkInfo[i].OnCnt != 0)
			{
				s_tLedBlinkInfo[i].OnCnt--;
			}
			else if (s_tLedBlinkInfo[i].State == LED_ON && s_tLedBlinkInfo[i].OnCnt == 0)
			{
				s_tLedBlinkInfo[i].State = LED_OFF;
				Bsp_LedOff((LedID_e)i);
				s_tLedBlinkInfo[i].OffCnt--;
			}
			else if (s_tLedBlinkInfo[i].State == LED_OFF && s_tLedBlinkInfo[i].OffCnt != 0)
			{
				s_tLedBlinkInfo[i].OffCnt--;
			}
		}
	}
}



void Bsp_LedTest(void)
{
	uint8_t i;
	uint8_t j;
	for (i = 0; i < LED_NUM; i++)
	{
		Bsp_LedOn((LedID_e)i);
		HAL_Delay(1000);
		Bsp_LedOff((LedID_e)i);
		HAL_Delay(1000);
	}
	HAL_Delay(3000);
	for (i = 0; i < LED_NUM; i++)
	{
		Bsp_LedToggle((LedID_e)i);
		HAL_Delay(1000);
		Bsp_LedToggle((LedID_e)i);
		HAL_Delay(1000);
	}
	for (i = 0; i < LED_NUM; i++)
	{
		for (j = 0; j < LED_NUM; j++)
		{
			Bsp_LedBlink((LedID_e)i, (LedOprate_e)j, 3);
			HAL_Delay(7000);
		}
	}

}

