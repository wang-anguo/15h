#include <console/console.h>
#include <device/pci.h>
#include "NbPlatform.h"

#define MISC_ENABLE	0x62
#define IDE_GPIO_CNTRL	0xA0
#define IDE_GPIO_IN	0xA4
#define IDE_GPIO_WIRED	0x3F

void mainboard_rd890_override_cfg(AMD_NB_CONFIG_BLOCK *gConfigPtr) {
	pci_devfn_t sm_dev       = PCI_DEV(0, 0x14, 0);
	UINT8 r8, v8;
	UINT16 r16;
	UINT32 r32, v32;
	UINT8 Slot5Absent = 0;

	// Set IDE pins to GPIO
	r8 = pci_s_read_config8(sm_dev, MISC_ENABLE);
	v8 = r8 | BIT7;
	pci_s_write_config8(sm_dev, MISC_ENABLE, v8);

	// Set the GPIOs as inputs (default)
	r32 = pci_s_read_config32(sm_dev, IDE_GPIO_CNTRL);
	v32 = r32 | (0xFFFF) << 16;
	pci_s_write_config32(sm_dev, IDE_GPIO_CNTRL, v32);

	// Read GPIOs
	r16 = pci_s_read_config32(sm_dev, IDE_GPIO_IN) & IDE_GPIO_WIRED;
	printk(BIOS_DEBUG, "%s: IDE_GPIO_IN = %04X\n", __func__, r16);

	if(r16 & BIT4) Slot5Absent = 1;

	printk(BIOS_DEBUG, "%s: Slot5Absent = %d\n", __func__, Slot5Absent);

	if(Slot5Absent) {
		printk(BIOS_DEBUG, "%s: PCIe Slot4 = x16\n", __func__);
		gConfigPtr->Northbridges[0].pPcieConfig->CoreConfiguration[1] = GFX_CONFIG_AAAA;
		printk(BIOS_DEBUG, "%s: PCIe Slot5 = OFF\n", __func__);
		gConfigPtr->Northbridges[0].pPcieConfig->PortConfiguration[12].PortPresent = OFF;
		gConfigPtr->Northbridges[0].pPcieConfig->PortConfiguration[12].PortLinkMode = 0;
	}
};
