/*		cpu.c

		Routines to detect and init CPU's compatible with the I386

	History:

		Version 0.02.00	28 May 2000	19:58		ObliVion
		Added AMD specific extended functions support		

		Version 0.01.01	28 May 2000	18:05		ObliVion
		Bug fix.
		I don't think "print_cpu_model" would report any CPU-type correctly

		Version 0.01.00	20 May 2000	00:42		ObliVion
		Initial version.

	Info from:
		Intel Processor Identification and the CPUID Instruction
*/
#include <stdio.h>
#include <string.h>
#include <kernel/cpu.h>

struct cpu_info_x86		cpu_info;
struct cpu_vend_x86		cpu_vend[]=
						{
							{CPU_INTEL, "GenuineIntel", "Intel"},
							{CPU_AMD, "AuthenticAMD", "AMD"},
							{CPU_CYRIX, "CyrixInstead", "Cyrix"},
							{CPU_UNKNOWN, "Unknown"}
						};

/*
 * These are standard capabilities definitions shared by all know processors at current release date
 * vendor specific extension is added when vendor is detected
 */
char 					*x86_feat[] =
						{"fpu", "vme", "de", "pse", "tsc", "msr", "pae", "mce",
						 "cx8", "apic", "10", "sep", "mtrr", "pge", "mca", "cmov",
						 "16", "pse36", "18", "19", "20", "21", "22", "mmx",
						 "24", "25", "26", "27", "28", "29", "30", "31"
						};

int				detect_cpu(void);
int 			get_amd_ext_func_level(void);
unsigned long 	get_amd_ext_feat(void);
void		 	get_amd_name(char *cpu_name);
int 			get_vendor(char *vendor);
int				print_cpu_model(void);
void 			print_cpu_info(struct cpu_info_x86 *cpu_inf);
int				check_cpuid(void);
void 			cpuid(int op, struct cpuid_regs *regs);
int 			is_486(void);
int 			is_386DX(void);
int 			has_fpu(void);

int init_cpu(void)
{
	printf("Initializing CPU...\n");

	detect_cpu();
	print_cpu_info(&cpu_info);

	return(0);
}

int detect_cpu(void)
{
	struct cpuid_regs		regs;
	char					vendor[12];

	printf("Detecting CPU...\n");

	if (check_cpuid())
	{
		cpu_info.cpuid_info.supported=1;
		printf("Using CPUID...\n");

		cpuid(0, &regs);

		cpu_info.cpuid_info.std_func_level=regs.eax;

		memcpy(vendor, &regs.ebx, sizeof(regs.ebx));
		memcpy(vendor+4, &regs.edx, sizeof(regs.edx));
		memcpy(vendor+8, &regs.ecx, sizeof(regs.ecx));
		vendor[12]='\0';

		cpu_info.vendor=get_vendor(vendor);

		if (cpu_info.cpuid_info.std_func_level>0)
		{
			cpuid(1, &regs);

			cpu_info.family=(char)((regs.eax>>8) & 0xf);
			cpu_info.model=(char)((regs.eax>>4) & 0xf);
			cpu_info.stepping=(char)(regs.eax & 0xf);
			cpu_info.feat_flags=regs.edx;

			switch(cpu_info.vendor)
			{
				case CPU_INTEL:		x86_feat[16]="pat";
									x86_feat[24]="fxsr";
									break;

				case CPU_AMD:		x86_feat[15]="icmov";
									x86_feat[16]="fcmov";
									x86_feat[31]="3DNow!";
									cpu_info.cpuid_info.ext_func_level=get_amd_ext_func_level();
									if (cpu_info.cpuid_info.ext_func_level>0)
										cpu_info.feat_flags|=get_amd_ext_feat();;
									break;

			    default:			/* Unknown CPU vendor*/
									break;
			}

		}
	}
	else
	{
		cpu_info.cpuid_info.supported=0;
		cpu_info.feat_flags=0;
		cpu_info.stepping=0;

		printf("Using anal probe detection...\n");

		if (is_486())
		{
			cpu_info.vendor=CPU_UNKNOWN;
			cpu_info.cpuid_info.std_func_level=0;
			cpu_info.family=4;
			cpu_info.model=0;
		}
		else
			if (is_386DX())
			{
				cpu_info.vendor=CPU_UNKNOWN;
				cpu_info.cpuid_info.std_func_level=0;
				cpu_info.family=3;
				cpu_info.model=0;
			}
			else
			{
				cpu_info.vendor=CPU_UNKNOWN;
				cpu_info.cpuid_info.std_func_level=0;
				cpu_info.family=3;
				cpu_info.model=1;
			}

		if (has_fpu())
			cpu_info.feat_flags=X86_FEATURE_FPU;

	}

	return(0);
}

int get_amd_ext_func_level(void)
{
	struct cpuid_regs		regs;

	cpuid(0x80000000, &regs);

	return(regs.eax);
}

unsigned long get_amd_ext_feat(void)
{
	struct cpuid_regs		regs;

	cpuid(0x80000001, &regs);

	return(regs.edx);
}

void get_amd_name(char *cpu_name)
{
	struct cpuid_regs		regs;

	cpuid(0x80000002, &regs);
	memcpy(cpu_name, &regs.eax, sizeof(regs.eax));
	memcpy(cpu_name+4, &regs.ebx, sizeof(regs.ebx));
	memcpy(cpu_name+8, &regs.ecx, sizeof(regs.ecx));
	memcpy(cpu_name+12, &regs.edx, sizeof(regs.edx));

	cpuid(0x80000003, &regs);
	memcpy(cpu_name+16, &regs.eax, sizeof(regs.eax));
	memcpy(cpu_name+20, &regs.ebx, sizeof(regs.ebx));
	memcpy(cpu_name+24, &regs.ecx, sizeof(regs.ecx));
	memcpy(cpu_name+28, &regs.edx, sizeof(regs.edx));

	cpuid(0x80000004, &regs);
	memcpy(cpu_name+32, &regs.eax, sizeof(regs.eax));
	memcpy(cpu_name+36, &regs.ebx, sizeof(regs.ebx));
	memcpy(cpu_name+40, &regs.ecx, sizeof(regs.ecx));
	memcpy(cpu_name+44, &regs.edx, sizeof(regs.edx));
}


int get_vendor(char *vendor)
{
	unsigned long	i;

	for (i=0; i<CPU_UNKNOWN; i++)
		if (!strcmp(vendor, cpu_vend[i].vendorID))
			return(i);

	return(CPU_UNKNOWN);
}

int print_cpu_model(void)
{
	char	cpu_name[48];

	switch(cpu_info.vendor)
	{
		case CPU_INTEL:	switch(cpu_info.family)
						{
							case 4:		switch(cpu_info.model)
										{
											case 0:		printf("i80486DX-25/33");
											return(0);
											case 1:		printf("i80486DX-33");
											return(0);
											case 2:		printf("i80486SX");
											return(0);
											case 3:		printf("i80486DX2");
											return(0);
											case 4:		printf("i80486SL");
											return(0);
											case 5:		printf("i80486SX2");
											return(0);
											case 7:		printf("i80486DX2WB");
											return(0);
											case 8:		printf("i80486DX4");
											return(0);
											case 9:		printf("i80486DX4WB");
											return(0);
											default:	printf("Unknown i486");
											return(1);
										}
										break;
							case 5:		switch(cpu_info.model)
										{
											case 0:		printf("Pentium P5 A-Step");
											return(0);
											case 1:		printf("Pentium P5");
											return(0);
											case 2:		printf("Pentium P54C");
											return(0);
											case 3:		printf("Pentium P24T OverDrive");
											return(0);
											case 4:		printf("Pentium P55C");
											return(0);
											case 7:		printf("Pentium P54C");
											return(0);
											case 8:		printf("Pentium P55C (0.25um)");
											return(0);
											default:	printf("Unknown Pentium");
											return(1);
										}
										break;
							case 6:		switch(cpu_info.model)
										{
											case 0:		printf("Pentium Pro A-step");
											return(0);
											case 1:		printf("Pentium Pro");
											return(0);
											case 3:		printf("Pentium II (0.28um)");
											return(0);
											case 5:		printf("Pentium II (0.25um)");
											return(0);
											case 6:		printf("Pentium II/Celeron");
											return(0);
											case 7:		printf("Pentium III");
											return(0);
											default:	printf("Unknown Pentium II");
											return(1);
										}
										break;
							default:	printf("Unknown Intel model");
										return(1);
						}
						break;
		case CPU_AMD:	if (cpu_info.cpuid_info.ext_func_level>0)
						{
							get_amd_name(cpu_name);
							printf("%s", cpu_name);
							return(0);
						}
						else		/* Most of the processors are handled above so this needs clean-up */
							switch(cpu_info.family)
							{
								case 4:		switch(cpu_info.model)
											{
												case 3:		printf("AMD 80486DX2");
												return(0);
												case 7:		printf("AMD 80486DX2WB");
												return(0);
												case 8:		printf("AMD 80486DX4");
												return(0);
												case 9:		printf("AMD 80486DX4WB");
												return(0);
												case 0xE:	printf("AMD 5x86");
												return(0);
												case 0xF:	printf("AMD 5x86WB");
												return(0);
												default:	printf("unknown AMD 486");
												return(1);
											}
											break;
								case 5:		switch(cpu_info.model)
											{
												case 0:		printf("K5");
												return(0);
												case 1:		printf("K5");
												return(0);
												case 2:		printf("K5");
												return(0);
												case 3:		printf("K5");
												return(0);
												case 6:		printf("K6");
												return(0);
												case 7:		printf("K6");
												return(0);
												case 8:		printf("K6-II");
												return(0);
												case 9:		printf("K6-III");
												return(0);
												default:	printf("AMD K?");
												return(1);
											}
											break;
								default:	printf("AMD Unknown model");
											return(1);
							}
							break;
		case CPU_CYRIX:	switch(cpu_info.family)
						{
							case 4:		switch(cpu_info.model)
										{
											case 9:		printf("Cyrix 5x86");
											return(0);
											case 4:		printf("MediaGX");
											return(0);
											default:	printf("Unknown model 4 Cyrix");
											return(1);
										}
										break;
							case 5:		switch(cpu_info.model)
										{
											case 2:		printf("Cyrix 6x86");
											return(0);
											case 4:		printf("6x86 MediaGXm");
											return(0);
											default:	printf("Unknown model 5 Cyrix");
											return(1);
										}
										break;
							case 6:		switch(cpu_info.model)
										{
											case 0:		printf("6x86MX");
											return(0);
											default:	printf("Unknown model 6 Cyrix");
											return(1);
										}
										break;
							default:	printf("Unknown Cyrix model");
										return(1);
						}
						break;
		case CPU_UNKNOWN:	switch(cpu_info.family)
							{
								case 3:		switch(cpu_info.model)
											{
												case 1:		printf("Unknown 386SX");
												return(1);
												default:	printf("Unknown 386");
												return(1);
											}
											break;
								case 4:		printf("Unknown 486");
											return(1);
								case 5:		printf("Unknown 586");
											return(1);
								case 6:		printf("Unknown 686");
											return(1);
								default:	printf("Unknown");
											return(1);
							}
							break;
		default:			printf("Ahm...");
							return(1);
	}
	return(1);
}

void print_cpu_info(struct cpu_info_x86 *cpu_inf)
{
	unsigned long	i;

	printf("CPU info:\n");
	
	printf("CPU:      ");
	if (print_cpu_model())
	{
		printf("\n");
		printf("Family:   %d Model: %d Stepping: %d\n", (int)cpu_inf->family, (int)cpu_inf->model, (int)cpu_inf->stepping);
	}
	else
		printf("\n");

	printf("Features:");

	if (cpu_info.feat_flags)
		for (i=0; i<32; i++)
		{
			if (cpu_info.feat_flags & (1 << i))
				printf(" %s", x86_feat[i]);
		}
	else
		printf("none");
	printf("\n");
}

int check_cpuid(void)
{
	int rc=0;

	__asm__("pushfl\n"
			"popl	%%eax\n"
			"movl	%%eax, %%ebx\n"
			"xorl	$0x200000, %%eax\n"
			"pushl	%%eax\n"
			"popfl\n"
			"pushfl\n"
			"popl	%%eax\n"
			"cmpl	%%ebx, %%eax\n"
			"jz	1f\n"
			"movl	$1, %%eax\n"
			"jmp	2f\n"
			"1:\n"
			"xorl	%%eax, %%eax\n"
			"2:\n"
			"pushl	%%ebx\n"
			"popfl\n"
			: "=a" (rc)
			:
			:"memory");
	
	return rc;
}

void cpuid(int op, struct cpuid_regs *regs)
{
	__asm__("cpuid"
			: "=a" (regs->eax), "=b" (regs->ebx), "=c" (regs->ecx), "=d" (regs->edx)
			: "a" (op)
			:"memory");
}

int is_486 (void)
{
	int     result;

	__asm__(
		    "pushf\n"	/* save EFLAGS */
		    "popl	%%eax\n"	/* get EFLAGS */
		    "movl	%%eax, %%ecx\n"		/* temp storage EFLAGS */
		    "xorl	$0x200000, %%eax\n"	/* change ID bit in EFLAGS */
		    "pushl	%%eax\n"	/* put new EFLAGS value on stack */
		    "popf\n"	/* replace current EFLAGS value */
		    "pushf\n"	/* get EFLAGS */
		    "popl	%%eax\n"	/* save new EFLAGS in EAX */
		    "cmp	%%ecx, %%eax\n"		/* compare temp and new EFLAGS */
		    "je	1f\n"
		    "movl	$1, %%eax\n"	/* 80486 present */
		    "jmp	2f\n"

		    "1:"
		    "movl	$0, %%eax\n"	/* 80486 not present */

		    "2:"
		    "pushl	%%ecx\n"	/* get original EFLAGS */
		    "popf\n"	/* restore EFLAGS */
      :	    "=a" (result)
      :
      :	    "memory");

	return result;
}

int is_386DX (void)			/* return TRUE for 386DX, and FALSE for 386SX */
{
	int     result;

	__asm__(
		    "movl	%%cr0,%%edx\n"	/* Get CR0 */
		    "pushl	%%edx\n"	/* save CR0 */
		    "andb	$0xef, %%dl\n"	/* clear bit4 */
		    "movl	%%edx, %%cr0\n"		/* set CR0 */
		    "movl	%%cr0, %%edx\n"		/* and read CR0 */
		    "andb	$0x10, %%dl\n"	/* bit4 forced high? */
		    "popl	%%edx\n"	/* restore reg w/ CR0 */
		    "movl	%%edx, %%cr0\n"		/* restore CR0 */
		    "movl	$1, %%eax\n"	/* TRUE, 386DX */
		    "jz	1f\n"
		    "movl	$0, %%eax\n"	/* FALSE, 386SX */

		    "1:"
      :	    "=a" (result)
      :
      :	    "memory");

	return result;
}

int has_fpu (void)
{
	int     result;

	__asm__(
		    "fninit\n"
		    "movl    $0x5a5a, %%eax\n"
		    "fnstsw  %%eax\n"
		    "cmpl    $0, %%eax\n"
		    "jne     1f\n"
		    "movl    $1, %%eax\n"
		    "jmp     2f\n"

		    "1:"
		    "movl    $0, %%eax\n"

		    "2:"
      :	    "=a" (result)
      :
      :	    "memory");

	return result;
}
