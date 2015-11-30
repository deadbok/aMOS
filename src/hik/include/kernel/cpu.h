/*		cpu.h

		Routines to detect and init CPU's compatbile with the I386

	History:

		Version 0.01	20 May 2000	00:42		ObliVion
		Initial version.
*/

#ifndef _HIK_CPU_H_
#define _HIK_CPU_H_

/* Feature flags */
#define X86_FEATURE_FPU		0x00000001	/* onboard FPU */
#define X86_FEATURE_VME		0x00000002	/* Virtual Mode Extensions */
#define X86_FEATURE_DE		0x00000004	/* Debugging Extensions */
#define X86_FEATURE_PSE		0x00000008	/* Page Size Extensions */
#define X86_FEATURE_TSC		0x00000010	/* Time Stamp Counter */
#define X86_FEATURE_MSR		0x00000020	/* Model-Specific Registers, RDMSR, WRMSR */
#define X86_FEATURE_PAE		0x00000040	/* Physical Address Extensions */
#define X86_FEATURE_MCE		0x00000080	/* Machine Check Exceptions */
#define X86_FEATURE_CX8		0x00000100	/* CMPXCHG8 instruction */
#define X86_FEATURE_APIC	0x00000200	/* onboard APIC */
#define X86_FEATURE_10		0x00000400	/* Reserved */ 
#define X86_FEATURE_SEP		0x00000800	/* Fast System Call */ 
#define X86_FEATURE_MTRR	0x00001000	/* Memory Type Range Registers */
#define X86_FEATURE_PGE		0x00002000	/* Page Global Enable */
#define X86_FEATURE_MCA		0x00004000	/* Machine Check Architecture */
#define X86_FEATURE_CMOV	0x00008000	/* CMOV instruction (FCMOVCC and FCOMI too if FPU present) */
#define X86_FEATURE_PAT		0x00010000	/* Page Attribute Table */
#define X86_FEATURE_PSE36	0x00020000	/* 36-bit PSEs */
#define X86_FEATURE_18		0x00040000	/* Reserved */ 
#define X86_FEATURE_19		0x00080000	/* Reserved */
#define X86_FEATURE_20		0x00100000	/* Reserved */ 
#define X86_FEATURE_21		0x00200000	/* Reserved */ 
#define X86_FEATURE_22		0x00400000	/* Reserved */ 
#define X86_FEATURE_MMX		0x00800000	/* multimedia extensions */
#define X86_FEATURE_FXSR	0x01000000	/* FXSAVE and FXRSTOR instructions (fast save and restore of FPU context), and CR4.OSFXSR*/

struct cpuid_regs
{
	unsigned long	eax		__attribute__ ((packed));
	unsigned long	ebx		__attribute__ ((packed));
	unsigned long	ecx		__attribute__ ((packed));
	unsigned long	edx		__attribute__ ((packed));
};

struct cpuid_info_x86
{
	unsigned char	supported;
	unsigned long	std_func_level;
	unsigned long	ext_func_level;
};

struct cpu_info_x86
{
	unsigned char			vendor;
	unsigned char			family;
	unsigned char			model;
	unsigned char			stepping;
	unsigned long			feat_flags;
	struct cpuid_info_x86	cpuid_info;
};

/* Vendor identification numbers */
#define CPU_INTEL	0
#define CPU_AMD		1
#define CPU_CYRIX	2
#define CPU_UNKNOWN	3

struct cpu_vend_x86
{
	int		id;
	char	*vendorID;
	char	*vendor_name;
};

extern int init_cpu(void);

#endif
