obj-m := pl_parallel.o
pl_parallel-obj += pl_parallel_module.o

EXTRA_CFLAGS += -I$(PWD)/include -std=gnu11 -Wall -Ofast
KDIR ?= /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean