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

#include <stdint.h>
#include <arch/cpu.h>
#include <console/console.h>
#include <device/device.h>
#include "w83795.h"

#if CONFIG(SOUTHBRIDGE_AMD_CIMX_SB700)
// AGESA codebase
#include <southbridge/amd/cimx/sb700/smbus.h>
#else
// Raptor codebase
#include <device/smbus.h>
#endif
#include "chip.h"

// Sensor Boolean Helpers
#define DTS_ENABLED (config->temp_ctrl1 & W83795_REG_TEMP_CTRL1_DTS)
#define DTS1_ENABLED (config->dtse & W83795_REG_DTSE_D1E)
#define DTS2_ENABLED (config->dtse & W83795_REG_DTSE_D2E)
#define DTS3_ENABLED (config->dtse & W83795_REG_DTSE_D3E)
#define DTS4_ENABLED (config->dtse & W83795_REG_DTSE_D4E)
#define DTS5_ENABLED (config->dtse & W83795_REG_DTSE_D5E)
#define DTS6_ENABLED (config->dtse & W83795_REG_DTSE_D6E)
#define DTS7_ENABLED (config->dtse & W83795_REG_DTSE_D7E)
#define DTS8_ENABLED (config->dtse & W83795_REG_DTSE_D8E)
#define VSEN1_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN1)
#define VSEN2_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN2)
#define VSEN3_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN3)
#define VSEN4_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN4)
#define VSEN5_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN5)
#define VSEN6_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN6)
#define VSEN7_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN7)
#define VSEN8_ENABLED (config->volt_ctrl1 & W83795_REG_VOLT_CTRL1_VSEN8)
#define VSEN9_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_VSEN9)
#define VSEN10_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_VSEN10)
#define VSEN11_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_VSEN11)
#define VSEN13_ENABLED ((config->temp_ctrl1 & W83795_REG_TEMP_CTRL1_TR6_MASK) == W83795_REG_TEMP_CTRL1_TR6_IS_VSEN13)
#define VSEN12_ENABLED ((config->temp_ctrl1 & W83795_REG_TEMP_CTRL1_TR5_MASK) == W83795_REG_TEMP_CTRL1_TR5_IS_VSEN12)
#define VDSEN17_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR4_MASK) == W83795_REG_TEMP_CTRL2_TR4_IS_VDSEN17)
#define VDSEN16_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR3_MASK) == W83795_REG_TEMP_CTRL2_TR3_IS_VDSEN16)
#define VDSEN15_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR2_MASK) == W83795_REG_TEMP_CTRL2_TR2_IS_VDSEN15)
#define VDSEN14_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR1_MASK) == W83795_REG_TEMP_CTRL2_TR1_IS_VDSEN14)
#define VTT_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_VTT)
#define _3VDD_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_3VDD)
#define _3VSB_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_3VSB)
#define VBAT_ENABLED (config->volt_ctrl2 & W83795_REG_VOLT_CTRL2_VBAT)
#define TR1_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR1_MASK) == W83795_REG_TEMP_CTRL2_TR1_IS_TR1)
#define TR2_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR2_MASK) == W83795_REG_TEMP_CTRL2_TR2_IS_TR2)
#define TR3_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR3_MASK) == W83795_REG_TEMP_CTRL2_TR3_IS_TR3)
#define TR4_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR4_MASK) == W83795_REG_TEMP_CTRL2_TR4_IS_TR4)
#define TR5_ENABLED ((config->temp_ctrl1 & W83795_REG_TEMP_CTRL1_TR5_MASK) == W83795_REG_TEMP_CTRL1_TR5_IS_TR5)
#define TR6_ENABLED ((config->temp_ctrl1 & W83795_REG_TEMP_CTRL1_TR6_MASK) == W83795_REG_TEMP_CTRL1_TR6_IS_TR6)
#define TD1_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR1_MASK) == W83795_REG_TEMP_CTRL2_TR1_IS_TD1)
#define TD2_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR2_MASK) == W83795_REG_TEMP_CTRL2_TR2_IS_TD2)
#define TD3_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR3_MASK) == W83795_REG_TEMP_CTRL2_TR3_IS_TD3)
#define TD4_ENABLED ((config->temp_ctrl2 & W83795_REG_TEMP_CTRL2_TR4_MASK) == W83795_REG_TEMP_CTRL2_TR4_IS_TD4)
#define FANIN1_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN1)
#define FANIN2_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN2)
#define FANIN3_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN3)
#define FANIN4_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN4)
#define FANIN5_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN5)
#define FANIN6_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN6)
#define FANIN7_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN7)
#define FANIN8_ENABLED (config->fanin_ctrl1 & W83795_REG_FANIN_CTRL1_FANIN8)
#define FANIN9_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN9)
#define FANIN10_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN10)
#define FANIN11_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN11)
#define FANIN12_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN12)
#define FANIN13_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN13)
#define FANIN14_ENABLED (config->fanin_ctrl2 & W83795_REG_FANIN_CTRL2_FANIN14)

static int smbus_r8(struct device *dev, uint8_t reg) {
#if CONFIG(SOUTHBRIDGE_AMD_CIMX_SB700)
	struct drivers_i2c_w83795_config *config = dev->chip_info;
	if(!config->smbus_aux) return do_smbus_read_byte(SMBUS_IO_BASE, W83795_DEV, reg);
	else return do_smbus_read_byte(SMBUS_AUX_IO_BASE, W83795_DEV, reg);
#else
	return smbus_read_byte(dev, reg);
#endif
}

static int smbus_w8(struct device *dev, uint8_t reg, uint8_t value) {
#if CONFIG(SOUTHBRIDGE_AMD_CIMX_SB700)
	struct drivers_i2c_w83795_config *config = dev->chip_info;
	if(!config->smbus_aux) return do_smbus_write_byte(SMBUS_IO_BASE, W83795_DEV, reg, value);
	else return do_smbus_write_byte(SMBUS_AUX_IO_BASE, W83795_DEV, reg, value);
#else
	return smbus_write_byte(dev, reg, value);
#endif
}

static int w83795_set_bank(struct device *dev, uint8_t bank)
{
	return smbus_w8(dev, W83795_REG_BANK_SELECT, bank);
}

static uint8_t w83795_read(struct device *dev, uint16_t reg)
{
	int ret;

	ret = w83795_set_bank(dev, reg >> 8);
	if (ret < 0) {
		printk(BIOS_DEBUG, "read failed to set bank %x\n", reg >> 8);
		return -1;
	}

	ret = smbus_r8(dev, reg & 0xff);
	return ret & 0xFF;
}

static uint8_t w83795_write(struct device *dev, uint16_t reg, uint8_t value)
{
	int err;

	err = w83795_set_bank(dev, reg >> 8);
	if (err < 0) {
		printk(BIOS_DEBUG, "write failed to set bank %x\n", reg >> 8);
		return -1;
	}

	err = smbus_w8(dev, reg & 0xff, value);
	return err;
}


static uint8_t fan_pct_to_cfg_val(uint8_t percent)
{
	uint16_t cfg = (((unsigned int)percent * 255) / 100);
	if (cfg > 0xff)
		cfg = 0xff;
	return cfg;
}

static uint32_t get_voltin_mv(struct device *dev, uint16_t reg) {
	uint32_t hi, lo, val;

	hi = w83795_read(dev, reg);
	lo = w83795_read(dev, W83795_REG_VR_LSB);
	val = (hi << 2) + (lo >> 6);
	
	if(W83795_REG_READ_3VDD <= reg && reg <= W83795_REG_READ_VBAT)
		return val * 6; // 3VDD, 3VSB, VBAT
	else
		return val * 2; // VSEN1 - VSEN13, VDSEN14 - VDSEN17, VTT
}

static uint32_t get_fanin_rpm(struct device *dev, uint16_t reg) {
	uint32_t hi, lo, val;

	hi = w83795_read(dev, reg);
	lo = w83795_read(dev, W83795_REG_VR_LSB);
	val = (hi << 4) + (lo >> 4);
	if(val == 0) return 0;

	return 1350000 / val;
}

static void w83795_init(struct device *dev, u8 dts_src)
{
	struct drivers_i2c_w83795_config *config = dev->chip_info;
	uint8_t i;
	uint8_t val;
	const char *label;
	uint8_t fan_mode;

	fan_mode = config->fan_mode;

#if CONFIG(SMBUS_HAS_AUX_CHANNELS)
	/* Switch to aux channel */
	uint8_t smbus_aux_channel_prev = smbus_get_current_channel();
	smbus_switch_to_channel(config->smbus_aux);
	printk(BIOS_DEBUG, "Set SMBUS controller to channel %d\n", config->smbus_aux);
#endif

	/* Find H/W Monitor */
	if (smbus_r8(dev, 0x00) < 0) {
#if CONFIG(SMBUS_HAS_AUX_CHANNELS)
		/* Restore SMBUS channel setting */
		smbus_switch_to_channel(smbus_aux_channel_prev);
		printk(BIOS_DEBUG, "Set SMBUS controller to channel %d\n", smbus_aux_channel_prev);
#endif
		printk(BIOS_ERR, "W83795G/ADG Nuvoton H/W Monitor not found\n");
		return;
	}

	/* Identify H/W Monitor */
	val = w83795_read(dev, W83795_REG_CONFIG);
	if ((val & W83795_REG_CONFIG_CONFIG48) == 0)
		printk(BIOS_INFO, "Found 64 pin W83795G Nuvoton H/W Monitor\n");
	else
		printk(BIOS_INFO, "Found 48 pin W83795ADG Nuvoton H/W Monitor\n");
	/* Reset H/W Monitor */
	val |= W83795_REG_CONFIG_INIT;
	w83795_write(dev, W83795_REG_CONFIG, val);

	/* --- Configure Senor Inputs --- */
	/* Digital Temperature Sensor Configuration */
	/* DTSC and DTSE need to be set early */
	val = w83795_read(dev, W83795_REG_DTSC) & ~W83795_REG_DTSC_DIS;
	val |= dts_src & W83795_REG_DTSC_DIS;
	w83795_write(dev, W83795_REG_DTSC, val);
	/* Digital Temperature Sensor Enable */
	w83795_write(dev, W83795_REG_DTSE, config->dtse);

	/* Voltage Monitoring Control Register */
	w83795_write(dev, W83795_REG_VOLT_CTRL1, config->volt_ctrl1);
	w83795_write(dev, W83795_REG_VOLT_CTRL2, config->volt_ctrl2);

	/* Temperature Monitoring Control Register */
	/* MUST be programmed AFTER DTSC and DTSE */
	w83795_write(dev, W83795_REG_TEMP_CTRL1, config->temp_ctrl1);
	w83795_write(dev, W83795_REG_TEMP_CTRL2, config->temp_ctrl2);

	/* FANIN Monitoring Control Register */
	w83795_write(dev, W83795_REG_FANIN_CTRL1, config->fanin_ctrl1);
	w83795_write(dev, W83795_REG_FANIN_CTRL2, config->fanin_ctrl2);

	/* List Enabled Sensors */
	if(DTS_ENABLED) printk(BIOS_DEBUG, "DTS Enabled\n");
	if(DTS1_ENABLED) printk(BIOS_DEBUG, "DTS1 Enabled\n");
	if(DTS2_ENABLED) printk(BIOS_DEBUG, "DTS2 Enabled\n");
	if(DTS3_ENABLED) printk(BIOS_DEBUG, "DTS3 Enabled\n");
	if(DTS4_ENABLED) printk(BIOS_DEBUG, "DTS4 Enabled\n");
	if(DTS5_ENABLED) printk(BIOS_DEBUG, "DTS5 Enabled\n");
	if(DTS6_ENABLED) printk(BIOS_DEBUG, "DTS6 Enabled\n");
	if(DTS7_ENABLED) printk(BIOS_DEBUG, "DTS7 Enabled\n");
	if(DTS8_ENABLED) printk(BIOS_DEBUG, "DTS8 Enabled\n");
	if(TR1_ENABLED) printk(BIOS_DEBUG, "TR1 Enabled\n");
	if(TR2_ENABLED) printk(BIOS_DEBUG, "TR2 Enabled\n");
	if(TR3_ENABLED) printk(BIOS_DEBUG, "TR3 Enabled\n");
	if(TR4_ENABLED) printk(BIOS_DEBUG, "TR4 Enabled\n");
	if(TR5_ENABLED) printk(BIOS_DEBUG, "TR5 Enabled\n");
	if(TR6_ENABLED) printk(BIOS_DEBUG, "TR6 Enabled\n");
	if(TD1_ENABLED) printk(BIOS_DEBUG, "TD1 Enabled\n");
	if(TD2_ENABLED) printk(BIOS_DEBUG, "TD2 Enabled\n");
	if(TD3_ENABLED) printk(BIOS_DEBUG, "TD3 Enabled\n");
	if(TD4_ENABLED) printk(BIOS_DEBUG, "TD4 Enabled\n");
	if(VSEN1_ENABLED) printk(BIOS_DEBUG, "VSEN1 Enabled\n");
	if(VSEN2_ENABLED) printk(BIOS_DEBUG, "VSEN2 Enabled\n");
	if(VSEN3_ENABLED) printk(BIOS_DEBUG, "VSEN3 Enabled\n");
	if(VSEN4_ENABLED) printk(BIOS_DEBUG, "VSEN4 Enabled\n");
	if(VSEN5_ENABLED) printk(BIOS_DEBUG, "VSEN5 Enabled\n");
	if(VSEN6_ENABLED) printk(BIOS_DEBUG, "VSEN6 Enabled\n");
	if(VSEN7_ENABLED) printk(BIOS_DEBUG, "VSEN7 Enabled\n");
	if(VSEN8_ENABLED) printk(BIOS_DEBUG, "VSEN8 Enabled\n");
	if(VSEN9_ENABLED) printk(BIOS_DEBUG, "VSEN9 Enabled\n");
	if(VSEN10_ENABLED) printk(BIOS_DEBUG, "VSEN10 Enabled\n");
	if(VSEN11_ENABLED) printk(BIOS_DEBUG, "VSEN11 Enabled\n");
	if(VSEN12_ENABLED) printk(BIOS_DEBUG, "VSEN12 Enabled\n");
	if(VSEN13_ENABLED) printk(BIOS_DEBUG, "VSEN13 Enabled\n");
	if(VDSEN14_ENABLED) printk(BIOS_DEBUG, "VDSEN14 Enabled\n");
	if(VDSEN15_ENABLED) printk(BIOS_DEBUG, "VDSEN15 Enabled\n");
	if(VDSEN16_ENABLED) printk(BIOS_DEBUG, "VDSEN16 Enabled\n");
	if(VDSEN17_ENABLED) printk(BIOS_DEBUG, "VDSEN17 Enabled\n");
	if(VTT_ENABLED) printk(BIOS_DEBUG, "VTT Enabled\n");
	if(_3VDD_ENABLED) printk(BIOS_DEBUG, "3VDD Enabled\n");
	if(_3VSB_ENABLED) printk(BIOS_DEBUG, "3VSB Enabled\n");
	if(VBAT_ENABLED) printk(BIOS_DEBUG, "VBAT Enabled\n");
	if(FANIN1_ENABLED) printk(BIOS_DEBUG, "FANIN1 Enabled\n");
	if(FANIN2_ENABLED) printk(BIOS_DEBUG, "FANIN2 Enabled\n");
	if(FANIN3_ENABLED) printk(BIOS_DEBUG, "FANIN3 Enabled\n");
	if(FANIN4_ENABLED) printk(BIOS_DEBUG, "FANIN4 Enabled\n");
	if(FANIN5_ENABLED) printk(BIOS_DEBUG, "FANIN5 Enabled\n");
	if(FANIN6_ENABLED) printk(BIOS_DEBUG, "FANIN6 Enabled\n");
	if(FANIN7_ENABLED) printk(BIOS_DEBUG, "FANIN7 Enabled\n");
	if(FANIN8_ENABLED) printk(BIOS_DEBUG, "FANIN8 Enabled\n");
	if(FANIN9_ENABLED) printk(BIOS_DEBUG, "FANIN9 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN9 Disabled: PVID0 and GPIO1 Available\n");
	if(FANIN10_ENABLED) printk(BIOS_DEBUG, "FANIN10 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN10 Disabled: PVID1 and GPIO2 Available\n");
	if(FANIN11_ENABLED) printk(BIOS_DEBUG, "FANIN11 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN11 Disabled: PECI_REQ#, PVID2, and GPIO3 Available\n");
	if(FANIN12_ENABLED) printk(BIOS_DEBUG, "FANIN12 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN12 Disabled: PVID3 and GPIO4 Available\n");
	if(FANIN13_ENABLED) printk(BIOS_DEBUG, "FANIN13 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN13 Disabled: SYSRSTIN# and PROCHOT3# Available\n");
	if(FANIN14_ENABLED) printk(BIOS_DEBUG, "FANIN14 Enabled\n");
	else printk(BIOS_DEBUG, "FANIN14 Disabled: SMI# and PROCHOT4# Available\n");

	if(fan_mode == SPEED_CRUISE_MODE) {
		printk(BIOS_ERR, "ERROR: W83795G/ADG Speed Cruise Mode is not implemented, switching to Manual Mode\n");
		fan_mode = MANUAL_MODE;
		// w83795_write(dev, W83795_REG_FCMS1, 0xFF);
	}

	/* Default Fan Speed at Power-on */
	w83795_write(dev, W83795_REG_DFSP, fan_pct_to_cfg_val(config->poweron_speed));

	if(fan_mode != MANUAL_MODE) {
		/* Set limits before connecting sensors/temperatures/fans */
		/* Critical Temperature to Full Speed all fan */
		w83795_write(dev, W83795_REG_CTFS(0), config->temp1_critical_temperature);
		w83795_write(dev, W83795_REG_CTFS(1), config->temp2_critical_temperature);
		w83795_write(dev, W83795_REG_CTFS(2), config->temp3_critical_temperature);
		w83795_write(dev, W83795_REG_CTFS(3), config->temp4_critical_temperature);
		w83795_write(dev, W83795_REG_CTFS(4), config->temp5_critical_temperature);
		w83795_write(dev, W83795_REG_CTFS(5), config->temp6_critical_temperature);
		printk(BIOS_DEBUG, "Temperature1 Critical: %d C\n", config->temp1_critical_temperature);
		printk(BIOS_DEBUG, "Temperature2 Critical: %d C\n", config->temp2_critical_temperature);
		printk(BIOS_DEBUG, "Temperature3 Critical: %d C\n", config->temp3_critical_temperature);
		printk(BIOS_DEBUG, "Temperature4 Critical: %d C\n", config->temp4_critical_temperature);
		printk(BIOS_DEBUG, "Temperature5 Critical: %d C\n", config->temp5_critical_temperature);
		printk(BIOS_DEBUG, "Temperature6 Critical: %d C\n", config->temp6_critical_temperature);

		/* Hystersis of Temperature */
		/* Temperature +/- values for target/critical temperatures */
		w83795_write(dev, W83795_REG_HT(0), ((config->temp1_critical_hysteresis & 0xF) << 4) | (config->temp1_target_hysteresis & 0xF));
		w83795_write(dev, W83795_REG_HT(1), ((config->temp2_critical_hysteresis & 0xF) << 4) | (config->temp2_target_hysteresis & 0xF));
		w83795_write(dev, W83795_REG_HT(2), ((config->temp3_critical_hysteresis & 0xF) << 4) | (config->temp3_target_hysteresis & 0xF));
		w83795_write(dev, W83795_REG_HT(3), ((config->temp4_critical_hysteresis & 0xF) << 4) | (config->temp4_target_hysteresis & 0xF));
		w83795_write(dev, W83795_REG_HT(4), ((config->temp5_critical_hysteresis & 0xF) << 4) | (config->temp5_target_hysteresis & 0xF));
		w83795_write(dev, W83795_REG_HT(5), ((config->temp6_critical_hysteresis & 0xF) << 4) | (config->temp6_target_hysteresis & 0xF));
		printk(BIOS_DEBUG, "Temperature1 HT Target: +/- %d C\n", config->temp1_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature2 HT Target: +/- %d C\n", config->temp2_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature3 HT Target: +/- %d C\n", config->temp3_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature4 HT Target: +/- %d C\n", config->temp4_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature5 HT Target: +/- %d C\n", config->temp5_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature6 HT Target: +/- %d C\n", config->temp6_target_hysteresis);
		printk(BIOS_DEBUG, "Temperature1 HT Critical: +/- %d C\n", config->temp1_critical_hysteresis);
		printk(BIOS_DEBUG, "Temperature2 HT Critical: +/- %d C\n", config->temp2_critical_hysteresis);
		printk(BIOS_DEBUG, "Temperature3 HT Critical: +/- %d C\n", config->temp3_critical_hysteresis);
		printk(BIOS_DEBUG, "Temperature4 HT Critical: +/- %d C\n", config->temp4_critical_hysteresis);
		printk(BIOS_DEBUG, "Temperature5 HT Critical: +/- %d C\n", config->temp5_critical_hysteresis);
		printk(BIOS_DEBUG, "Temperature6 HT Critical: +/- %d C\n", config->temp6_critical_hysteresis);

		/* Fan Output Nonstop Value */
		/* Sets the minimum fan speed */
		w83795_write(dev, W83795_REG_FONV(0), fan_pct_to_cfg_val(config->fan1_nonstop));
		w83795_write(dev, W83795_REG_FONV(1), fan_pct_to_cfg_val(config->fan2_nonstop));
		w83795_write(dev, W83795_REG_FONV(2), fan_pct_to_cfg_val(config->fan3_nonstop));
		w83795_write(dev, W83795_REG_FONV(3), fan_pct_to_cfg_val(config->fan4_nonstop));
		w83795_write(dev, W83795_REG_FONV(4), fan_pct_to_cfg_val(config->fan5_nonstop));
		w83795_write(dev, W83795_REG_FONV(5), fan_pct_to_cfg_val(config->fan6_nonstop));
		w83795_write(dev, W83795_REG_FONV(6), fan_pct_to_cfg_val(config->fan7_nonstop));
		w83795_write(dev, W83795_REG_FONV(7), fan_pct_to_cfg_val(config->fan8_nonstop));
		printk(BIOS_DEBUG, "Minimum Fan1 Percent: %d%%\n", config->fan1_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan2 Percent: %d%%\n", config->fan2_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan3 Percent: %d%%\n", config->fan3_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan4 Percent: %d%%\n", config->fan4_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan5 Percent: %d%%\n", config->fan5_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan6 Percent: %d%%\n", config->fan6_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan7 Percent: %d%%\n", config->fan7_nonstop);
		printk(BIOS_DEBUG, "Minimum Fan8 Percent: %d%%\n", config->fan8_nonstop);

		/* SmartFan Output Step Up/Down Time */
		w83795_write(dev, W83795_REG_SFOSUT, config->step_up_time);
		w83795_write(dev, W83795_REG_SFOSDT, config->step_down_time);

		/* --- Configure Fan Temperature Inputs and PWM Outputs--- */
		/* Temperature Source Selection Register */
		w83795_write(dev, W83795_REG_T12TSS, ((config->temp2_source_select & 0x0f) << 4) | (config->temp1_source_select & 0x0f));
		w83795_write(dev, W83795_REG_T34TSS, ((config->temp4_source_select & 0x0f) << 4) | (config->temp3_source_select & 0x0f));
		w83795_write(dev, W83795_REG_T56TSS, ((config->temp6_source_select & 0x0f) << 4) | (config->temp5_source_select & 0x0f));
		/* Temperature to Fan Mapping Relationships Register */
		w83795_write(dev, W83795_REG_TFMR(0), config->temp1_fan_select);
		w83795_write(dev, W83795_REG_TFMR(1), config->temp2_fan_select);
		w83795_write(dev, W83795_REG_TFMR(2), config->temp3_fan_select);
		w83795_write(dev, W83795_REG_TFMR(3), config->temp4_fan_select);
		w83795_write(dev, W83795_REG_TFMR(4), config->temp5_fan_select);
		w83795_write(dev, W83795_REG_TFMR(5), config->temp6_fan_select);
		/* Show Fan Mappings */
		label = "DISABLED";
		if(config->temp1_source_select == T1SS_TR1_TD1) label = TR1_ENABLED ? "TR1":"TD1";
		if(config->temp1_source_select == T1SS_DTS1) label = "DTS1";
		if(config->temp1_source_select == T1SS_DTS5) label = "DTS5";
		if(config->temp1_source_select == T1SS_TR5) label = "TR5";
		printk(BIOS_DEBUG, "Temperature1: %s ->", label);
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp1_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
		label = "DISABLED";
		if(config->temp2_source_select == T2SS_TR2_TD2) label = TR2_ENABLED ? "TR2":"TD2";
		if(config->temp2_source_select == T2SS_DTS2) label = "DTS2";
		if(config->temp2_source_select == T2SS_DTS6) label = "DTS6";
		if(config->temp2_source_select == T2SS_TR6) label = "TR6";
		printk(BIOS_DEBUG, "Temperature2: %s ->", label);
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp2_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
		label = "DISABLED";
		if(config->temp3_source_select == T3SS_TR3_TD3) label = TR3_ENABLED ? "TR3":"TD3";
		if(config->temp3_source_select == T3SS_DTS3) label = "DTS3";
		if(config->temp3_source_select == T3SS_DTS7) label = "DTS7";
		if(config->temp3_source_select == T3SS_TR5) label = "TR5";
		printk(BIOS_DEBUG, "Temperature3: %s ->", label);
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp3_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
		label = "DISABLED";
		if(config->temp4_source_select == T4SS_TR4_TD4) label = TR4_ENABLED ? "TR4":"TD4";
		if(config->temp4_source_select == T4SS_DTS4) label = "DTS4";
		if(config->temp4_source_select == T4SS_DTS8) label = "DTS8";
		if(config->temp4_source_select == T4SS_TR6) label = "TR6";
		printk(BIOS_DEBUG, "Temperature4: %s ->", label);
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp4_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
		label = "DISABLED";
		if(config->temp5_source_select == T5SS_TR5) label = "TR5";
		if(config->temp5_source_select == T5SS_TR1_TD1) label = TR1_ENABLED ? "TR1":"TD1";
		if(config->temp5_source_select == T5SS_TR3_TD3) label = TR3_ENABLED ? "TR3":"TD3";
		printk(BIOS_DEBUG, "Temperature5: %s ->", label);
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp5_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
		label = "DISABLED";
		if(config->temp6_source_select == T6SS_TR6) label = "TR6";
		if(config->temp6_source_select == T6SS_TR2_TD2) label = TR2_ENABLED ? "TR2":"TD2";
		if(config->temp6_source_select == T6SS_TR4_TD4) label = TR4_ENABLED ? "TR4":"TD4";
		printk(BIOS_DEBUG, "Temperature6: %s ->", label);
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN1) printk(BIOS_DEBUG, " FAN1");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN2) printk(BIOS_DEBUG, " FAN2");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN3) printk(BIOS_DEBUG, " FAN3");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN4) printk(BIOS_DEBUG, " FAN4");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN5) printk(BIOS_DEBUG, " FAN5");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN6) printk(BIOS_DEBUG, " FAN6");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN7) printk(BIOS_DEBUG, " FAN7");
		if(config->temp6_fan_select & W83795_REG_TFMR_FAN8) printk(BIOS_DEBUG, " FAN8");
		printk(BIOS_DEBUG, "\n");
	}

	/* Temperature Limits for Alarms */
	w83795_write(dev, W83795_REG_TEMP_CRIT(0), config->tr1_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT(1), config->tr2_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT(2), config->tr3_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT(3), config->tr4_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT(4), config->tr5_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT(5), config->tr6_critical_temperature);
	w83795_write(dev, W83795_REG_DTS_CRIT, config->dts_critical_temperature);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(0), config->tr1_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(1), config->tr2_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(2), config->tr3_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(3), config->tr4_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(4), config->tr5_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_CRIT_HYSTER(5), config->tr6_critical_hysteresis);
	w83795_write(dev, W83795_REG_DTS_CRIT_HYSTER, config->dts_critical_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN(0), config->tr1_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN(1), config->tr2_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN(2), config->tr3_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN(3), config->tr4_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN(4), config->tr5_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN(5), config->tr6_warning_temperature);
	w83795_write(dev, W83795_REG_DTS_WARN, config->dts_warning_temperature);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(0), config->tr1_warning_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(1), config->tr2_warning_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(2), config->tr3_warning_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(3), config->tr4_warning_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(4), config->tr5_warning_hysteresis);
	w83795_write(dev, W83795_REG_TEMP_WARN_HYSTER(5), config->tr6_warning_hysteresis);
	w83795_write(dev, W83795_REG_DTS_WARN_HYSTER, config->dts_warning_hysteresis);

	/* Voltage high/low limits */
	// 10-bit value... higheset 8 bits are written to main registers, lowest 2 bits written to LSB registers
	/* VSEN1-13, VDSEN14-17, VTT: V = bitvalue * 0.002 */
	// (mV * 0.001) = bitvalue * 0.002
	// bitvalue = mV / 2
	w83795_write(dev, W83795_REG_VSEN1_HL, ((config->vsen1_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN2_HL, ((config->vsen2_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN3_HL, ((config->vsen3_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN4_HL, ((config->vsen4_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN5_HL, ((config->vsen5_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN6_HL, ((config->vsen6_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN7_HL, ((config->vsen7_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN8_HL, ((config->vsen8_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN9_HL, ((config->vsen9_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN10_HL, ((config->vsen10_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN11_HL, ((config->vsen11_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN12_HL, ((config->vsen12_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN13_HL, ((config->vsen13_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN14_HL, ((config->vdsen14_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN15_HL, ((config->vdsen15_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN16_HL, ((config->vdsen16_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN17_HL, ((config->vdsen17_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VTT_HL, ((config->vtt_high_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN1_LL, ((config->vsen1_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN2_LL, ((config->vsen2_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN3_LL, ((config->vsen3_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN4_LL, ((config->vsen4_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN5_LL, ((config->vsen5_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN6_LL, ((config->vsen6_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN7_LL, ((config->vsen7_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN8_LL, ((config->vsen8_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN9_LL, ((config->vsen9_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN10_LL, ((config->vsen10_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN11_LL, ((config->vsen11_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN12_LL, ((config->vsen12_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VSEN13_LL, ((config->vsen13_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN14_LL, ((config->vdsen14_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN15_LL, ((config->vdsen15_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN16_LL, ((config->vdsen16_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VDSEN17_LL, ((config->vdsen17_low_limit_mv / 2) >> 2));
	w83795_write(dev, W83795_REG_VTT_LL, ((config->vtt_low_limit_mv / 2) >> 2));
	/* 3VSB, 3VDD, VBAT: V = bitvalue * 0.006 */
	// (mV * 0.001) = bitvalue * 0.006
	// bitvalue = mV / 6 
	w83795_write(dev, W83795_REG_3VSB_HL, ((config->vsb_high_limit_mv / 6) >> 2));
	w83795_write(dev, W83795_REG_3VDD_HL, ((config->vdd_high_limit_mv / 6) >> 2));
	w83795_write(dev, W83795_REG_VBAT_HL, ((config->vbat_high_limit_mv / 6) >> 2));
	w83795_write(dev, W83795_REG_3VSB_LL, ((config->vsb_low_limit_mv / 6) >> 2));
	w83795_write(dev, W83795_REG_3VDD_LL, ((config->vdd_low_limit_mv / 6) >> 2));
	w83795_write(dev, W83795_REG_VBAT_LL, ((config->vbat_low_limit_mv / 6) >> 2));
	/* Lower bit values */
	val = 0x00;
	val |= ((config->vsen4_high_limit_mv / 2) & 0x3) << 6;
	val |= ((config->vsen3_high_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen2_high_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen1_high_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT1_HL_LSB, val);
	val = 0x00;
	val |= ((config->vsen8_high_limit_mv / 2) & 0x3) << 6;
	val |= ((config->vsen7_high_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen6_high_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen5_high_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT2_HL_LSB, val);
	val = 0x00;
	val |= ((config->vsen11_high_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen10_high_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen9_high_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT3_HL_LSB, val);
	val = 0x00;
	val |= ((config->vbat_high_limit_mv / 6) & 0x3) << 6;
	val |= ((config->vsb_high_limit_mv / 6) & 0x3) << 4;
	val |= ((config->vdd_high_limit_mv / 6) & 0x3) << 2;
	val |= ((config->vtt_high_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT4_HL_LSB, val);
	w83795_write(dev, W83795_REG_VDSEN14_HL_LSB, (((config->vdsen14_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN15_HL_LSB, (((config->vdsen15_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN16_HL_LSB, (((config->vdsen16_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN17_HL_LSB, (((config->vdsen17_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VSEN12_HL_LSB, (((config->vsen12_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VSEN13_HL_LSB, (((config->vsen13_low_limit_mv / 2) & 0x3) << 6));
	val = 0x00;
	val |= ((config->vsen4_low_limit_mv / 2) & 0x3) << 6;
	val |= ((config->vsen3_low_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen2_low_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen1_low_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT1_LL_LSB, val);
	val = 0x00;
	val |= ((config->vsen8_low_limit_mv / 2) & 0x3) << 6;
	val |= ((config->vsen7_low_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen6_low_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen5_low_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT2_LL_LSB, val);
	val = 0x00;
	val |= ((config->vsen11_low_limit_mv / 2) & 0x3) << 4;
	val |= ((config->vsen10_low_limit_mv / 2) & 0x3) << 2;
	val |= ((config->vsen9_low_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT3_LL_LSB, val);
	val = 0x00;
	val |= ((config->vbat_low_limit_mv / 6) & 0x3) << 6;
	val |= ((config->vsb_low_limit_mv / 6) & 0x3) << 4;
	val |= ((config->vdd_low_limit_mv / 6) & 0x3) << 2;
	val |= ((config->vtt_low_limit_mv / 2) & 0x3) << 0;
	w83795_write(dev, W83795_REG_VOLT4_LL_LSB, val);
	w83795_write(dev, W83795_REG_VDSEN14_LL_LSB, (((config->vdsen14_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN15_LL_LSB, (((config->vdsen15_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN16_LL_LSB, (((config->vdsen16_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VDSEN17_LL_LSB, (((config->vdsen17_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VSEN12_LL_LSB, (((config->vsen12_low_limit_mv / 2) & 0x3) << 6));
	w83795_write(dev, W83795_REG_VSEN13_LL_LSB, (((config->vsen13_low_limit_mv / 2) & 0x3) << 6));

	if(fan_mode == MANUAL_MODE) {
		/* Fan Output Value */
		w83795_write(dev, W83795_REG_FOV(0), fan_pct_to_cfg_val(config->fan1_duty));
		w83795_write(dev, W83795_REG_FOV(1), fan_pct_to_cfg_val(config->fan2_duty));
		w83795_write(dev, W83795_REG_FOV(2), fan_pct_to_cfg_val(config->fan3_duty));
		w83795_write(dev, W83795_REG_FOV(3), fan_pct_to_cfg_val(config->fan4_duty));
		w83795_write(dev, W83795_REG_FOV(4), fan_pct_to_cfg_val(config->fan5_duty));
		w83795_write(dev, W83795_REG_FOV(5), fan_pct_to_cfg_val(config->fan6_duty));
		w83795_write(dev, W83795_REG_FOV(6), fan_pct_to_cfg_val(config->fan7_duty));
		w83795_write(dev, W83795_REG_FOV(7), fan_pct_to_cfg_val(config->fan8_duty));
		printk(BIOS_INFO, "W83795G/ADG work in Manual Mode\n");
		printk(BIOS_DEBUG, "Fan 1 Speed: %d%%\n", config->fan1_duty);
		printk(BIOS_DEBUG, "Fan 2 Speed: %d%%\n", config->fan2_duty);
		printk(BIOS_DEBUG, "Fan 3 Speed: %d%%\n", config->fan3_duty);
		printk(BIOS_DEBUG, "Fan 4 Speed: %d%%\n", config->fan4_duty);
		printk(BIOS_DEBUG, "Fan 5 Speed: %d%%\n", config->fan5_duty);
		printk(BIOS_DEBUG, "Fan 6 Speed: %d%%\n", config->fan6_duty);
		printk(BIOS_DEBUG, "Fan 7 Speed: %d%%\n", config->fan7_duty);
		printk(BIOS_DEBUG, "Fan 8 Speed: %d%%\n", config->fan8_duty);
	}

	/* Target Temperature of Temperature Inputs (Thermal Cruise Mode Only) */
	if(fan_mode == THERMAL_CRUISE_MODE) {
		w83795_write(dev, W83795_REG_FCMS1, 0x00);
		w83795_write(dev, W83795_REG_FCMS2, 0x00);
		printk(BIOS_INFO, "W83795G/ADG work in Thermal Cruise Mode\n");
		w83795_write(dev, W83795_REG_TTTI(0), config->temp1_target_temperature);
		w83795_write(dev, W83795_REG_TTTI(1), config->temp2_target_temperature);
		w83795_write(dev, W83795_REG_TTTI(2), config->temp3_target_temperature);
		w83795_write(dev, W83795_REG_TTTI(3), config->temp4_target_temperature);
		w83795_write(dev, W83795_REG_TTTI(4), config->temp5_target_temperature);
		w83795_write(dev, W83795_REG_TTTI(5), config->temp6_target_temperature);
		printk(BIOS_DEBUG, "Temperature1 Target: %d C\n", config->temp1_target_temperature);
		printk(BIOS_DEBUG, "Temperature2 Target: %d C\n", config->temp2_target_temperature);
		printk(BIOS_DEBUG, "Temperature3 Target: %d C\n", config->temp3_target_temperature);
		printk(BIOS_DEBUG, "Temperature4 Target: %d C\n", config->temp4_target_temperature);
		printk(BIOS_DEBUG, "Temperature5 Target: %d C\n", config->temp5_target_temperature);
		printk(BIOS_DEBUG, "Temperature6 Target: %d C\n", config->temp6_target_temperature);
	}

	if(fan_mode == SMART_FAN_MODE) {
		uint8_t fcms2 = 0x00;
		if(config->temp1_fan_select) fcms2 |= (1 << 0);
		if(config->temp2_fan_select) fcms2 |= (1 << 1);
		if(config->temp3_fan_select) fcms2 |= (1 << 2);
		if(config->temp4_fan_select) fcms2 |= (1 << 3);
		if(config->temp5_fan_select) fcms2 |= (1 << 4);
		if(config->temp6_fan_select) fcms2 |= (1 << 5);
		w83795_write(dev, W83795_REG_FCMS1, 0x00);
		w83795_write(dev, W83795_REG_FCMS2, fcms2);
		printk(BIOS_INFO, "W83795G/ADG work in Smart Fan Mode\n");
		/* Set the Relative Register-at SMART FAN IV Control Mode Table */
		for(i = 0; i < W83795_SFIV_POINTS; i++) {
			w83795_write(dev, W83795_REG_SFIV_T1_TEMPS(i), config->sfiv_table[0].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T2_TEMPS(i), config->sfiv_table[1].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T3_TEMPS(i), config->sfiv_table[2].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T4_TEMPS(i), config->sfiv_table[3].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T5_TEMPS(i), config->sfiv_table[4].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T6_TEMPS(i), config->sfiv_table[5].temperatures[i]);
			w83795_write(dev, W83795_REG_SFIV_T1_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[0].fan_percents[i]));
			w83795_write(dev, W83795_REG_SFIV_T2_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[1].fan_percents[i]));
			w83795_write(dev, W83795_REG_SFIV_T3_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[2].fan_percents[i]));
			w83795_write(dev, W83795_REG_SFIV_T4_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[3].fan_percents[i]));
			w83795_write(dev, W83795_REG_SFIV_T5_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[4].fan_percents[i]));
			w83795_write(dev, W83795_REG_SFIV_T6_PWM(i), fan_pct_to_cfg_val(config->sfiv_table[5].fan_percents[i]));
		}
	}

	printk(BIOS_INFO, "--- Sensor Readings ---\n");
	if(DTS1_ENABLED) printk(BIOS_INFO, "DTS1: %d C\n", w83795_read(dev, W83795_REG_READ_DTS1));
	if(DTS2_ENABLED) printk(BIOS_INFO, "DTS2: %d C\n", w83795_read(dev, W83795_REG_READ_DTS2));
	if(DTS3_ENABLED) printk(BIOS_INFO, "DTS3: %d C\n", w83795_read(dev, W83795_REG_READ_DTS3));
	if(DTS4_ENABLED) printk(BIOS_INFO, "DTS4: %d C\n", w83795_read(dev, W83795_REG_READ_DTS4));
	if(DTS5_ENABLED) printk(BIOS_INFO, "DTS5: %d C\n", w83795_read(dev, W83795_REG_READ_DTS5));
	if(DTS6_ENABLED) printk(BIOS_INFO, "DTS6: %d C\n", w83795_read(dev, W83795_REG_READ_DTS6));
	if(DTS7_ENABLED) printk(BIOS_INFO, "DTS7: %d C\n", w83795_read(dev, W83795_REG_READ_DTS7));
	if(DTS8_ENABLED) printk(BIOS_INFO, "DTS8: %d C\n", w83795_read(dev, W83795_REG_READ_DTS8));
	if(TR1_ENABLED) printk(BIOS_INFO, "TR1: %d C\n", w83795_read(dev, W83795_REG_READ_TR1));
	if(TR2_ENABLED) printk(BIOS_INFO, "TR2: %d C\n", w83795_read(dev, W83795_REG_READ_TR2));
	if(TR3_ENABLED) printk(BIOS_INFO, "TR3: %d C\n", w83795_read(dev, W83795_REG_READ_TR3));
	if(TR4_ENABLED) printk(BIOS_INFO, "TR4: %d C\n", w83795_read(dev, W83795_REG_READ_TR4));
	if(TR5_ENABLED) printk(BIOS_INFO, "TR5: %d C\n", w83795_read(dev, W83795_REG_READ_TR5));
	if(TR6_ENABLED) printk(BIOS_INFO, "TR6: %d C\n", w83795_read(dev, W83795_REG_READ_TR6));
	if(TD1_ENABLED) printk(BIOS_INFO, "TD1: %d C\n", w83795_read(dev, W83795_REG_READ_TD1));
	if(TD2_ENABLED) printk(BIOS_INFO, "TD2: %d C\n", w83795_read(dev, W83795_REG_READ_TD2));
	if(TD3_ENABLED) printk(BIOS_INFO, "TD3: %d C\n", w83795_read(dev, W83795_REG_READ_TD3));
	if(TD4_ENABLED) printk(BIOS_INFO, "TD4: %d C\n", w83795_read(dev, W83795_REG_READ_TD4));
	if(VSEN1_ENABLED) printk(BIOS_INFO, "VSEN1: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN1));
	if(VSEN2_ENABLED) printk(BIOS_INFO, "VSEN2: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN2));
	if(VSEN3_ENABLED) printk(BIOS_INFO, "VSEN3: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN3));
	if(VSEN4_ENABLED) printk(BIOS_INFO, "VSEN4: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN4));
	if(VSEN5_ENABLED) printk(BIOS_INFO, "VSEN5: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN5));
	if(VSEN6_ENABLED) printk(BIOS_INFO, "VSEN6: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN6));
	if(VSEN7_ENABLED) printk(BIOS_INFO, "VSEN7: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN7));
	if(VSEN8_ENABLED) printk(BIOS_INFO, "VSEN8: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN8));
	if(VSEN9_ENABLED) printk(BIOS_INFO, "VSEN9: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN9));
	if(VSEN10_ENABLED) printk(BIOS_INFO, "VSEN10: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN10));
	if(VSEN11_ENABLED) printk(BIOS_INFO, "VSEN11: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN11));
	if(VSEN12_ENABLED) printk(BIOS_INFO, "VSEN12: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN12));
	if(VSEN13_ENABLED) printk(BIOS_INFO, "VSEN13: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VSEN13));
	if(VDSEN14_ENABLED) printk(BIOS_INFO, "VDSEN14: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VDSEN14));
	if(VDSEN15_ENABLED) printk(BIOS_INFO, "VDSEN15: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VDSEN15));
	if(VDSEN16_ENABLED) printk(BIOS_INFO, "VDSEN16: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VDSEN16));
	if(VDSEN17_ENABLED) printk(BIOS_INFO, "VDSEN17: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VDSEN17));
	if(VTT_ENABLED) printk(BIOS_INFO, "VTT: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VTT));
	if(_3VDD_ENABLED) printk(BIOS_INFO, "3VDD: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_3VDD));
	if(_3VSB_ENABLED) printk(BIOS_INFO, "3VSB: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_3VSB));
	if(VBAT_ENABLED) printk(BIOS_INFO, "VBAT: %d mV\n", get_voltin_mv(dev, W83795_REG_READ_VBAT));
	if(FANIN1_ENABLED) printk(BIOS_INFO, "FANIN1: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN1));
	if(FANIN2_ENABLED) printk(BIOS_INFO, "FANIN2: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN2));
	if(FANIN3_ENABLED) printk(BIOS_INFO, "FANIN3: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN3));
	if(FANIN4_ENABLED) printk(BIOS_INFO, "FANIN4: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN4));
	if(FANIN5_ENABLED) printk(BIOS_INFO, "FANIN5: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN5));
	if(FANIN6_ENABLED) printk(BIOS_INFO, "FANIN6: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN6));
	if(FANIN7_ENABLED) printk(BIOS_INFO, "FANIN7: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN7));
	if(FANIN8_ENABLED) printk(BIOS_INFO, "FANIN8: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN8));
	if(FANIN9_ENABLED) printk(BIOS_INFO, "FANIN9: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN9));
	if(FANIN10_ENABLED) printk(BIOS_INFO, "FANIN10: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN10));
	if(FANIN11_ENABLED) printk(BIOS_INFO, "FANIN11: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN11));
	if(FANIN12_ENABLED) printk(BIOS_INFO, "FANIN12: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN12));
	if(FANIN13_ENABLED) printk(BIOS_INFO, "FANIN13: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN13));
	if(FANIN14_ENABLED) printk(BIOS_INFO, "FANIN14: %d rpm\n", get_fanin_rpm(dev, W83795_REG_READ_FANIN14));

	/* Start monitoring / fan control */
	val = w83795_read(dev, W83795_REG_CONFIG);
	val |= W83795_REG_CONFIG_START;
	w83795_write(dev, W83795_REG_CONFIG, val);

#if CONFIG(SMBUS_HAS_AUX_CHANNELS)
	/* Restore SMBUS channel setting */
	smbus_switch_to_channel(smbus_aux_channel_prev);
	printk(BIOS_DEBUG, "Set SMBUS controller to channel %d\n", smbus_aux_channel_prev);
#endif
}

static void w83795_hwm_init(struct device *dev)
{
	struct device *cpu;
	struct cpu_info *info;

	info = cpu_info();
	cpu = info->cpu;
	if (!cpu)
		die("CPU: missing CPU device structure");

	if (cpu->vendor == X86_VENDOR_AMD)
		w83795_init(dev, DTS_SRC_AMD_SBTSI);
	else if (cpu->vendor == X86_VENDOR_INTEL)
		w83795_init(dev, DTS_SRC_INTEL_PECI);
	else
		printk(BIOS_ERR, "Neither AMD nor INTEL CPU detected\n");
}

static struct device_operations w83795_operations = {
	.read_resources = DEVICE_NOOP,
	.set_resources = DEVICE_NOOP,
	.enable_resources = DEVICE_NOOP,
	.init = w83795_hwm_init,
};

static void enable_dev(struct device *dev)
{
	dev->ops = &w83795_operations;
}

struct chip_operations drivers_i2c_w83795_ops = {
	CHIP_NAME("Nuvoton W83795G/ADG Hardware Monitor")
	.enable_dev = enable_dev,
};
