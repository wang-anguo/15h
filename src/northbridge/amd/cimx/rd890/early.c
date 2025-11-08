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

#include <stdint.h>
#include <string.h>
#include "NbPlatform.h"
#include "nb_cimx.h"
#include "cfg.h"

static u32 rd890_callout_entry(u32 func, uintptr_t data, void *config)
{
	u32 ret = 0;

	switch(func) {
		case CB_AmdSetNbPorConfig: // 0x9000
			printk(BIOS_DEBUG, "%s (early): CB_AmdSetNbPorConfig\n", __func__);
			break;
		case CB_AmdSetHtConfig: // 0x9001
			printk(BIOS_DEBUG, "%s (early): CB_AmdSetHtConfig\n", __func__);
			break;
		default:
			printk(BIOS_DEBUG, "%s (early): func = %08X\n", __func__, func);
			break;
	}
	return ret;
}

/**
 * @brief disable GPP1 Port0,1, GPP2, GPP3a Port0,1,2,3,4,5, GPP3b
 *
 * SR5650/5670/5690 RD890 chipset, read pci config space hang at POR,
 * Disable all Pcie Bridges to work around It.
 */
void sr56x0_rd890_disable_pcie_bridge(void)
{
	AMD_NB_CONFIG_BLOCK gConfig;
	AMD_NB_CONFIG_BLOCK *gConfigPtr = &gConfig;
	NB_CONFIG nb_cfg[MAX_NB_COUNT];
	HT_CONFIG ht_cfg[MAX_NB_COUNT];
	PCIE_CONFIG pcie_cfg[MAX_NB_COUNT];
	u32 mask;
	u32 val;
	u32 i;

	memset(&gConfig, 0, sizeof(AMD_NB_CONFIG_BLOCK));
	for (i = 0; i < MAX_NB_COUNT; i++) {
		memset(&nb_cfg[i], 0, sizeof(NB_CONFIG));
		memset(&ht_cfg[i], 0, sizeof(NB_CONFIG));
		memset(&pcie_cfg[i], 0, sizeof(NB_CONFIG));
		gConfig.Northbridges[i].pNbConfig = &nb_cfg[i];
		gConfig.Northbridges[i].pHtConfig = &ht_cfg[i];
		gConfig.Northbridges[i].pPcieConfig = &pcie_cfg[i];
		gConfig.Northbridges[i].ConfigPtr = &gConfigPtr;
	}

	val = (1 << 2) | (1 << 3); /*GPP1*/
	val |= (1 << 4) | (1 << 5) | (1 << 6) | (1 << 7) | (1 << 16) | (1 << 17); /*GPP3a*/
	val |= (1 << 18) | (1 << 19); /*GPP2*/
	val |= (1 << 20); /*GPP3b*/
	mask = ~val;
	for(i = 0; i < MAX_NB_COUNT; i++) {
		LibNbPciIndexRMW(MAKE_SBDFO(0, (i * 0x40), 0x0, 0x0, 0x0) | NB_MISC_INDEX, NB_MISC_REG0C,
				 AccessS3SaveWidth32, mask, val, &(gConfig.Northbridges[i]));
	}
}


/**
 * @brief North Bridge CIMx romstage entry,
 *  wrapper of AmdPowerOnResetInit entry point.
 */
void nb_Poweron_Init(void)
{
	AMD_NB_CONFIG_BLOCK gConfig;
	AMD_NB_CONFIG_BLOCK *gConfigPtr = &gConfig;
	NB_CONFIG nb_cfg[MAX_NB_COUNT];
	HT_CONFIG ht_cfg[MAX_NB_COUNT];
	PCIE_CONFIG pcie_cfg[MAX_NB_COUNT];
	AGESA_STATUS status;

	printk(BIOS_DEBUG, "cimx/rd890 early.c %s() Start\n", __func__);

	CIMX_INIT_TRACE();
	CIMX_TRACE((BIOS_DEBUG, "NbPowerOnResetInit entry\n"));

	rd890_cimx_config(&gConfigPtr, &nb_cfg[0], &ht_cfg[0], &pcie_cfg[0]);
	gConfig.StandardHeader.CalloutPtr = &rd890_callout_entry;

	status = AmdPowerOnResetInit(gConfigPtr);

	printk(BIOS_DEBUG, "cimx/rd890 early.c %s() End. return status=%x\n", __func__, status);
}

/**
 * @brief North Bridge CIMx romstage entry,
 *  wrapper of AmdHtInit entry point.
 */
void nb_Ht_Init(void)
{
	AMD_NB_CONFIG_BLOCK gConfig;
	AMD_NB_CONFIG_BLOCK *gConfigPtr = &gConfig;
	NB_CONFIG nb_cfg[MAX_NB_COUNT];
	HT_CONFIG ht_cfg[MAX_NB_COUNT];
	PCIE_CONFIG pcie_cfg[MAX_NB_COUNT];
	AGESA_STATUS status;

	rd890_cimx_config(&gConfigPtr, &nb_cfg[0], &ht_cfg[0], &pcie_cfg[0]);
	gConfig.StandardHeader.CalloutPtr = &rd890_callout_entry;

	//Initialize HT structure
	LibSystemApiCall(AmdHtInitializer, gConfigPtr);

	status = LibSystemApiCall(AmdHtInit, gConfigPtr);
	printk(BIOS_DEBUG, "AmdHtInit status: %x\n", status);
}

void nb_S3_Init(void)
{
	//TODO
}
