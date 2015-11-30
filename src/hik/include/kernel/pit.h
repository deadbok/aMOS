/*		pic.h

		Routines for the 8253/54 PIT

	History:

		Version 0.01	18 May 2000	15:13		ObliVion
		Initial version.
*/

#ifndef _HIK_PIT_H
#define _HIK_PIT_H

#define PIT_CNT_0	0x40	/* Counter 0 					*/
#define PIT_CNT_1	0x41	/* Counter 1 					*/
#define PIT_CNT_2	0x42	/* Counter 2 					*/
#define PIT_MODE	0x43	/* Mode port (CW)				*/


#define SEL_CNT_0	0x00	/* Select counter 0 			*/
#define SEL_CNT_1	0x40	/* Select counter 1 			*/
#define SEL_CNT_2	0x80	/* Select counter 2 			*/

#define CNT_LMSB	0x30	/* Read LSB then MSB of counter	*/
#define CNT_MODE_3	0x06	/* Set counter to mode 3		*/

int init_pit(void);

#endif
