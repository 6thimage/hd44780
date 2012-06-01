# output name images
ELFIMAGE = hd44780.elf
BINIMAGE = $(ELFIMAGE:.elf=.bin)
MAPFILE = $(ELFIMAGE:.elf=.map)

# user source code
C_DIR = code
C_SOURCE = test.c hd44780.c

# optimisation level
# 0 - reduce compilation time and make debugging work (default)
# 1 - tries to reduce code size and execution time, without extremely extending compilation time
# 2 - optimise more than 1. Enables all optimisations that don't have a speed-space tradeoff
# 3 - optimise even more than 2. Very aggressive
# s - optimise for speed, enables all level 2 (with some exceptions) and a few others
# fast - optimise for speed, disregards strict standards compliance and enables level 3
OPTIM = 0

# debugging mode (1 enabled, 0 disabled)
DEBUG_MODE = 1

# work directory - hold all the .obj files
WORK_DIR = work

# library dir
LIB_DIR = ../libs

# location of the cmsis library
CMSIS_INC = $(LIB_DIR)/CMSIS/include

# stm specific
STM32_GEN_INC = $(LIB_DIR)/CMSIS/Device/ST/STM32F4xx/Include
STM32_PERIPH_INC = $(LIB_DIR)/STM32F4xx_StdPeriph_Driver/inc
STM32_PERIPH_LIB = $(LIB_DIR)/STM32F4xx_StdPeriph_Driver/STM32F4xx_StdPeriph_Driver.a

# startup code
START_SRC = $(LIB_DIR)/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc_ride7/startup_stm32f4xx.s
START_OBJ = $(WORK_DIR)/startup_stm32f4xx.o
SYSTEM_SRC = $(LIB_DIR)/system_stm32f4xx.c
SYSTEM_OBJ = $(WORK_DIR)/system_stm32f4xx.o

# linker script
LINK_SCRIPT = $(LIB_DIR)/stm32_flash.ld

# gcc
AS = arm-none-eabi-as.exe
CC = arm-none-eabi-gcc.exe
OBJCOPY = arm-none-eabi-objcopy.exe
CODESIZE = arm-none-eabi-size.exe

# target arch
CPU = cortex-m4

# user source code objs
WORK_OBJS = $(addprefix $(WORK_DIR)/,$(C_SOURCE:.c=.o))

# assembler flags
AFLAGS = -mcpu=$(CPU) -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork
AFLAGS += -I$(CMSIS_INC) -I$(STM32_GEN_INC) -I$(STM32_PERIPH_INC) -I$(LIB_DIR)

# compiler flags
CFLAGS = -c -mcpu=$(CPU) -mthumb -Wall -O$(OPTIM) -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork -mtune=$(CPU)
CFLAGS += -I$(CMSIS_INC) -I$(STM32_GEN_INC) -I$(STM32_PERIPH_INC) -I$(LIB_DIR)
CFLAGS += -DSTM32F4XX -DHSE_VALUE=8000000

# linker flags
LK    = -static -mcpu=$(CPU) -mthumb -mlitle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork -nostartfiles
MAP   = -Xlinker -Map -Xlinker
LDESC = -Xlinker -T

# strip debug if debug not enabled
ifeq ($(DEBUG_MODE),1)

# In order to generate the application code with DEBUG function
# All debug information will be remained (Un-strip)

ELFSTRIP	= $(OBJCOPY) -O elf32-littlearm
ELFTOBIN	= $(OBJCOPY) -I elf32-little -O binary
BINTOELF	= $(OBJCOPY) -I binary -O elf32-little

else

# In order to generate the final application code without DEBUG
# All debug information will be remove (strip)

ELFSTRIP	= $(OBJCOPY) -O elf32-littlearm --strip-all
ELFTOBIN	= $(OBJCOPY) -I elf32-little -O binary --strip-debug --strip-unneeded
BINTOELF	= $(OBJCOPY) -I elf32-little -O elf32-little --strip-all

endif

# ==========================================================================================
# make rules
# ==========================================================================================

default: rom

rom: dirs $(WORK_OBJS) $(START_OBJ) $(SYSTEM_OBJ)
	@echo Linking ...
	@$(CC) $(WORK_OBJS) $(START_OBJ) $(SYSTEM_OBJ) $(STM32_PERIPH_LIB) \
	$(LK) $(MAP) $(MAPFILE) $(LDESC) $(LINK_SCRIPT) -o $(ELFIMAGE)
	@echo Converting to bin ...
	@$(ELFTOBIN) $(ELFIMAGE) $(BINIMAGE)
	@echo Code stats
	@$(CODESIZE) $(ELFIMAGE)

.PHONY: dirs clean rom

dirs:
	@echo Creating work folder structure
	@if not exist $(WORK_DIR) mkdir $(subst /,\,$(WORK_DIR))

clean:
	@echo Removing work folder structure
	@if exist work rmdir /Q /S work
	@if exist $(ELFIMAGE) del $(ELFIMAGE)
	@if exist $(BINIMAGE) del $(BINIMAGE)
	@if exist $(MAPFILE) del $(MAPFILE)

$(addprefix $(WORK_DIR)/,%.o):$(addprefix $(C_DIR)/,%.c)
	@echo $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(START_OBJ):$(START_SRC)
	@echo $<
	@$(AS) $(AFLAGS) $< -o $@

$(SYSTEM_OBJ):$(SYSTEM_SRC)
	@echo $<
	@$(CC) $(CFLAGS) -c $< -o $@
