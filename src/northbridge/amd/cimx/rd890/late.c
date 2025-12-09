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
#include <console/console.h>
#include <device/device.h>
#include <arch/ioapic.h>
#include <device/pci.h>
#include <device/pci_ids.h>
#include <delay.h>
#include "NbPlatform.h"
#include "nb_cimx.h"
#include "cfg.h"

#include <northbridge/amd/cimx/rd890/chip.h>

#include <vendorcode/amd/cimx/rd890/nbIommu.h>

/**
 * Global RD890 CIMX Configuration structure
 */
static AMD_NB_CONFIG_BLOCK gConfig;
static AMD_NB_CONFIG_BLOCK *gConfigPtr = &gConfig;
static NB_CONFIG nb_cfg[MAX_NB_COUNT];
static HT_CONFIG ht_cfg[MAX_NB_COUNT];
static PCIE_CONFIG pcie_cfg[MAX_NB_COUNT];
static uint8_t ivrs_buffer[0x2000]; // IVRS_BUFFER_SIZE

static unsigned long rd890_iommu_write_acpi_tables(struct device *dev, unsigned long current, struct acpi_rsdp *rsdp)
{
	struct northbridge_amd_cimx_rd890_config *rd890_info = dev->chip_info;
	AMD_NB_CONFIG *NbConfigPtr = &(gConfig.Northbridges[rd890_info->index]);
	IOMMU_IVRS_HEADER *IvrsHeader;
	u32 i;

	if(rd890_info->index == 0) {
		current = ALIGN(current, 8);
		IvrsHeader = (IOMMU_IVRS_HEADER *)ivrs_buffer;
		for (i = 0; i < MAX_NB_COUNT; i ++) {
			gConfig.Northbridges[i].pNbConfig->IommuIvrsBuffer = current;
			gConfig.Northbridges[i].pNbConfig->AcpiRsdp = (UINT32)rsdp;
		}
		printk(BIOS_DEBUG, "%s: copying IVRS from %08X to %08X (%d bytes)\n", __func__, (unsigned int)ivrs_buffer, (unsigned int)current, IvrsHeader->Length);
		memcpy((void*)current, ivrs_buffer, IvrsHeader->Length);
		current += IvrsHeader->Length;
		acpi_add_table(rsdp, (void*)NbConfigPtr->pNbConfig->IommuIvrsBuffer);

		LibSystemApiCall(AmdLatePostInitIommuAcpi, gConfigPtr);
	}
	return current;
}

static void rd890_iommu_enable_resources(struct device *dev)
{
	struct northbridge_amd_cimx_rd890_config *rd890_info = dev->chip_info;

	pci_dev_enable_resources(dev);

	if(rd890_info->index == 0) {
		LibSystemApiCall(AmdMidPostInitIommu, gConfigPtr);
	}
}

static void rd890_iommu_set_resources(struct device *dev)
{
	struct northbridge_amd_cimx_rd890_config *rd890_info = dev->chip_info;
	AMD_NB_CONFIG *NbConfigPtr = &(gConfig.Northbridges[rd890_info->index]);
	struct resource *res;

	/* Get the normal pci resources of this device */
	pci_dev_read_resources(dev);

	/* Get the allocated range */
	res = find_resource(dev, 0x44);

	if (res->base == 0) {
		printk(BIOS_WARNING, "%s: Unable to allocate MMIO range to IOMMU\n", __func__);
	}
	printk(BIOS_DEBUG, "%s: IommuBaseAddress = %08X\n", __func__, (unsigned int)res->base);
	/* Tell CIMx the IOMMU base address */
	NbConfigPtr->pNbConfig->IommuBaseAddress = res->base;
	NbConfigPtr->pNbConfig->IommuIvrsBuffer = (UINT32)ivrs_buffer;

	pci_dev_set_resources(dev);
}

static void rd890_iommu_read_resource(struct device *dev)
{
	struct resource *res;

	pci_dev_read_resources(dev);

	res = new_resource(dev, 0x44);          /* IOMMU */
	res->base = 0x00;
	res->size = 0x4000;
	res->limit = 0xFFFFFFFFUL;              /* res->base + res->size -1; */
	res->align = 14;                        /* 16k alignment */
	res->gran = 14;
	res->flags = IORESOURCE_MEM | IORESOURCE_RESERVE;

	compact_resources(dev);
}

static struct pci_operations rd890_iommu_ops_pci = {
	.set_subsystem = pci_dev_set_subsystem,
};

static struct device_operations rd890_iommu_ops = {
	.read_resources = rd890_iommu_read_resource,
	.set_resources = rd890_iommu_set_resources,
	.enable_resources = rd890_iommu_enable_resources,
	.write_acpi_tables = rd890_iommu_write_acpi_tables,
	.init = 0,
	.scan_bus = 0,
	.ops_pci = &rd890_iommu_ops_pci,
};

static const struct pci_driver rd890_iommu_driver __pci_driver = {
	.ops = &rd890_iommu_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_AMD_RD890_IOMMU,
};

static void rd890_ht_init(struct device *dev)
{
	struct northbridge_amd_cimx_rd890_config *rd890_info = dev->chip_info;
	void *ioapic_base;

	pci_write_config32(dev, 0xF8, 0x1);
	ioapic_base = (void *)(uintptr_t)(pci_read_config32(dev, 0xFC) & 0xfffffff0);
	clear_ioapic(ioapic_base);
	setup_ioapic(ioapic_base, 1);

	if(rd890_info->index == 0) {
		LibSystemApiCall(AmdPcieLateInit, gConfigPtr);
		LibSystemApiCall(AmdLatePostInit, gConfigPtr);
	}
}

/* If IOAPIC's index changes, we should replace the pci_dev_set_resource(). */
static void rd890_ht_set_resources(struct device *dev)
{
	/* set IOAPIC's index as 1 and make sure no one changes it. */
	pci_write_config32(dev, 0xF8, 0x1);

	/* Get the normal pci resources of this device */
	pci_dev_read_resources(dev);

	pci_dev_set_resources(dev);
}

static void rd890_ht_read_resource(struct device *dev)
{
	pci_dev_read_resources(dev);

	/* rpr6.2.(1). Write the Base Address Register (BAR) */
	pci_write_config32(dev, 0xF8, 0x1); /* set IOAPIC's index as 1 and make sure no one changes it. */
	pci_get_resource(dev, 0xFC); /* APIC located in sr5690 */

	compact_resources(dev);
}

static const unsigned short ht_devices[] = {
	PCI_DEVICE_ID_AMD_SR5690_HT,
	PCI_DEVICE_ID_AMD_SR5670_HT,
	PCI_DEVICE_ID_AMD_SR5650_HT,
	PCI_DEVICE_ID_AMD_RD890TV_HT,
	PCI_DEVICE_ID_AMD_RD890_HT,
	PCI_DEVICE_ID_AMD_990FX_HT,
	0
};

static struct pci_operations rd890_ht_ops_pci = {
	.set_subsystem = pci_dev_set_subsystem,
};

static struct device_operations rd890_ht_ops = {
	.read_resources = rd890_ht_read_resource,
	.set_resources = rd890_ht_set_resources,
	.init = rd890_ht_init,
	.scan_bus = 0,
	.ops_pci = &rd890_ht_ops_pci,
};

static const struct pci_driver rd890_ht_driver __pci_driver = {
	.ops = &rd890_ht_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.devices = ht_devices,
};

static void rd890_enable(struct device *dev)
{
	struct northbridge_amd_cimx_rd890_config *rd890_info = dev->chip_info;
	u32 devfn = dev->path.pci.devfn;

	printk(BIOS_INFO, "%s: Bus-%x Dev-%X Fun-%X, enable=%x\n", __func__,
			0, (devfn >> 3), (devfn & 0x07), dev->enabled);

	if(devfn == 0 && rd890_info->index == 0) {
		/* Reset PCIE Cores, Training the Ports selected by port_enable of devicetree
		 * After this call EP are fully operational on particular NB
		 */
		LibSystemApiCall(AmdPcieEarlyInit, gConfigPtr);

		LibSystemApiCall(AmdEarlyPostInit, gConfigPtr);

		LibSystemApiCall(AmdMidPostInit, gConfigPtr);
	}

}

static void set_pcie_dereset(void *nbconfig) {
	AMD_NB_CONFIG_BLOCK *pConfig = (AMD_NB_CONFIG_BLOCK*)nbconfig;
	u32 nb_addr;
	u32 val;
	u32 i;

	val = 0x00000007UL;
	for (i = 0; i < MAX_NB_COUNT; i ++) {
		nb_addr = pConfig->Northbridges[i].NbPciAddress.AddressValue | NB_HTIU_INDEX;
		LibNbPciIndexRMW(nb_addr, NB_HTIU_REGA8, AccessS3SaveWidth32, ~val, val, &(pConfig->Northbridges[i]));
	}
}

static u32 rd890_callout_entry(u32 func, uintptr_t data, void *config)
{
	u32 ret = 0;
	
	switch(func) {
		case PHCB_AmdPortResetDeassert: // 0x8001
			printk(BIOS_DEBUG, "%s (late): PHCB_AmdPortResetDeassert\n", __func__);
			set_pcie_dereset(config);
			break;
		default:
			printk(BIOS_DEBUG, "%s (late): function = %08X\n", __func__, func);
			break;
	}
	return ret;
}

static void rd890_init(void *chip_info) {
	struct northbridge_amd_cimx_rd890_config *rd890_info = chip_info;
	uint8_t nb_index = rd890_info->index;
	int i;

	if(nb_index == 0) {
		/* CIMx configuration defualt initialize */
		rd890_cimx_config(&gConfigPtr, &nb_cfg[0], &ht_cfg[0], &pcie_cfg[0]);

		/* CIMx callout handle */
		gConfig.StandardHeader.CalloutPtr = &rd890_callout_entry;
	}

	switch(rd890_info->gpp1_configuration) {
		case 0:
			pcie_cfg[nb_index].CoreConfiguration[0] = GFX_CONFIG_AAAA;
			break;
		case 1:
			pcie_cfg[nb_index].CoreConfiguration[0] = GFX_CONFIG_AABB;
			break;
		default:
			printk(BIOS_ERR, "%s: Unhandled gpp1_configuration value (%d)\n", __func__, rd890_info->gpp1_configuration);
			break;
	}

	switch(rd890_info->gpp2_configuration) {
		case 0:
			pcie_cfg[nb_index].CoreConfiguration[1] = GFX_CONFIG_AAAA;
			break;
		case 1:
			pcie_cfg[nb_index].CoreConfiguration[1] = GFX_CONFIG_AABB;
			break;
		default:
			printk(BIOS_ERR, "%s: Unhandled gpp2_configuration value (%d)\n", __func__, rd890_info->gpp2_configuration);
			break;
	}

	switch(rd890_info->gpp3a_configuration) {
		case GPP_CONFIG_GPP420000:
		case GPP_CONFIG_GPP411000:
		case GPP_CONFIG_GPP222000:
		case GPP_CONFIG_GPP221100:
		case GPP_CONFIG_GPP211110:
		case GPP_CONFIG_GPP111111:
			pcie_cfg[nb_index].CoreConfiguration[2] = rd890_info->gpp3a_configuration;
			break;
		default:
			printk(BIOS_ERR, "%s: Unhandled gpp3a_configuration value (%d)\n", __func__, rd890_info->gpp3a_configuration);
			break;
			
	}

	// CFG_TEMP_PCIE_MMIO_BASE_ADDRESS
	pcie_cfg[nb_index].TempMmioBaseAddress = (UINT16)(0xD0000000 >> 20);
	nb_cfg[nb_index].IoApicBaseAddress = IO_APIC_ADDR;

	for (i = 0; i <= MAX_CORE_ID; i++) {
		pcie_cfg[nb_index].CoreSetting[i].SkipConfiguration = OFF;
		pcie_cfg[nb_index].CoreSetting[i].PerformanceMode = OFF;
	}

	for (i = MIN_PORT_ID; i <= MAX_PORT_ID; i++) {
		if ((rd890_info->port_enable & (1 << i)) != 0) {
			pcie_cfg[nb_index].PortConfiguration[i].PortPresent = ON;
			pcie_cfg[nb_index].PortConfiguration[i].PortLinkMode = PcieLinkModeGen2;
		}
	}

	#ifdef RD890_MAINBOARD_OVERRIDE_CFG
	mainboard_rd890_override_cfg(gConfigPtr);
	#endif
}

static void rd890_final(void *chip_info) {
	PCI_ADDR PciAddress;
	uint32_t value;

	if(RD890_TEST_THERMAL_SHUTDOWN) {
		PciAddress.AddressValue = MAKE_SBDFO(0, 0, 0x18, 0x03, 0xE4);
		LibNbPciRead(PciAddress.AddressValue, AccessWidth32, &value, &(gConfigPtr->Northbridges[0]));
		value |= BIT31;
		printk(BIOS_ERR, "%s: Triggering Thermal Shutdown in 5 seconds!!!\n", __func__);
		mdelay(5000);
		printk(BIOS_ERR, "%s: Triggering Thermal Shutdown NOW!!!\n", __func__);
		LibNbPciWrite(PciAddress.AddressValue, AccessWidth32, &value, &(gConfigPtr->Northbridges[0]));
	}
}

struct chip_operations northbridge_amd_cimx_rd890_ops = {
	CHIP_NAME("ATI RD890")
	.init = rd890_init,
	.enable_dev = rd890_enable,
	.final = rd890_final,
};
