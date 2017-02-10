#******************************************************************************
# @file      Makefile
# @author    Eduardo MENDEZ
# @version   V1.0
# @date      20/09/2010
# @copy
# ******************************************************************************

# Project name
PROJECT_NAME=sensedin

# Directory definition.
RTOS_BASE_PATH=./
ST_LIB_BASE_PATH = ./
USB_LIB_BASE_PATH = ./

RTOS_SOURCE_DIR=$(RTOS_BASE_PATH)FreeRTOS/Source
ST_LIB_DIR=$(ST_LIB_BASE_PATH)stm32f10x_stdperiph_lib/STM32F10x_StdPeriph_Lib_V3.3.0/Libraries/STM32F10x_StdPeriph_Driver
ARM_CMSIS_DIR=$(ST_LIB_BASE_PATH)stm32f10x_stdperiph_lib/STM32F10x_StdPeriph_Lib_V3.3.0/Libraries/CMSIS
USB_CORE_LIB_DIR=$(USB_LIB_BASE_PATH)USB/core
USB_PROFILE_LIB_DIR=$(USB_LIB_BASE_PATH)USB/profile

DRIVERS_SOURCE_DIR=./drivers
SOURCE_DIR=./source
MODBUS_DIR=./modbus

# Directory for output files (lst, obj, dep, elf, sym, map, hex, bin etc.).
OUTDIR = Debug

# Toolchain definition.
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
OBJDUMP=arm-none-eabi-size
NM = arm-none-eabi-nm
STRIP = arm-none-eabi-strip
LDSCRIPT=stm32_flash.ld


# should use --gc-sections but the debugger does not seem to be able to cope with the option.
LINKER_FLAGS=-nostartfiles -Xlinker -o$(PROJECT_NAME).axf -Xlinker -M -Xlinker -Map=$(PROJECT_NAME).map -Xlinker --no-gc-sections


# Debugging format.
#DEBUG = stabs
#DEBUG = dwarf-2
DEBUG=

# Optimization level, can be [0, 1, 2, 3, s].
# 0 = turn off optimization. s = optimize for size.
# (Note: 3 is not always the best optimization level. See avr-libc FAQ.)
#OPT = s
#OPT = 1
#OPT = 2
#OPT = 3
OPT = 0


# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = gnu89
#		-std=$(CSTANDARD) \

# Compiler flags definition.
CFLAGS= -g$(DEBUG) \
		-O$(OPT) \
		-T $(LDSCRIPT) \
		-I . \
		-I ./STCode \
		-I ./includes \
		-I $(RTOS_SOURCE_DIR)/include \
		-I $(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3 \
		-I $(DRIVERS_SOURCE_DIR)/includes \
		-I $(ST_LIB_DIR)/inc \
		-I $(ARM_CMSIS_DIR)/CM3/DeviceSupport/ST/STM32F10x \
		-I $(ARM_CMSIS_DIR)/CM3/CoreSupport \
		-I $(USB_CORE_LIB_DIR)/inc \
		-I $(USB_PROFILE_LIB_DIR)/inc \
		-D STM32F10X_HD \
		-D HAVE_CONFIG_H \
		-D USE_STDPERIPH_DRIVER \
		-D USE_STDPERIPH_DRIVER \
		-D VECT_TAB_FLASH \
		-D GCC_ARMCM3 \
		-D gcc \
		-mthumb \
		-mcpu=cortex-m3 \
		-ffunction-sections \
		-fdata-sections \
		-I $(MODBUS_DIR)/port \
		-I $(MODBUS_DIR)/include \
		-I $(MODBUS_DIR)/ascii \
		-I $(MODBUS_DIR)/rtu \
		-I $(MODBUS_DIR)/tcp \
#		-D inline= \		
#-D PACK_STRUCT_END=__attribute\(\(packed\)\) \
#-D ALIGN_STRUCT_END=__attribute\(\(aligned\(4\)\)\) \

# Source files
SOURCE=	$(SOURCE_DIR)/stf_syscalls_minimal.c 	\
		$(SOURCE_DIR)/stm32f10x_it.c 			\
		$(SOURCE_DIR)/rtd.c           \
		$(SOURCE_DIR)/analog.c        \
		$(SOURCE_DIR)/sai.c						\
		$(SOURCE_DIR)/median.c        \
		$(SOURCE_DIR)/application.c        \
		$(SOURCE_DIR)/main.c		 			\
		#$(SOURCE_DIR)/test.c		 			\
		#$(SOURCE_DIR)/thermocouple.c  \
		
# ST Library source files.
ST_LIB_SOURCE= $(ARM_CMSIS_DIR)/CM3/CoreSupport/core_cm3.c \
		$(ST_LIB_DIR)/src/misc.c \
		$(ST_LIB_DIR)/src/stm32f10x_rcc.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_gpio.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_fsmc.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_dac.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_dma.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_spi.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_tim.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_adc.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_flash.c \
		$(ST_LIB_DIR)/src/stm32f10x_pwr.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_exti.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_usart.c \
		$(ST_LIB_DIR)/src/stm32f10x_dbgmcu.c \
		$(ST_LIB_DIR)/src/stm32f10x_i2c.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_rtc.c 	\
		$(ST_LIB_DIR)/src/stm32f10x_wwdg.c 	\

# FreeRTOS source files.
FREERTOS_SOURCE= $(RTOS_SOURCE_DIR)/list.c \
		$(RTOS_SOURCE_DIR)/queue.c \
		$(RTOS_SOURCE_DIR)/tasks.c \
		$(RTOS_SOURCE_DIR)/portable/GCC/ARM_CM3/port.c \
		$(RTOS_SOURCE_DIR)/portable/MemMang/heap_2.c

# Drivers Files
DRIVERS_SOURCE=  \
		$(DRIVERS_SOURCE_DIR)/source/hardware.c 	\
		$(DRIVERS_SOURCE_DIR)/source/adc.c 	\
		$(DRIVERS_SOURCE_DIR)/source/serie.c 	\
		$(DRIVERS_SOURCE_DIR)/source/extADC.c 	\
		$(DRIVERS_SOURCE_DIR)/source/i2c.c 	\
		$(DRIVERS_SOURCE_DIR)/source/eeprom.c 	\
		$(DRIVERS_SOURCE_DIR)/source/halLora.c 	\
		$(DRIVERS_SOURCE_DIR)/source/radio.c 	\
		
#Modbus Files
MODBUS_SOURCE = \
		$(MODBUS_DIR)/port/modbus_main.c 	\
		$(MODBUS_DIR)/port/portevent.c 	\
		$(MODBUS_DIR)/port/portserial.c 	\
		$(MODBUS_DIR)/port/porttimer.c 	\
		$(MODBUS_DIR)/port/portother.c 	\
		$(MODBUS_DIR)/mb.c 	\
		$(MODBUS_DIR)/ascii/mbascii.c 	\
		$(MODBUS_DIR)/functions/mbfunccoils.c  \
		$(MODBUS_DIR)/functions/mbfuncdiag.c  \
		$(MODBUS_DIR)/functions/mbfuncdisc.c  \
		$(MODBUS_DIR)/functions/mbfuncholding.c \
		$(MODBUS_DIR)/functions/mbfuncinput.c \
		$(MODBUS_DIR)/functions/mbfuncother.c \
		$(MODBUS_DIR)/functions/mbutils.c \
		$(MODBUS_DIR)/rtu/mbrtu.c \
		$(MODBUS_DIR)/rtu/mbcrc.c \
		$(MODBUS_DIR)/tcp/mbtcp.c \

		
SOURCE+=$(ST_LIB_SOURCE)
SOURCE+=$(FREERTOS_SOURCE)
SOURCE+=$(DRIVERS_SOURCE)
SOURCE+=$(USB_CORE_SOURCE)
SOURCE+=$(USB_PROFILE_SOURCE)
SOURCE+=$(MODBUS_SOURCE)

# List of all source files without directory and file-extension.
ALLSRCBASE = $(notdir $(basename $(SOURCE)))

# List of all objects files.
OBJS = $(addprefix $(OUTDIR)/, $(addsuffix .o, $(ALLSRCBASE)))

# Define Messages.
# English
MSG_BEGIN = -------- begin --------
MSG_END = --------  end  --------


# Rules definition. ***********************************************************

all: begin gccversion $(OUTDIR)/$(PROJECT_NAME).bin end

$(OUTDIR)/$(PROJECT_NAME).bin : $(PROJECT_NAME).axf
	$(OBJCOPY) $(PROJECT_NAME).axf -O binary $(PROJECT_NAME).bin
	$(OBJCOPY) $(PROJECT_NAME).axf -O elf32-littlearm $(PROJECT_NAME).elf

$(PROJECT_NAME).axf : $(OBJS) $(OUTDIR)/startup_stm32f10x.o Makefile
	$(CC) $(CFLAGS) $(OUTDIR)/startup_stm32f10x.o $(OBJS) $(LIBS) $(LINKER_FLAGS) -lm


# Compile: create object files from C source files.
define COMPILE_C_TEMPLATE
$(OUTDIR)/$(notdir $(basename $(1))).o : $(1)
##	@echo
	@echo $$< "->" $$@
	$(CC) -c  $$(CFLAGS) $$< -o $$@
endef
$(foreach src, $(SOURCE), $(eval $(call COMPILE_C_TEMPLATE, $(src))))

$(OUTDIR)/startup_stm32f10x.o : $(SOURCE_DIR)/startup_stm32f10x.c Makefile
	$(CC) -c $(CFLAGS) -O1 $(SOURCE_DIR)/startup_stm32f10x.c -o $(OUTDIR)/startup_stm32f10x.o

clean :
	rm -f $(OBJS)
	rm -f $(OUTDIR)/startup_stm32f10x.o
	rm -f $(PROJECT_NAME).axf
	rm -f $(PROJECT_NAME).elf
	rm -f $(PROJECT_NAME).bin
	rm -f $(PROJECT_NAME).map
	rm -f $(PROJECT_NAME)_SymbolTable.txt
	rm -f $(PROJECT_NAME)_MemoryListingSummary.txt
	rm -f $(PROJECT_NAME)_MemoryListingDetails.txt

log : $(PROJECT_NAME).axf
	$(NM) -n $(PROJECT_NAME).axf > $(PROJECT_NAME)_SymbolTable.txt
	$(OBJDUMP) --format=SysV $(PROJECT_NAME).axf > $(PROJECT_NAME)_MemoryListingSummary.txt
	$(OBJDUMP) $(OBJS) > $(PROJECT_NAME)_MemoryListingDetails.txt

# Eye candy.
begin:
##	@echo
	@echo $(MSG_BEGIN)

end:
	@echo $(MSG_END)
##	@echo

# Display compiler version information.
gccversion :
	@$(CC) --version

$(shell mkdir $(OUTDIR) 2>NUL)
