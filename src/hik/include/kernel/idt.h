/*		idt.h

		The IDT stuff

	History:

		Version 0.01	02 July 2000	14:15		ObliVion
		Initial version.
*/
#ifndef _HIK_IDT_H_
#define _HIK_IDT_H_


__inline__ void lidt(struct dt_ptr *pdt);

extern	void init_idt(void);

extern	void _intr0(void);
extern	void _intr1(void);
extern	void _intr2(void);
extern	void _intr3(void);
extern	void _intr4(void);
extern	void _intr5(void);
extern	void _intr6(void);
extern	void _intr7(void);
extern	void _intr8(void);
extern	void _intr9(void);

extern	void _intr10(void);
extern	void _intr11(void);
extern	void _intr12(void);
extern	void _intr13(void);
extern	void _intr14(void);
extern	void _intr15(void);
extern	void _intr16(void);
extern	void _intr17(void);
extern	void _intr18(void);
extern	void _intr19(void);

extern	void _intr20(void);
extern	void _intr21(void);
extern	void _intr22(void);
extern	void _intr23(void);
extern	void _intr24(void);
extern	void _intr25(void);
extern	void _intr26(void);
extern	void _intr27(void);
extern	void _intr28(void);
extern	void _intr29(void);

extern	void _intr30(void);
extern	void _intr31(void);
extern	void _intr32(void);
extern	void _intr33(void);
extern	void _intr34(void);
extern	void _intr35(void);
extern	void _intr36(void);
extern	void _intr37(void);
extern	void _intr38(void);
extern	void _intr39(void);

extern	void _intr40(void);
extern	void _intr41(void);
extern	void _intr42(void);
extern	void _intr43(void);
extern	void _intr44(void);
extern	void _intr45(void);
extern	void _intr46(void);
extern	void _intr47(void);
extern	void _intr48(void);
extern	void _intr49(void);

extern	void _intr50(void);
extern	void _intr51(void);
extern	void _intr52(void);
extern	void _intr53(void);
extern	void _intr54(void);
extern	void _intr55(void);
extern	void _intr56(void);
extern	void _intr57(void);
extern	void _intr58(void);
extern	void _intr59(void);

extern	void _intr60(void);
extern	void _intr61(void);
extern	void _intr62(void);
extern	void _intr63(void);

#endif /* _KERNEL_IDT_H_ */

