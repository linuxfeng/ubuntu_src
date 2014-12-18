#
# Makefile for the Altix device drivers.
#
#

obj-m := libNet.o
libNet-objs := net_manager.o client.o


KERNELDIR ?= /lib/modules/$(shell uname -r)/build

PWD:=$(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR)  M=$(PWD) modules
clean:
	rm -rf *.o *.mod.c *.mod.o *.ko
	

