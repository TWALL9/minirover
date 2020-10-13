# # Be silent per default, but 'make V=1' will show all compiler calls.
# ifneq ($(V),1)
# Q		:= @
# NULL		:= 2>/dev/null
# endif

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