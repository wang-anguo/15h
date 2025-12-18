/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
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

#include "AGESA.h"
#include <vendorcode/amd/cimx/sb700/OEM.h>
#include <northbridge/amd/agesa/BiosCallOuts.h>
#include <device/pci_ops.h>
#include <spd_bin.h>
#include <southbridge/amd/cimx/sb700/gpio.h>

#include "gpio.h"

#ifdef __PRE_RAM__

int do_smbus_read_byte(u32 smbus_io_base, u32 device, u32 address);

static AGESA_STATUS read_spd_buffer(UINT32 unused1, UINTN unused2, AGESA_READ_SPD_PARAMS *info) {
	UINT8 spdAddress;
	UINT16 i;
	int data;

	/* Socket range: 0-1 (2)
	 * Channel range: 0-3 (4)
	 * Dimm range: 0-1 (2)
	*/
	printk(BIOS_INFO, "SPD: Socket %d, Channel %d, Dimm %d\n", info->SocketId, info->MemChannelId, info->DimmId);

	if(info->SocketId > 1) return AGESA_ERROR;
	if(info->MemChannelId > 3) return AGESA_ERROR;
	if(info->DimmId > 1) return AGESA_ERROR;

	switch(((info->MemChannelId & 0xFF) << 8) | (info->DimmId & 0xFF)) {
		case 0x0000: spdAddress = 0x50; break;
		case 0x0001: spdAddress = 0x51; break;
		case 0x0100: spdAddress = 0x52; break;
		case 0x0101: spdAddress = 0x53; break;
		case 0x0200: spdAddress = 0x54; break;
		case 0x0201: spdAddress = 0x55; break;
		case 0x0300: spdAddress = 0x56; break;
		case 0x0301: spdAddress = 0x57; break;
		default: spdAddress = 0x00;
	}
	printk(BIOS_DEBUG, "SPD: Address 0x%02X\n", spdAddress);
	if(!spdAddress) return AGESA_ERROR;

	for(i = 0; i <= 0xFF; i++) {
		data = do_smbus_read_byte(SMBUS1_BASE_ADDRESS, spdAddress, i);
		if(data < 0) return AGESA_ERROR;
		info->Buffer[i] = data;
	}
	print_spd_info(info->Buffer);
	printk(BIOS_INFO, "\n");

	return AGESA_SUCCESS;
}

/* Voltages are specified using DDR3Voltage values from AGESA */
static AGESA_STATUS set_ddr3_voltage(uint8_t socket_id, uint8_t voltage)
{
	uint8_t byte;
	uint8_t value;

	printk(BIOS_DEBUG, "set_ddr3_voltage: socket %d, voltage %d\n", socket_id, voltage);
	/* Set value for GPIO based on detected voltage:
	 * 0x0: 1.5V
	 * 0x1: 1.35V
	 * 0x4: 1.25V
	 *
	 * 0x5: 1.15V is unused/unsupported
	 */
	switch (voltage) {
		case VOLT1_5:
			value = 0x0;
			break;
		case VOLT1_35:
			value = 0x1;
			break;
		case VOLT1_25:
			value = 0x4;
			break;
		default:
			/* Default to 1.5V to be consistent with the documented
			   board behavior in the manual and as labelled on
			   the voltage selection jumpers on the board */
			printk(BIOS_WARNING, "set_ddr3_voltage: invalid voltage requested (voltage = 0x%02X), defaulting to 1.5V\n", voltage);
			value = 0x0;
	}

	if (socket_id == 1)
		value <<= 1;

	/* Set GPIOs */
	byte = pci_read_config8(PCI_DEV(0, 0x14, 3), 0xd1);
	if (socket_id == 0)
		byte &= ~0x5;
	if (socket_id == 1)
		byte &= ~0xa;
	byte |= value;
	pci_write_config8(PCI_DEV(0, 0x14, 3), 0xd1, byte);

	/* Enable GPIO output drivers */
	byte = pci_read_config8(PCI_DEV(0, 0x14, 3), 0xd0);
	byte &= 0x0f;
	pci_write_config8(PCI_DEV(0, 0x14, 3), 0xd0, byte);

	return AGESA_SUCCESS;
}
#endif

static AGESA_STATUS board_ReadSpd (UINT32 Func, UINTN Data, VOID *ConfigPtr);
static AGESA_STATUS board_BeforeDramInit(UINT32 Func, UINTN Data, VOID *ConfigPtr);

#include <stdlib.h>
const BIOS_CALLOUT_STRUCT BiosCallouts[] =
{
	{AGESA_DO_RESET,			agesa_Reset },
	{AGESA_READ_SPD,			board_ReadSpd },
	{AGESA_READ_SPD_RECOVERY,		agesa_NoopUnsupported },
	{AGESA_RUNFUNC_ONAP,			agesa_RunFuncOnAp },
	{AGESA_GET_IDS_INIT_DATA,		agesa_EmptyIdsInitData },
	{AGESA_HOOKBEFORE_DQS_TRAINING,		agesa_NoopSuccess },
	{AGESA_HOOKBEFORE_DRAM_INIT, 		board_BeforeDramInit},
	{AGESA_HOOKBEFORE_EXIT_SELF_REF,	agesa_NoopSuccess },
};
const int BiosCalloutsLen = ARRAY_SIZE(BiosCallouts);

static AGESA_STATUS board_ReadSpd (UINT32 Func, UINTN Data, VOID *ConfigPtr)
{
	AGESA_STATUS Status;
#ifdef __PRE_RAM__

	if (ConfigPtr == NULL)
		return AGESA_ERROR;

	// Set I2C Mux to Socket 0/1 SPD
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT0, ((AGESA_READ_SPD_PARAMS *)ConfigPtr)->SocketId);
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT1, 1);

	Status = read_spd_buffer(Func, Data, ConfigPtr);

#else
	Status = AGESA_UNSUPPORTED;
#endif

	return Status;
}

/* AGESA calls this prior to DRAM init, but after we have SPD data,
   making it the perfect place to do the set the GPIOs required to
   set the DIMM voltage for each CPU.
   This is a unique ASUS implementation for voltage control. */
static AGESA_STATUS board_BeforeDramInit(UINT32 Func, UINTN Data, VOID *ConfigPtr)
{
	AGESA_STATUS Status;
#ifdef __PRE_RAM__
	MEM_DATA_STRUCT *MemData;
	ID_INFO CpuInfo;

	CpuInfo.IdInformation = Data;
	MemData = ConfigPtr;

	if(CpuInfo.IdField.ModuleId == 0)
		Status = set_ddr3_voltage(CpuInfo.IdField.SocketId, MemData->ParameterListPtr->DDR3Voltage);
	else // Skip re-setting voltage on the secondary DCT
		Status = AGESA_SUCCESS;
#else
	Status = AGESA_UNSUPPORTED;
#endif

	return Status;
}
