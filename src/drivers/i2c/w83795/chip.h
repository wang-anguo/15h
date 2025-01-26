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

#define W83795_SFIV_CURVES 6
#define W83795_SFIV_POINTS 7

struct w83795_sfiv_table {
	uint8_t temperatures[W83795_SFIV_POINTS];
	uint8_t fan_percents[W83795_SFIV_POINTS];
};

struct drivers_i2c_w83795_config {
	/* Digital Sensors */
	uint8_t dtse;
	/* Voltage Sensors */
	uint8_t volt_ctrl1;
	uint8_t volt_ctrl2;
	/* Temperature Sensors */
	uint8_t temp_ctrl1;
	uint8_t temp_ctrl2;
	/* Fan Sensors */
	uint8_t fanin_ctrl1;
	uint8_t fanin_ctrl2;

	/* Fan Temperature Source Selection */
	uint8_t temp1_source_select;
	uint8_t temp2_source_select;
	uint8_t temp3_source_select;
	uint8_t temp4_source_select;
	uint8_t temp5_source_select;
	uint8_t temp6_source_select;

	/* Temperature Limits for Alarms*/
	int8_t tr1_critical_temperature;	/* °C */
	int8_t tr1_critical_hysteresis;		/* °C */
	int8_t tr1_warning_temperature;		/* °C */
	int8_t tr1_warning_hysteresis;		/* °C */
	int8_t tr2_critical_temperature;	/* °C */
	int8_t tr2_critical_hysteresis;		/* °C */
	int8_t tr2_warning_temperature;		/* °C */
	int8_t tr2_warning_hysteresis;		/* °C */
	int8_t tr3_critical_temperature;	/* °C */
	int8_t tr3_critical_hysteresis;		/* °C */
	int8_t tr3_warning_temperature;		/* °C */
	int8_t tr3_warning_hysteresis;		/* °C */
	int8_t tr4_critical_temperature;	/* °C */
	int8_t tr4_critical_hysteresis;		/* °C */
	int8_t tr4_warning_temperature;		/* °C */
	int8_t tr4_warning_hysteresis;		/* °C */
	int8_t tr5_critical_temperature;	/* °C */
	int8_t tr5_critical_hysteresis;		/* °C */
	int8_t tr5_warning_temperature;		/* °C */
	int8_t tr5_warning_hysteresis;		/* °C */
	int8_t tr6_critical_temperature;	/* °C */
	int8_t tr6_critical_hysteresis;		/* °C */
	int8_t tr6_warning_temperature;		/* °C */
	int8_t tr6_warning_hysteresis;		/* °C */
	int8_t dts_critical_temperature;	/* °C */
	int8_t dts_critical_hysteresis;		/* °C */
	int8_t dts_warning_temperature;		/* °C */
	int8_t dts_warning_hysteresis;		/* °C */

	uint8_t temp1_fan_select;
	uint8_t temp2_fan_select;
	uint8_t temp3_fan_select;
	uint8_t temp4_fan_select;
	uint8_t temp5_fan_select;
	uint8_t temp6_fan_select;

	/* Voltage Limits for Alarms */
	uint32_t vsen1_high_limit_mv;		/* mV */
	uint32_t vsen1_low_limit_mv;		/* mV */
	uint32_t vsen2_high_limit_mv;		/* mV */
	uint32_t vsen2_low_limit_mv;		/* mV */
	uint32_t vsen3_high_limit_mv;		/* mV */
	uint32_t vsen3_low_limit_mv;		/* mV */
	uint32_t vsen4_high_limit_mv;		/* mV */
	uint32_t vsen4_low_limit_mv;		/* mV */
	uint32_t vsen5_high_limit_mv;		/* mV */
	uint32_t vsen5_low_limit_mv;		/* mV */
	uint32_t vsen6_high_limit_mv;		/* mV */
	uint32_t vsen6_low_limit_mv;		/* mV */
	uint32_t vsen7_high_limit_mv;		/* mV */
	uint32_t vsen7_low_limit_mv;		/* mV */
	uint32_t vsen8_high_limit_mv;		/* mV */
	uint32_t vsen8_low_limit_mv;		/* mV */
	uint32_t vsen9_high_limit_mv;		/* mV */
	uint32_t vsen9_low_limit_mv;		/* mV */
	uint32_t vsen10_high_limit_mv;		/* mV */
	uint32_t vsen10_low_limit_mv;		/* mV */
	uint32_t vsen11_high_limit_mv;		/* mV */
	uint32_t vsen11_low_limit_mv;		/* mV */
	uint32_t vsen12_high_limit_mv;		/* mV */
	uint32_t vsen12_low_limit_mv;		/* mV */
	uint32_t vsen13_high_limit_mv;		/* mV */
	uint32_t vsen13_low_limit_mv;		/* mV */
	uint32_t vdsen14_high_limit_mv;		/* mV */
	uint32_t vdsen14_low_limit_mv;		/* mV */
	uint32_t vdsen15_high_limit_mv;		/* mV */
	uint32_t vdsen15_low_limit_mv;		/* mV */
	uint32_t vdsen16_high_limit_mv;		/* mV */
	uint32_t vdsen16_low_limit_mv;		/* mV */
	uint32_t vdsen17_high_limit_mv;		/* mV */
	uint32_t vdsen17_low_limit_mv;		/* mV */
	uint32_t vtt_high_limit_mv;		/* mV */
	uint32_t vtt_low_limit_mv;		/* mV */
	uint32_t vdd_high_limit_mv;		/* mV */
	uint32_t vdd_low_limit_mv;		/* mV */
	uint32_t vsb_high_limit_mv;		/* mV */
	uint32_t vsb_low_limit_mv;		/* mV */
	uint32_t vbat_high_limit_mv;		/* mV */
	uint32_t vbat_low_limit_mv;		/* mV */

	int8_t temp1_critical_temperature;	/* °C */
	int8_t temp2_critical_temperature;	/* °C */
	int8_t temp3_critical_temperature;	/* °C */
	int8_t temp4_critical_temperature;	/* °C */
	int8_t temp5_critical_temperature;	/* °C */
	int8_t temp6_critical_temperature;	/* °C */
	uint8_t temp1_critical_hysteresis;	/* °C */
	uint8_t temp2_critical_hysteresis;	/* °C */
	uint8_t temp3_critical_hysteresis;	/* °C */
	uint8_t temp4_critical_hysteresis;	/* °C */
	uint8_t temp5_critical_hysteresis;	/* °C */
	uint8_t temp6_critical_hysteresis;	/* °C */

	int8_t temp1_target_temperature;	/* °C */
	int8_t temp2_target_temperature;	/* °C */
	int8_t temp3_target_temperature;	/* °C */
	int8_t temp4_target_temperature;	/* °C */
	int8_t temp5_target_temperature;	/* °C */
	int8_t temp6_target_temperature;	/* °C */
	uint8_t temp1_target_hysteresis;	/* °C */
	uint8_t temp2_target_hysteresis;	/* °C */
	uint8_t temp3_target_hysteresis;	/* °C */
	uint8_t temp4_target_hysteresis;	/* °C */
	uint8_t temp5_target_hysteresis;	/* °C */
	uint8_t temp6_target_hysteresis;	/* °C */

	uint8_t step_up_time;
	uint8_t step_down_time;

	uint8_t fan1_nonstop;			/* % of full speed (0-100) */
	uint8_t fan2_nonstop;			/* % of full speed (0-100) */
	uint8_t fan3_nonstop;			/* % of full speed (0-100) */
	uint8_t fan4_nonstop;			/* % of full speed (0-100) */
	uint8_t fan5_nonstop;			/* % of full speed (0-100) */
	uint8_t fan6_nonstop;			/* % of full speed (0-100) */
	uint8_t fan7_nonstop;			/* % of full speed (0-100) */
	uint8_t fan8_nonstop;			/* % of full speed (0-100) */

	uint8_t poweron_speed;			/* % of full speed (0-100) */

	uint8_t fan1_duty;			/* % of full speed (0-100) */
	uint8_t fan2_duty;			/* % of full speed (0-100) */
	uint8_t fan3_duty;			/* % of full speed (0-100) */
	uint8_t fan4_duty;			/* % of full speed (0-100) */
	uint8_t fan5_duty;			/* % of full speed (0-100) */
	uint8_t fan6_duty;			/* % of full speed (0-100) */
	uint8_t fan7_duty;			/* % of full speed (0-100) */
	uint8_t fan8_duty;			/* % of full speed (0-100) */

	uint8_t smbus_aux;			/* 0   == device located on primary SMBUS,
						 * 1   == device located on first auxiliary
						 *        SMBUS channel,
						 * <n> == device located on <n> auxiliary
						 *        SMBUS channel
						 */
	uint8_t fan_mode;			/* See enum w83795_fan_mode */
	struct w83795_sfiv_table sfiv_table[W83795_SFIV_CURVES];
};
