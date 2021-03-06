CONFIG_MODULE_SIG = n
TARGET_MODULE := fibdrv_mudule

obj-m := $(TARGET_MODULE).o 

$(TARGET_MODULE)-y = \
	fibdrv.o \
	bignum/bignum.o \
	bignum/apm.o \
	bignum/sqr.o \
	bignum/mul.o \
	bignum/format.o \

ccflags-y := -std=gnu99 -Wno-declaration-after-statement

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

GIT_HOOKS := .git/hooks/applied

all: $(GIT_HOOKS) client
	$(MAKE) -C $(KDIR) M=$(PWD) modules

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	$(RM) client client_timer out
	$(RM) *.png
	$(RM) *.data
	$(RM) expected.txt
load:
	sudo insmod $(TARGET_MODULE).ko
unload:
	sudo rmmod $(TARGET_MODULE) || true >/dev/null

client: client.c
	$(CC) -o $@ $^

client_timer: client.c
	$(CC) -D USERSPACE_TIMER -o $@ $^

PRINTF = env printf
PASS_COLOR = \e[32;01m
NO_COLOR = \e[0m
pass = $(PRINTF) "$(PASS_COLOR)$1 Passed [-]$(NO_COLOR)\n"

check: all
	$(MAKE) unload
	$(MAKE) load
	sudo ./client > out
	$(MAKE) unload
	python3 scripts/fib-gen.py > expected.txt
	@diff -u out expected.txt && $(call pass)
	@rm expected.txt out
