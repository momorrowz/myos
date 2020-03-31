# デフォルト動作

default :
	make img

# ファイル生成規則

ipl.bin : ipl.asm
	nasm ipl.asm -o ipl.bin -l ipl.lst

haribote.sys : haribote.asm Makefile
	nasm haribote.asm -o haribote.sys -l haribote.lst

haribote.img : ipl.bin haribote.sys Makefile
	mformat -f 1440 -C -B ipl.bin -i haribote.img ::
	mcopy haribote.sys -i haribote.img ::

# コマンド

asm :
	make ipl.bin

img :
	make haribote.img

run :
	make img
	qemu-system-i386 -fda haribote.img

clean :
	rm -rf *.img *.bin *.lst *.sys
