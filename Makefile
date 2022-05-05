#################################################################################
# Example modified from Warren Gay's opencm3 implementation for the Bluepill   	#
# of which rules were modified from the original libopencm3 makefiles.			#
# https://github.com/ve3wwg/stm32f103c8t6 for original implementation			#
#################################################################################

###############################################################################

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

# Executables
BINARY 		= otomo
DEFS		+= -DSTM32F4

TOP_DIR := $(shell pwd)
OPENCM3_DIR := $(TOP_DIR)/lib/libopencm3
SRC_DIRS ?= $(TOP_DIR)/src
OBJ_DIR = obj
BIN_DIR = bin

SRCFILES := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(SRCFILES))))

# Modifications from original STM32F1 implementation
# Change from Cortex M3 to M4 with hardware floating point
FP_FLAGS	?= -mfloat-abi=hard -mfpu=fpv4-sp-d16
ARCH_FLAGS	= -mthumb -mcpu=cortex-m4 $(FP_FLAGS)
ASFLAGS = -mthumb -mcpu=cortex-m4

PREFIX		?= arm-none-eabi-

CC		:= $(PREFIX)gcc
CXX		:= $(PREFIX)g++
LD		:= $(PREFIX)g++
AR		:= $(PREFIX)ar
AS		:= $(PREFIX)as
OBJCOPY		:= $(PREFIX)objcopy
SIZE		:= $(PREFIX)size
OBJDUMP		:= $(PREFIX)objdump
GDB		:= $(PREFIX)gdb
STFLASH		= $(shell which st-flash)
OPT		:= -Os -g
DEBUG		:= -ggdb3
CSTD		?= -std=c11
CXXSTD		?= -std=c++11

INCLUDES = -I$(TOP_DIR)/inc
INCLUDES += -I$(OPENCM3_DIR)/include

LDSCRIPT	?= $(TOP_DIR)/stm32f4-discovery.ld
TGT_CFLAGS	+= $(OPT) $(CSTD)
TGT_CFLAGS	+= $(ARCH_FLAGS)
TGT_CFLAGS	+= -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS	+= -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS	+= -fno-common -ffunction-sections -fdata-sections
TGT_CFLAGS  += $(INCLUDES)
#TGT_CFLAGS	+= -I$(TOP_DIR)/rtos/libwwg/include

TGT_CXXFLAGS	+= $(OPT) $(CXXSTD)
TGT_CXXFLAGS	+= $(ARCH_FLAGS)
TGT_CXXFLAGS	+= -Wextra -Wshadow -Wredundant-decls  -Weffc++
TGT_CXXFLAGS	+= -fno-common -ffunction-sections -fdata-sections

TGT_CPPFLAGS	+= -MD
TGT_CPPFLAGS	+= -Wall -Wundef
TGT_CPPFLAGS	+= $(DEFS)
TGT_CPPFLAGS  	+= $(INCLUDES)
#TGT_CPPFLAGS	+= -I$(TOP_DIR)/rtos/libwwg/include

TGT_LDFLAGS	+= --static -nostartfiles
TGT_LDFLAGS	+= -T$(LDSCRIPT)
TGT_LDFLAGS	+= $(ARCH_FLAGS)
TGT_LDFLAGS	+= -Wl,-Map=$(BIN_DIR)/$(*).map
TGT_LDFLAGS	+= -Wl,--gc-sections

LDLIBS		+= -specs=nosys.specs
LDLIBS		+= -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group
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

all: libopencm3 $(BIN_DIR)/$(BINARY).bin 

# These rules simply denote where to place the build files in your system.
print-%:
	@echo $*=$($*)

libopencm3:
	$(MAKE) -C $(OPENCM3_DIR) TARGETS=stm32/f4

$(BIN_DIR)/%.bin: $(BIN_DIR)/%.elf
	@#printf "  OBJCOPY $@\n"
	$(OBJCOPY) -Obinary $< $@

$(BIN_DIR)/%.elf $(BIN_DIR)/%.map: $(OBJS) $(LDSCRIPT)
	$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@
	$(SIZE) $(BINARY).elf

$(OBJ_DIR)/%.o: %.c
	@#printf "  CC      $(*).c\n"
	mkdir -p $(dir $@)
	$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: %.cxx
	@#printf "  CXX     $(*).cxx\n"
	mkdir -p $(dir $@)
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: %.cpp
	@#printf "  CXX     $(*).cpp\n"
	mkdir -p $(dir $@)
	$(CXX) $(TGT_CXXFLAGS) $(CXXFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

$(OBJ_DIR)/%.o: %.asm
	$(AS) $(ASFLAGS) -o $*.o -c $<

clean:
	@#printf "  CLEAN\n"
	rm -f $(OPENCM3_DIR)/lib/libopencm3_stm32f4.a
	-$(MAKE) -$(MAKEFLAGS) -C $(OPENCM3_DIR) clean
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Flash Device
flash:	$(BINARY).bin
	# $(STFLASH) $(FLASHSIZE) write $(BINARY).bin 0x8000000
	$(STFLASH) write $(BINARY).bin 0x8000000

.PHONY: images clean elf bin hex srec list all

-include $(OBJS:.o=.d)