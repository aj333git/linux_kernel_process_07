# Makefile for versioned unified_vma_inspector kernel module

obj-m += unified_vma_inspector.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

# VERSION variable, default 1.1
VERSION ?= 1.1
KO_FILE := unified_vma_inspector_$(VERSION).ko

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	mv -f unified_vma_inspector.ko $(KO_FILE)
	@echo "Built module: $(KO_FILE)"

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f unified_vma_inspector_*.ko

