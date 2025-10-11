libagesa-y += F10HyHtPhyTables.c
libagesa-y += F10HyInitEarlyTable.c
libagesa-y += F10HyLogicalIdTables.c
ifneq ($(CONFIG_REMOVE_MICROCODE),y)
libagesa-y += F10HyEquivalenceTable.c
libagesa-y += F10HyMicrocodePatchTables.c
endif
libagesa-y += F10HyMsrTables.c
libagesa-y += F10HyPciTables.c
