.386
.model flat, stdcall
option casemap :none
include masm32\include\windows.inc
include masm32\include\kernel32.inc
include masm32\include\masm32.inc
include masm32\include\user32.inc
include masm32\include\msvcrt.inc
includelib masm32\lib\kernel32.lib
includelib masm32\lib\masm32.lib
includelib masm32\lib\user32.lib
includelib masm32\lib\msvcrt.lib

.DATA
;===User Data==============
	a	dw	00h
	b	dw	00h
	x	dw	00h

;===Addition Data===========================================
	hConsoleInput	dd	?
	hConsoleOutput	dd	?
	ErrorMessage	db	"Error: division by zero", 0
	InputBuf	db	15 dup (?)
	InMessage	db	"SCAN: ", 0
	OutMessage	db	"PRINT: "," %d",0
	ResMessage	db	20 dup (?)
	NumberOfCharsRead	dd	?
	NumberOfCharsWrite	dd	?
	msg1310	dw	13,10
	buf	dd	0
	lb1	dd	?
	lb2	dd	?

.CODE
start:
invoke AllocConsole
invoke GetStdHandle, STD_INPUT_HANDLE
mov hConsoleInput, EAX
invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hConsoleOutput, EAX
finit
	call Input
	fild buf
	fistp a
	call Input
	fild buf
	fistp b
	call Input
	fild buf
	fistp x
	fild a
	fild b
	call Eq_
	fild b
	fild x
	call Eq_
	call And_
	fild a
	fild x
	call Eq_
	call And_
	fistp buf
	cmp word ptr buf, 0
je ifLabel1
	mov dword ptr buf,1
	fild buf
	fistp buf
	call Output
	jmp endIf1
ifLabel1:
	mov dword ptr buf,0
	fild buf
	fistp buf
	call Output
endIf1:
	fild a
	mov dword ptr buf,0
	fild buf
	call Less_
	fild b
	mov dword ptr buf,0
	fild buf
	call Less_
	call Or_
	fild x
	mov dword ptr buf,0
	fild buf
	call Less_
	call Or_
	fistp buf
	cmp word ptr buf, 0
je ifLabel2
	mov dword ptr buf,1
	fild buf
	FLDZ
	FSUBR
	fistp buf
	call Output
	jmp endIf2
ifLabel2:
	mov dword ptr buf,0
	fild buf
	fistp buf
	call Output
endIf2:
	fild a
	fild b
	fild x
	fadd
	call Less_
	call Not_
	fistp buf
	cmp word ptr buf, 0
je ifLabel3
	mov dword ptr buf,10
	fild buf
	fistp buf
	call Output
	jmp endIf3
ifLabel3:
	mov dword ptr buf,0
	fild buf
	fistp buf
	call Output
endIf3:
exit_label:
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,1,ADDR NumberOfCharsRead,0
invoke ExitProcess, 0

;===Procedure Input==========================================================================
Input PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke WriteConsoleA,hConsoleOutput,ADDR InMessage,SIZEOF InMessage,ADDR NumberOfCharsWrite,0
invoke ReadConsoleA,hConsoleInput,ADDR InputBuf,11,ADDR NumberOfCharsRead,0
invoke  crt_atoi, addr InputBuf
mov dword ptr buf, eax
ret
Input ENDP
;============================================================================================


;===Procedure Output=========================================================================
Output PROC
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
invoke wsprintf, addr ResMessage, addr OutMessage, dword ptr buf
invoke WriteConsoleA,hConsoleOutput,ADDR ResMessage,eax,ADDR NumberOfCharsWrite,0
ret
Output ENDP
;============================================================================================


;===Procedure Mod=========================
Mod_ PROC

	fistp lb1
	fistp lb2
	fild lb1
	fild lb2
	fprem
	fistp lb1
	fistp lb2
	fild lb1
	ret
Mod_ ENDP
;=========================================


;===Procedure And=========================
And_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	cmp eax,0
	jnz and_t1
	jz and_false
and_t1:
	mov eax,lb2
	cmp eax,0
	jnz and_true
and_false:
	fldz
	jmp and_fin
and_true:
	fld1
and_fin:

	popf
	pop edx
	pop eax

	ret
And_ ENDP
;=========================================


;===Procedure Or==========================
Or_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	cmp eax,0
	jnz or_true
	mov eax,lb2
	cmp eax,0
	jnz or_true
	fldz
	jmp or_fin
or_true:
	fld1
or_fin:

	popf
	pop edx
	pop eax

	ret
Or_ ENDP
;=========================================


;===Procedure Not=========================
Not_ PROC
	push eax
	pushf
	fistp lb1
	mov eax,lb1
	cmp eax,0
	jne not_false
	fld1
	jmp not_fin
not_false:
	fldz
not_fin:

	popf
	pop eax

	ret
Not_ ENDP
;=========================================


;===Procedure Eq==========================
Eq_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	mov edx,lb2
	cmp eax,edx
	jne not_eq
	fld1
	jmp eq_fin
not_eq:
	fldz
eq_fin:
	popf
	pop edx
	pop eax

	ret
Eq_ ENDP
;=========================================


;===Procedure Greate======================
Greate_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax,lb1
	mov edx,lb2
	cmp edx,eax
	jle lov
	fld1
	jmp gr_fin
lov:
	fldz
gr_fin:
	popf
	pop edx
	pop eax

	ret
Greate_ ENDP
;=========================================


;===Procedure Less========================
Less_ PROC
	push eax
	push edx
	pushf
	fistp lb1
	fistp lb2
	mov eax, lb1
	mov edx, lb2
	cmp edx,eax
	jge gr
lo:
	fld1
	jmp less_fin
gr:
	fldz
less_fin:
	popf
	pop edx
	pop eax

	ret
Less_ ENDP
;=========================================


;===Procedure Check=======================
Check_ PROC
	push eax
	fldz
	fcomp
	fstsw ax
	sahf
	jne end_check
invoke WriteConsoleA,hConsoleOutput,ADDR msg1310,SIZEOF msg1310,ADDR NumberOfCharsWrite,0
	invoke WriteConsoleA, hConsoleOutput, ADDR ErrorMessage, SIZEOF ErrorMessage, ADDR NumberOfCharsWrite, 0
	jmp exit_label
end_check:
	pop eax
	ret
Check_ ENDP
;=========================================

end start
