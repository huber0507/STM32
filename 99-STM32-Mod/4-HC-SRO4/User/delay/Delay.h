#ifndef __DELAY_H
#define __DELAY_H

//void Delay_us(uint32_t us);
//void Delay_ms(uint32_t ms);
//void Delay_s(uint32_t s);



void DelayInit(void);
void DelayMs(unsigned int nms);
void DelayUs(unsigned long nus);
void DelayS(unsigned int ns);

#endif
