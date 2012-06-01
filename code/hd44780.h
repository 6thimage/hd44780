#ifndef _HD44780_H_
#define _HD44780_H_

#include <stdint.h>

#ifdef STM32F4XX
#include "hd44780_stm32.h"
#endif

#define HD44780_CMD_CLEAR            0x1      /*!< Clears display */
#define HD44780_CMD_RETURN_HOME      0x2      /*!< Sets DDRAM pointer to 0 */
#define HD44780_CMD_ENTRY_MODE       0x4      /*!< Sets how the pointer is updated after a character write */
#define HD44780_CMD_DISPLAY          0x8      /*!< Display settings */
#define HD44780_CMD_SHIFT            0x10     /*!< Cursor and display movement */
#define HD44780_CMD_FUNCTION         0x20     /*!< Screen type setup */
#define HD44780_CMD_CGRAM_ADDR       0x40     /*!< Sets CGRAM address */
#define HD44780_CMD_DDRAM_ADDR       0x80     /*!< Sets DDRAM address */

#define HD44780_BIT_ENTRY_SHIFT      0x1
#define HD44780_BIT_ENTRY_INC        0x2      /*!< Increments pointer after writing, decrements if not set */

#define HD44780_BIT_DISPLAY_BLINK    0x1      /*!< Enables display blinking */
#define HD44780_BIT_DISPLAY_CURSOR   0x2      /*!< Enables cursor */
#define HD44780_BIT_DISPLAY_ON       0x4      /*!< Enables the display */

#define HD44780_BIT_SHIFT_RIGHT      0x4      /*!< Shift is to the right, shifts to the left if not set */
#define HD44780_BIT_SHIFT_DISPLAY    0x8      /*!< Shifts the display or shifts the cursor if not set */

#define HD44780_BIT_FUNCTION_FONT    0x4      /*!< 5x10 font if set, 5x8 if not */
#define HD44780_BIT_FUNCTION_LINES   0x8      /*!< 2 lines if set, 1 line if not */
#define HD44780_BIT_FUNCTION_BUS     0x10     /*!< 8 bit bus if set, 4 bit if not */

#define HD44780_BIT_BUSY_FLAG        0x80    /*!< Busy flag */

#define hd44780_clear()                       hd44780_command(0,HD44780_CMD_CLEAR)
#define hd44780_cursor_home()                 hd44780_command(0,HD44780_CMD_RETURN_HOME)
#define hd44780_entry(inc,shift)              hd44780_command(0,HD44780_CMD_ENTRY_MODE|(!inc?0:HD44780_BIT_ENTRY_INC)|(!shift?0:HD44780_BIT_ENTRY_SHIFT))
#define hd44780_display(on,blink,cursor)      hd44780_command(0,HD44780_CMD_DISPLAY|(!on?0:HD44780_BIT_DISPLAY_ON)|(!blink?0:HD44780_BIT_DISPLAY_BLINK)|(!cursor?0:HD44780_BIT_DISPLAY_CURSOR))
#define hd44780_shift(lr,cd)                  hd44780_command(0,HD44780_CMD_SHIFT|(!lr?0:HD44780_BIT_SHIFT_RIGHT)|(!cd?0:HD44780_BIT_SHIFT_DISPLAY))
#define hd44780_function(bus,font,lines)      hd44780_command(0,HD44780_CMD_FUNCTION|(!bus?0:HD44780_BIT_FUNCTION_BUS)|(!font?0:HD44780_BIT_FUNCTION_FONT)|(!lines?0:HD44780_BIT_FUNCTION_LINES))
#define hd44780_cgram(addr)                   hd44780_command(0,HD44780_CMD_CGRAM_ADDR|(addr&0x20))
#define hd44780_ddram(addr)                   hd44780_command(0,HD44780_CMD_DDRAM_ADDR|(addr&0x40))
#define hd44780_write_char(c)                 hd44780_command(1,c&0xff)

void hd44780_command(uint16_t rs, uint16_t cmd);
void hd44780_init(void);
void hd44780_write_string(char *s);

#endif /* !_HD44780_H_ */