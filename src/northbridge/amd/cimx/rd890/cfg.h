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


#ifndef _RD890_CFG_H_
#define _RD890_CFG_H_

#include <stdint.h>

/**
 * @brief North Bridge CIMx configuration
 *
 */
void rd890_cimx_config(AMD_NB_CONFIG_BLOCK **pConfigPtr, NB_CONFIG *nbConfig, HT_CONFIG *htConfig, PCIE_CONFIG *pcieConfig);

#endif //_RD890_CFG_H_
