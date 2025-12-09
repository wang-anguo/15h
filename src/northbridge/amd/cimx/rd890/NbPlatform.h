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

#ifndef _NB_PLATFORM_H_
#define _NB_PLATFORM_H_

#if CONFIG(ENABLE_NB_CIMX_DEBUGGER)
	#define CIMX_TRACE_SUPPORT
	#define CIMX_ASSERT_SUPPORT
#endif

#define CIMX_INIT_TRACE(Arguments)

#ifdef  CIMX_TRACE_SUPPORT
	#define TRACE_DATA(Ptr, Level) BIOS_DEBUG //always enable
	#define CIMX_TRACE(Argument) do {do_printk Argument;} while (0)
#else
	#define CIMX_TRACE(Argument)
#endif

#ifdef CIMX_ASSERT_SUPPORT
	#define CIMX_ASSERT(x)  if (!(x)) {\
		printk(BIOS_ERR, "ASSERT !!! %s:%d\n", __FILE__, __LINE__); \
	}
#else
	#define CIMX_ASSERT(x)
#endif


/*----------------------------------------------------------------------------------------
 *                          E X P O R T E D    F U N C T I O N S
 *----------------------------------------------------------------------------------------
 */

#define STALL(Ptr, TimeUs, Flag) LibAmdSbStall(TimeUs, Ptr)

#define REPORT_EVENT(Class, Info, Param1, Param2, Param3, Param4, CfgPtr)

// CIMX configuration parameters
/*
 * PCIEX_BASE_ADDRESS  - Define PCIE base address
 */
#ifdef  MOVE_PCIEBAR_TO_F0000000
#define PCIEX_BASE_ADDRESS           0xF7000000
#else
#define PCIEX_BASE_ADDRESS           CONFIG_MMCONF_BASE_ADDRESS
#endif



#define CIMX_S3_SAVE 1
#include "cbtypes.h"
#include <console/console.h>

#include "amd.h" //cimx typedef
#include <amdlib.h>
#include "amdAcpiLib.h"
#include "amdAcpiMadt.h"
#include "amdAcpiIvrs.h"
#include "amdSbLib.h"
#include "nbPcie.h"

//must put before the nbType.h
#include "platform_cfg.h" /*platform dependented configuration */
#include "nbType.h"

#include "nbLib.h"
#include "nbDef.h"
#include "nbInit.h"
#include "nbHtInit.h"
#include "nbIommu.h"
#include "nbEventLog.h"
#include "nbRegisters.h"
#include "nbPcieAspm.h"
#include "nbPcieLinkWidth.h"
#include "nbPcieHotplug.h"
#include "nbPciePortRemap.h"
#include "nbPcieWorkarounds.h"
#include "nbPcieCplBuffers.h"
#include "nbPciePllControl.h"
#include "nbMiscInit.h"
#include "nbIoApic.h"
#include "nbPcieSb.h"
#include "nbRecovery.h"
#include "nbMaskedMemoryInit.h"


#define FIX_PTR_ADDR(x, y) x

#define TRACE_ALWAYS  0xffffffff

#define AmdNbDispatcher NULL

#define CIMX_TRACE_ALL    0xFFFFFFFF
#define CIMX_NBPOR_TRACE  0xFFFFFFFF
#define CIMX_NBHT_TRACE   0xFFFFFFFF
#define CIMX_NBPCIE_TRACE 0xFFFFFFFF
#define CIMX_NB_TRACE     0xFFFFFFFF
#define CIMX_NBPCIE_MISC  0xFFFFFFFF

#ifndef RD890_TEST_THERMAL_SHUTDOWN
#define RD890_TEST_THERMAL_SHUTDOWN 0
#endif

#endif
