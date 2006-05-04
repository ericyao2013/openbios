/* 
 *   Creation Date: <1999/11/16 00:49:26 samuel>
 *   Time-stamp: <2004/04/12 16:26:50 samuel>
 *   
 *	<init.c>
 *	
 *	Initialization
 *   
 *   Copyright (C) 1999, 2000, 2001, 2002, 2003, 2004 Samuel & David Rydh 
 #      (samuel@ibrium.se, dary@lindesign.se)
 *   
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation
 *   
 */

#include "openbios/config.h"
#include "openbios/bindings.h"
#include "openbios/nvram.h"
#include "mol/mol.h"
#include "ofmem.h"
#include "mol/prom.h"
#include "openbios-version.h"
#include "osi_calls.h"
#include "boothelper_sh.h"

extern void unexpected_excep( int vector );

int
get_bool_res( const char *res )
{
	char buf[8], *p;

	p = BootHGetStrRes( res, buf, sizeof(buf) );
	if( !p )
		return -1;
	if( !strcasecmp(p,"true") || !strcasecmp(p,"yes") || !strcasecmp(p,"1") )
		return 1;
	return 0;
}

void
unexpected_excep( int vector )
{
	printk("MOL panic: Unexpected exception %x\n", vector );
	for( ;; )
		;
}

void
entry( void )
{
	printk("\n");
	printk("=============================================================\n");
	printk("OpenBIOS %s [%s]\n", OPENBIOS_RELEASE, OPENBIOS_BUILD_DATE );
	
	ofmem_init();
	initialize_forth();
	/* won't return */

	printk("of_startup returned!\n");
	for( ;; )
		;
}

static void
setenv( char *env, char *value )
{
	push_str( value );
	push_str( env );
	fword("$setenv");
}

void
arch_of_init( void )
{
	mol_phandle_t ph;
	int autoboot;
	
	devtree_init();
	node_methods_init();
	nvram_init();
	modules_init();
	pseudodisk_init();
	osiblk_init();
	osiscsi_init();
	init_video();

	if( (ph=prom_find_device("/rtas")) == -1 )
		printk("Warning: No /rtas node\n");
	else {
		ulong size = 0x1000;
		while( size < (ulong)of_rtas_end - (ulong)of_rtas_start )
			size *= 2;
		prom_set_prop( ph, "rtas-size", (char*)&size, sizeof(size) );
	}

	/* tweak boot settings */
	autoboot = !!get_bool_res("autoboot");
	if( !autoboot )
		printk("Autobooting disabled - dropping into OpenFirmware\n");
	setenv("auto-boot?", autoboot ? "true" : "false" );
	setenv("boot-command", "molboot");

	if( get_bool_res("tty-interface") == 1 )
		fword("activate-tty-interface");

	/* hack */
	device_end();
	bind_func("molboot", boot );
}