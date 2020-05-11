#
# Machine makefile
#

DEFINES		+= -D__ARM32_ARCH__=6 

ASFLAGS		:= -g -ggdb -Wall -O3
CFLAGS		:= -g -ggdb -Wall -O3
MCFLAGS		:= -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=softfp -marm -mno-thumb-interwork
LDFLAGS		:= -T arch/$(ARCH)/$(MACH)/xboot.ld -nostdlib

LIBDIRS		:=
LIBS 		:= `$(CROSS_COMPILE)gcc -print-file-name=libgcc.a $(MCFLAGS)`
INCDIRS		:=
SRCDIRS		:=

xend:
	@echo Generate kernel.img For Raspberry Pi 0/1
	@$(CP) $(X_NAME).bin $(X_OUT)/kernel.img
