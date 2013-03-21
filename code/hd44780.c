#include "hd44780.h"

void hd44780_command(uint16_t rs, uint16_t cmd)
{
    /* send the command to the data pins */
    hd44780_set_command(cmd);
    /* set rs if needed */
    if(rs)
        hd44780_set_rs();
    /* tell the lcd that we have a command to read in */
    hd44780_set_ce();
    /* wait long enough so that the lcd can see the command */
    hd44780_ce_high_delay();
    /* reset the ce line */
    hd44780_unset_ce();
    /* reset rs if needed */
    if(rs)
        hd44780_unset_rs();
    /* reset the data bus pins */
    hd44780_unset_command(cmd);
}

void hd44780_init(void)
{
    /* wait initial delay for lcd to settle */
    hd44780_init_delay();
    /* reset procedure - 3 functon calls resets the device */
    hd44780_function(1,0,1);
    hd44780_function(1,0,1);
    hd44780_function(1,0,1);
    /* turn the display on with no cursor or blinking */
    hd44780_display(1,0,0);
    hd44780_clear(); /* clear the display */
    hd44780_entry(1,0); /* cursor increment, no shift */
    /* end delay so that calls following won't be ignored */
    hd44780_init_end_delay();
}

void hd44780_write_string(char *s)
{
    uint32_t i;
    for(i=0;s[i]!='\0';++i)
        hd44780_write_char(s[i]);
}