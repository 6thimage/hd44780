#ifndef _HD44780_STM32_H_
#define _HD44780_STM32_H_

#include <stm32l1xx.h>
#include <stm32l1xx_gpio.h>

void delay(uint32_t c);
void delayms(uint32_t c);

#define hd44780_set_command(cmd)    GPIO_SetBits(GPIOC, cmd)
#define hd44780_unset_command(cmd)  GPIO_ResetBits(GPIOC, cmd)
#define hd44780_set_ce()            GPIO_SetBits(GPIOB, GPIO_Pin_8)
#define hd44780_unset_ce()          GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define hd44780_set_rs()            GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define hd44780_unset_rs()          GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define hd44780_ce_high_delay()     delay(0xff)
#define hd44780_init_delay()        delayms(16)
#define hd44780_init_end_delay()    delayms(2)

#endif /* !_HD44780_STM32_H_ */
