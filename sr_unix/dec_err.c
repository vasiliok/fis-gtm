/****************************************************************
 *								*
 *	Copyright 2001, 2009 Fidelity Information Services, Inc	*
 *								*
 *	This source code contains the intellectual property	*
 *	of its copyright holder(s), and is made available	*
 *	under a license.  If you do not know the terms of	*
 *	the license, please stop and do not read further.	*
 *								*
 ****************************************************************/

#include "mdef.h"
#include <stdarg.h>
#include "fao_parm.h"
#include "error.h"
#include "msg.h"
#include "util.h"
#include "util_out_print_vaparm.h"
#include "gtmmsg.h"

GBLREF bool dec_nofac;
GBLREF va_list last_va_list_ptr;

void dec_err(uint4 argcnt, ...)
{
	va_list		var;
	uint4		i, j, count, err;
	const err_ctl 	*ec;
	const err_msg	*em;
	char		msgbuff[2048];
	mstr		msgstr;

	util_out_print(0, RESET, 0);	/* reset the buffer */
	VAR_START(var, argcnt);
	assert (argcnt >= 1);
	err = va_arg(var, uint4);
	ec = err_check(err);
	em = NULL;
	if (ec)
	{
		assert((err & FACMASK(ec->facnum)) && (MSGMASK(err, ec->facnum) <= ec->msg_cnt));
		j = MSGMASK(err, ec->facnum);
		em = ec->fst_msg + j - 1;
	}
	msgstr.addr = msgbuff;
	msgstr.len = SIZEOF(msgbuff);
	gtm_getmsg(err, &msgstr);

	if (!em)
		util_out_print(msgstr.addr, FLUSH, 1, err);
	else
	{
		argcnt--;
		if (argcnt)
		{
			count = va_arg(var, int4);
			assert (count <= argcnt);
		} else
			count = 0;
		util_out_print_vaparm(msgstr.addr, FLUSH, var, count);
		va_end(last_va_list_ptr);
	}
	va_end(var);
}
