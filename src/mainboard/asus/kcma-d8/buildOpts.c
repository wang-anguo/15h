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

#include <AGESA.h>

/****** Feature Removals ******/

/* Custom support for removing microcode updates. */
#if CONFIG(REMOVE_MICROCODE)
#define BLDOPT_REMOVE_MICROCODE_UPDATES           TRUE
#endif

/****** Build Configuration ******/

#define BLDCFG_AMD_PLATFORM_TYPE                  AMD_PLATFORM_SERVER

/*** Memory Configuration ***/

/* BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT
This is the maximum memory clock at which the platform memory busses are
capable of performing. */
#define BLDCFG_MEMORY_BUS_FREQUENCY_LIMIT         MEM_DDR1866_FREQUENCY

/* BLDCFG_MEMORY_CLOCK_SELECT
This is the specified memory clock to be used in the system as determined by
the selected mode. (See also BLDCFG_TIMING_MODE_SELECT.)
	MEM_DDR400_FREQUENCY MEM_DDR533_FREQUENCY
	MEM_DDR667_FREQUENCY MEM_DDR800_FREQUENCY
	MEM_DDR1066_FREQUENCY MEM_DDR1333_FREQUENCY
	MEM_DDR1600_FREQUENCY MEM_DDR1866_FREQUENCY
	MEM_DDR2100_FREQUENCY MEM_DDR2133_FREQUENCY
	MEM_DDR2400_FREQUENCY */
#define BLDCFG_MEMORY_CLOCK_SELECT                MEM_DDR1866_FREQUENCY

/* BLDCFG_TIMING_MODE_SELECT
Allows the platform to select how the determination is made for the memory
clock that is to be used in the system. (See also BLDCFG_MEMORY_CLOCK_SELECT.)

TIMING_MODE_AUTO — The AGESA software calculates the best
memory clock rate (default).

TIMING_MODE_LIMITED — The AGESA software calculates the best
memory clock, restricted by a maximum user limit provided in
BLDCFG_MEMORY_CLOCK_SELECT.

TIMING_MODE_SPECIFIC — The platform specifies the clock rate, which
is provided in BLDCFG_MEMORY_CLOCK.*/
#define BLDCFG_TIMING_MODE_SELECT                 MEM_TIMING_MODE_AUTO

/* BLDCFG_MEMORY_ENABLE_BANK_INTERLEAVING
Specifies if the system should use DRAM bank (also known as chip-select)
interleaving. If the build option is set to include the code for this feature, then
this setting activates the feature. If the feature code is removed from the build,
then this element has no effect. */
#define BLDCFG_MEMORY_ENABLE_BANK_INTERLEAVING    FALSE

/* BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING
Specifies if the system should use memory node interleaving. If the build
option is set to include the code for this feature, then this setting activates the
feature. If the feature code is removed from the build, then this element has no
effect. */
#define BLDCFG_MEMORY_ENABLE_NODE_INTERLEAVING    FALSE

/* BLDCFG_MEMORY_CHANNEL_INTERLEAVING
Specifies if the system should use memory channel interleaving for
performance tuning. If the build option is set to include the code for this
feature, then this setting activates the feature. If the feature code is removed
from the build, then this element has no effect. Also, this option has no effect if
the channel ganged mode is selected. */
#define BLDCFG_MEMORY_CHANNEL_INTERLEAVING        TRUE

/* BLDCFG_MEMORY_ALL_CLOCKS_ON
This is a power-usage control. To save power, unused memory clocks are
disabled. Some platforms may not prefer to use this feature.
	TRUE = Enable all memclocks whether they are used or not.
	FALSE = Unused memclocks are disabled (default). */
#define BLDCFG_MEMORY_ALL_CLOCKS_ON               FALSE

/* BLDCFG_MEMORY_POWER_DOWN
This feature conserves power in the DIMMs by placing them into self-refresh
when the memory on a channel is not actively being accessed. They quickly
exit self-refresh upon the next processor access. */
#define BLDCFG_MEMORY_POWER_DOWN                  TRUE

/* BLDCFG_POWER_DOWN_MODE
This is a power-usage control that performs memory clock enable-based
power-down control. If the platform enables power-down capability, then this
element describes the platform method chosen, which is one of the following:
	MEM_POWER_DOWN_BY_CHANNEL
	MEM_POWER_DOWN_BY_CHIP_SELECT
	MEM_POWER_DOWN_MODE_AUTO (default)
If the platform is not indicated to have power-down capability, then this
element has no affect. */
#define BLDCFG_POWER_DOWN_MODE                    MEM_POWER_DOWN_BY_CHIP_SELECT

/* BLDCFG_LIMIT_MEMORY_TO_BELOW_1TB
This item provides for ensuring that the top of memory is limited to below 1
TByte. This may be needed for certain operating systems.

FALSE - Not limited. Top of memory will be greater than 1 TByte if sufficient
memory is installed. Memory may be hoisted above 1 TByte to make it
accessible.

TRUE - Limited (default). Top of memory will be below 1 TByte no matter
how much memory is installed; any additional memory will not be
accessible. */
#define BLDCFG_LIMIT_MEMORY_TO_BELOW_1TB          TRUE

/* BLDCFG_MEMORY_SODIMM_CAPABLE
Specifies if the platform is designed to be capable of supporting SoDIMMs. */
#define BLDCFG_MEMORY_SODIMM_CAPABLE              FALSE

/* BLDCFG_MEMORY_LRDIMM_CAPABLE
Specifies if the platform is designed to be capable of supporting LRDIMMs. */
#define BLDCFG_MEMORY_LRDIMM_CAPABLE              TRUE

/* BLDCFG_MEMORY_RDIMM_CAPABLE
Specifies if the platform is designed to be capable of supporting RDIMMs */
#define BLDCFG_MEMORY_RDIMM_CAPABLE               TRUE

/* BLDCFG_MEMORY_UDIMM_CAPABLE
Specifies if the platform is designed to be capable of supporting UDIMMs. */
#define BLDCFG_MEMORY_UDIMM_CAPABLE               TRUE

/* BLDCFG_MEMORY_QUAD_RANK_CAPABLE
This is a platform-specific setting indicating that the memory slots are capable
of supporting Quad Rank DIMMs. */
#define BLDCFG_MEMORY_QUAD_RANK_CAPABLE           TRUE

/* BLDCFG_MEMORY_QUADRANK_TYPE
If the platform sets QRankCapable, then the Quad Rank DIMM slot type is one
of the following:
	QUADRANK_REGISTERED (4-Rank Registered DIMMs) (default)
	QUADRANK_UNBUFFERED (Unbuffered SO-DIMMs)
If the platform is not indicated to be Quad Rank capable, then this element has
no effect. */
#define BLDCFG_MEMORY_QUADRANK_TYPE               MEM_QUADRANK_REGISTERED

/* BLDCFG_IGNORE_SPD_CHECKSUM
When the checksum of the SPD record fails, the typical action is to drop the
DIMM and not attempt to configure it into the system. The software indicates
via return code that this condition has occurred.

Under certain conditions, the platform can decide to accept the associated risk
and choose to ignore the SPD checksum. */
#define BLDCFG_IGNORE_SPD_CHECKSUM                FALSE

/* BLDCFG_MEMORY_MODE_UNGANGED
The platform should be set to use the memory channel unganged mode. */
#define BLDCFG_MEMORY_MODE_UNGANGED               TRUE

/* BLDCFG_DQS_TRAINING_CONTROL
DQS signal timing training control is a platform selection to specify whether
the automatic timing training routine is desired or a set of pre-defined timing
values should be used. This can provide boot speed improvement by bypassing
the active training algorithm and using previously stored values. */
#define BLDCFG_DQS_TRAINING_CONTROL               TRUE

/* BLDCFG_ONLINE_SPARE
This feature is recommended only for expert users and is described in the BIOS
and Kernel Developer's Guides (BKDG). */
#define BLDCFG_ONLINE_SPARE                       FALSE

/* BLDCFG_BANK_SWIZZLE
Address swizzle is a performance fine-tuning element that swaps some address
lines. See the BKDG for description. */
#define BLDCFG_BANK_SWIZZLE                       TRUE

/* BLDCFG_USE_BURST_MODE
This is a performance fine-tuning element. The effect is described in the
BKDG. This element may not be available in all families.
	TRUE — enable (4-beat burst when width is 64 bits)
	FALSE — disable (default) */
#define BLDCFG_USE_BURST_MODE                     FALSE

/*** ECC Configuration ***/

/* BLDCFG_ENABLE_ECC_FEATURE
This turns on the correction action and enables the ability of the MCA sub-
system to report errors. It does not activate the MCA error report interrupts. If
the build option is set to include the code for the ECC feature, then this setting
activates the feature. If the feature code is removed from the build, then this
element has no affect. */
#define BLDCFG_ENABLE_ECC_FEATURE                 TRUE

/* ECC Scrub Rates
Scrubbers are used to periodically read cacheline sized data locations and associated tags, correcting any cor-
rectable errors which are discovered before they can migrate into uncorrectable errors. This is particularly
important for soft errors, which are caused by external sources such as radiation and which are temporary con-
ditions which do not indicate malfunctioning hardware. Scrubbers also help identify marginal or failed hard-
ware by finding and logging repeated errors at the same location

There are many factors which influence scrub rates. Among these are:
	• The size of memory or cache to be scrubbed
	• Resistance to upsets
	• Geographic location and altitude
	• Alpha particle contribution of packaging
	• Performance sensitivity
	• Risk aversion

For steady state operation, finding a range of useful scrub rates may be done by selecting a scrub rate which is
high enough to give good confidence about protection from accumulating errors and low enough that it has no
measurable effect on performance. The below baselines ar e made to maximize error coverage without affecting
performance and not based on specific processor soft error rates. Refer to JEDEC standards for guidelines on
adjusting for geographic location.

	ECCSCRUBRATE_DISABLE
	ECCSCRUBRATE_40NS           40 ns
	ECCSCRUBRATE_80NS           80 ns
	ECCSCRUBRATE_160NS          160 ns
	ECCSCRUBRATE_320NS          320 ns
	ECCSCRUBRATE_640NS          640 ns
	ECCSCRUBRATE_1US            1.28 us
	ECCSCRUBRATE_3US            2.56 us
	ECCSCRUBRATE_5US            5.12 us
	ECCSCRUBRATE_10US           10.2 us
	ECCSCRUBRATE_21US           20.5 us
	ECCSCRUBRATE_41US           41.0 us
	ECCSCRUBRATE_82US           81.9 us
	ECCSCRUBRATE_164US          163.8 us
	ECCSCRUBRATE_328US          327.7 us
	ECCSCRUBRATE_655US          655.4 us
	ECCSCRUBRATE_1MS            1.31 ms
	ECCSCRUBRATE_3MS            2.62 ms
	ECCSCRUBRATE_5MS            5.24 ms
	ECCSCRUBRATE_10MS           10.49 ms
	ECCSCRUBRATE_21MS           20.97 ms
	ECCSCRUBRATE_42MS           42 ms
	ECCSCRUBRATE_84MS           84 ms
	ECCSCRUBRATE_AUTO           Scale scrub rate to DCT node memory (DRAM ONLY)
*/

/* BLDCFG_SCRUB_DRAM_RATE
This value selects how often the ECC background scrubber makes a pass
through the DRAM. This is a numeric value and the value can vary from
processor family to family.
	0 = Disabled (default)
	x_x_x = Scrub Rate, see the BKDG. */
#define BLDCFG_SCRUB_DRAM_RATE                    ECCSCRUBRATE_AUTO

/* BLDCFG_SCRUB_L2_RATE
This value selects how often the ECC background scrubber makes a pass
through the L2 cache. This is a numeric value and the value can vary from
processor family to family.
	0 =Disabled (default) 
	x_x_x =Scrub Rate, see the BKDG. */
#define BLDCFG_SCRUB_L2_RATE                      ECCSCRUBRATE_1MS

/* BLDCFG_SCRUB_L3_RATE
This value selects how often the ECC background scrubber makes a pass
through the L3 cache. This is a numeric value and the value can vary from
processor family to family and may not be available on all families.
	0 =Disabled (default)
	x_x_x =Scrub Rate, see the BKDG. */
#define BLDCFG_SCRUB_L3_RATE                      ECCSCRUBRATE_1MS

/* BLDCFG_SCRUB_DC_RATE
This value selects how often the ECC background scrubber makes a pass
through the DC (Data Cache). This is a numeric value and the value can vary
from processor family to family.
	0 = Disabled (default)
	x_x_x = Scrub Rate, see the BKDG. */
#define BLDCFG_SCRUB_DC_RATE                      ECCSCRUBRATE_5MS

/* BLDCFG_ECC_SYNC_FLOOD
This indicates whether or not to cause a sync flood in the system when
uncorrectable ECC errors are detected. This is a BOOLEAN value:
	FALSE = Do not enable the MCA feature (default).
	TRUE = Please see the BKDG for a description of the use of the Sync Flood
	and why a platform may choose to make use of this feature. */
#define BLDCFG_ECC_SYNC_FLOOD                     TRUE

/* BLDCFG_ECC_REDIRECTION
DRAM ECC redirection is a data-protection feature. Redirection is a special
ECC feature that enables the scrubber to immediately scrub any address in
which a correctable error is discovered. */
#define BLDCFG_ECC_REDIRECTION                    FALSE

/* BLDCFG_ECC_SYMBOL_SIZE
This is an error-detection control. Please see the BKDG. for a description. This
is a numeric value of 0, 4, or 8. Zero means to use the BKDG recommendation.
The default is 4. */
#define BLDCFG_ECC_SYMBOL_SIZE                    4

/*** CPU Configuration ***/

/* BLDCFG_CORE_LEVELING_MODE
A multi-socket system can be populated with processors having various
number of CPU cores. Many operating systems require a homogeneous system
with regard to processor feature set and some operating systems are limited in
how many cores they can support. This parameter allows control of how the
number of CPU cores is leveled to meet the various requirements. */
#define BLDCFG_CORE_LEVELING_MODE                CORE_LEVEL_LOWEST

/* BLDCFG_PLATFORM_CONTROL_FLOW_MODE
This value is used to select the optimum flow control method for the platform.
Considerations include Display Refresh, Isochronous Flow and IOMMU. The
available values are listed in the AGESA.h file in the enumeration declaration
for PLATFORM_CONTROL_FLOW. The default is Nfcm, for Normal Control Flow Mode.
	Nfcm    = Normal Flow Control Mode
	UmaDr   = UMA using Display Refresh flow control
	UmaIfcm = UMA using Isochronous Flow Control
	Ifcm    = Isochronous Flow Control Mode (other than for UMA)
	Iommu   = An IOMMU is in use in the system */
#if CONFIG(NORTHBRIDGE_AMD_CIMX_RD890_IOMMU)
#define BLDCFG_IOMMU_SUPPORT                     TRUE
#define BLDCFG_PLATFORM_CONTROL_FLOW_MODE        Iommu
#else
#define BLDCFG_PLATFORM_CONTROL_FLOW_MODE        Nfcm
#endif

/* BLDCFG_PLATFORM_C1E_MODE
Enable Message Based C1e CPU feature in multi-socket systems. BLDCFG_PLATFORM_C1E_OPDATA element
be defined with a valid IO port value, else the feature cannot be enabled.
 	C1eModeDisabled                   = Disabled
	C1eModeAuto                       = Enables the best C1e method for the currently installed processor
	C1eModeHardware                   = Hardware method
	C1eModeMsgBased                   = Message-based method
	C1eModeSoftwareDeprecated         = Deprecated software SMI method.
	C1eModeHardwareSoftwareDeprecated = Hardware or deprecated software SMI method

	BLDCFG_PLATFORM_C1E_OPDATA  = Specifies the P_LVL3 I/O port of the platform (C1eModeHardware, C1eModeMsgBased)
	BLDCFG_PLATFORM_C1E_OPDATA1 = Specifies the address of chipset's SMI command port (C1eModeSoftwareDeprecated / C1eModeHardwareSoftwareDeprecated)
	BLDCFG_PLATFORM_C1E_OPDATA2 = Specifies the unique number used by the SMI handler to identify SMI source (C1eModeSoftwareDeprecated / C1eModeHardwareSoftwareDeprecated)
	BLDCFG_PLATFORM_C1E_OPDATA3 = Specifies the P_LVL3 I/O port of the platform for HW C1e (C1eModeAuto) */
#define BLDCFG_PLATFORM_C1E_MODE                  C1eModeMsgBased
#define BLDCFG_PLATFORM_C1E_OPDATA                0x80
#define BLDCFG_PLATFORM_C1E_OPDATA1               0x00
#define BLDCFG_PLATFORM_C1E_OPDATA2               0x00
#define BLDCFG_PLATFORM_C1E_OPDATA3               0x80

/* BLDCFG_AMD_PSTATE_CAP_VALUE
Disables CPU PStates based on TDP (mW) */
#define BLDCFG_AMD_PSTATE_CAP_VALUE               0

/* BLDCFG_USE_HT_ASSIST
Enable the probe filtering performance tuning feature. The probe filter
provides filtering of broadcast probes to improve link bandwidth and
performance for multi-node systems.
 
This feature may interact with other performance features.

	TRUE  -Enable the feature (default) if supported by all processors,
	       based on revision and presence of L3 cache.
	       The feature is not enabled if there are no coherent HT links.

	FALSE -Do not enable the feature regardless of the configuration.
 */
#define BLDCFG_USE_HT_ASSIST	                  FALSE

/* BLDCFG_PLATFORM_DEEMPHASIS_LIST
HyperTransport links will typically require an equalization at high frequencies.
This is called deemphasis.

Deemphasis is specified as levels, for example, -3 db.
There are two levels for each link, its receiver deemphasis level and its DCV level,
which is based on the far side transmitter's deemphasis.
For each link, different levels may be required at each link frequency.

Coherent connections between processors should have an entry for the port on each processor.
There should be one entry for the host root port of each non-coherent chain.

AGESA initialization code does not set deemphasis on IO Devices.
A default is provided for internal links of MCM processors, and
those links will generally not need deemphasis structures.
	DeemphasisLevelNone		DcvLevelNone		DcvLevelMinus8
	DeemphasisLevelMinus3		DcvLevelMinus2		DcvLevelMinus9
	DeemphasisLevelMinus6		DcvLevelMinus3		DcvLevelMinus11
	DeemphasisLevelMinus8		DcvLevelMinus5
	DeemphasisLevelMinus11		DcvLevelMinus6
	DeemphasisLevelMinus11pre8	DcvLevelMinus7 */
CONST CPU_HT_DEEMPHASIS_LEVEL ROMDATA board_deemphasis_list[] =
{
	/* Socket, Link, LowFreq, HighFreq, Receiver Deemphasis, Dcv Deemphasis */

	/* Coherent Links */
	{HT_LIST_MATCH_ANY, HT_LIST_MATCH_ANY, HT_FREQUENCY_3200M, HT_FREQUENCY_3200M, DeemphasisLevelMinus6, DcvLevelMinus6},

	/* Non-Coherent Links, See RD890_TRANSMITTER_DEEMPHASIS */
	{HT_LIST_MATCH_ANY, HT_LIST_MATCH_ANY, HT_FREQUENCY_2600M, HT_FREQUENCY_2600M, DeemphasisLevelMinus3, DcvLevelMinus3},

	/* End of the list */
	{
		HT_LIST_TERMINAL
	}
};
#define BLDCFG_PLATFORM_DEEMPHASIS_LIST		&board_deemphasis_list

/* BLDCFG_AP_MTRR_SETTINGS_LIST
The ApMtrrSettingsList allows the customization of the APs' Fixed-Sized
MTRR settings. The MTRR settings are applied after early initialization before
IBV takes control of the APs. This control only applies to the x86-64
Architecture APUs.

The default value is NULL, indicating that no MTRR override setting is
provided. To provide an override setting list, assign the build element to the
name of a user defined settings table. For details, refer to the internal
documentation, “Internal Documentation” on page 26.

Only the MTRRs listed in the user defined table will be overwritten; the
settings for the other fixed-sized MTRRs will be set to default values.
*/
CONST AP_MTRR_SETTINGS ROMDATA board_ap_mtrr_list[] =
{
	{AMD_AP_MTRR_FIX64k_00000, 0x1E1E1E1E1E1E1E1E},
	{AMD_AP_MTRR_FIX16k_80000, 0x1E1E1E1E1E1E1E1E},
	{AMD_AP_MTRR_FIX16k_A0000, 0x0000000000000000},
	{AMD_AP_MTRR_FIX4k_C0000,  0x0000000000000000},
	{AMD_AP_MTRR_FIX4k_C8000,  0x0000000000000000},
	{AMD_AP_MTRR_FIX4k_D0000,  0x0000000000000000},
	{AMD_AP_MTRR_FIX4k_D8000,  0x0000000000000000},
	{AMD_AP_MTRR_FIX4k_E0000,  0x1818181818181818},
	{AMD_AP_MTRR_FIX4k_E8000,  0x1818181818181818},
	{AMD_AP_MTRR_FIX4k_F0000,  0x1818181818181818},
	{AMD_AP_MTRR_FIX4k_F8000,  0x1818181818181818},
	{CPU_LIST_TERMINAL}
};
#define BLDCFG_AP_MTRR_SETTINGS_LIST		&board_ap_mtrr_list

/*** VRM Configuration ***/

/* BLDCFG_VRM_CURRENT_LIMIT
This value indicates the maximum current that the voltage regulators is capable
of providing to the (Core | NB | GFX) processor on a continuous basis. This a
numeric value and must be in milliamperes. */
#define BLDCFG_VRM_CURRENT_LIMIT                  120000
#define BLDCFG_VRM_INRUSH_CURRENT_LIMIT           0

/* BLDCFG_VRM_LOW_POWER_THRESHOLD
Some models of processor support an ability (output signal to the VRM) to
indicate when the processor is in a low-power state. This feature can be used
by the power regulator to place itself into a more power-efficient mode. This
parameter specifies the maximum current in mA that the power regulator can
supply to the (Core | NB | GFX) in its power efficiency mode. This value is
translated into a PState threshold. When the processor enters a PState under
this threshold, the power regulator is signaled that it can enter its power
efficiency mode. Note that some VRMs may require this value to reflect a
current “in-rush” component.

The default action is to set a value of zero, indicating to disable the feature. To
specify a threshold, the value must be specified in milli-amps. */
#define BLDCFG_VRM_LOW_POWER_THRESHOLD            0

/*** IO Configuration ***/

/* BLDCFG_PLAT_NUM_IO_APICS
This element provides the number of IO APICs that are used on the
motherboard. This is used by the software to appropriately assign APIC IDs to
the processors, leaving room for the motherboard devices. The default setting
is 3. */
#define BLDCFG_PLAT_NUM_IO_APICS                  3

/* BLDCFG_PCI_MMIO_BASE
Specifies an address to be used as the extended MMIO PCI configuration base
address. The address may be up to 48 bits and must be aligned on
“BLDCFG_PCI_MMIO_SIZE” . The default is zero, which disables the
extended MMIO method. */
#define BLDCFG_PCI_MMIO_BASE                      CONFIG_MMCONF_BASE_ADDRESS

/* BLDCFG_PCI_MMIO_SIZE
The size of the Extended MMIO region. Must be 1, 2, 4, 8, 16, 32, 64, 128, or
256 MBytes. Default is zero. */
#define BLDCFG_PCI_MMIO_SIZE                      CONFIG_MMCONF_BUS_NUMBER

/****** AMD Platform Install ******/
#include "SanMarinoInstall.h"
