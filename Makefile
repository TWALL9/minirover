#################################################################################
# Example modified from Warren Gay's opencm3 implementation for the Bluepill   	#
# of which rules were modified from the original libopencm3 makefiles.			#
# https://github.com/ve3wwg/stm32f103c8t6 for original implementation			#
#################################################################################

.PHONY = libopencm3 clean all

all: libopencm3
	$(MAKE) -C src/

libopencm3: libopencm3/lib/libopencm3_stm32f4.a

libopencm3/lib/libopencm3_stm32f4.a:
	$(MAKE) -C libopencm3 TARGETS=stm32/f4

clean:
	rm -f libopencm3/libopencm3_stm32f4.a
	-$(MAKE) -$(MAKEFLAGS) -C ./libopencm3 clean
	-$(MAKE) -$(MAKEFLAGS) -C ./src clean