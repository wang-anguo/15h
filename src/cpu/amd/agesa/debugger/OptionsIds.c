#include <console/console.h>

#include "AGESA.h"
#include "Ids.h"

void agesa_printk(long long unsigned int LEVEL, const char *fmt, ...)
{
	va_list args;
	int cb_level = BIOS_ERR;

	switch(LEVEL) {
		case MAIN_FLOW:
			cb_level = BIOS_DEBUG;
			break;
#if CONFIG(ENABLE_AGESA_DEBUG_CPU)
		case CPU_TRACE:
			cb_level = BIOS_DEBUG;
			break;
#endif
#if CONFIG(ENABLE_AGESA_DEBUG_MEM_STATUS)
		case MEM_STATUS:
			cb_level = BIOS_DEBUG;
			break;
#endif
#if CONFIG(ENABLE_AGESA_DEBUG_MEM_FLOW)
		case MEM_FLOW:
			cb_level = BIOS_DEBUG;
			break;
#endif
#if CONFIG(ENABLE_AGESA_DEBUG_MEM_REGISTERS)
		case MEM_GETREG:
			cb_level = BIOS_DEBUG;
			break;
		case MEM_SETREG:
			cb_level = BIOS_DEBUG;
			break;
#endif
#if CONFIG(ENABLE_AGESA_DEBUG_HT)
		case HT_TRACE:
			cb_level = BIOS_DEBUG;
			break;
#endif
#if CONFIG(ENABLE_AGESA_DEBUG_S3)
		case S3_TRACE:
			cb_level = BIOS_DEBUG;
			break;
#endif
		default:
			return;
	}

	va_start(args, fmt);
	do_vprintk(cb_level, fmt, args);
	va_end(args);
};
