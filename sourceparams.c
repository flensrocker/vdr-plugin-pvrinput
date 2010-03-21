#include "common.h"

#ifdef PVR_SOURCEPARAMS
const char *cPvrSourceParam::sPluginId = "";
#else
const char *cPvrSourceParam::sPluginId = "PVRINPUT|";
#endif

const char *cPvrSourceParam::sInputName[] = {
 "TV",
 "RADIO",
 "COMPOSITE0",
 "COMPOSITE1",
 "COMPOSITE2",
 "COMPOSITE3",
 "COMPOSITE4",
 "SVIDEO0",
 "SVIDEO1",
 "SVIDEO2",
 "SVIDEO3",
 "COMPONENT"
};

const eInputType cPvrSourceParam::sInputType[] = {
 eTelevision,
 eRadio,
 eComposite0,
 eComposite1,
 eComposite2,
 eComposite3,
 eComposite4,
 eSVideo0,
 eSVideo1,
 eSVideo2,
 eSVideo3,
 eComponent
};

const char *cPvrSourceParam::sCards[] = {
 "",
 "CARD0",
 "CARD1",
 "CARD2",
 "CARD3",
 "CARD4",
 "CARD5",
 "CARD6",
 "CARD7"
};

const char *cPvrSourceParam::sStandardName[] = {
  "",
  "PAL",
  "NTSC",
  "SECAM",
  "NTSC_M",
  "NTSC_M_JP",
  "NTSC_443",
  "NTSC_M_KR",
  "PAL_M",
  "PAL_N",
  "PAL_NC",
  "PAL_B",
  "PAL_B1",
  "PAL_G",
  "PAL_BG",
  "PAL_D",
  "PAL_D1",
  "PAL_K",
  "PAL_DK",
  "PAL_H",
  "PAL_I",
  "PAL_60",
  "SECAM_B",
  "SECAM_D",
  "SECAM_G",
  "SECAM_H",
  "SECAM_K",
  "SECAM_K1",
  "SECAM_DK",
  "SECAM_L",
  "SECAM_LC"
};

const uint64_t cPvrSourceParam::sStandardNorm[] = {
  0,
  V4L2_STD_PAL,
  V4L2_STD_NTSC,
  V4L2_STD_SECAM,
  V4L2_STD_NTSC_M,
  V4L2_STD_NTSC_M_JP,
  V4L2_STD_NTSC_443,
  V4L2_STD_NTSC_M_KR,
  V4L2_STD_PAL_M,
  V4L2_STD_PAL_N,
  V4L2_STD_PAL_Nc,
  V4L2_STD_PAL_B,
  V4L2_STD_PAL_B1,
  V4L2_STD_PAL_G,
  V4L2_STD_PAL_BG,
  V4L2_STD_PAL_D,
  V4L2_STD_PAL_D1,
  V4L2_STD_PAL_K,
  V4L2_STD_PAL_DK,
  V4L2_STD_PAL_H,
  V4L2_STD_PAL_I,
  V4L2_STD_PAL_60,
  V4L2_STD_SECAM_B,
  V4L2_STD_SECAM_D,
  V4L2_STD_SECAM_G,
  V4L2_STD_SECAM_H,
  V4L2_STD_SECAM_K,
  V4L2_STD_SECAM_K1,
  V4L2_STD_SECAM_DK,
  V4L2_STD_SECAM_L,
  V4L2_STD_SECAM_LC
};

const int cPvrSourceParam::sStandardLinesPerFrame[] = {
  -1,
  625,
  525,
  625,
  525,
  525,
  525,
  525,
  525,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  525,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625,
  625
};

cPvrSourceParam::cPvrSourceParam()
#ifdef PVR_SOURCEPARAMS
:cSourceParam(sSourceID, "analog (pvrinput)")
#endif
{
  param = 0;
  input = 0;
  standard = 0;
  card = 0;
}

cString cPvrSourceParam::ParametersToString(void) const
{
  if ((standard == 0) && (card == 0))
     return cString::sprintf("%s%s", sPluginId, sInputName[input]);
  if ((standard == 0) && (card != 0))
     return cString::sprintf("%s%s|%s", sPluginId, sInputName[input], sCards[card]);
  if ((standard != 0) && (card == 0))
     return cString::sprintf("%s%s|%s", sPluginId, sInputName[input], sStandardName[standard]);
  return cString::sprintf("%s%s|%s|%s", sPluginId, sInputName[input], sCards[card], sStandardName[standard]);
}

void cPvrSourceParam::SetData(cChannel *Channel)
{
#ifdef PVR_SOURCEPARAMS
  ParseParameters(Channel->Parameters(), &input, &standard, &card);
#endif
  param = 0;
}

void cPvrSourceParam::GetData(cChannel *Channel)
{
#ifdef PVR_SOURCEPARAMS
  Channel->SetTransponderData(Channel->Source(), Channel->Frequency(), Channel->Srate(), ParametersToString(), true);
#endif
}

cOsdItem *cPvrSourceParam::GetOsdItem(void)
{
  switch (param++) {
    case 0: return new cMenuEditStraItem(tr("SourceParam.pvrinput$Input"), &input, sNumInputs, sInputName);
    case 1: return new cMenuEditStraItem(tr("SourceParam.pvrinput$Card"), &card, sNumCards, sCards);
    case 2: return new cMenuEditStraItem(tr("SourceParam.pvrinput$Standard"), &standard, sNumStandards, sStandardName);
    default: return NULL;
  }
  return NULL;
}

bool cPvrSourceParam::IsPvr(int Code)
{
  return (Code & cSource::st_Mask) == stPvr;
}

bool    cPvrSourceParam::ParseParameters(const char *Parameters, int *InputIndex, int *StandardIndex, int *CardIndex)
{
  char *InputArg  = NULL;
  char *OptArg[2] = { NULL, NULL };
#ifdef PVR_SOURCEPARAMS
  sscanf(Parameters, "%a[^|]|%a[^|]|%a[^:\n]", &InputArg, &OptArg[0], &OptArg[1]);
#else
  char *PluginId  = NULL;
  sscanf(Parameters, "%a[^|]|%a[^|]|%a[^|]|%a[^:\n]", &PluginId, &InputArg, &OptArg[0], &OptArg[1]);
  if (strcasecmp(PluginId, "PVRINPUT"))
     return false;
#endif
  if (InputIndex) {
     *InputIndex = 0;
     if (InputArg != NULL) {
        for (int i = 0; i < sNumInputs; i++) {
            if (!strcasecmp(InputArg, sInputName[i])) {
               *InputIndex = i;
               break;
               }
            }
        }
     }
  if (CardIndex)
     *CardIndex = 0;
  if (StandardIndex)
     *StandardIndex = 0;
  for (int opt = 0; opt < 2; opt++) {
      if (OptArg[opt] != NULL) {
         bool parsed = false;
         for (int c = 1; c < sNumCards; c++) {
             if (!strcasecmp(OptArg[opt], sCards[c])) {
                if (CardIndex)
                   *CardIndex = c;
                parsed = true;
                break;
                }
             }
         if (!parsed) {
            for (int s = 1; s < sNumStandards; s++) {
                if (!strcasecmp(OptArg[opt], sStandardName[s])) {
                   if (StandardIndex)
                      *StandardIndex = s;
                   break;
                   }
                }
            }
         }
      }
  return true;
}

