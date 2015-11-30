/*		module.h

		The header for the dynamic module loader

	History:

		Version 0.01.00	04 Okt 2000	09:05		ObliVion
		Initial version.
*/
#include <stddef.h>
#include <hik/mailbox.h>

#ifndef _MODULE_H_
#define _MODULE_H_

typedef void (*mod_init_func_t)(void);

struct mod_info
{
	mod_init_funt_t		mod_init;
	struct mailbox		mod_mailbox;
};

#endif
