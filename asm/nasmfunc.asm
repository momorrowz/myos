section .text
		global io_hlt
		global io_cli,io_sti,io_stihlt
		global io_in8,io_in16,io_in32
		global io_out8,io_out16,io_out32
		global io_load_eflags,io_store_eflags
		global load_gdtr,load_idtr


io_hlt:	; void io_hlt(void);
		hlt
		ret

io_cli:	; void io_cli(void);
		cli
		ret

io_sti:	; void io_sti(void);
		sti
		ret

io_stihlt:	; void io_stihlt(void);
		sti
		hlt
		ret

io_in8: ; int io_in8(int port);
		mov edx,[esp+4]
		mov eax,0
		in al,dx
		ret

io_in16: ; int io_in16(int port);
		mov edx,[esp+4]
		mov eax,0
		in ax,dx
		ret

io_in32: ; int io_in32(int port);
		mov edx,[esp+4]
		mov eax,0
		;in eax,edx
		in eax,dx
		ret

io_out8: ;void io_out8(int port,int data);
		mov edx,[esp+4]
		mov al,[esp+8]
		out dx,al
		ret

io_out16: ;void io_out16(int port,int data);
		mov edx,[esp+4]
		mov eax,[esp+8]
		out dx,ax
		ret
io_out32: ;void io_out32(int port,int data);
		mov edx,[esp+4]
		mov eax,[esp+8]
		out dx,eax
		ret

io_load_eflags: ;int io_load_eflags();
		pushfd
		pop eax
		ret

io_store_eflags: ;void io_store_eflags(int flags);
		mov eax,[esp+4]
		push eax
		popfd	
		ret

load_gdtr:		; void load_gdtr(int limit, int addr);
		mov		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET

load_idtr:		; void load_idtr(int limit, int addr);
		MOV		AX,[ESP+4]		; limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET