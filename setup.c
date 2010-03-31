#include "common.h"

cPvrSetup::cPvrSetup(void)
{
  HideMainMenuEntry              = 1;            // hide main menu entry
  UseOnlyCard                    = 8;            // Use all cards
  LogLevel                       = 2;            // errors and info messages
  TunerAudioMode                 = 4;            // V4L2_TUNER_MODE_LANG1_LANG2
  StreamType.value               = 0;            // V4L2_MPEG_STREAM_TYPE_MPEG2_PS;
  AudioVolumeTVExceptionCard     = 8;            // AudioVolumeTVCommon for all cards
  SliceVBI                       = 1;            // Slice VBI data into mpeg stream
  UseExternChannelSwitchScript   = 0;            // don't call externchannelswitch.sh on external inputs
  ExternChannelSwitchSleep       = 0;            // sleep x seconds after call of externchannelswitch.sh
  ReadBufferSizeKB               = 64;           // size of buffer for reader in KB
  TsBufferSizeMB                 = 3;            // ring buffer size in MB
  TsBufferPrefillRatio           = 0;            // wait with delivering packets to vdr till buffer is filled
/*  first initialization of all v4l2 controls,
  most values will be re-initialized later one
  in QueryAllControls.  -wirbel-
*/
  Brightness.value               = INVALID_VALUE; // default value in QueryAllControls
  Contrast.value                 = INVALID_VALUE; // default value in QueryAllControls
  Saturation.value               = INVALID_VALUE; // default value in QueryAllControls
  Hue.value                      = INVALID_VALUE; // default value in QueryAllControls
  AudioVolumeTVCommon.value      = INVALID_VALUE; // default value in QueryAllControls
  AudioVolumeTVException.value   = INVALID_VALUE; // default value in QueryAllControls
  AudioVolumeFM.value            = INVALID_VALUE; // default value in QueryAllControls
  AudioSampling.value            = INVALID_VALUE; // default value in QueryAllControls
  AudioMute.value                = 0;             // not muted.
  VideoBitrateTV.value           = INVALID_VALUE; // default value in QueryAllControls
  VideoBitratePeak.value         = 15000000;      // 15Mbit/s
  AudioBitrate.value             = INVALID_VALUE; // default value in QueryAllControls
  BitrateMode.value              = INVALID_VALUE; // default value in QueryAllControls 
  AspectRatio.value              = INVALID_VALUE; // default value in QueryAllControls
  GopSize.value                  = INVALID_VALUE; // default value in QueryAllControls
  BFrames.value                  = INVALID_VALUE; // default value in QueryAllControls
  GopClosure.value               = INVALID_VALUE; // default value in QueryAllControls
  FilterSpatialMode.value        = INVALID_VALUE; // default value in QueryAllControls
  FilterSpatial.value            = INVALID_VALUE; // default value in QueryAllControls
  FilterLumaSpatialType.value    = INVALID_VALUE; // default value in QueryAllControls
  FilterChromaSpatialType.value  = INVALID_VALUE; // default value in QueryAllControls
  FilterTemporalMode.value       = INVALID_VALUE; // default value in QueryAllControls
  FilterTemporal.value           = INVALID_VALUE; // default value in QueryAllControls
  FilterMedianType.value         = INVALID_VALUE; // default value in QueryAllControls
  FilterLumaMedianBottom.value   = INVALID_VALUE; // default value in QueryAllControls
  FilterLumaMedianTop.value      = INVALID_VALUE; // default value in QueryAllControls
  FilterChromaMedianBottom.value = INVALID_VALUE; // default value in QueryAllControls
  FilterChromaMedianTop.value    = INVALID_VALUE; // default value in QueryAllControls

  HDPVR_AudioEncoding.value      = INVALID_VALUE;
  HDPVR_AudioInput               = 0;
}

cPvrSetup PvrSetup;
