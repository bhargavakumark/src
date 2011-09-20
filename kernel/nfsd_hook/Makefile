# $Id $
#ident "$Source $"

# $Copyright $

CC = gcc
KERNELDIR ?= /linux/x86_64-2.6.32.12-0.7
obj-m := nfsd_hook.o
#nfsd_hook-y := nfsd_hook_proc.o

all:
	make -C $(KERNELDIR)/default M=`pwd` CC=$(CC) modules
clean:
	make -C $(KERNELDIR)/default M=`pwd` clean
tags:
	ctags -R *.[ch] /linux/x86_64-2.6.32.12-0.7/default
	
