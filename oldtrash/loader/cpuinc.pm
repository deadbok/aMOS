;	cpu.inc
;
;	CPU related routines for the aMOS loader
;
;	History:
;
;		Version 0.1		17 Apr 2000  19:24		ObliVion
;		Initial version.

%ifndef		LOADER_CPU_INC
%define		LOADER_CPU_INC

;-----------------------------------------------------------------------------
;       check_386:			Check if the CPU is 386 or higher
;
;       Return:				1	the CPU is not 386 or higher
;							0	the CPU is 386 or higher
;-----------------------------------------------------------------------------
BITS 16

check_386
				pushf				; 8088/86
				pop 	ax
				mov 	cx, ax
				and 	ax, 0fffh
				push 	ax
				popf
				pushf
				pop		ax
				and		ax, 0f000h
				cmp		ax, 0f000h
				je		.no386

.no86:			or 		cx, 0f000h		; 80286
				push 	cx
				popf 	
				pushf
				pop 	ax
				and 	ax, 0f000h
				jnz		short .386

.no386:			mov		ax,	1
				jmp		short .exit

.386:			mov		ax, 0			; 80386 ;=) COOL;
.exit:			ret


;-----------------------------------------------------------------------------
;       pm_switch:			Switches into protected mode
;
;-----------------------------------------------------------------------------

BITS 16
pm_switch:

				cli
				
				lgdt	[ABS(gdt_ptr)]

				mov		eax, cr0
				or		eax, 01h
				mov		cr0, eax

				jmp		dword (gdt_code-gdt):pm_code_seg

BITS 32

pm_code_seg:		
				mov		ax, gdt_data-gdt
				mov		ds, ax
				mov		es, ax
				mov		fs, ax
				mov		gs, ax
				mov		ss, ax

				mov		eax, [esp]			;preserve return address at RM_STACK
				mov		[RM_STACK], eax

				mov		eax, [pm_stack]
				mov		esp, eax
				mov		ebp, eax

				mov		eax, [RM_STACK]		;get return address
				mov		[esp], eax
			
				ret 

;-----------------------------------------------------------------------------
;       rm_switch:			Switches into real mode
;
;-----------------------------------------------------------------------------

BITS 32
rm_switch:
				lgdt	[ABS(gdt_ptr)]


				mov		eax, esp			;Save pm stack
				mov		ebx, [pm_stack]
				mov		[ebx], eax

				mov		eax, [esp]			;save return		
				mov		[RM_STACK], eax

				mov		eax, RM_STACK		;Set rm stack
				mov		esp, eax
				mov		ebp, eax

				mov		ax, gdt_data16-gdt
				mov		ds, ax
				mov		es, ax
				mov		fs, ax
				mov		gs, ax
				mov		ss, ax

;	/* this might be an extra step */
;	ljmp	$PSEUDO_RM_CSEG, $tmpcseg	/* jump to a 16 bit segment */
				jmp		dword (gdt_code16-gdt):temp_code16_seg

temp_code16_seg:
;	.code16

;	/* clear the PE bit of CR0 */

BITS 16

				mov		eax, cr0
				and		eax, 0fffffffeh
				mov		cr0, eax

;	/* flush prefetch queue, reload %cs */
;	DATA32	ljmp	$0, $realcseg

				jmp		dword 0h:rm_code_seg

rm_code_seg:		
;	/* we are in real mode now
;	 * set up the real mode segment registers : DS, SS, ES
;	 */
				cli
				
				xor		ax, ax
				mov     ds, ax
				mov     es, ax
				mov     fs, ax
				mov     gs, ax
				mov		ss, ax

				sti

				ret


%endif
