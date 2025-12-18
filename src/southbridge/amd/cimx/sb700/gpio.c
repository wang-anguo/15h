#include <device/pci_ops.h>
#include <console/console.h>
#include "gpio.h"

// GPIO Group "A"
#define GPIO_52_TO_49_CNTRL	0x50
#define GPIO_56_TO_53_CNTRL	0x52
#define GPIO_60_TO_57_CNTRL	0x54
#define GPIO_64_TO_61_CNTRL	0x56
#define GPIO_69_68_66_65_CNTRL	0x7E
#define GPIO_3_TO_0_CNTRL	0x80

// GPIO Group "B"
#define GPIO_73_TO_70_CNTRL	0x5A
#define GPIO_32_31_14_13_CNTRL	0x82
#define GPIO_48_47_46_37_CNTRL	0xA6
#define PCI_INT_GPIO_CNTRL	0xBC

// GPIO Unique
#define MISC_ENABLE     	0x62
#define IDE_GPIO_CNTRL		0xA0
#define IDE_GPIO_IN		0xA4
#define GPIO_12_TO_4_CNTRL	0xA8
#define SATA_ACT_GPIO_CNTRL	0xAC

#define DBG_GPIO_SET		"sb700_gpio_set"
#define DBG_GPIO_GET		"sb700_gpio_get"
#define DBG_GPIO_CONFIG		"sb700_gpio_config"
#define DBG_GPIO_TOGGLE		"sb700_gpio_toggle"

#ifdef __PRE_RAM__
	#define PCI_R8	pci_read_config8
	#define PCI_R16	pci_read_config16
	#define PCI_R32	pci_read_config32
	#define PCI_W8	pci_write_config8
	#define PCI_W16	pci_write_config16
	#define PCI_W32	pci_write_config32
#else
	#define PCI_R8	pci_s_read_config8
	#define PCI_R16	pci_s_read_config16
	#define PCI_R32	pci_s_read_config32
	#define PCI_W8	pci_s_write_config8
	#define PCI_W16	pci_s_write_config16
	#define PCI_W32	pci_s_write_config32
#endif

static SB700_GPIO_VALUE gpio_A_get(int reg, int offset) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16;
	int bit_mode;
	int bit_output;
	int bit_input;

	bit_output = 1 << (offset);
	bit_mode = 1 << (offset + 4);
	bit_input = 1 << (offset + 8);

	r16 = PCI_R16(smbus, reg);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X\n", DBG_GPIO_GET, reg, r16);
	if(r16 & bit_mode) {
		// TRISTATE
		if(r16 & bit_input) return HIGH;
		return LOW;
	} else {
		// OUTPUT
		if(r16 & bit_output) return HIGH;
		return LOW;
	}
}

static void gpio_A_set(int reg, int offset, SB700_GPIO_VALUE value) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16, v16;
	int bit;

	bit = 1 << offset;
	r16 = PCI_R16(smbus, reg);
	v16 = r16 & ~bit;
	if(value == HIGH) v16 |= bit;
	PCI_W16(smbus, reg, v16);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X -> %04X\n", DBG_GPIO_SET, reg, r16, v16);
}

static void gpio_A_config(int reg, int offset, SB700_GPIO_MODE mode) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16, v16;
	int bit;

	bit = 1 << (offset + 4);

	r16 = PCI_R16(smbus, reg);
	v16 = r16 & ~bit;
	if(mode == TRISTATE) v16 |= bit;
	PCI_W16(smbus, reg, v16);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X -> %04X\n", DBG_GPIO_CONFIG, reg, r16, v16);
}

static SB700_GPIO_VALUE gpio_B_get(int reg, int offset) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16;
	int bit_mode;
	int bit_output;
	int bit_input;
	int bit_enabled;

	bit_output = 1 << (offset);
	bit_mode = 1 << (offset + 4);
	bit_input = 1 << (offset + 8);
	bit_enabled = 1 << (offset + 12);

	r16 = PCI_R16(smbus, reg);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X\n", DBG_GPIO_GET, reg, r16);
	if(!(r16 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, reg, offset);
		return ERROR;
	}
	if(r16 & bit_mode) {
		// TRISTATE
		if(r16 & bit_input) return HIGH;
		return LOW;
	} else {
		// OUTPUT
		if(r16 & bit_output) return HIGH;
		return LOW;
	}
}

static void gpio_B_set(int reg, int offset, SB700_GPIO_VALUE value) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16, v16;
	int bit;
	int bit_enabled;

	bit = 1 << offset;
	bit_enabled = 1 << (offset + 12);

	r16 = PCI_R16(smbus, reg);
	if(!(r16 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, reg, offset);
		return;
	}
	v16 = r16 & ~bit;
	if(value == HIGH) v16 |= bit;
	PCI_W16(smbus, reg, v16);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X -> %04X\n", DBG_GPIO_SET, reg, r16, v16);
}

static void gpio_B_config(int reg, int offset, SB700_GPIO_MODE mode) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16, v16;
	int bit;
	int bit_enabled;

	bit = 1 << (offset + 4);
	bit_enabled = 1 << (offset + 12);

	r16 = PCI_R16(smbus, reg);
	if(!(r16 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, reg, offset);
		return;
	}
	v16 = r16 & ~bit;
	if(mode == TRISTATE) v16 |= bit;
	PCI_W16(smbus, reg, v16);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X -> %04X\n", DBG_GPIO_CONFIG, reg, r16, v16);
}

static void gpio_B_toggle(int reg, int offset, int enabled) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u16 r16, v16;
	int bit;

	bit = 1 << (offset + 12);

	r16 = PCI_R16(smbus, reg);
	v16 = r16 & ~bit;
	if(enabled) v16 |= bit;
	PCI_W16(smbus, reg, v16);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X -> %04X\n", DBG_GPIO_TOGGLE, reg, r16, v16);
}

static int is_gpio_ide_enabled(void) {
	const int reg = MISC_ENABLE;
	const int bit = (1 << 7);
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u8 r8;

	r8 = PCI_R8(smbus, reg);
	if(r8 & bit) return 1;
	return 0;
}

static SB700_GPIO_VALUE gpio_ide_get(int offset) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u32 r32;
	u16 r16;
	int bit_mode;
	int bit_io;

	if(!is_gpio_ide_enabled()) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, IDE_GPIO_CNTRL, offset);
		return ERROR;
	}

	bit_io = 1 << (offset);
	bit_mode = 1 << (offset + 16);

	r32 = PCI_R32(smbus, IDE_GPIO_CNTRL);
	if(r32 & bit_mode) {
		// TRISTATE
		r16 = PCI_R16(smbus, IDE_GPIO_IN);
		printk(BIOS_DEBUG, "%s: SMBUS[%02X] %04X\n", DBG_GPIO_GET, IDE_GPIO_IN, r16);
		if(r16 & bit_io) return HIGH;
		return LOW;
	} else {
		// OUTPUT
		printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X\n", DBG_GPIO_GET, IDE_GPIO_CNTRL, r32);
		if(r32 & bit_io) return HIGH;
		return LOW;
	}
}

static void gpio_ide_set(int offset, SB700_GPIO_VALUE value) {
	const int reg = IDE_GPIO_CNTRL;
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	u32 r32, v32;
	int bit;

	if(!is_gpio_ide_enabled()) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, reg, offset);
		return;
	}

	bit = 1 << offset;
	r32 = PCI_R32(smbus, reg);
	v32 = r32 & ~bit;
	if(value == HIGH) v32 |= bit;
	PCI_W32(smbus, reg, v32);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X -> %08X\n", DBG_GPIO_SET, reg, r32, v32);
}

static void gpio_ide_config(int offset, SB700_GPIO_MODE mode) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = IDE_GPIO_CNTRL;
	u32 r32, v32;
	int bit;

	if(!is_gpio_ide_enabled()) {
		printk(BIOS_ERR, "%s: SMBUS[%02X.%d] GPIO IS DISABLED\n", __func__, reg, offset);
		return;
	}

	bit = 1 << (offset + 16);

	r32 = PCI_R32(smbus, reg);
	v32 = r32 & ~bit;
	if(mode == TRISTATE) v32 |= bit;
	PCI_W32(smbus, reg, v32);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X -> %08X\n", DBG_GPIO_CONFIG, reg, r32, v32);
}

static void gpio_ide_toggle(int enabled) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = MISC_ENABLE;
	const int bit = (1 << 7);
	u8 r8, v8;

	r8 = PCI_R8(smbus, reg);
	v8 = r8 & ~bit;
	if(enabled) v8 |= bit;
	PCI_W8(smbus, reg, v8);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %02X -> %02X\n", DBG_GPIO_TOGGLE, reg, r8, v8);
}

static void gpio_sata_set(SB700_GPIO_VALUE value) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = SATA_ACT_GPIO_CNTRL;
	const int bit_output = (1 << 0);
	const int bit_enabled = (1 << 3);
	u8 r8, v8;

	r8 = PCI_R8(smbus, reg);
	if(!(r8 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO IS DISABLED\n", __func__, reg);
		return;
	}
	v8 = r8 & ~bit_output;
	if(value == HIGH) v8 |= bit_output;
	PCI_W8(smbus, reg, v8);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %02X -> %02X\n", DBG_GPIO_SET, reg, r8, v8);
}

static SB700_GPIO_VALUE gpio_sata_get(void) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = SATA_ACT_GPIO_CNTRL;
	const int bit_output = (1 << 0);
	const int bit_mode = (1 << 1);
	const int bit_input = (1 << 2);
	const int bit_enabled = (1 << 3);
	u8 r8;

	r8 = PCI_R8(smbus, reg);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %02X\n", DBG_GPIO_GET, reg, r8);
	if(!(r8 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO IS DISABLED\n", __func__, reg);
		return ERROR;
	}
	if(r8 & bit_mode) {
		// TRISTATE
		if(r8 & bit_input) return HIGH;
		return LOW;
	} else {
		// OUTPUT
		if(r8 & bit_output) return HIGH;
		return LOW;
	}
}

static void gpio_sata_config(SB700_GPIO_MODE mode) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = SATA_ACT_GPIO_CNTRL;
	const int bit_mode = (1 << 1);
	const int bit_enabled = (1 << 3);
	u8 r8, v8;

	r8 = PCI_R8(smbus, reg);
	if(!(r8 & bit_enabled)) {
		printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO IS DISABLED\n", __func__, reg);
		return;
	}
	v8 = r8 & ~bit_mode;
	if(mode == TRISTATE) v8 |= bit_mode;
	PCI_W8(smbus, reg, v8);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %02X -> %02X\n", DBG_GPIO_CONFIG, reg, r8, v8);
}

static void gpio_sata_toggle(int enabled) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = SATA_ACT_GPIO_CNTRL;
	const int bit = (1 << 3);
	u8 r8, v8;

	r8 = PCI_R8(smbus, reg);
	v8 = r8 & ~bit;
	if(enabled) v8 |= bit;
	PCI_W8(smbus, reg, v8);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %02X -> %02X\n", DBG_GPIO_TOGGLE, reg, r8, v8);
}

static void gpio_C_config(int num, SB700_GPIO_MODE mode) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = GPIO_12_TO_4_CNTRL;
	u32 r32, v32;
	int bit_mode;
	int bit_enabled;

	r32 = PCI_R32(smbus, reg);

	if(4 <= num && num <= 9) {
		bit_mode = 1 << (num + 4);
	} else if(num == 10) {
		bit_mode = 1 << 25;
	} else if(11 <= num && num <= 12) {
		bit_enabled = 1 << (num + 19);
		if(!(r32 & bit_enabled)) {
			printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO %d IS DISABLED\n", __func__, reg, num);
			return;
		}
		bit_mode = 1 << (num + 3);
	} else {
		printk(BIOS_ERR, "%s: FATAL CODING ERROR\n", __func__);
		return;
	}
	v32 = r32 & ~bit_mode;
	if(mode == TRISTATE) v32 |= bit_mode;
	PCI_W32(smbus, reg, v32);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X -> %08X\n", DBG_GPIO_CONFIG, reg, r32, v32);
}

static SB700_GPIO_VALUE gpio_C_get(int num) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = GPIO_12_TO_4_CNTRL;
	u32 r32;
	int bit_io;
	int bit_mode;
	int bit_enabled;

	r32 = PCI_R32(smbus, reg);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X\n", DBG_GPIO_GET, reg, r32);

	if(4 <= num && num <= 9) {
		bit_mode = 1 << (num + 4);
		if(r32 & bit_mode) {
			// TRISTATE
			bit_io = 1 << (num + 12);
		} else {
			// OUTPUT
			bit_io = 1 << (num - 4);
		}
	} else if(num == 10) {
		bit_mode = 1 << 25;
		if(r32 & bit_mode) {
			// TRISTATE
			bit_io = 1 << 26;
		} else {
			// OUTPUT
			bit_io = 1 << 24;
		}
	} else if(11 <= num && num <= 12) {
		bit_enabled = 1 << (num + 19);
		if(!(r32 & bit_enabled)) {
			printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO %d IS DISABLED\n", __func__, reg, num);
			return ERROR;
		}
		bit_mode = 1 << (num + 3);
		if(r32 & bit_mode) {
			// TRISTATE
			bit_io = 1 << (num + 11);
		} else {
			// OUTPUT
			bit_io = 1 << (num - 5);
		}
	} else {
		printk(BIOS_ERR, "%s: FATAL CODING ERROR\n", __func__);
		return ERROR;
	}
	if(r32 & bit_io) return HIGH;
	return LOW;
}

static void gpio_C_set(int num, SB700_GPIO_VALUE value) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = GPIO_12_TO_4_CNTRL;
	u32 r32, v32;
	int bit_enabled;
	int bit_output;

	r32 = PCI_R32(smbus, reg);

	if(4 <= num && num <= 9) {
		bit_output = 1 << (num - 4);
	} else if(num == 10) {
		bit_output = 1 << 24;
	} else if(11 <= num && num <= 12) {
		bit_enabled = 1 << (num + 19);
		if(!(r32 & bit_enabled)) {
			printk(BIOS_ERR, "%s: SMBUS[%02X] GPIO %d IS DISABLED\n", __func__, reg, num);
			return;
		}
		bit_output = 1 << (num - 5);
	} else {
		printk(BIOS_ERR, "%s: FATAL CODING ERROR\n", __func__);
		return;
	}

	v32 = r32 & ~bit_output;
	if(value == HIGH) v32 |= bit_output;
	PCI_W32(smbus, reg, v32);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X -> %08X\n", DBG_GPIO_SET, reg, r32, v32);
}

static void gpio_C_toggle(int num, int enabled) {
	pci_devfn_t smbus = PCI_DEV(0, 0x14, 0);
	const int reg = GPIO_12_TO_4_CNTRL;
	u32 r32, v32;
	int bit;

	bit = 1 << (num + 19);

	r32 = PCI_R32(smbus, reg);
	v32 = r32 & ~bit;
	if(enabled) v32 |= bit;
	PCI_W32(smbus, reg, v32);
	printk(BIOS_DEBUG, "%s: SMBUS[%02X] %08X -> %08X\n", DBG_GPIO_TOGGLE, reg, r32, v32);
}

void sb700_gpio_config(int num, SB700_GPIO_MODE mode) {
	// Clean Input
	if(mode != OUTPUT && mode != TRISTATE) {
		printk(BIOS_ERR, "%s: INVALID GPIO MODE\n", __func__);
		return;
	}

	switch(num) {
		// Group "A"
		case 49: case 50: case 51: case 52:
			gpio_A_config(GPIO_52_TO_49_CNTRL, num - 49, mode);
			return;
		case 53: case 54: case 55: case 56:
			gpio_A_config(GPIO_56_TO_53_CNTRL, num - 53, mode);
			return;
		case 57: case 58: case 59: case 60:
			gpio_A_config(GPIO_60_TO_57_CNTRL, num - 57, mode);
			return;
		case 61: case 62: case 63: case 64:
			gpio_A_config(GPIO_64_TO_61_CNTRL, num - 61, mode);
			return;
		case 65: case 66:
			gpio_A_config(GPIO_69_68_66_65_CNTRL, num - 65, mode);
			return;
		case 68: case 69:
			gpio_A_config(GPIO_69_68_66_65_CNTRL, num - 66, mode);
			return;
		case 0: case 1: case 2: case 3:
			gpio_A_config(GPIO_3_TO_0_CNTRL, num, mode);
			return;
		// Group "B"
		case 70: case 71: case 72: case 73:
			gpio_B_config(GPIO_73_TO_70_CNTRL, num - 70, mode);
			return;
		case 13: case 14:
			gpio_B_config(GPIO_32_31_14_13_CNTRL, num - 13, mode);
			return;
		case 31: case 32:
			gpio_B_config(GPIO_32_31_14_13_CNTRL, num - 29, mode);
			return;
		case 37:
			gpio_B_config(GPIO_48_47_46_37_CNTRL, num - 37, mode);
			return;
		case 46: case 47: case 48:
			gpio_B_config(GPIO_48_47_46_37_CNTRL, num - 45, mode);
			return;
		case 33: case 34: case 35: case 36:
			gpio_B_config(PCI_INT_GPIO_CNTRL, num - 33, mode);
			return;
		// Group "C"
		case 4: case 5: case 6: case 7: case 8:
		case 9: case 10: case 11: case 12:
			gpio_C_config(num, mode);
			return;
		// IDE
		case 15: case 16: case 17: case 18:
		case 19: case 20: case 21: case 22:
		case 23: case 24: case 25: case 26:
		case 27: case 28: case 29: case 30:
			gpio_ide_config(num - 15, mode);
			return;
		// SATA
		case 67:
			gpio_sata_config(mode);
			return;
	}

	// Unhandled
	printk(BIOS_ERR, "%s: GPIO %d Unhandled\n", __func__, num);
}

SB700_GPIO_VALUE sb700_gpio_get(int num) {
	switch(num) {
		// Group "A"
		case 49: case 50: case 51: case 52:
			return gpio_A_get(GPIO_52_TO_49_CNTRL, num - 49);
		case 53: case 54: case 55: case 56:
			return gpio_A_get(GPIO_56_TO_53_CNTRL, num - 53);
		case 57: case 58: case 59: case 60:
			return gpio_A_get(GPIO_60_TO_57_CNTRL, num - 57);
		case 61: case 62: case 63: case 64:
			return gpio_A_get(GPIO_64_TO_61_CNTRL, num - 61);
		case 65: case 66:
			return gpio_A_get(GPIO_69_68_66_65_CNTRL, num - 65);
		case 68: case 69:
			return gpio_A_get(GPIO_69_68_66_65_CNTRL, num - 66);
		case 0: case 1: case 2: case 3:
			return gpio_A_get(GPIO_3_TO_0_CNTRL, num);
		// Group "B"
		case 70: case 71: case 72: case 73:
			return gpio_B_get(GPIO_73_TO_70_CNTRL, num - 70);
		case 13: case 14:
			return gpio_B_get(GPIO_32_31_14_13_CNTRL, num - 13);
		case 31: case 32:
			return gpio_B_get(GPIO_32_31_14_13_CNTRL, num - 29);
		case 37:
			return gpio_B_get(GPIO_48_47_46_37_CNTRL, num - 37);
		case 46: case 47: case 48:
			return gpio_B_get(GPIO_48_47_46_37_CNTRL, num - 45);
		case 33: case 34: case 35: case 36:
			return gpio_B_get(PCI_INT_GPIO_CNTRL, num - 33);
		// Group "C"
		case 4: case 5: case 6: case 7: case 8:
		case 9: case 10: case 11: case 12:
			return gpio_C_get(num);
		// IDE
		case 15: case 16: case 17: case 18:
		case 19: case 20: case 21: case 22:
		case 23: case 24: case 25: case 26:
		case 27: case 28: case 29: case 30:
			return gpio_ide_get(num - 15);
		// SATA
		case 67:
			return gpio_sata_get();
	}

	// Unhandled
	printk(BIOS_ERR, "%s: GPIO %d Unhandled\n", __func__, num);
	return ERROR;
}

void sb700_gpio_set(int num, SB700_GPIO_VALUE value) {
	// Clean Input
	if(value != LOW && value != HIGH) {
		printk(BIOS_ERR, "%s: INVALID GPIO VALUE\n", __func__);
		return;
	}

	switch(num) {
		// Group "A"
		case 49: case 50: case 51: case 52:
			gpio_A_set(GPIO_52_TO_49_CNTRL, num - 49, value);
			return;
		case 53: case 54: case 55: case 56:
			gpio_A_set(GPIO_56_TO_53_CNTRL, num - 53, value);
			return;
		case 57: case 58: case 59: case 60:
			gpio_A_set(GPIO_60_TO_57_CNTRL, num - 57, value);
			return;
		case 61: case 62: case 63: case 64:
			gpio_A_set(GPIO_64_TO_61_CNTRL, num - 61, value);
			return;
		case 65: case 66:
			gpio_A_set(GPIO_69_68_66_65_CNTRL, num - 65, value);
			return;
		case 68: case 69:
			gpio_A_set(GPIO_69_68_66_65_CNTRL, num - 66, value);
			return;
		case 0: case 1: case 2: case 3:
			gpio_A_set(GPIO_3_TO_0_CNTRL, num, value);
			return;
		// Group "B"
		case 70: case 71: case 72: case 73:
			gpio_B_set(GPIO_73_TO_70_CNTRL, num - 70, value);
			return;
		case 13: case 14:
			gpio_B_set(GPIO_32_31_14_13_CNTRL, num - 13, value);
			return;
		case 31: case 32:
			gpio_B_set(GPIO_32_31_14_13_CNTRL, num - 29, value);
			return;
		case 37:
			gpio_B_set(GPIO_48_47_46_37_CNTRL, num - 37, value);
			return;
		case 46: case 47: case 48:
			gpio_B_set(GPIO_48_47_46_37_CNTRL, num - 45, value);
			return;
		case 33: case 34: case 35: case 36:
			gpio_B_set(PCI_INT_GPIO_CNTRL, num - 33, value);
			return;
		// Group "C"
		case 4: case 5: case 6: case 7: case 8:
		case 9: case 10: case 11: case 12:
			gpio_C_set(num, value);
			return;
		// IDE
		case 15: case 16: case 17: case 18:
		case 19: case 20: case 21: case 22:
		case 23: case 24: case 25: case 26:
		case 27: case 28: case 29: case 30:
			gpio_ide_set(num - 15, value);
			return;
		// SATA
		case 67:
			gpio_sata_set(value);
			return;
	}

	// Unhandled
	printk(BIOS_ERR, "%s: GPIO %d Unhandled\n", __func__, num);
}

static void gpio_toggle(int num, int enabled) {
	switch(num) {
		// Group "A", no toggle needed
		case 49: case 50: case 51: case 52:
		case 53: case 54: case 55: case 56:
		case 57: case 58: case 59: case 60:
		case 61: case 62: case 63: case 64:
			return;
		// Group "B"
		case 70: case 71: case 72: case 73:
			gpio_B_toggle(GPIO_73_TO_70_CNTRL, num - 70, enabled);
			return;
		case 13: case 14:
			gpio_B_toggle(GPIO_32_31_14_13_CNTRL, num - 13, enabled);
			return;
		case 31: case 32:
			gpio_B_toggle(GPIO_32_31_14_13_CNTRL, num - 29, enabled);
			return;
		case 37:
			gpio_B_toggle(GPIO_48_47_46_37_CNTRL, num - 37, enabled);
			return;
		case 46: case 47: case 48:
			gpio_B_toggle(GPIO_48_47_46_37_CNTRL, num - 45, enabled);
			return;
		case 33: case 34: case 35: case 36:
			gpio_B_toggle(PCI_INT_GPIO_CNTRL, num - 33, enabled);
			return;
		// Group "C"
		case 11: case 12:
			gpio_C_toggle(num, enabled);
			return;
		// IDE
		case 15: case 16: case 17: case 18:
		case 19: case 20: case 21: case 22:
		case 23: case 24: case 25: case 26:
		case 27: case 28: case 29: case 30:
			gpio_ide_toggle(enabled);
			return;
		// SATA
		case 67:
			gpio_sata_toggle(enabled);
			return;
	}

	// Unhandled
	printk(BIOS_ERR, "%s: GPIO %d Unhandled\n", __func__, num);
}

void sb700_gpio_enable(int num) {
	gpio_toggle(num, 1);
}

void sb700_gpio_disable(int num) {
	gpio_toggle(num, 0);
}
