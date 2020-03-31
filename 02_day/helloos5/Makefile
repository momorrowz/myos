.PHONY: clean
default:
	make img

ipl.bin: ipl.asm
	nasm ipl.asm -o ipl.bin -l ipl.lst
tail.bin: tail.asm
	nasm tail.asm -o tail.bin -l tail.lst
helloos.img: ipl.bin tail.bin
	cat ipl.bin tail.bin > helloos.img

asm:
	make -r ipl.bin
img:
	make -r helloos.img
run:
	make img
	qemu-system-i386 helloos.img
clean:
	rm -rf *.img *.bin *.lst