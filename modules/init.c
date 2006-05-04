/* 
 *   Creation Date: <2003/12/23 00:28:05 samuel>
 *   Time-stamp: <2003/12/28 19:43:41 samuel>
 *   
 *	<init.c>
 *	
 *	Module intialization
 *   
 *   Copyright (C) 2003 Samuel Rydh (samuel@ibrium.se)
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   version 2
 *   
 */

#include "openbios/config.h"
#include "openbios/kernel.h"
#include "modules.h"

void
modules_init( void )
{
#ifdef CONFIG_CMDLINE
	cmdline_init();
#endif
#ifdef CONFIG_DEBLOCKER
	deblocker_init();
#endif	
#ifdef CONFIG_DISK_LABEL
	disklabel_init();
#endif
#ifdef CONFIG_FS
	filesystem_init();
#endif
#ifdef CONFIG_MAC_PARTS
	macparts_init();
#endif
#ifdef CONFIG_PC_PARTS
	pcparts_init();
#endif
}