# デフォルト動作

default :
	make img

# ファイル生成規則

ipl.bin : ipl.asm
	nasm ipl.asm -o ipl.bin -l ipl.lst

haribote.img : ipl.bin
	cat ipl.bin  > haribote.img

# コマンド

asm :
	make ipl.bin

img :
	make haribote.img

run :
	make img
	qemu-system-i386 -fda haribote.img

clean :
	rm -rf *.img *.bin *.lst
