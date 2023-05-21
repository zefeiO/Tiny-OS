GCC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
GCCFLAGS = -mcpu=cortex-a7 -fpic -ffreestanding -std=gnu99 -O2 -Wall -Wextra

all: tinyos.img

boot.o: boot.S
	$(GCC) -mcpu=cortex-a7 -fpic -ffreestanding -c boot.S -o boot.o

kernel.o: kernel.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

tinyos.img: boot.o kernel.o
	$(GCC) -T linker.ld -o tinyos.elf -ffreestanding -O2 -nostdlib boot.o kernel.o
	$(OBJCOPY) -O binary tinyos.elf tinyos.img

clean:
	rm boot.o kernel.o tinyos.img tinyos.elf