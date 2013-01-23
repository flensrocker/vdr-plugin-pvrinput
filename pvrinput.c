#include "common.h"

#if VDRVERSNUM < 10713
#ifndef PLUGINPARAMPATCHVERSNUM
#error "You must apply the pluginparam patch for VDR!"
#endif
#endif

static const char *VERSION        = "2013-01-23";
static const char *DESCRIPTION    = tr("use Hauppauge PVR as input device");
static const char *MAINMENUENTRY  = tr("PVR picture settings");


cPluginPvrInput *PluginPvrInput;

cPluginPvrInput::cPluginPvrInput(void)
{
  PluginPvrInput = this;
#ifdef __DYNAMIC_DEVICE_PROBE
  cPvrDeviceProbe::Init();
#endif
}

cPluginPvrInput::~cPluginPvrInput()
{
#ifdef __DYNAMIC_DEVICE_PROBE
  cPvrDeviceProbe::Shutdown();
#endif
  PluginPvrInput = NULL;
}

const char *cPluginPvrInput::Version(void)
{
  return VERSION;
}

const char *cPluginPvrInput::Description(void)
{
  return tr(DESCRIPTION);
}

const char *cPluginPvrInput::CommandLineHelp(void)
{
  return NULL;
}

bool cPluginPvrInput::ProcessArgs(int argc, char *argv[])
{
  return true;
}

bool cPluginPvrInput::Initialize(void)
{
#if VDRVERSNUM < 10507
  RegisterI18n(pvrinput_Phrases); 
#endif //VDRVERSNUM < 10507
  cPvrDevice::Initialize();
  return true;
}

bool cPluginPvrInput::Start(void)
{
/* Start() is called after the primary device and user interface has
   been set up, but before the main program loop is entered. Is called
   after Initialize(). */
  return true;
}

void cPluginPvrInput::Stop(void)
{
/* Any threads the plugin may have created shall be stopped
   in the Stop() function. See VDR/PLUGINS.html */
  cPvrDevice::StopAll();
};

void cPluginPvrInput::Housekeeping(void)
{
}

const char *cPluginPvrInput::MainMenuEntry(void)
{
  if (PvrSetup.HideMainMenuEntry)
     return NULL;
  return tr(MAINMENUENTRY);
}

cOsdObject *cPluginPvrInput::MainMenuAction(void)
{
  cChannel *channel = Channels.GetByNumber(cDevice::CurrentChannel());
#if VDRVERSNUM < 10713
  if (channel && channel->IsPlug())
     return new cPvrMenuMain();
#else
  // TODO: rework cPvrSourceParams for use with older vdr
  if (channel && ((channel->Source() >> 24) == 'V'))
     return new cPvrMenuMain();
#endif
  Skins.Message(mtError, tr("Setup.pvrinput$Not on an analogue channel!"), 2);
  return NULL;
}

cMenuSetupPage *cPluginPvrInput::SetupMenu(void)
{
  return new cPvrMenuSetup();
}

bool cPluginPvrInput::SetupParse(const char *Name, const char *Value)
{
  if      (!strcasecmp(Name, "LogLevel"))                     PvrSetup.LogLevel                       = atoi(Value);
  else if (!strcasecmp(Name, "UseOnlyCard"))                  PvrSetup.UseOnlyCard                    = atoi(Value);
  else if (!strcasecmp(Name, "SliceVBI"))                     PvrSetup.SliceVBI                       = atoi(Value);
  else if (!strcasecmp(Name, "TunerAudioMode"))               PvrSetup.TunerAudioMode                 = atoi(Value);
  else if (!strcasecmp(Name, "Brightness"))                   PvrSetup.Brightness.value               = atoi(Value);
  else if (!strcasecmp(Name, "Contrast"))                     PvrSetup.Contrast.value                 = atoi(Value);
  else if (!strcasecmp(Name, "Saturation"))                   PvrSetup.Saturation.value               = atoi(Value);
  else if (!strcasecmp(Name, "Hue"))                          PvrSetup.Hue.value                      = atoi(Value);
  else if (!strcasecmp(Name, "AudioVolumeTVCommon"))          PvrSetup.AudioVolumeTVCommon.value      = atoi(Value);
  else if (!strcasecmp(Name, "AudioVolumeTVException"))       PvrSetup.AudioVolumeTVException.value   = atoi(Value);
  else if (!strcasecmp(Name, "AudioVolumeTVExceptionCard"))   PvrSetup.AudioVolumeTVExceptionCard     = atoi(Value);
  else if (!strcasecmp(Name, "AudioVolumeFM"))                PvrSetup.AudioVolumeFM.value            = atoi(Value);
  else if (!strcasecmp(Name, "AudioSampling"))                PvrSetup.AudioSampling.value            = atoi(Value);
  else if (!strcasecmp(Name, "VideoBitrateTV"))               PvrSetup.VideoBitrateTV.value           = atoi(Value) * 1000;
  else if (!strcasecmp(Name, "AudioBitrate"))                 PvrSetup.AudioBitrate.value             = atoi(Value);
  else if (!strcasecmp(Name, "BitrateMode"))                  PvrSetup.BitrateMode.value              = atoi(Value);
  else if (!strcasecmp(Name, "AspectRatio"))                  PvrSetup.AspectRatio.value              = atoi(Value);
  else if (!strcasecmp(Name, "StreamType"))                   PvrSetup.StreamType.value               = atoi(Value);
  else if (!strcasecmp(Name, "FilterSpatialMode"))            PvrSetup.FilterSpatialMode.value        = atoi(Value);
  else if (!strcasecmp(Name, "FilterSpatial"))                PvrSetup.FilterSpatial.value            = atoi(Value);
  else if (!strcasecmp(Name, "FilterLumaSpatialType"))        PvrSetup.FilterLumaSpatialType.value    = atoi(Value);
  else if (!strcasecmp(Name, "FilterChromaSpatialType"))      PvrSetup.FilterChromaSpatialType.value  = atoi(Value);
  else if (!strcasecmp(Name, "FilterTemporalMode"))           PvrSetup.FilterTemporalMode.value       = atoi(Value);
  else if (!strcasecmp(Name, "FilterTemporal"))               PvrSetup.FilterTemporal.value           = atoi(Value);
  else if (!strcasecmp(Name, "FilterMedianType"))             PvrSetup.FilterMedianType.value         = atoi(Value);
  else if (!strcasecmp(Name, "FilterLumaMedianBottom"))       PvrSetup.FilterLumaMedianBottom.value   = atoi(Value);
  else if (!strcasecmp(Name, "FilterLumaMedianTop"))          PvrSetup.FilterLumaMedianTop.value      = atoi(Value);
  else if (!strcasecmp(Name, "FilterChromaMedianBottom"))     PvrSetup.FilterChromaMedianBottom.value = atoi(Value);
  else if (!strcasecmp(Name, "FilterChromaMedianTop"))        PvrSetup.FilterChromaMedianTop.value    = atoi(Value);
  else if (!strcasecmp(Name, "HideMainMenuEntry"))            PvrSetup.HideMainMenuEntry              = atoi(Value);
  else if (!strcasecmp(Name, "ReadBufferSizeKB"))             PvrSetup.ReadBufferSizeKB               = atoi(Value);
  else if (!strcasecmp(Name, "TsBufferSizeMB"))               PvrSetup.TsBufferSizeMB                 = atoi(Value);
  else if (!strcasecmp(Name, "TsBufferPrefillRatio"))         PvrSetup.TsBufferPrefillRatio           = atoi(Value);
  else if (!strcasecmp(Name, "UseExternChannelSwitchScript")) PvrSetup.UseExternChannelSwitchScript   = atoi(Value);
  else if (!strcasecmp(Name, "ExternChannelSwitchSleep"))     PvrSetup.ExternChannelSwitchSleep       = atoi(Value);
  else if (!strcasecmp(Name, "HDPVR_AudioEncoding"))          PvrSetup.HDPVR_AudioEncoding.value      = atoi(Value) + 3;
  else if (!strcasecmp(Name, "HDPVR_AudioInput"))             PvrSetup.HDPVR_AudioInput               = atoi(Value);
  else return false;
  return true;
}

VDRPLUGINCREATOR(cPluginPvrInput); // Don't touch this!
