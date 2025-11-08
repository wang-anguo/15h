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

#ifndef _PLATFORM_CFG_H_
#define _PLATFORM_CFG_H_

/********** SB700 **********/

/**
 * @def SB_HPET_TIMER
 * @brief
 *  0 - Disable hpet
 *  1 - Enable  hpet
 */
#define HPET_TIMER                      1

/**
 * @def PCI_CLOCK_CTRL
 * @brief bit[0-4] used for PCI Slots Clock Control,
 *   0 - disable
 *   1 - enable
 *  PCI SLOT 0 define at BIT0
 *  PCI SLOT 1 define at BIT1
 *  PCI SLOT 2 define at BIT2
 *  PCI SLOT 3 define at BIT3
 *  PCI SLOT 4 define at BIT4
 */
#define PCI_CLOCK_CTRL                  0x1F

/**
 * @def PCI_CLOCK_CTRL_5
 * @brief bit[0] used for PCI Slot 5 Clock Control,
 *   0 - disable
 *   1 - enable
 *  PCI SLOT 5 define at BIT0
 */
#define PCI_CLOCK_CTRL_5                1

/**
 * @def   AZALIA_AUTO
 * @brief Detect Azalia controller automatically.
 *
 * @def   AZALIA_DISABLE
 * @brief Disable Azalia controller.

 * @def   AZALIA_ENABLE
 * @brief Enable Azalia controller.
 */
#define AZALIA_AUTO                     0
#define AZALIA_DISABLE                  1
#define AZALIA_ENABLE                   2

/**
 * @brief INCHIP HDA controller
 */
#define AZALIA_CONTROLLER               AZALIA_AUTO

/**
 * @def AZALIA_PIN_CONFIG
 * @brief
 *  0 - disable
 *  1 - enable
 */
#define AZALIA_PIN_CONFIG               1

/**
 * @def AZALIA_SDIN_PIN
 * @brief
 *  SDIN0 is define at BIT0 & BIT1
 *   00 - GPIO PIN
 *   01 - Reserved
 *   10 - As a Azalia SDIN pin
 *  SDIN1 is define at BIT2 & BIT3
 *  SDIN2 is define at BIT4 & BIT5
 *  SDIN3 is define at BIT6 & BIT7
 */
#define AZALIA_SDIN_PIN                 0x2A

/********** RD890 **********/

/**
 * Max number of northbridges in the system
 */
#define MAX_NB_COUNT		1

/**
 * Northbridge HT Deemphasis
 	0x00 = Disabled
	0x01 = 1.32dB (0 to 4.5" trace length)
	0x02 = 2.08dB (4.5" to 8" trace length)
	0x03 = 3.10dB (8" to 11" trace length)
	0x04 = 4.22dB (11" to 14" trace length)
	0x05 = 5.50dB (14" to 18" trace length)
	0x06 = 7.05dB (18+" trace length) */
#define RD890_TRANSMITTER_DEEMPHASIS	0x03

/**
 * Trigger a thermal shutdown event
	0 = Disabled
	1 = Enabled */
#define RD890_TEST_THERMAL_SHUTDOWN     0

#endif //_PLATFORM_CFG_H_
