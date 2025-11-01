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
#include "NbPlatform.h"
#include "nb_cimx.h"
#include "rd890_cfg.h"

#include <vendorcode/amd/cimx/rd890/nbIommu.h>

/**
 * Global RD890 CIMX Configuration structure
 */
static NB_CONFIG nb_cfg[MAX_NB_COUNT];
static HT_CONFIG ht_cfg[MAX_NB_COUNT];
static PCIE_CONFIG pcie_cfg[MAX_NB_COUNT];
static AMD_NB_CONFIG_BLOCK gConfig;
static uint8_t ivrs_buffer[0x2000]; // IVRS_BUFFER_SIZE

static unsigned long rd890_iommu_write_acpi_tables(struct device *device, unsigned long current, struct acpi_rsdp *rsdp)
{
	AMD_NB_CONFIG *NbConfigPtr = NULL;
	IOMMU_IVRS_HEADER *IvrsHeader;

	// TODO, support multiple NB. See MAX_NB_COUNT
	NbConfigPtr = &(gConfig.Northbridges[0]);

	current = ALIGN(current, 8);
	IvrsHeader = (IOMMU_IVRS_HEADER *)ivrs_buffer;
	NbConfigPtr->pNbConfig->IommuIvrsBuffer = current;
	NbConfigPtr->pNbConfig->AcpiRsdp = (UINT32)rsdp;
	printk(BIOS_DEBUG, "%s: copying IVRS from %08X to %08X (%d bytes)\n", __func__, (unsigned int)ivrs_buffer, (unsigned int)current, IvrsHeader->Length);
	memcpy((void*)current, ivrs_buffer, IvrsHeader->Length);
	current += IvrsHeader->Length;
	acpi_add_table(rsdp, (void*)NbConfigPtr->pNbConfig->IommuIvrsBuffer);

	LibSystemApiCall(AmdLatePostInitIommuAcpi, &gConfig);
	return current;
}

static void rd890_iommu_enable_resources(struct device *dev)
{
	AMD_NB_CONFIG *NbConfigPtr = NULL;
	NbConfigPtr = &(gConfig.Northbridges[0]);

	pci_dev_enable_resources(dev);

	LibSystemApiCall(AmdMidPostInitIommu, &gConfig);
}

static void rd890_iommu_set_resources(struct device *dev)
{
	AMD_NB_CONFIG *NbConfigPtr = NULL;
	struct resource *res;

	// TODO, support multiple NB. See MAX_NB_COUNT
	NbConfigPtr = &(gConfig.Northbridges[0]);

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
	void *ioapic_base;

	pci_write_config32(dev, 0xF8, 0x1);
	ioapic_base = (void *)(uintptr_t)(pci_read_config32(dev, 0xFC) & 0xfffffff0);
	clear_ioapic(ioapic_base);
	setup_ioapic(ioapic_base, 1);

	LibSystemApiCall(AmdPcieLateInit, &gConfig);
	LibSystemApiCall(AmdLatePostInit, &gConfig);
}

/* If IOAPIC's index changes, we should replace the pci_dev_set_resource(). */
static void rd890_ht_set_resources(struct device *dev)
{
	AMD_NB_CONFIG *NbConfigPtr = NULL;

	// TODO, support multiple NB. See MAX_NB_COUNT
	NbConfigPtr = &(gConfig.Northbridges[0]);

	/* set IOAPIC's index as 1 and make sure no one changes it. */
	pci_write_config32(dev, 0xF8, 0x1);

	/* Get the normal pci resources of this device */
	pci_dev_read_resources(dev);

	/* Tell CIMx the IO APIC base address */
	NbConfigPtr->pNbConfig->IoApicBaseAddress = IO_APIC_ADDR;

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

static void rd890_enable(struct device *dev)
{
	u32 devfn;
	AMD_NB_CONFIG *NbConfigPtr = NULL;

	u8 nb_index = 0; /* The first IO Hub, TODO: other NBs. See MAX_NB_COUNT */
	NbConfigPtr = &(gConfig.Northbridges[nb_index]);

	devfn = dev->path.pci.devfn;
	printk(BIOS_INFO, "rd890_enable  ");
	printk(BIOS_INFO, "Bus-%x Dev-%X Fun-%X, enable=%x\n",
			0, (devfn >> 3), (devfn & 0x07), dev->enabled);

	/* we only do this once */
	if (devfn == 0) {
		/* CIMX configuration defualt initialize */
		rd890_cimx_config(&gConfig, &nb_cfg[0], &ht_cfg[0], &pcie_cfg[0]);
		if (gConfig.StandardHeader.CalloutPtr != NULL) {
			gConfig.StandardHeader.CalloutPtr(CB_AmdSetPcieEarlyConfig,
					(uintptr_t)dev, (VOID*)NbConfigPtr);
		}
		/* Reset PCIE Cores, Training the Ports selected by port_enable of devicetree
		 * After this call EP are fully operational on particular NB
		 */
		LibSystemApiCall(AmdPcieEarlyInit, &gConfig);

		if (gConfig.StandardHeader.CalloutPtr != NULL) {
			gConfig.StandardHeader.CalloutPtr(CB_AmdSetEarlyPostConfig, 0, (VOID*)NbConfigPtr);
		}
		LibSystemApiCall(AmdEarlyPostInit, &gConfig);

		if (gConfig.StandardHeader.CalloutPtr != NULL) {
			gConfig.StandardHeader.CalloutPtr(CB_AmdSetMidPostConfig, 0, (VOID*)NbConfigPtr);
		}
		LibSystemApiCall(AmdMidPostInit, &gConfig);
	}
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

struct chip_operations northbridge_amd_cimx_rd890_ops = {
	CHIP_NAME("ATI RD890")
	.enable_dev = rd890_enable,
};
