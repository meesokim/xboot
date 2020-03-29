SFILES	:= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.S))
CFILES	:= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.c))
PFILES	:= $(foreach dir, $(SRCDIR), $(wildcard $(dir)/*.cpp))

SDEPS	:= $(patsubst %, .obj/%, $(SFILES:.S=.o.d))
CDEPS	:= $(patsubst %, .obj/%, $(CFILES:.c=.o.d))
PDEPS	:= $(patsubst %, .obj/%, $(PFILES:.cpp=.o.d))
DEPS	:= $(SDEPS) $(CDEPS) $(PDEPS)

SOBJS	:= $(patsubst %, .obj/%, $(SFILES:.S=.o))
COBJS	:= $(patsubst %, .obj/%, $(CFILES:.c=.o))
POBJS  	:= $(patsubst %, .obj/%, $(PFILES:.cpp=.o))
OBJS	:= $(SOBJS) $(COBJS) $(POBJS)

ifneq ($(strip $(NAME)),)
all : $(NAME)
else
all : $(OBJS)
endif

$(NAME) : $(OBJS)
ifneq ($(strip $(OBJS)),)
	$(LD) -r -o $@ $^
else
	@$(AR) -rcs $@
endif

$(SOBJS) : .obj/%.o : %.S
	@echo [AS] $<
	@$(AS) $(X_ASFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

$(COBJS) : .obj/%.o : %.c
	@echo [CC] $@ $<
	@$(CC) $(X_CFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

$(POBJS) : .obj/%.o : %.cpp
	@echo [CXX] $<
	@$(CROSS_COMPILE)$(CXX) $(X_CFLAGS) -MD -MP -MF $@.d $(X_INCDIRS) -c $< -o $@

sinclude $(DEPS)
