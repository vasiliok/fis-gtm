/****************************************************************
 *								*
 *	Copyright 2001, 2012 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

/* If this is not the vax, define string.h. This is because the Vax
   has its own built-in instructions for string manipulation.
*/
#ifndef GTM_STRINGH
#define GTM_STRINGH

#include <string.h>

#define STRERROR	strerror

#define	STRCPY(SOURCE, DEST)		strcpy((char *)(SOURCE), (char *)(DEST))
#define STRNCPY_LIT(SOURCE, LITERAL)	strncpy((char *)(SOURCE), (char *)(LITERAL), SIZEOF(LITERAL) - 1)	/* BYPASSOK */
#define	STRNCPY_STR(SOURCE, STRING, LEN) strncpy((char *)(SOURCE), (char *)(STRING), LEN)

#define	STRCMP(SOURCE, DEST)		strcmp((char *)(SOURCE), (char *)(DEST))
#define	STRNCMP_LIT(SOURCE, LITERAL)	strncmp(SOURCE, LITERAL, SIZEOF(LITERAL) - 1)		/* BYPASSOK */
/* Make sure that SIZEOF(SOURCE) > 0 or SOURCE != NULL before running. */
#define	STRNCMP_LIT_FULL(SOURCE, LITERAL)	strncmp(SOURCE, LITERAL, SIZEOF(LITERAL))	/* BYPASSOK */
#define	STRNCMP_STR(SOURCE, STRING, LEN) strncmp(SOURCE, STRING, LEN)

/* We need to catch any memcpy() that is used when the source and target strings overlap in any fashion so we can change
 * them to a memmove. So in debug builds, assert fail if this is the case.
 */
#if defined(DEBUG) && !defined(BYPASS_MEMCPY_OVERRIDE)
#  include "gtm_memcpy_validate_and_execute.h"
#  ifdef memcpy
#    undef memcpy	/* Some platforms like AIX create memcpy as a #define which needs removing before re-define */
#  endif
#  define memcpy(TARGET, SRC, LEN) gtm_memcpy_validate_and_execute((void *)(TARGET), (const void *)(SRC), (LEN))
#endif

#endif
