/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef _AMD_SB_CIMx_PLATFORM_H_
#define _AMD_SB_CIMx_PLATFORM_H_

#pragma pack(push,1)

#include "cbtypes.h"
#include <console/console.h>
#include <commonlib/loglevel.h>
#ifdef NULL
#undef NULL
#endif
#define NULL            0

typedef struct _EXT_PCI_ADDR{
	UINT32                  Reg :16;
	UINT32                  Func:3;
	UINT32                  Dev :5;
	UINT32                  Bus :8;
}EXT_PCI_ADDR;


typedef union _PCI_ADDR{
	UINT32                  ADDR;
	EXT_PCI_ADDR            Addr;
}PCI_ADDR;

#ifdef TRACE
#undef TRACE
#endif

#if CONFIG(ENABLE_SB_CIMX_DEBUGGER)
	#define TRACE(Arguments) printk Arguments
	#if CONFIG(CONSOLE_USB)
		#define DISABLE_USB_RESET
	#endif
#else
	#define TRACE(Arguments) do {} while (0)
#endif

#define FIXUP_PTR(ptr)  ptr

#pragma pack(pop)

#include "platform_cfg.h"       /* mainboard specific configuration */
#include "OEM.h"
#include "Amd.h"
#include "ACPILIB.h"
#include "SBTYPE.h"
#include "sbAMDLIB.h"
#include "SBCMNLIB.h"
#include "SB700.h"
#include "SBDEF.h"

#define DMSG_SB_TRACE   0x02

#ifndef SB700_DUMP_GPIO
#define SB700_DUMP_GPIO 0
#endif

#endif /* _AMD_SB_CIMx_PLATFORM_H_ */
