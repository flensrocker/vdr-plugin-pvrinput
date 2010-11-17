#include "common.h"

void log(int level, const char *fmt, ...)
{
  char tmpstr[BUFSIZ];
  char timestr[16];
  va_list ap;
  time_t now;
  struct tm local;

  if (PvrSetup.LogLevel >= level) {
     va_start(ap, fmt);
     time(&now);
     localtime_r(&now, &local);
     vsnprintf(tmpstr, sizeof(tmpstr), fmt, ap);
     strftime(timestr, sizeof(timestr), "%H:%M:%S", &local);
     printf("pvrinput: %s %s\n", timestr, tmpstr);
     switch (level) {
       case pvrERROR:
              esyslog("%s",tmpstr);
              break;
       case pvrINFO:
              isyslog("%s",tmpstr);
              break;
       case pvrDEBUG1:
       case pvrDEBUG2:
       case pvrDEBUG3:
       default:
              dsyslog("%s",tmpstr);
              break;
       }
     va_end(ap);
     }
}

/*
function IOCTL
retries the ioctl given six times before giving up,
improves stability if device/driver is actually busy
*/
int IOCTL(int fd, int cmd, void *data)
{
  if (fd < 0) {
     log(pvrERROR, "Error IOCTL: %s is not open", fd);
     return -1;
     }
  for (int retry = 5; retry >= 0; ) {
      if (ioctl(fd, cmd, data) != 0) {
         if (retry) {
            usleep(20000); /* 20msec */
            retry--;
            continue;
            }
         return -1;
         }
      else
         return 0;  /* all went okay :) */
      }
  return 0;  /* should never reach this */
}

/*
This function takes a ivtv V4L2 integer value
and returns it as rounded 0..100% integer value
-wirbel-
*/

int IntVal2Percent(int NumVal, int MinVal, int MaxVal)
{
  double t = 0;
  log(pvrDEBUG1, "IntVal2Percent(NumVal=%d, MinVal=%d, MaxVal=%d)", NumVal, MinVal, MaxVal);
  t = (double)(MaxVal - MinVal);
  t = (NumVal - MinVal) / t;
  t = 100.0 * t + 0.5;    /* rounding to nearest int) */
  return (int)t;
}

/*
This function takes a 0..100% integer value
and returns it as rounded ivtv V4L2 integer value
-wirbel-
*/
int Percent2IntVal(int Percent, int MinVal, int MaxVal)
{
  double t = 0;
  log(pvrDEBUG1, "Percent2IntVal(Percent=%d, MinVal=%d, MaxVal=%d)", Percent, MinVal, MaxVal);
  t = Percent / 100.0;
  t = 0.5 + t * (MaxVal - MinVal);  /* +0.5 is rounding to int */
  t = MinVal + t;
  return (int)t;
}
