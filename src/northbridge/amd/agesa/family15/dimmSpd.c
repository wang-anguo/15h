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

#include <device/pci_def.h>
#include <device/device.h>
#include <stdlib.h>
#include "OEM.h"		/* SMBUS0_BASE_ADDRESS */

/* warning: Porting.h includes an open #pragma pack(1) */
#include "Porting.h"
#include "AGESA.h"
#include "chip.h"

#include <northbridge/amd/agesa/dimmSpd.h>

/**
 * Gets the SMBus address for an SPD from the array in devicetree.cb
 * then read the SPD into the supplied buffer.
 */
AGESA_STATUS AmdMemoryReadSPD (UINT32 unused1, UINTN unused2, AGESA_READ_SPD_PARAMS *info)
{
	UINT8 spdAddress;
	int err;
	int i;

	DEVTREE_CONST struct device *dev = dev_find_slot(0, PCI_DEVFN(0x18, 2));
	if (dev == NULL)
		return AGESA_ERROR;

	DEVTREE_CONST struct northbridge_amd_agesa_family15_config *config = dev->chip_info;
	if (config == NULL)
		return AGESA_ERROR;

	if (info->SocketId >= ARRAY_SIZE(config->spdAddrLookup))
		return AGESA_ERROR;
	if (info->MemChannelId >= ARRAY_SIZE(config->spdAddrLookup[0]))
		return AGESA_ERROR;
	if (info->DimmId >= ARRAY_SIZE(config->spdAddrLookup[0][0]))
		return AGESA_ERROR;

	spdAddress = config->spdAddrLookup
		[info->SocketId][info->MemChannelId][info->DimmId];

	if (spdAddress == 0)
		return AGESA_ERROR;

	err = smbus_readSpd(spdAddress, (void *) info->Buffer, 256);
	if (err)
		return AGESA_ERROR;

	printk(BIOS_DEBUG, "AmdMemoryReadSPD, Socket %d, Channel %d, Dimm %d, SpdAddr %02Xh\n", info->SocketId, info->MemChannelId, info->DimmId, spdAddress);
	printk(BIOS_DEBUG, "SPD Hex dump:\n");
	for(i = 0; i < 256; i++) {
		printk(BIOS_DEBUG, " %02X", (uint8_t)(info->Buffer[i]));
		if((i+1) % 64 == 0) printk(BIOS_DEBUG, "\n");
	}
	printk(BIOS_DEBUG, "\n\n");
	return AGESA_SUCCESS;
}
