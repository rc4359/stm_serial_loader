-include ../../../configs/product_config.make

CONFIG_MAKE = ../../../Rules.make

ifeq ($(wildcard $(CONFIG_MAKE)),$(CONFIG_MAKE))
include $(CONFIG_MAKE)
endif

.PHONY: all clean depend

OBJS = uart.o stm_command.o main.o

CFLAGS += -Wall

INCLUDES = -I./include 

APCFG_LIBS = -lrt

ifeq ($(CONFIG_HAS_LATEST_TOOLCHAIN),y)
APCFG_LIBS += $(CROSSLIBS_USER_LIB_PATH)/libpcre.so -lgcc_s
else
APCFG_LIBS += -L$(CROSS_COMPILE_USER_LIBS_PATH) -lpcre
endif

LIBS = $(APCFG_LIBS)

# 20140630, idleman: add for kernel v3.12.x, modify product_config.make to enable it
ifeq ($(CONFIG_HAS_KERNEL_3_12),y)
CFLAGS += -DCONFIG_KERNEL_3_12=1
endif

all: stm_serial_loader

stm_serial_loader: $(OBJS)
	@echo 'CC $@'
	@$(CC)  $(CFLAGS) $(INCLUDES) -o $@ $? $(LIBS) -lpthread

%.o: %.c
	@echo 'CC $@'
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

clean:
	rm -f *.elf *.gdb *.o *~ *.a .depend cscope* syslog include/*~
	rm stm_serial_loader
depend: 

install: stm_serial_loader $(DESTDIR) $(APP_BIN_PATH)
	install stm_serial_loader $(DESTDIR)
	install stm_serial_loader $(APP_BIN_PATH)

