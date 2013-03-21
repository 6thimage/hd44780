#ifndef _HD44780_STM32_H_
#define _HD44780_STM32_H_

#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>

void delay(uint32_t c);
void delayms(uint32_t c);

#define hd44780_set_command(cmd)    GPIO_SetBits(GPIOA, cmd)
#define hd44780_unset_command(cmd)  GPIO_ResetBits(GPIOA, cmd)
#define hd44780_set_ce()            GPIO_SetBits(GPIOC, GPIO_Pin_0)
#define hd44780_unset_ce()          GPIO_ResetBits(GPIOC, GPIO_Pin_0)
#define hd44780_set_rs()            GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define hd44780_unset_rs()          GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define hd44780_ce_high_delay()     delay(0xff)
#define hd44780_init_delay()        delayms(16)
#define hd44780_init_end_delay()    delayms(2)

#endif /* !_HD44780_STM32_H_ */
