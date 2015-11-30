/*		tss.h

		The TSS stuff

	History:

		Version 0.01.00	06 Oct 2000	04:37		ObliVion
		Initial version.
*/
#ifndef _HIK_TSS_H_
#define _HIK_TSS_H_

struct tss
{
    unsigned long			back;
    unsigned long			esp0;
    unsigned long			ss0;
    unsigned long			esp1;
    unsigned long			ss1;
    unsigned long			esp2;
    unsigned long			ss2;
    unsigned long			cr3;
    unsigned long			eip;
    unsigned long			eflags;
    unsigned long			eax;
    unsigned long			ecx;
    unsigned long			edx;
    unsigned long			ebx;
    unsigned long			esp;
    unsigned long			ebp;
    unsigned long			esi;
    unsigned long			edi;
    unsigned long			es;
    unsigned long			cs;
    unsigned long			ss;
    unsigned long			ds;
    unsigned long			fs;
    unsigned long			gs;
    unsigned long			ldt;
    unsigned long			ioperm;
} __attribute__((packed));

extern struct tss		kernel_tss;
extern unsigned long		kstack;

extern void init_tss(void);
extern void	k32_startup(void);

#endif

