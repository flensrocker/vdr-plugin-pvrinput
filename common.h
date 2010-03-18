#ifndef _PVRINPUT_COMMON_H_
#define _PVRINPUT_COMMON_H_

enum eLogLevel { pvrUNUSED, pvrERROR, pvrINFO, pvrDEBUG1, pvrDEBUG2, pvrDEBUG3 };

#include <string>
#include <strings.h>
#include <linux/types.h>
#include <linux/videodev.h>
#include <linux/videodev2.h>

#include <sys/ioctl.h>
#include <sys/time.h>

#include <stdarg.h>
#include <time.h>
#include <stdio.h>

#include <vdr/osd.h>
#include <vdr/plugin.h>
#include <vdr/channels.h>
#include <vdr/sources.h>
#include <vdr/device.h>
#include <vdr/plugin.h>


#include "setup.h"
#include "filter.h"
#include "device.h"
#include "global.h"
#include "reader.h"
#include "pvrinput.h"
#include "menu.h"

#if VDRVERSNUM < 10507
#include "i18n.h"
#endif //VDRVERSNUM < 10507


void log(int level, const char *fmt, ...);

int IOCTL(int fd, int cmd, void *data);

int Percent2IntVal(int Percent, int MinVal, int MaxVal);
int IntVal2Percent(int NumVal, int MinVal, int MaxVal);

#endif
