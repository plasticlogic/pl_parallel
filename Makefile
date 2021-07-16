use_dma = y

obj-m := pl_parallel.o
pl_parallel-objs += pl_parallel_module.o
pl_parallel-objs += ctrl/am335x_ctrl.o

EXTRA_CFLAGS += -I$(PWD)/include -std=gnu11 -Wall -O3
KDIR ?= /lib/modules/$(shell uname -r)/build

ifeq ($(use_dma),y)
	EXTRA_CFLAGS += -DBURST_DMA
endif

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean