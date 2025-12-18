#include <southbridge/amd/cimx/sb700/gpio.h>
#include <console/console.h>
#include "NbPlatform.h"

#include "gpio.h"

void mainboard_rd890_override_cfg(AMD_NB_CONFIG_BLOCK *gConfigPtr) {
	if(sb700_gpio_get(SMBUS_GPIO_PCIE5_ABSENT)) {
		printk(BIOS_DEBUG, "%s: PCIe Slot4 = x16\n", __func__);
		gConfigPtr->Northbridges[0].pPcieConfig->CoreConfiguration[1] = GFX_CONFIG_AAAA;
		printk(BIOS_DEBUG, "%s: PCIe Slot5 = OFF\n", __func__);
		gConfigPtr->Northbridges[0].pPcieConfig->PortConfiguration[12].PortPresent = OFF;
		gConfigPtr->Northbridges[0].pPcieConfig->PortConfiguration[12].PortLinkMode = 0;
	}
};
