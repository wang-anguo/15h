/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2011 Advanced Micro Devices, Inc.
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
/**
 * @file
 *
 * IDS Option File
 *
 * This file is used to switch on/off IDS features.
 *
 */
#ifndef _OPTION_IDS_H_
#define _OPTION_IDS_H_

void agesa_printk(long long unsigned int LEVEL, const char *fmt, ...);

#if CONFIG(ENABLE_AGESA_DEBUGGER)
#define IDSOPT_IDS_ENABLED     TRUE
#define IDSOPT_TRACING_ENABLED TRUE
#define IDSOPT_ASSERT_ENABLED  TRUE

#define IDS_HDT_CONSOLE(f, s, ...) agesa_printk(f, s, ##__VA_ARGS__)

#if CONFIG(ENABLE_AGESA_DEBUG_CONFIG)
#define IDSOPT_TRACE_USER_OPTIONS TRUE
#else
#define IDSOPT_TRACE_USER_OPTIONS FALSE
#endif

#endif

#endif
