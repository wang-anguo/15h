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

#include <lib.h>
#include <reset.h>
#include <stdint.h>
#include <arch/io.h>
#include <arch/cpu.h>
#include <console/console.h>
#include <arch/stages.h>
#include <cpu/x86/bist.h>
#include <cpu/x86/lapic.h>
#include <cpu/amd/car.h>
#include <northbridge/amd/agesa/agesawrapper.h>
#include <northbridge/amd/agesa/agesa_helper.h>
#include <northbridge/amd/agesa/family15/reset_test.h>
#include <southbridge/amd/cimx/sb700/gpio.h>
#include <nb_cimx.h>
#include <sb_cimx.h>
#include <cbmem.h>
#include <superio/winbond/common/winbond.h>
#include <superio/winbond/w83667hg-a/w83667hg-a.h>

#include "gpio.h"

#define SERIAL_0_DEV PNP_DEV(0x2e, W83667HG_A_SP1)
#define SERIAL_1_DEV PNP_DEV(0x2e, W83667HG_A_SP2)

static void setup_gpio(void) {
	printk(BIOS_DEBUG, "Setting SP5100 GPIO Defaults\n");

	// Set GPIO Enable/Disable and Tristate/Output
	sb700_gpio_config(0, TRISTATE);
	sb700_gpio_config(1, TRISTATE);
	sb700_gpio_config(2, TRISTATE);
	sb700_gpio_config(3, TRISTATE);
	sb700_gpio_config(4, TRISTATE);
	sb700_gpio_config(5, TRISTATE);
	sb700_gpio_config(6, TRISTATE);
	sb700_gpio_config(7, TRISTATE);
	sb700_gpio_config(8, TRISTATE);
	sb700_gpio_config(9, TRISTATE);
	sb700_gpio_config(10, TRISTATE);
	sb700_gpio_disable(11);
	sb700_gpio_disable(12);
	sb700_gpio_disable(13);
	sb700_gpio_disable(14);
	sb700_gpio_enable(15); // Enables 15-30
	sb700_gpio_config(15, TRISTATE);
	sb700_gpio_config(16, TRISTATE);
	sb700_gpio_config(17, TRISTATE);
	sb700_gpio_config(18, TRISTATE);
	sb700_gpio_config(SMBUS_GPIO_PCIE5_ABSENT, TRISTATE);
	sb700_gpio_config(20, TRISTATE);
	sb700_gpio_config(21, TRISTATE);
	sb700_gpio_config(22, TRISTATE);
	sb700_gpio_config(23, TRISTATE);
	sb700_gpio_config(24, TRISTATE);
	sb700_gpio_config(25, TRISTATE);
	sb700_gpio_config(26, TRISTATE);
	sb700_gpio_config(27, TRISTATE);
	sb700_gpio_config(28, TRISTATE);
	sb700_gpio_config(29, TRISTATE);
	sb700_gpio_config(30, TRISTATE);
	sb700_gpio_disable(31);
	sb700_gpio_disable(32);
	sb700_gpio_disable(33);
	sb700_gpio_disable(34);
	sb700_gpio_disable(35);
	sb700_gpio_disable(36);
	sb700_gpio_disable(37);
	sb700_gpio_disable(46);
	sb700_gpio_disable(47);
	sb700_gpio_disable(48);
	sb700_gpio_config(SMBUS_GPIO_SPD_MUX_ENABLE, OUTPUT);
	sb700_gpio_config(53, TRISTATE);
	sb700_gpio_config(54, TRISTATE);
	sb700_gpio_config(56, TRISTATE);
	sb700_gpio_config(57, OUTPUT);
	sb700_gpio_config(58, OUTPUT);
	sb700_gpio_config(SMBUS_GPIO_SPD_MUX_BIT0, OUTPUT);
	sb700_gpio_config(SMBUS_GPIO_SPD_MUX_BIT1, OUTPUT);
	sb700_gpio_config(61, TRISTATE);
	sb700_gpio_config(62, TRISTATE);
	sb700_gpio_config(63, OUTPUT);
	sb700_gpio_config(65, TRISTATE);
	sb700_gpio_config(66, TRISTATE);
	sb700_gpio_disable(67);
	sb700_gpio_disable(70);
	sb700_gpio_disable(71);
	sb700_gpio_disable(72);
	sb700_gpio_disable(73);

	// Set GPIO Output Values
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_ENABLE, HIGH);
	sb700_gpio_set(57, HIGH);
	sb700_gpio_set(58, HIGH);
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT0, HIGH);
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT1, LOW);
	sb700_gpio_set(63, LOW);
}

void cache_as_ram_main(unsigned long bist, unsigned long cpu_init_detectedx)
{
	u32 val;

	/* Must come first to enable PCI MMCONF. */
	amd_initmmio();

	post_code(0x31);

	/* Halt if there was a built in self test failure */
	post_code(0x33);
	report_bist_failure(bist);

	/* Setup early serial */
	winbond_set_pinmux(SERIAL_1_DEV, W83667HG_SPI_PINMUX_OFFSET, W83667HG_SPI_PINMUX_GPIO4_SERIAL_B_MASK, W83667HG_SPI_PINMUX_SERIAL_B);
	if(CONFIG_UART_FOR_CONSOLE == 0) winbond_enable_serial(SERIAL_0_DEV, CONFIG_TTYS0_BASE);
	if(CONFIG_UART_FOR_CONSOLE == 1) winbond_enable_serial(SERIAL_1_DEV, CONFIG_TTYS0_BASE);
	post_code(0x34);

	/* Start coreboot console */
	post_code(0x35);
	console_init();

	val = cpuid_eax(1);
	printk(BIOS_DEBUG, "BSP Family_Model: %08x\n", val);
	printk(BIOS_DEBUG, "cpu_init_detectedx = %08lx\n", cpu_init_detectedx);

	post_code(0x37);
	agesawrapper_amdinitreset();

	if (!cpu_init_detectedx && boot_cpu()) {
		post_code(0x38);
		/*
		 * SR5650/5670/5690 RD890 chipset, read pci config space hang at POR,
		 * Disable all Pcie Bridges to work around It.
		 */
		sr56x0_rd890_disable_pcie_bridge();
		post_code(0x39);
		nb_Poweron_Init();
		post_code(0x3A);
		sb_Poweron_Init();
	}
	post_code(0x3B);
	agesawrapper_amdinitearly();

	post_code(0x3C);

	nb_Ht_Init();
	post_code(0x3D);
	/* Reset for HT, FIDVID, PLL and ucode patch(errata) changes to take affect. */
	if (!warm_reset_detect(0)) {
		printk(BIOS_INFO, "...WARM RESET...\n\n\n");
		distinguish_cpu_resets(0);
		board_reset();
		die("After soft_reset - shouldn't see this message!!!\n");
	}

	setup_gpio();

	post_code(0x40);
	agesawrapper_amdinitpost();

	cbmem_initialize_empty();

	post_code(0x41);
	agesawrapper_amdinitenv();
	post_code(0x42);

	// Disable SPD Mux
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT0, HIGH);
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_BIT1, LOW);
	sb700_gpio_set(SMBUS_GPIO_SPD_MUX_ENABLE, LOW);

	post_code(0x50);
	printk(BIOS_DEBUG, "Disabling cache as RAM ");
	disable_cache_as_ram();
	printk(BIOS_DEBUG, "done\n");

	post_code(0x51);
	copy_and_run();

	/* We will not return,  Should never see this message and post code. */
	printk(BIOS_DEBUG, "should not be here -\n");
	post_code(0x54);
}
