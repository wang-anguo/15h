/*
 * This file is part of the coreboot project.
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

#ifndef _W83795_H_
#define _W83795_H_

#define W83795_DEV                      	0x2F /* Host I2c Addr (strap to addr1 addr0 1 1, 0x5E) */

/* --- Bank 0 Registers --- */
/* Bank Select */
#define W83795_REG_BANK_SELECT			0x00
/* Configuration */
#define W83795_REG_CONFIG			0x01
#define W83795_REG_CONFIG_START			(1 << 0)
#define W83795_REG_CONFIG_CONFIG48		(1 << 2)
#define W83795_REG_CONFIG_INIT			(1 << 7)
/* Voltage monitoring control registers */
#define W83795_REG_VOLT_CTRL1			0x02
#define W83795_REG_VOLT_CTRL1_VSEN1		(1 << 0)
#define W83795_REG_VOLT_CTRL1_VSEN2		(1 << 1)
#define W83795_REG_VOLT_CTRL1_VSEN3		(1 << 2)
#define W83795_REG_VOLT_CTRL1_VSEN4		(1 << 3)
#define W83795_REG_VOLT_CTRL1_VSEN5		(1 << 4)
#define W83795_REG_VOLT_CTRL1_VSEN6		(1 << 5)
#define W83795_REG_VOLT_CTRL1_VSEN7		(1 << 6)
#define W83795_REG_VOLT_CTRL1_VSEN8		(1 << 7)
#define W83795_REG_VOLT_CTRL2			0x03
#define W83795_REG_VOLT_CTRL2_VSEN9		(1 << 0)
#define W83795_REG_VOLT_CTRL2_VSEN10		(1 << 1)
#define W83795_REG_VOLT_CTRL2_VSEN11		(1 << 2)
#define W83795_REG_VOLT_CTRL2_VTT		(1 << 3)
#define W83795_REG_VOLT_CTRL2_3VDD		(1 << 4)
#define W83795_REG_VOLT_CTRL2_3VSB		(1 << 5)
#define W83795_REG_VOLT_CTRL2_VBAT		(1 << 6)
/* Temperature monitoring control registers */
#define W83795_REG_TEMP_CTRL1			0x04
#define W83795_REG_TEMP_CTRL1_DTS		(1 << 5)
#define W83795_REG_TEMP_CTRL1_TR6_MASK		(3 << 2)
#define W83795_REG_TEMP_CTRL1_TR6_IS_VSEN13	(2 << 2)
#define W83795_REG_TEMP_CTRL1_TR6_IS_TR6	(3 << 2)
#define W83795_REG_TEMP_CTRL1_TR5_MASK		(3 << 0)
#define W83795_REG_TEMP_CTRL1_TR5_IS_VSEN12	(2 << 0)
#define W83795_REG_TEMP_CTRL1_TR5_IS_TR5	(3 << 0)
#define W83795_REG_TEMP_CTRL2			0x05
#define W83795_REG_TEMP_CTRL2_TR4_MASK		(3 << 6)
#define W83795_REG_TEMP_CTRL2_TR4_IS_TD4	(1 << 6)
#define W83795_REG_TEMP_CTRL2_TR4_IS_VDSEN17	(2 << 6)
#define W83795_REG_TEMP_CTRL2_TR4_IS_TR4	(3 << 6)
#define W83795_REG_TEMP_CTRL2_TR3_MASK		(3 << 4)
#define W83795_REG_TEMP_CTRL2_TR3_IS_TD3	(1 << 4)
#define W83795_REG_TEMP_CTRL2_TR3_IS_VDSEN16	(2 << 4)
#define W83795_REG_TEMP_CTRL2_TR3_IS_TR3	(3 << 4)
#define W83795_REG_TEMP_CTRL2_TR2_MASK		(3 << 2)
#define W83795_REG_TEMP_CTRL2_TR2_IS_TD2	(1 << 2)
#define W83795_REG_TEMP_CTRL2_TR2_IS_VDSEN15	(2 << 2)
#define W83795_REG_TEMP_CTRL2_TR2_IS_TR2	(3 << 2)
#define W83795_REG_TEMP_CTRL2_TR1_MASK		(3 << 0)
#define W83795_REG_TEMP_CTRL2_TR1_IS_TD1	(1 << 0)
#define W83795_REG_TEMP_CTRL2_TR1_IS_VDSEN14	(2 << 0)
#define W83795_REG_TEMP_CTRL2_TR1_IS_TR1	(3 << 0)
/* FANIN monitoring control registers */
#define W83795_REG_FANIN_CTRL1			0x06
#define W83795_REG_FANIN_CTRL1_FANIN8		(1 << 7)
#define W83795_REG_FANIN_CTRL1_FANIN7		(1 << 6)
#define W83795_REG_FANIN_CTRL1_FANIN6		(1 << 5)
#define W83795_REG_FANIN_CTRL1_FANIN5		(1 << 4)
#define W83795_REG_FANIN_CTRL1_FANIN4		(1 << 3)
#define W83795_REG_FANIN_CTRL1_FANIN3		(1 << 2)
#define W83795_REG_FANIN_CTRL1_FANIN2		(1 << 1)
#define W83795_REG_FANIN_CTRL1_FANIN1		(1 << 0)
#define W83795_REG_FANIN_CTRL2			0x07
#define W83795_REG_FANIN_CTRL2_FANIN14		(1 << 5)
#define W83795_REG_FANIN_CTRL2_FANIN13		(1 << 4)
#define W83795_REG_FANIN_CTRL2_FANIN12		(1 << 3)
#define W83795_REG_FANIN_CTRL2_FANIN11		(1 << 2)
#define W83795_REG_FANIN_CTRL2_FANIN10		(1 << 1)
#define W83795_REG_FANIN_CTRL2_FANIN9		(1 << 0)
/* Monitored channel readout high byte */
/* There is register overlap */
#define W83795_REG_READ_VSEN1			0x10
#define W83795_REG_READ_VSEN2			0x11
#define W83795_REG_READ_VSEN3			0x12
#define W83795_REG_READ_VSEN4			0x13
#define W83795_REG_READ_VSEN5			0x14
#define W83795_REG_READ_VSEN6			0x15
#define W83795_REG_READ_VSEN7			0x16
#define W83795_REG_READ_VSEN8			0x17
#define W83795_REG_READ_VSEN9			0x18
#define W83795_REG_READ_VSEN10			0x19
#define W83795_REG_READ_VSEN11			0x1A
#define W83795_REG_READ_VTT			0x1B
#define W83795_REG_READ_3VDD			0x1C
#define W83795_REG_READ_3VSB			0x1D
#define W83795_REG_READ_VBAT			0x1E
#define W83795_REG_READ_TR5			0x1F
#define W83795_REG_READ_TR6			0x20
#define W83795_REG_READ_TR1			0x21
#define W83795_REG_READ_TR2			0x22
#define W83795_REG_READ_TR3			0x23
#define W83795_REG_READ_TR4			0x24
#define W83795_REG_READ_VSEN12			0x1F
#define W83795_REG_READ_VSEN13			0x20
#define W83795_REG_READ_VDSEN14			0x21
#define W83795_REG_READ_VDSEN15			0x22
#define W83795_REG_READ_VDSEN16			0x23
#define W83795_REG_READ_VDSEN17			0x24
#define W83795_REG_READ_TD1			0x21
#define W83795_REG_READ_TD2			0x22
#define W83795_REG_READ_TD3			0x23
#define W83795_REG_READ_TD4			0x24
#define W83795_REG_READ_DTS1			0x26
#define W83795_REG_READ_DTS2			0x27
#define W83795_REG_READ_DTS3			0x28
#define W83795_REG_READ_DTS4			0x29
#define W83795_REG_READ_DTS5			0x2A
#define W83795_REG_READ_DTS6			0x2B
#define W83795_REG_READ_DTS7			0x2C
#define W83795_REG_READ_DTS8			0x2D
#define W83795_REG_READ_FANIN1			0x2E
#define W83795_REG_READ_FANIN2			0x2F
#define W83795_REG_READ_FANIN3			0x30
#define W83795_REG_READ_FANIN4			0x31
#define W83795_REG_READ_FANIN5			0x32
#define W83795_REG_READ_FANIN6			0x33
#define W83795_REG_READ_FANIN7			0x34
#define W83795_REG_READ_FANIN8			0x35
#define W83795_REG_READ_FANIN9			0x36
#define W83795_REG_READ_FANIN10			0x37
#define W83795_REG_READ_FANIN11			0x38
#define W83795_REG_READ_FANIN12			0x39
#define W83795_REG_READ_FANIN13			0x3A
#define W83795_REG_READ_FANIN14			0x3B
/* Monitored channel readout low byte */
#define W83795_REG_VR_LSB			0x3C
/* Voltage high/low limits */
#define W83795_REG_VSEN1_HL			0x70
#define W83795_REG_VSEN1_LL			0x71
#define W83795_REG_VSEN2_HL			0x72
#define W83795_REG_VSEN2_LL			0x73
#define W83795_REG_VSEN3_HL			0x74
#define W83795_REG_VSEN3_LL			0x75
#define W83795_REG_VSEN4_HL			0x76
#define W83795_REG_VSEN4_LL			0x77
#define W83795_REG_VSEN5_HL			0x78
#define W83795_REG_VSEN5_LL			0x79
#define W83795_REG_VSEN6_HL			0x7A
#define W83795_REG_VSEN6_LL			0x7B
#define W83795_REG_VSEN7_HL			0x7C
#define W83795_REG_VSEN7_LL			0x7D
#define W83795_REG_VSEN8_HL			0x7E
#define W83795_REG_VSEN8_LL			0x7F
#define W83795_REG_VSEN9_HL			0x80
#define W83795_REG_VSEN9_LL			0x81
#define W83795_REG_VSEN10_HL			0x82
#define W83795_REG_VSEN10_LL			0x83
#define W83795_REG_VSEN11_HL			0x84
#define W83795_REG_VSEN11_LL			0x85
#define W83795_REG_VTT_HL			0x86
#define W83795_REG_VTT_LL			0x87
#define W83795_REG_3VDD_HL			0x88
#define W83795_REG_3VDD_LL			0x89
#define W83795_REG_3VSB_HL			0x8A
#define W83795_REG_3VSB_LL			0x8B
#define W83795_REG_VBAT_HL			0x8C
#define W83795_REG_VBAT_LL			0x8D
#define W83795_REG_VOLT1_HL_LSB			0x8E
#define W83795_REG_VOLT1_LL_LSB			0x8F
#define W83795_REG_VOLT2_HL_LSB			0x90
#define W83795_REG_VOLT2_LL_LSB			0x91
#define W83795_REG_VOLT3_HL_LSB			0x92
#define W83795_REG_VOLT3_LL_LSB			0x93
#define W83795_REG_VOLT4_HL_LSB			0x94
#define W83795_REG_VOLT4_LL_LSB			0x95
#define W83795_REG_VDSEN14_HL			0x96
#define W83795_REG_VDSEN14_LL			0x97
#define W83795_REG_VDSEN14_HL_LSB		0x98
#define W83795_REG_VDSEN14_LL_LSB		0x99
#define W83795_REG_VDSEN15_HL			0x9A
#define W83795_REG_VDSEN15_LL			0x9B
#define W83795_REG_VDSEN15_HL_LSB		0x9C
#define W83795_REG_VDSEN15_LL_LSB		0x9D
#define W83795_REG_VDSEN16_HL			0x9E
#define W83795_REG_VDSEN16_LL			0x9F
#define W83795_REG_VDSEN16_HL_LSB		0xA0
#define W83795_REG_VDSEN16_LL_LSB		0xA1
#define W83795_REG_VDSEN17_HL			0xA2
#define W83795_REG_VDSEN17_LL			0xA3
#define W83795_REG_VDSEN17_HL_LSB		0xA4
#define W83795_REG_VDSEN17_LL_LSB		0xA5
#define W83795_REG_VSEN12_HL			0xA6
#define W83795_REG_VSEN12_LL			0xA7
#define W83795_REG_VSEN12_HL_LSB		0xA8
#define W83795_REG_VSEN12_LL_LSB		0xA9
#define W83795_REG_VSEN13_HL			0xAA
#define W83795_REG_VSEN13_LL			0xAB
#define W83795_REG_VSEN13_HL_LSB		0xAC
#define W83795_REG_VSEN13_LL_LSB		0xAD
/* Temperature Limits */
#define W83795_REG_TEMP_CRIT(n)			(0x96 + (n * 4))
#define W83795_REG_TEMP_CRIT_HYSTER(n)		(0x97 + (n * 4))
#define W83795_REG_TEMP_WARN(n)			(0x98 + (n * 4))
#define W83795_REG_TEMP_WARN_HYSTER(n)		(0x99 + (n * 4))
#define W83795_REG_DTS_CRIT			0xB2
#define W83795_REG_DTS_CRIT_HYSTER		0xB3
#define W83795_REG_DTS_WARN			0xB4
#define W83795_REG_DTS_WARN_HYSTER		0xB5

/* --- Bank 2 Registers --- */
/* Fan Control Mode Selection */
#define W83795_REG_FCMS1			0x201
#define W83795_REG_FCMS2			0x208
/* Temperature Source Selection */
#define W83795_REG_T12TSS			0x209
#define W83795_REG_T34TSS			0x20A
#define W83795_REG_T56TSS			0x20B
#define T1SS_TR1_TD1				0x0
#define T1SS_DTS1				0x1
#define T1SS_DTS5				0x2
#define T1SS_TR5				0x3
#define T2SS_TR2_TD2				0x0
#define T2SS_DTS2				0x1
#define T2SS_DTS6				0x2
#define T2SS_TR6				0x3
#define T3SS_TR3_TD3				0x0
#define T3SS_DTS3				0x1
#define T3SS_DTS7				0x2
#define T3SS_TR5				0x3
#define T4SS_TR4_TD4				0x0
#define T4SS_DTS4				0x1
#define T4SS_DTS8				0x2
#define T4SS_TR6				0x3
#define T5SS_TR5				0x0
#define T5SS_TR1_TD1				0x1
#define T5SS_TR3_TD3				0x2
#define T6SS_TR6				0x0
#define T6SS_TR2_TD2				0x1
#define T6SS_TR4_TD4				0x2
/* SmartFan Output Step Up Time */
#define W83795_REG_SFOSUT			0x20D
/* SmartFan Output Step Down Time */
#define W83795_REG_SFOSDT			0x20E
/* Temperature to Fan Mapping Relationships Register */
#define W83795_REG_TFMR(n)			(0x202 + (n))
#define W83795_REG_TFMR_FAN1			(1 << 0)
#define W83795_REG_TFMR_FAN2			(1 << 1)
#define W83795_REG_TFMR_FAN3			(1 << 2)
#define W83795_REG_TFMR_FAN4			(1 << 3)
#define W83795_REG_TFMR_FAN5			(1 << 4)
#define W83795_REG_TFMR_FAN6			(1 << 5)
#define W83795_REG_TFMR_FAN7			(1 << 6)
#define W83795_REG_TFMR_FAN8			(1 << 7)
/* Target Temperature of Temperature Inputs */
#define W83795_REG_TTTI(n)			(0x260 + (n))
/* Critical Temperature to Full Speed all fan */
#define W83795_REG_CTFS(n)			(0x268 + (n))
/* Fan Output Value */
#define W83795_REG_FOV(n)			(0x210 + n)
/* Default Fan Speed at Power-on */
#define W83795_REG_DFSP				0x20C
/* Fan Output Nonstop Value */
#define W83795_REG_FONV(n)			(0x228 + (n))
/* Smart Fan IV Temperature and DC/PWM Registers */
#define W83795_REG_SFIV_T1_TEMPS(n)		(0x280 + (n))
#define W83795_REG_SFIV_T2_TEMPS(n)		(0x290 + (n))
#define W83795_REG_SFIV_T3_TEMPS(n)		(0x2A0 + (n))
#define W83795_REG_SFIV_T4_TEMPS(n)		(0x2B0 + (n))
#define W83795_REG_SFIV_T5_TEMPS(n)		(0x2C0 + (n))
#define W83795_REG_SFIV_T6_TEMPS(n)		(0x2D0 + (n))
#define W83795_REG_SFIV_T1_PWM(n)		(0x288 + (n))
#define W83795_REG_SFIV_T2_PWM(n)		(0x298 + (n))
#define W83795_REG_SFIV_T3_PWM(n)		(0x2A8 + (n))
#define W83795_REG_SFIV_T4_PWM(n)		(0x2B8 + (n))
#define W83795_REG_SFIV_T5_PWM(n)		(0x2C8 + (n))
#define W83795_REG_SFIV_T6_PWM(n)		(0x2D8 + (n))
/* Hysteresis of Temperature */
#define W83795_REG_HT(n)			(0x270 + (n))

/* --- Bank 3 Registers --- */
/* Digital Temperature Sensor Configuration */
#define W83795_REG_DTSC				0x301
#define W83795_REG_DTSC_DIS			(1 << 0)
#define DTS_SRC_INTEL_PECI			0x00
#define DTS_SRC_AMD_SBTSI			0x01
/* Digital Temperature Sensor Enable */
#define W83795_REG_DTSE				0x302
#define W83795_REG_DTSE_D1E			(1 << 0)
#define W83795_REG_DTSE_D2E			(1 << 1)
#define W83795_REG_DTSE_D3E			(1 << 2)
#define W83795_REG_DTSE_D4E			(1 << 3)
#define W83795_REG_DTSE_D5E			(1 << 4)
#define W83795_REG_DTSE_D6E			(1 << 5)
#define W83795_REG_DTSE_D7E			(1 << 6)
#define W83795_REG_DTSE_D8E			(1 << 7)

typedef enum w83795_fan_mode {
	MANUAL_MODE = 0,		///< control manually
	SPEED_CRUISE_MODE = 1,		///< Fan Speed Cruise mode keeps the fan speed in a specified range
	THERMAL_CRUISE_MODE = 2,	///< Thermal Cruise mode is an algorithm to control the fan speed to keep the temperature source around the TTTI
	SMART_FAN_MODE = 3,		///< Smart Fan mode offers 6 slopes to control the fan speed
} w83795_fan_mode_t;

#endif
