/* 
 *   Creation Date: <2003/12/20 01:51:22 samuel>
 *   Time-stamp: <2004/01/07 19:02:17 samuel>
 *   
 *	<vsprintf.h>
 *	
 *	
 *   
 *   Copyright (C) 2003, 2004 Samuel Rydh (samuel@ibrium.se)
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   version 2
 *   
 */

#ifndef _H_VSPRINTF
#define _H_VSPRINTF

#include <stdarg.h>
extern int	vsprintf(char *buf, const char *fmt, va_list args );
extern int	sprintf(char * buf, const char *fmt, ...);

#endif   /* _H_VSPRINTF */