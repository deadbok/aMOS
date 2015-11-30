/*		test.c

		MD5 (Message-digest algorithm) inplementation test

	History:

		Version 0.01.00		11 Jan 2002		17:13	ObliVion
		Initial version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

unsigned char	one[16]		= {0xd4, 0x1d, 0x8c, 0xd9, 0x8f, 0x00, 0xb2, 0x04, 0xe9, 0x80, 0x09, 0x98, 0xec, 0xf8, 0x42, 0x7e};
unsigned char	two[16]		= {0x0c, 0xc1, 0x75, 0xb9, 0xc0, 0xf1, 0xb6, 0xa8, 0x31, 0xc3, 0x99, 0xe2, 0x69, 0x77, 0x26, 0x61};
unsigned char	three[16]	= {0x90, 0x01, 0x50, 0x98, 0x3c, 0xd2, 0x4f, 0xb0, 0xd6, 0x96, 0x3f, 0x7d, 0x28, 0xe1, 0x7f, 0x72};
unsigned char	four[16]	= {0xf9, 0x6b, 0x69, 0x7d, 0x7c, 0xb7, 0x93, 0x8d, 0x52, 0x5a, 0x2f, 0x31, 0xaa, 0xf1, 0x61, 0xd0};
unsigned char	five[16]	= {0xc3, 0xfc, 0xd3, 0xd7, 0x61, 0x92, 0xe4, 0x00, 0x7d, 0xfb, 0x49, 0x6c, 0xca, 0x67, 0xe1, 0x3b};
unsigned char	six[16]		= {0xd1, 0x74, 0xab, 0x98, 0xd2, 0x77, 0xd9, 0xf5, 0xa5, 0x61, 0x1c, 0x2c, 0x9f, 0x41, 0x9d, 0x9f};
unsigned char	seven[16]	= {0x57, 0xed, 0xf4, 0xa2, 0x2b, 0xe3, 0xc9, 0x55, 0xac, 0x49, 0xda, 0x2e, 0x21, 0x07, 0xb6, 0x7a};

int	main(void)
{
	unsigned char		*digest;

	digest = (unsigned char *)malloc(16);

	printf("\n");

	md5(digest, "", 0);
	md5_print(one);
	printf(" : ");
	md5_print(digest);
 	if (!memcmp(digest, one, 15))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "a", 1);
	md5_print(two);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, two, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "abc", 3);
	md5_print(three);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, three, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "message digest", 14);
	md5_print(four);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, four, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "abcdefghijklmnopqrstuvwxyz", 26);
	md5_print(five);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, five, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789", 62);
	md5_print(six);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, six, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	md5(digest, "12345678901234567890123456789012345678901234567890123456789012345678901234567890", 80);
	md5_print(seven);
	printf(" : ");
	md5_print(digest);
	if (!memcmp(digest, seven, 16))
		printf(" OK\n");
	else
		printf(" Error\n");

	free(digest);

	return(0);
}

