;	disk.inc
;
;	Disk IO routines for the aMOS loader
;
;	History:
;
;		Version 0.1		18 Apr 2000  14:33		ObliVion
;		Initial version.

%ifndef		LOADER_DISK_INC
%define		LOADER_DISK_INC

;-----------------------------------------------------------------------------
;		int _disk_reset(char drive):		Resets the disk controller
;
;-----------------------------------------------------------------------------

proc disk_reset
						mov		dl, [bp+%$arg]
						xor		eax, eax
						int		13h
						xor		al, al
						xchg	al, ah
endproc

;-----------------------------------------------------------------------------
;		_disk_info_standard(char drive, ):		Uses standard int 13 AH=08h to get disk info
;
;-----------------------------------------------------------------------------

proc disk_info_standard
						mov		dl, [bp+%$arg]
						mov		eax, 0800h
						int		13h 

						jc		.exit

						xor		eax, eax	;heads
						mov		al, dh
						inc		eax
						mov		si, [bp+%$arg+4]
						mov		[si], eax
						
						xor		eax, eax	;sector
						mov		al, cl
						and		al, 00111111b
						mov		si, [bp+%$arg+6]
						mov		[si], eax

						xor		eax, eax	;cylinder
						mov		cl, al
						and		al, 11000000b
						shl		eax, 2
						mov		al, ch
						inc		eax
						mov		si, [bp+%$arg+2]
						mov		[si], eax

						xor		eax, eax			;everything should be clear now!!!

.exit:
						xor		al, al
						xchg	al, ah


endproc

;-----------------------------------------------------------------------------
;		int disk_read_standard((char disk,
;							    unsigned int cylinder,
;							    unsigned int head,
;							    unsigned int sector,
;							    unsigned int sectors,
;							    unsigned int segment));
;
;		Uses standard int 13 AH=02h to read sectors from disk
;
;-----------------------------------------------------------------------------

proc disk_read_standard
						mov		eax, [bp+%$arg+2]
						mov		ch, al

						mov		al, [bp+%$arg+6]	; al=sector. rest of eax cylinder
						shl		al, 2			
						shr		ax, 2
						mov		cl, al

						mov		eax, [bp+%$arg+4]    ;head
						mov		dh, al

						mov		eax, [bp+%$arg+10]
						mov		es, ax
						xor		ebx, ebx

						mov		eax, [bp+%$arg]
						mov		dl, al

						mov		eax, [bp+%$arg+8]
						mov		ah, 02h
						mov		di, ax

						mov		si, 3

.read					int		13h
						jnc		.ok
						
						xor		eax, eax
						int		13h

						cmp		si, 0
						je		.exit
						
						dec		si
						mov		ax, di
						jmp		.read

.ok:
						xor		eax, eax			;everything should be clear now!!!
						jnc		.exit

.exit:
						xor		al, al
						xchg	al, ah
endproc

%endif
