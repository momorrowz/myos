SRC_DIR = ./src
ASM_DIR = ./asm
BIN_DIR = ./build
SRCS := $(wildcard $(SRC_DIR)/*.c)

default:
	make img

$(BIN_DIR)/ipl10.bin : $(ASM_DIR)/ipl10.asm
	@if [ ! -d $(BIN_DIR) ]; \
      then echo "mkdir -p $(BIN_DIR)"; mkdir -p $(BIN_DIR); \
      fi
	nasm $< -o $@ -l $(BIN_DIR)/ipl10.lst

$(BIN_DIR)/asmhead.bin : $(ASM_DIR)/asmhead.asm
	nasm $< -o $@ -l $(BIN_DIR)/asmhead.lst

$(BIN_DIR)/nasmfunc.o : $(ASM_DIR)/nasmfunc.asm
	nasm -g -f elf $< -o $@

#$(BIN_DIR)/hankaku_converter.o : $(SRC_DIR)/hankaku_converter.c
#	gcc -o $@ $<

#$(SRC_DIR)/hankaku.c : $(BIN_DIR)/hankaku_converter.o
#	./$<

$(BIN_DIR)/bootpack.hrb : $(SRCS) har.ld $(BIN_DIR)/nasmfunc.o
	gcc -march=i486 -m32 -fno-pic -nostdlib -T har.ld -g $(SRCS) $(BIN_DIR)/nasmfunc.o -o $@

$(BIN_DIR)/haribote.sys : $(BIN_DIR)/asmhead.bin $(BIN_DIR)/bootpack.hrb
	cat $(BIN_DIR)/asmhead.bin $(BIN_DIR)/bootpack.hrb > $@

$(BIN_DIR)/haribote.img : $(BIN_DIR)/ipl10.bin $(BIN_DIR)/haribote.sys
	mformat -f 1440 -C -B $(BIN_DIR)/ipl10.bin -i $@ ::
	mcopy $(BIN_DIR)/haribote.sys -i $@ ::

.PHONY: asm img run debug clean 
asm :
	make -r $(BIN_DIR)/ipl10.bin

img :
	make -r $(BIN_DIR)/haribote.img

run :
	make img
	qemu-system-i386 -fda $(BIN_DIR)/haribote.img  # "-fda" for floppy disk

debug :
	make img
	qemu-system-i386 -fda $(BIN_DIR)/haribote.img -gdb tcp::10000 -S

clean :
	rm -rf build
