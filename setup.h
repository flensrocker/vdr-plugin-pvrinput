#ifndef _PVRINPUT_SETUP_H_
#define _PVRINPUT_SETUP_H_

struct valSet {
  v4l2_queryctrl queryctrl;
  int value;
  __u32 ctrl_class;
  bool query_isvalid;
};

class cPvrSetup {
public:
  int HideMainMenuEntry;
  int UseOnlyCard;
  int LogLevel;
  int TunerAudioMode;
  int SliceVBI;
  int AudioVolumeTVExceptionCard;
  int UseExternChannelSwitchScript;
  int ExternChannelSwitchSleep;
  int ReadBufferSizeKB;
  int TsBufferSizeMB;
  int TsBufferPrefillRatio;
  valSet Brightness;
  valSet Contrast;
  valSet Saturation;
  valSet Hue;
  valSet AudioVolumeTVCommon;
  valSet AudioVolumeTVException;
  valSet AudioVolumeFM;
  valSet AudioMute;
  valSet AudioSampling;
  valSet AudioEncoding;
  valSet StreamType;
  valSet VideoBitrateTV;
  valSet VideoBitratePeak;
  valSet AudioBitrate;
  valSet BitrateMode;
  valSet AspectRatio;
  valSet BFrames;
  valSet GopSize;
  valSet GopClosure;
  valSet FilterSpatialMode;
  valSet FilterSpatial;
  valSet FilterLumaSpatialType;
  valSet FilterChromaSpatialType;
  valSet FilterTemporalMode;
  valSet FilterTemporal;
  valSet FilterMedianType;
  valSet FilterLumaMedianBottom;
  valSet FilterLumaMedianTop;
  valSet FilterChromaMedianBottom;
  valSet FilterChromaMedianTop;
  valSet VBIformat;

  valSet HDPVR_AudioEncoding;
  int HDPVR_AudioInput;

public:
  cPvrSetup(void);
  bool repeat_ReInitAll_after_next_encoderstop;
};

extern cPvrSetup PvrSetup;

#endif
