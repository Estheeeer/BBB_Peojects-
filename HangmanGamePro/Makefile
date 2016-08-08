#Makefile for game_driver.c

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
ccflags-y := -std=gnu99

obj-m += game_driver.o

# Kernel source directory
KDIR =/home/station98/bb-kernel/KERNEL
#/opt/PHYTEC_BSPs/yocto_ti/build/tmp-glibc/work/phyboard_wega_am335x_1-phytec-linux-gnueabi/linux-ti/3.12.30-phy2-r0/git
PWD = $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
clean:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) clean
