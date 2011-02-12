#ifndef _PVRINPUT_SOURCEPARAMS_H
#define _PVRINPUT_SOURCEPARAMS_H

class cPvrSourceParam
#ifdef __SOURCEPARAMS_H
 : public cSourceParam
#endif
{
private:
  int param;
  int input;
  int standard;
  int card;

public:
  cPvrSourceParam();
  cString ParametersToString(void) const;
  virtual void SetData(cChannel *Channel);
  virtual void GetData(cChannel *Channel);
  virtual cOsdItem *GetOsdItem(void);

  static bool IsPvr(int Code);
  static bool ParseParameters(const char *Parameters, int *InputIndex, int *StandardIndex, int *CardIndex);

#ifdef PVR_SOURCEPARAMS
  static const char *sPluginId;
  static const char  sSourceID = 'V';
  static const uint  stPvr = (sSourceID << 24);
#else
  static const char *sPluginId;
  static const char  sSourceID = 'P';
  static const uint  stPvr = cSource::stPlug;
#endif

  static const int         sNumInputs = 12;
  static const char       *sInputName[];
  static const eInputType  sInputType[];

  static const int   sNumCards = 9;
  static const char *sCards[];

  static const int       sNumStandards = 31;
  static const char     *sStandardName[];
  static const uint64_t  sStandardNorm[];
  static const int       sStandardLinesPerFrame[];
};

#endif

