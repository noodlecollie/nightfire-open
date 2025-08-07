/*  SPDX-License-Identifier: MIT */
/*
 *  Copyright (c) 2012 by Jonathan Toppins <jtoppins@users.sourceforge.net>
 *  Copyright (c) 2012, 2013 by Cisco Systems, Inc
 */

#ifndef __TEST_PRIVATE_H__
#define __TEST_PRIVATE_H__

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/string.h>
#define printf(...) printk(KERN_INFO __VA_ARGS__)

#else

#include <stdio.h>
#include <string.h>

#endif

#ifdef DEBUG
#  ifdef __KERNEL__
#    define debug_printf(...)  printk(KERN_DEBUG __VA_ARGS__)
#  else
#    define debug_printf printf
#  endif
#else
#define debug_printf(...)
#endif

#endif /* __TEST_PRIVATE_H__ */
