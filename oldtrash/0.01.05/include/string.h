/*		string.h

		String function C Header for libc

	History:

		Version 0.01	02 July 2000	17:00		ObliVion
		Initial version.
*/



#ifndef __STRING_H
#define __STRING_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int strlen(const char *p);
extern int strcmp(const char* s1, const char* s2);
extern void *memset(void *dest, int c, size_t count);
extern void *memcpy(void *dest, void *src, size_t count);

#ifdef __cplusplus
}
#endif

#endif
