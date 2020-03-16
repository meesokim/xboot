#
# Machine makefile
#

DEFINES		+= -D__ARM32_ARCH__=7 -D__CORTEX_A7__ -D__ARM32_NEON__

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
MCFLAGS		:= -march=armv7-a -mtune=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard -marm -mno-thumb-interwork
LDFLAGS		:= -T arch/$(ARCH)/$(MACH)/xboot.ld -nostdlib

LIBDIRS		:=
LIBS 		:= `$(CROSS_COMPILE)gcc -print-file-name=libgcc.a $(MCFLAGS)`
INCDIRS		:=
SRCDIRS		:=

xend:
	@echo Generate kernel7.img For Raspberry Pi 2
	@$(CP) $(X_NAME).bin $(X_OUT)/kernel7.img
