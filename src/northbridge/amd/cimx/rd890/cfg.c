/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 - 2012 Advanced Micro Devices, Inc.
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

#include "NbPlatform.h"
#include "cfg.h"
#include "nbInitializer.h"

#include <string.h>
#include <console/console.h>
#include <northbridge/amd/cimx/rd890/chip.h>


/**
 * @brief North Bridge CIMx configuration
 *
 * should be called before exeucte CIMx function.
 * this function will be called in romstage and ramstage.
 */
void rd890_cimx_config(AMD_NB_CONFIG_BLOCK **pConfigPtr, NB_CONFIG *nbConfig, HT_CONFIG *htConfig, PCIE_CONFIG *pcieConfig)
{
	AMD_NB_CONFIG_BLOCK *pConfig;
	PCI_ADDR PciAddress;
	u32 val, sbNode, sbLink;
	u16 i = 0;

	if(!pConfigPtr) return;
	pConfig = *pConfigPtr;
	if(!pConfig) return;

	memset(pConfig, 0, sizeof(AMD_NB_CONFIG_BLOCK));
	for (i = 0; i < MAX_NB_COUNT; i++) {
		memset(&nbConfig[i], 0, sizeof(NB_CONFIG));
		memset(&htConfig[i], 0, sizeof(HT_CONFIG));
		memset(&pcieConfig[i], 0, sizeof(PCIE_CONFIG));
		pConfig->Northbridges[i].pNbConfig	= &nbConfig[i];
		pConfig->Northbridges[i].pHtConfig	= &htConfig[i];
		pConfig->Northbridges[i].pPcieConfig	= &pcieConfig[i];
		pConfig->Northbridges[i].ConfigPtr	= pConfigPtr;
	}

	/* Initialize all NB structures */
	AmdInitializer(pConfig);

	pConfig->NumberOfNorthbridges = MAX_NB_COUNT - 1;
	pConfig->StandardHeader.PcieBasePtr = (VOID *)PCIEX_BASE_ADDRESS;

	for(i = 0; i < MAX_NB_COUNT; i++) {
		/* Configure HT Links */
		pConfig->Northbridges[i].NbPciAddress.AddressValue = MAKE_SBDFO(0, (i * 0x40), 0x0, 0x0, 0x0);
		PciAddress.AddressValue = MAKE_SBDFO(0, 0, CONFIG_CDB + (i * 2), FUNC_0, 0x60);
		LibNbPciRead(PciAddress.AddressValue, AccessWidth32, &val, &(pConfig->Northbridges[i]));
		sbNode = (val >> 8) & 0x07;
		PciAddress.AddressValue = MAKE_SBDFO(0, 0, CONFIG_CDB + (i * 2), FUNC_0, 0x64);
		LibNbPciRead(PciAddress.AddressValue, AccessWidth32, &val, &(pConfig->Northbridges[i]));
		sbLink = (val >> 8) & 0x07; //assum ganged
		pConfig->Northbridges[i].NbHtPath.NodeID = sbNode;
		pConfig->Northbridges[i].NbHtPath.LinkID = sbLink;
		/* Configure HT Deemphasis */
		pConfig->Northbridges[i].pHtConfig->NbTransmitterDeemphasis = RD890_TRANSMITTER_DEEMPHASIS;
	}

#ifndef __PRE_RAM__
	/* If temporrary MMIO enable set up CPU MMIO */
	for (i = 0; i <= pConfig->NumberOfNorthbridges; i++) {
		UINT32  MmioBase;
		UINT32  LinkId;
		UINT32  SubLinkId;

		MmioBase = pConfig->Northbridges[i].pPcieConfig->TempMmioBaseAddress;
		if (MmioBase != 0) {
			LinkId = pConfig->Northbridges[i].NbHtPath.LinkID & 0xf;
			SubLinkId = ((pConfig->Northbridges[i].NbHtPath.LinkID & 0xF0) == 0x20) ? 1 : 0;
			/* Set Limit */
			LibNbPciRMW(MAKE_SBDFO (0, 0, 0x18 + i*2, 0x1, (i * 4) + 0x84),
					AccessWidth32,
					0x0,
					((MmioBase << 12) + 0xF00) | (LinkId << 4) | (SubLinkId << 6),
					&(pConfig->Northbridges[i]));
			/* Set Base */
			LibNbPciRMW(MAKE_SBDFO (0, 0, 0x18 + i*2, 0x1, (i * 4) + 0x80),
					AccessWidth32,
					0x0,
					(MmioBase << 12) | 0x3,
					&(pConfig->Northbridges[i]));
		}
	}
#endif
}
