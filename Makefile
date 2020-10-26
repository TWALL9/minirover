#################################################################################
# Example modified from Warren Gay's opencm3 implementation for the Bluepill   	#
# of which rules were modified from the original libopencm3 makefiles.			#
# https://github.com/ve3wwg/stm32f103c8t6 for original implementation			#
#################################################################################

###############################################################################
# Executables

BINARY 		= otomo
DEFS		+= -DSTM32F4

TOP_DIR 	:= $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
OPENCM3_DIR 	:= $(abspath $(dir $(abspath $(lastword $(MAKEFILE_LIST))))/libopencm3)
SRC_DIRS ?= $(TOP_DIR)/src

SRCFILES := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addsuffix .o, $(basename $(SRCFILES)))

# Modifications from original STM32F1 implementation
# Change from Cortex M3 to M4 with hardware floating point
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)
ASFLAGS = -mthumb -mcpu=cortex-m4

PREFIX		?= arm-none-eabi-

CC		:= $(PREFIX)gcc
CXX		:= $(PREFIX)g++
LD		:= $(PREFIX)gcc
AR		:= $(PREFIX)ar
AS		:= $(PREFIX)as
OBJCOPY		:= $(PREFIX)objcopy
SIZE		:= $(PREFIX)size
OBJDUMP		:= $(PREFIX)objdump
GDB		:= $(PREFIX)gdb
STFLASH		= $(shell which st-flash)
OPT		:= -Os -g
DEBUG		:= -ggdb3
CSTD		?= -std=c99

LDSCRIPT	?= $(TOP_DIR)/stm32f4-discovery.ld
TGT_CFLAGS	+= $(OPT) $(CSTD)
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections
TGT_CFLAGS  += -I$(TOP_DIR)/inc
TGT_CFLAGS	+= -I$(OPENCM3_DIR)/include
#TGT_CFLAGS	+= -I$(TOP_DIR)/rtos/libwwg/include

TGT_CXXFLAGS	+= $(OPT) $(CXXSTD)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls  -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)
TGT_CPPFLAGS  	+= -I$(TOP_DIR)/inc
TGT_CPPFLAGS	+= -I$(OPENCM3_DIR)/include
#TGT_CPPFLAGS	+= -I$(TOP_DIR)/rtos/libwwg/include

TGT_LDFLAGS	+= --static -nostartfiles
TGT_LDFLAGS	+= -T$(LDSCRIPT)
TGT_LDFLAGS	+= $(ARCH_FLAGS)
TGT_LDFLAGS	+= -Wl,-Map=$(*).map
TGT_LDFLAGS	+= -Wl,--gc-sections

LDLIBS		+= -specs=nosys.specs
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
#LDLIBS		+= -L$(TOP_DIR)/rtos/libwwg -lwwg
LDLIBS		+= -L$(OPENCM3_DIR)/lib -lopencm3_stm32f4

.SUFFIXES:	.elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

elf:	$(DEPS) $(BINARY).elf
bin:	$(DEPS) $(BINARY).bin
hex:	$(DEPS) $(BINARY).hex
srec:	$(DEPS) $(BINARY).srec
list:	$(DEPS) $(BINARY).list

# Define a helper macro for debugging make errors online
# you can type "make print-OPENCM3_DIR" and it will show you
# how that ended up being resolved by all of the included
# makefiles.

# These rules simply denote where to place the build files in your system.
print-%:
	@echo $*=$($*)

%.elf %.map: libopencm3/lib/libopencm3_stm32f4.a $(OBJS) $(LDSCRIPT)
	$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(*).elf
	$(SIZE) $(BINARY).elf

libopencm3/lib/libopencm3_stm32f4.a:
	$(MAKE) -C libopencm3 TARGETS=stm32/f4

%.images: %.bin %.hex %.srec %.list %.map
	@#printf "*** $* images generated ***\n"

%.bin: %.elf
	@#printf "  OBJCOPY $(*).bin\n"
	$(OBJCOPY) -Obinary $(*).elf $(*).bin

%.hex: %.elf
	@#printf "  OBJCOPY $(*).hex\n"
	$(OBJCOPY) -Oihex $(*).elf $(*).hex

%.srec: %.elf
	@#printf "  OBJCOPY $(*).srec\n"
	$(OBJCOPY) -Osrec $(*).elf $(*).srec

%.list: %.elf
	@#printf "  OBJDUMP $(*).list\n"
	$(OBJDUMP) -S $(*).elf > $(*).list

%.o: %.c
	@#printf "  CC      $(*).c\n"
	$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).c

%.o: %.cxx
	@#printf "  CXX     $(*).cxx\n"
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).cxx

%.o: %.cpp
	@#printf "  CXX     $(*).cpp\n"
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $(*).o -c $(*).cpp

%.o: %.asm
	$(AS) $(ASFLAGS) -o $*.o -c $<

clean:
	@#printf "  CLEAN\n"
	rm -f libopencm3/lib/libopencm3_stm32f4.a
	-$(MAKE) -$(MAKEFLAGS) -C ./libopencm3 clean
	$(RM) *.o *.d generated.* $(OBJS) $(patsubst %.o,%.d,$(OBJS))
	rm -f *.elf *.bin *.hex *.srec *.list *.map *.img

# Flash Device
flash:	$(BINARY).bin
	$(STFLASH) $(FLASHSIZE) write $(BINARY).bin 0x8000000

.PHONY: images clean elf bin hex srec list all

-include $(OBJS:.o=.d)