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


#include <device/device.h>      /* device_t */
#include <device/pci.h>         /* device_operations */
#include <device/pci_ids.h>
#include <arch/ioapic.h>
#include <device/smbus.h>       /* smbus_bus_operations */
#include <pc80/mc146818rtc.h>
#include <pc80/i8254.h>
#include <pc80/i8259.h>
#include <console/console.h>    /* printk */
#include <device/pci_ehci.h>
#include <arch/acpi.h>
#include "lpc.h"                /* lpc_read_resources */
#include "Platform.h"		/* Platform Specific Definitions */
#include "cfg.h"                /* sb700 CIMx configuration */
#include "chip.h"               /* struct southbridge_amd_cimx_sb700_config */
#include "sb_cimx.h"		/* AMD CIMx wrapper entries */

static AMDSBCFG sb_late_cfg; //global, init in sb700_cimx_config
static AMDSBCFG *sb_config = &sb_late_cfg;

/**
 * @brief Entry point of Southbridge CIMx callout
 *
 * prototype UINT32 (*SBCIM_HOOK_ENTRY)(UINT32 Param1, UINT32 Param2, void* pConfig)
 *
 * @param[in] func      Southbridge CIMx Function ID.
 * @param[in] data      Southbridge Input Data.
 * @param[in] config    Southbridge configuration structure pointer.
 *
 */
static u32 sb700_callout_entry(u32 func, u32 data, void* config)
{
	u32 ret = 0;

	printk(BIOS_DEBUG, "SB700 - Late.c - sb700_callout_entry - Start.\n");
	printk(BIOS_DEBUG, "SB700 - Late.c - sb700_callout_entry - End.\n");
	return ret;
}


static struct pci_operations lops_pci = {
	.set_subsystem = pci_dev_set_subsystem,
};

static void lpc_enable_resources(struct device *dev)
{

	printk(BIOS_SPEW, "SB700 - Late.c - %s - Start.\n", __func__);
	pci_dev_enable_resources(dev);
	lpc_enable_childrens_resources(dev);
	printk(BIOS_SPEW, "SB700 - Late.c - %s - End.\n", __func__);
}

static void lpc_init(struct device *dev)
{
	printk(BIOS_DEBUG, "SB700 - Late.c - lpc_init - Start.\n");

	cmos_check_update_date();

	/* Initialize the real time clock.
	 * The 0 argument tells cmos_init not to
	 * update CMOS unless it is invalid.
	 * 1 tells cmos_init to always initialize the CMOS.
	 */
	cmos_init(0);

	setup_i8259(); /* Initialize i8259 pic */
	setup_i8254(); /* Initialize i8254 timers */

	printk(BIOS_DEBUG, "SB700 - Late.c - lpc_init - End.\n");
}

unsigned long acpi_fill_mcfg(unsigned long current)
{
	/* Just a dummy */
	return current;
}


static struct device_operations lpc_ops = {
	.read_resources = lpc_read_resources,
	.set_resources = lpc_set_resources,
	.enable_resources = lpc_enable_resources,
#if CONFIG(HAVE_ACPI_TABLES)
	.write_acpi_tables = acpi_write_hpet,
#endif
	.init = lpc_init,
	.scan_bus = scan_static_bus,
	.ops_pci = &lops_pci,
};

static const struct pci_driver lpc_driver __pci_driver = {
	.ops = &lpc_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_LPC,
};


static struct device_operations sata_ops = {
	.read_resources = pci_dev_read_resources,
	.set_resources = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init = 0,
	.scan_bus = 0,
	.ops_pci = &lops_pci,
};

static const struct pci_driver sata_driver __pci_driver = {
	.ops = &sata_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_SATA, //SATA IDE Mode 4390
};

static struct device_operations usb_ops = {
	.read_resources = pci_ehci_read_resources,
	.set_resources = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init = 0,
	.scan_bus = 0,
	.ops_pci = &lops_pci,
};

/*
 * The pci id of usb ctrl 0 and 1 are the same.
 */
static const struct pci_driver usb_ohci123_driver __pci_driver = {
	.ops = &usb_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_USB_18_0, /* OHCI-USB1, OHCI-USB2, OHCI-USB3 */
};

static const struct pci_driver usb_ohci3_driver __pci_driver = {
	.ops = &usb_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_USB_18_1,
};

static const struct pci_driver usb_ehci123_driver __pci_driver = {
	.ops = &usb_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_USB_18_2, /* EHCI-USB1, EHCI-USB2, EHCI-USB3 */
};

static const struct pci_driver usb_ohci4_driver __pci_driver = {
	.ops = &usb_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_USB_20_5, /* OHCI-USB4 */
};

static struct device_operations azalia_ops = {
	.read_resources = pci_dev_read_resources,
	.set_resources = pci_dev_set_resources,
	.enable_resources = pci_dev_enable_resources,
	.init = 0,
	.scan_bus = 0,
	.ops_pci = &lops_pci,
};

static const struct pci_driver azalia_driver __pci_driver = {
	.ops = &azalia_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_HDA,
};


static struct device_operations pci_ops = {
	.read_resources = pci_bus_read_resources,
	.set_resources = pci_dev_set_resources,
	.enable_resources = pci_bus_enable_resources,
	.init = 0,
	.scan_bus = pci_scan_bridge,
	.reset_bus = pci_bus_reset,
	.ops_pci = &lops_pci,
};

static const struct pci_driver pci_driver __pci_driver = {
	.ops = &pci_ops,
	.vendor = PCI_VENDOR_ID_ATI,
	.device = PCI_DEVICE_ID_ATI_SB700_PCI,
};

static void sb700_enable(struct device *dev)
{
	struct southbridge_amd_cimx_sb700_config *sb_chip =
		(struct southbridge_amd_cimx_sb700_config *)(dev->chip_info);

	printk(BIOS_DEBUG, "%s\n", __func__);
	switch (dev->path.pci.devfn) {
		case (0x11 << 3) | 0: /* 0:11.0  SATA */
			if (dev->enabled) {
				sb_config->SataController = CIMX_OPTION_ENABLED;
				if (1 == sb_chip->boot_switch_sata_ide)
					sb_config->SataIdeCombMdPriSecOpt = 0; //0 -IDE as primary.
				else if (0 == sb_chip->boot_switch_sata_ide)
					sb_config->SataIdeCombMdPriSecOpt = 1; //1 -IDE as secondary.
			} else {
				sb_config->SataController = CIMX_OPTION_DISABLED;
			}
			break;

		case (0x12 << 3) | 0: /* 0:12:0 OHCI-USB1 */
		case (0x12 << 3) | 2: /* 0:12:2 EHCI-USB1 */
		case (0x13 << 3) | 0: /* 0:13:0 OHCI-USB2 */
		case (0x13 << 3) | 2: /* 0:13:2 EHCI-USB2 */
			break;

		case (0x14 << 3) | 0: /* 0:14:0 SMBUS */
			{
				uintptr_t ioapic_base;
				printk(BIOS_DEBUG, "sm_init().\n");
				ioapic_base = IO_APIC_ADDR;
				clear_ioapic((void *)ioapic_base);
				/* I/O APIC IDs are normally limited to 4-bits. Enforce this limit. */
				if (CONFIG_MAX_CPUS >= 16)
					setup_ioapic((void *)ioapic_base, 0);
				else
					setup_ioapic((void *)ioapic_base, CONFIG_MAX_CPUS + 1);
			}
			break;

		case (0x14 << 3) | 1: /* 0:14:1 IDE */
			break;

		case (0x14 << 3) | 2: /* 0:14:2 HDA */
			if (dev->enabled) {
				if (AZALIA_DISABLE == sb_config->AzaliaController) {
					sb_config->AzaliaController = AZALIA_AUTO;
				}
				printk(BIOS_DEBUG, "hda enabled\n");
			} else {
				sb_config->AzaliaController = AZALIA_DISABLE;
				printk(BIOS_DEBUG, "hda disabled\n");
			}
			break;


		case (0x14 << 3) | 3: /* 0:14:3 LPC */
			break;

		case (0x14 << 3) | 4: /* 0:14:4 PCI */
			break;

		case (0x14 << 3) | 5: /* 0:14:5 OHCI-USB4 */
			/* FIXME: Not part of DF 14.5, but these need to execute last */

			//sb_config->StdHeader.Func = SB_BEFORE_PCI_INIT;
			//AmdSbDispatcher(sb_config);
			sbBeforePciInit(sb_config);

			//sb_config->StdHeader.Func = SB_AFTER_PCI_INIT;
			//AmdSbDispatcher(sb_config);
			sbAfterPciInit(sb_config);

			//sb_config->StdHeader.Func = SB_LATE_POST_INIT;
			//AmdSbDispatcher(sb_config);
			sbLatePost(sb_config);
			break;

		default:
			break;
	}
}

static void sb700_gpio_dump1(int reg) {
	pci_devfn_t sm_dev       = PCI_DEV(0, 0x14, 0); //SMBus
	UINT16    r16, v16;

	r16 = pci_s_read_config16(sm_dev, reg);
	v16 = r16| BIT4|BIT5|BIT6|BIT7;
	pci_s_write_config16(sm_dev, reg, v16);
	v16 = pci_s_read_config16(sm_dev, reg);
	pci_s_write_config16(sm_dev, reg, r16);
	printk(BIOS_DEBUG, "SB700 GPIO Reg %02X = %04X\n", reg, v16);
}

static void sb700_gpio_dump2(int reg) {
	pci_devfn_t sm_dev       = PCI_DEV(0, 0x14, 0); //SMBus
	UINT16    r16, v16;

	r16 = pci_s_read_config16(sm_dev, reg);
	v16 = r16| BIT4|BIT5|BIT6|BIT7 |BIT12|BIT13|BIT14|BIT15;
	pci_s_write_config16(sm_dev, reg, v16);
	v16 = pci_s_read_config16(sm_dev, reg);
	pci_s_write_config16(sm_dev, reg, r16);
	printk(BIOS_DEBUG, "SB700 GPIO Reg %02X = %04X\n", reg, v16);
}

static void sb700_gpio_dump3(int ctrl, int status) {
	pci_devfn_t sm_dev       = PCI_DEV(0, 0x14, 0); //SMBus
	UINT32    r32, v32;
	UINT16 v16;

	r32 = pci_s_read_config32(sm_dev, ctrl);
	v32 = r32| BIT16|BIT17|BIT18|BIT19|BIT20|BIT21|BIT22|BIT23|BIT24|BIT25|BIT26|BIT27|BIT28|BIT29|BIT30|BIT31;
	pci_s_write_config32(sm_dev, ctrl, v32);
	v32 = pci_s_read_config32(sm_dev, ctrl);
	v16 = pci_s_read_config16(sm_dev, status);
	pci_s_write_config32(sm_dev, ctrl, r32);
	printk(BIOS_DEBUG, "SB700 GPIO Reg %02X = %04X\n", status, v16);
}

static void sb700_final(void *chip_info) {
	if(SB700_DUMP_GPIO) {
		printk(BIOS_DEBUG, "--- DUMPING SB700 GPIO BEG ---\n");
		sb700_gpio_dump1(0x50); // GPIO_52_to_49_Cntrl
		sb700_gpio_dump1(0x52); // GPIO_56_to_53_Cntrl
		sb700_gpio_dump1(0x54); // GPIO_60_to_57_Cntrl
		sb700_gpio_dump1(0x56); // GPIO_64_to_61_Cntrl
		sb700_gpio_dump2(0x5A); // GPIO_73_to_70_Cntrl
		sb700_gpio_dump1(0x7E); // GPIO_69_68_66_65_Cntrl
		sb700_gpio_dump1(0x80); // GPIO_3_to_0_Cntrl
		sb700_gpio_dump2(0x82); // GPIO_32_31_14_13_Cntrl
		sb700_gpio_dump3(0xA0, 0xA4); // IDE_GPIO_Cntrl / IDE_GPIO_In
		sb700_gpio_dump2(0xA6); // GPIO_48_47_46_37_Cntrl
		// GPIO_12_to_4_Cntrl		0xA8
		// SATA_ACT_GPIO_Cntrl 		0xAC
		// PCI_INT_GPIO_Cntrl		0xBC
		printk(BIOS_DEBUG, "--- DUMPING SB700 GPIO END ---\n");
	}

}

static void sb700_init(void *chip_info)
{
	printk(BIOS_DEBUG, "SB700: %s\n", __func__);
	sb_config->StdHeader.pCallBack = sb700_callout_entry;
	sb700_cimx_config(sb_config);
}

struct chip_operations southbridge_amd_cimx_sb700_ops = {
	CHIP_NAME("ATI SB700")
	.init = sb700_init,
	.enable_dev = sb700_enable,
	.final = sb700_final,
};
