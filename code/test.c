#if defined(STM32F4XX)
#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#elif defined(STM32L1XX)
#include <stm32l1xx.h>
#include <stm32l1xx_gpio.h>
#include <stm32l1xx_rcc.h>
#else
#error "Unknown processor"
#endif

#include "hd44780.h"

volatile uint32_t SysTickCount;

void SysTick_Handler(void)
{
  ++SysTickCount;
}

void delay(uint32_t c)
{
    while(--c);
}

void delayms(uint32_t c)
{
    for(c+=SysTickCount;SysTickCount<c;);
}

void init_io(void)
{
    GPIO_InitTypeDef io_init;

#if defined(STM32F4XX)
    /* enable peripheral clock for pa and pc */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* settings for both pa and pc */
    io_init.GPIO_Mode = GPIO_Mode_OUT;
    io_init.GPIO_OType = GPIO_OType_PP;
    io_init.GPIO_Speed = GPIO_Speed_100MHz;
    io_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

    /* init pa */
    io_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_Init(GPIOA, &io_init);

    /* init pc */
    io_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
    GPIO_Init(GPIOC, &io_init);
#elif defined(STM32L1XX)
    /* enable peripheral clock for pb and pc */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

    /* settings for both pa and pc */
    io_init.GPIO_Mode = GPIO_Mode_OUT;
    io_init.GPIO_OType = GPIO_OType_PP;
    io_init.GPIO_Speed = GPIO_Speed_40MHz;
    io_init.GPIO_PuPd = GPIO_PuPd_NOPULL;

    /* init pc */
    io_init.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_Init(GPIOC, &io_init);

    /* init pb */
    io_init.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
    GPIO_Init(GPIOB, &io_init);
#endif
}

int main(void)
{
    char msg[]="A nice long message for you to read !!!!"
               "A second very long message to read :) !!";

    /* set up systick */
    if(SysTick_Config(SystemCoreClock/1000))
    {
        while(1);
    }

    /* set the pins up */
    init_io();
    /* start the lcd */
    hd44780_init();

    hd44780_write_string(msg);

    while(1)
    {
        delayms(300);
        hd44780_shift(0,1);
    }

    return 0;
}
