/*		tss.c

		The TSS stuff

	History:

		Version 0.01.00	06 Oct 2000	04:37		ObliVion
		Initial version.
*/
#include <string.h>
#include <stdio.h>
#include <kernel/dt.h>
#include <kernel/tss.h>
#include <kernel/setup.h>
#include <kernel/selector.h>

struct tss		kernel_tss;

void ltr(unsigned short selector);

void init_tss(void)
{
	printf("Initializing hardware task switching...\n");

	kernel_tss.back=0;
	kernel_tss.esp0=(unsigned long)&kstack;
	kernel_tss.ss0=KERNEL_DATA_SEL;		
	kernel_tss.esp1=0;
	kernel_tss.ss1=0;
	kernel_tss.esp2=0;
	kernel_tss.ss2=0;
	kernel_tss.cr3=0;
	kernel_tss.eip=(unsigned long)k32_startup;
	kernel_tss.eflags=0;
	kernel_tss.eax=0;
	kernel_tss.ecx=0;
	kernel_tss.edx=0;
	kernel_tss.ebx=0;
	kernel_tss.esp=(unsigned long)&kstack;
	kernel_tss.ebp=(unsigned long)&kstack;
	kernel_tss.esi=0;
	kernel_tss.edi=0;
	kernel_tss.es=KERNEL_DATA_SEL;
	kernel_tss.cs=KERNEL_CODE_SEL;
	kernel_tss.ss=KERNEL_DATA_SEL;
	kernel_tss.ds=KERNEL_DATA_SEL;
	kernel_tss.fs=KERNEL_DATA_SEL;
	kernel_tss.gs=KERNEL_DATA_SEL;
	kernel_tss.ldt=0;
	kernel_tss.ioperm=0;

	ltr(KERNEL_TSS_SEL);
}

void ltr(unsigned short selector)
{
	__asm__ __volatile__ ("ltr %0": :"r" (selector));
}

