/*		string.h

		String function C Header for libc

	History:
		Version 0.01.03	25 Nov 2001		03:01		ObliVion
		Added memcmp.

		Version 0.01.02	28 Apr 2001		20:37		ObliVion
		Added strcpy.

		Version 0.01.01	02 Jul 2000		17:00		ObliVion
		Initial version.
*/

#ifndef __STRING_H
#define __STRING_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern	size_t strlen(char *p);
extern	int strcmp(char *s1, char *s2);
extern	char *strcpy(char *dest, const char *src);
extern	void *memset(void *dest, int c, size_t count);
extern	void *memcpy(void *dest, void *src, size_t count);
extern	int memcmp(const void* p1, const void* p2, size_t n);
extern	void *malloc(size_t size);
extern	void free(void *addr);

#ifdef __cplusplus
};
#endif

#endif
