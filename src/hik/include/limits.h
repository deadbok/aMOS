/*		limits.h

		Limits for libc

	History:
		Version 0.01.02	28 April 2001	20:37		ObliVion
		Initial version.
*/

#ifndef _LIMITS_H_
#define _LIMITS_H_

/* These assume 8-bit char, 16-bit short int, and 32-bit int and long int.*/

/* Number of bits in a char */
#define CHAR_BIT	8

/* Minimum and maximum values of a signed char */
#define SCHAR_MIN	(-128)
#define SCHAR_MAX	127

/* Maximum value of an unsigned char. */
#define UCHAR_MAX	255

/* Minimum and maximum values of a char. */
#define CHAR_MIN	SCHAR_MIN
#define CHAR_MAX	SCHAR_MAX

/* Minimum and maximum values of a signed short int. */
#define SHRT_MIN	(-32768)
#define SHRT_MAX	32767

/* Maximum value of an unsigned short int. */
#define USHRT_MAX	65535

/* Minimum and maximum values of a signed int. */
#define INT_MIN	(-INT_MAX - 1)
#define INT_MAX	2147483647

/* Maximum value of an unsigned int. */
#define UINT_MAX	4294967295

/* Minimum and maximum values of a signed long int. */
#define LONG_MAX	2147483647L
#define LONG_MIN	(-LONG_MAX - 1L)

/* Maximum value of an unsigned long int. */
#define ULONG_MAX	4294967295L

#endif
