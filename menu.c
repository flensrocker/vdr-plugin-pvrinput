/**
 *  pvrinput plugin for the Video Disk Recorder 
 *
 *  menu.c  -  setup menu class
 *
 *  (c) 2005 Andreas Regel <andreas.regel AT powarman.de>
 **/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program;                                              *
 *   if not, write to the Free Software Foundation, Inc.,                  *
 *   59 Temple Place, Suite 330, Boston, MA  02111-1307  USA               *
 *                                                                         *
 ***************************************************************************/

/*uncomment the following line to allow setting of PvrSetup.UseOnlyCard
and StreamType.value via OSD menu (see README) */
#define PVR_DEBUG

#include "common.h"

static const char * audioBitrateValues[] = {
  "32",
  "48",
  "56",
  "64",
  "80",
  "96",
  "112",
  "128",
  "160",
  "192",
  "224",
  "256",
  "320",
  "384"
  };

static const char *bitrateModes[] = {
  "VBR",
  "CBR"
  };

/*  order must match the 'audmode' values in videodev2.h */
static const char *tunerAudioModes[] = {
  "mono",
  "stereo",
  "lang2",
  "lang1",
  "bilingual"
  };

#ifdef PVR_DEBUG
static const char *useOnlyCard[] = {
  "/dev/video0",
  "/dev/video1",
  "/dev/video2",
  "/dev/video3",
  "/dev/video4",
  "/dev/video5",
  "/dev/video6",
  "/dev/video7",
  "all",         // 8
  "PVR150",      // 9
  "PVR250",      //10
  "PVR350",      //11
  "PVR500#1",    //12
  "PVR500#2",    //13  
  "HVR1300",     //14
  "HVR1600",     //15
  "HVR1900",     //16
  "HVR1950",     //17
  "PVRUSB2",     //18
  "HDPVR"        //19
  };
#endif

static const char *aspectRatios[] = {
  "1:1",
  "4:3",
  "16:9",
  "2.21:1"
  };

#ifdef PVR_DEBUG
static const char *streamType[] = {
  "MPEG2 PS",
  "MPEG2 DVD"
  };
#endif

static const char *exceptionVolumeForCard[] = {
  "/dev/video0",
  "/dev/video1",
  "/dev/video2",
  "/dev/video3",
  "/dev/video4",
  "/dev/video5",
  "/dev/video6",
  "/dev/video7",
  "-",           // 8
  "PVR150",      // 9
  "PVR250",      //10
  "PVR350",      //11
  "PVR500#1",    //12
  "PVR500#2",    //13  
  "HVR1300",     //14
  "HVR1600",     //15
  "HVR1900",     //16
  "HVR1950",     //17
  "PVRUSB2"      //18
  };

cPvrMenuSetup::cPvrMenuSetup()
{
  static const char *FilterModes[2];
  FilterModes[0] = tr("Setup.pvrinput$manual");
  FilterModes[1] = tr("Setup.pvrinput$auto");

  static const char *SpatialTypes[5];
  SpatialTypes[0] = tr("Setup.pvrinput$off");
  SpatialTypes[1] = tr("Setup.pvrinput$1D horizontal");
  SpatialTypes[2] = tr("Setup.pvrinput$1D vertical");
  SpatialTypes[3] = tr("Setup.pvrinput$2D hv separable");
  SpatialTypes[4] = tr("Setup.pvrinput$2D sym non separable");

  static const char *MedianTypes[5];
  MedianTypes[0] = tr("Setup.pvrinput$off");
  MedianTypes[1] = tr("Setup.pvrinput$horizontal");
  MedianTypes[2] = tr("Setup.pvrinput$vertical");
  MedianTypes[3] = tr("Setup.pvrinput$horizontal + vertical");
  MedianTypes[4] = tr("Setup.pvrinput$diagonal");

  static const char *SamplingFreqs[3];
  SamplingFreqs[0] = tr("Setup.pvrinput$44.1 kHz");
  SamplingFreqs[1] = tr("Setup.pvrinput$48 kHz");
  SamplingFreqs[2] = tr("Setup.pvrinput$32 kHz");

  static const char *HDPVR_AudioEncodings[2];
  HDPVR_AudioEncodings[0] = tr("Setup.pvrinput$AAC");
  HDPVR_AudioEncodings[1] = tr("Setup.pvrinput$AC3");

  static const char *HDPVR_AudioInputs[3];
  HDPVR_AudioInputs[0] = tr("Setup.pvrinput$RCA back");
  HDPVR_AudioInputs[1] = tr("Setup.pvrinput$RCA front");
  HDPVR_AudioInputs[2] = tr("Setup.pvrinput$SPDIF");

  newPvrSetup = PvrSetup;
  // videobitrate in setup.conf is 0..27000, but ivtv 0..27000000
  newPvrSetup.VideoBitrateTV.value = (int)(PvrSetup.VideoBitrateTV.value / 1000);
  newPvrSetup.VideoBitrateTV.queryctrl.minimum = (int)(PvrSetup.VideoBitrateTV.queryctrl.minimum / 1000);
  newPvrSetup.VideoBitrateTV.queryctrl.maximum = (int)(PvrSetup.VideoBitrateTV.queryctrl.maximum / 1000);

  newPvrSetup.Brightness.value = IntVal2Percent(PvrSetup.Brightness.value,
                                                PvrSetup.Brightness.queryctrl.minimum,
                                                PvrSetup.Brightness.queryctrl.maximum);

  newPvrSetup.Contrast.value = IntVal2Percent(PvrSetup.Contrast.value,
                                              PvrSetup.Contrast.queryctrl.minimum,
                                              PvrSetup.Contrast.queryctrl.maximum);

  newPvrSetup.Saturation.value = IntVal2Percent(PvrSetup.Saturation.value,
                                                PvrSetup.Saturation.queryctrl.minimum,
                                                PvrSetup.Saturation.queryctrl.maximum);

  newPvrSetup.Hue.value = IntVal2Percent(PvrSetup.Hue.value,
                                         PvrSetup.Hue.queryctrl.minimum,
                                         PvrSetup.Hue.queryctrl.maximum);

  newPvrSetup.AudioVolumeTVCommon.value = IntVal2Percent(PvrSetup.AudioVolumeTVCommon.value,
                                                         PvrSetup.AudioVolumeTVCommon.queryctrl.minimum,
                                                         PvrSetup.AudioVolumeTVCommon.queryctrl.maximum);

  newPvrSetup.AudioVolumeTVException.value = IntVal2Percent(PvrSetup.AudioVolumeTVException.value,
                                                            PvrSetup.AudioVolumeTVException.queryctrl.minimum,
                                                            PvrSetup.AudioVolumeTVException.queryctrl.maximum);

  newPvrSetup.AudioVolumeFM.value = IntVal2Percent(PvrSetup.AudioVolumeFM.value,
                                                   PvrSetup.AudioVolumeFM.queryctrl.minimum,
                                                   PvrSetup.AudioVolumeFM.queryctrl.maximum);

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$Hide main menu entry"), &newPvrSetup.HideMainMenuEntry));
#ifdef PVR_DEBUG
  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Use only card"), &newPvrSetup.UseOnlyCard, 20, useOnlyCard));
#endif
  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Log level"), &newPvrSetup.LogLevel, 0, 4));

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$Slice VBI Teletext"), &newPvrSetup.SliceVBI));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Tuner Audio Mode"), &newPvrSetup.TunerAudioMode, 5, tunerAudioModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Brightness"), &newPvrSetup.Brightness.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Contrast"), &newPvrSetup.Contrast.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Saturation"), &newPvrSetup.Saturation.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Hue"), &newPvrSetup.Hue.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Common Audio volume (TV)"), &newPvrSetup.AudioVolumeTVCommon.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Exception Audio volume (TV)"), &newPvrSetup.AudioVolumeTVException.value, 0, 100));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Exception TV Volume for Card"), &newPvrSetup.AudioVolumeTVExceptionCard, 19, exceptionVolumeForCard));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Audio radio volume"), &newPvrSetup.AudioVolumeFM.value, 0, 100));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Audio bitrate (kbit/s)"),
                            &newPvrSetup.AudioBitrate.value,
                            newPvrSetup.AudioBitrate.queryctrl.maximum + 1,
                            audioBitrateValues));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Audio sampling rate"),
                            &newPvrSetup.AudioSampling.value,
                            newPvrSetup.AudioSampling.queryctrl.maximum + 1,
                            SamplingFreqs));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$HDPVR audio encoding"), &newPvrSetup.HDPVR_AudioEncoding.value, 2, HDPVR_AudioEncodings));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$HDPVR audio input"), &newPvrSetup.HDPVR_AudioInput, 3, HDPVR_AudioInputs));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Video bitrate TV (kbit/s)"),
                           &newPvrSetup.VideoBitrateTV.value,
                           newPvrSetup.VideoBitrateTV.queryctrl.minimum,
                           newPvrSetup.VideoBitrateTV.queryctrl.maximum));

  newPvrSetup.StreamType.value = (newPvrSetup.StreamType.value == 0 ? 0 : 1);

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Bitrate mode"),
                            &newPvrSetup.BitrateMode.value,
                            newPvrSetup.BitrateMode.queryctrl.maximum + 1,
                            bitrateModes));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Aspect ratio"),
                            &newPvrSetup.AspectRatio.value,
                            newPvrSetup.AspectRatio.queryctrl.maximum + 1,
                            aspectRatios));

#ifdef PVR_DEBUG
  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Stream type"), &newPvrSetup.StreamType.value, 2, streamType));
#endif

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Mode Spatial Video Filter"),
                            &newPvrSetup.FilterSpatialMode.value,
                            newPvrSetup.FilterSpatialMode.queryctrl.maximum + 1,
                            FilterModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Strength Spatial Video Filter"),
                           &newPvrSetup.FilterSpatial.value,
                           newPvrSetup.FilterSpatial.queryctrl.minimum,
                           newPvrSetup.FilterSpatial.queryctrl.maximum));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Luma Spatial Filter Type"),
                            &newPvrSetup.FilterLumaSpatialType.value,
                            newPvrSetup.FilterLumaSpatialType.queryctrl.maximum + 1,
                            SpatialTypes));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Chroma Spatial Filter Type"),
                            &newPvrSetup.FilterChromaSpatialType.value,
                            newPvrSetup.FilterChromaSpatialType.queryctrl.maximum + 1,
                            SpatialTypes));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Mode Temporal Video Filter"),
                            &newPvrSetup.FilterTemporalMode.value,
                            newPvrSetup.FilterTemporalMode.queryctrl.maximum + 1,
                            FilterModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Strength Temporal Video Filter"),
                           &newPvrSetup.FilterTemporal.value,
                           newPvrSetup.FilterTemporal.queryctrl.minimum,
                           newPvrSetup.FilterTemporal.queryctrl.maximum));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Median Filter Type"),
                            &newPvrSetup.FilterMedianType.value,
                            newPvrSetup.FilterMedianType.queryctrl.maximum + 1,
                            MedianTypes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Luma Median Filter Bottom"),
                           &newPvrSetup.FilterLumaMedianBottom.value,
                           newPvrSetup.FilterLumaMedianBottom.queryctrl.minimum,
                           newPvrSetup.FilterLumaMedianBottom.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Luma Median Filter Top"),
                           &newPvrSetup.FilterLumaMedianTop.value,
                           newPvrSetup.FilterLumaMedianTop.queryctrl.minimum,
                           newPvrSetup.FilterLumaMedianTop.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Chroma Median Filter Bottom"),
                           &newPvrSetup.FilterChromaMedianBottom.value,
                           newPvrSetup.FilterChromaMedianBottom.queryctrl.minimum,
                           newPvrSetup.FilterChromaMedianBottom.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Chroma Median Filter Top"),
                           &newPvrSetup.FilterChromaMedianTop.value,
                           newPvrSetup.FilterChromaMedianTop.queryctrl.minimum,
                           newPvrSetup.FilterChromaMedianTop.queryctrl.maximum));

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$use externchannelswitch.sh"), &newPvrSetup.UseExternChannelSwitchScript));
}

void cPvrMenuSetup::Store()
{
  SetupStore("HideMainMenuEntry", PvrSetup.HideMainMenuEntry = newPvrSetup.HideMainMenuEntry);

  SetupStore("UseOnlyCard", PvrSetup.UseOnlyCard = newPvrSetup.UseOnlyCard);

  SetupStore("LogLevel", PvrSetup.LogLevel = newPvrSetup.LogLevel);
      
  SetupStore("TunerAudioMode", PvrSetup.TunerAudioMode = newPvrSetup.TunerAudioMode);
      
  SetupStore("StreamType", PvrSetup.StreamType.value = newPvrSetup.StreamType.value);

  SetupStore("AudioVolumeTVExceptionCard", PvrSetup.AudioVolumeTVExceptionCard = newPvrSetup.AudioVolumeTVExceptionCard);

  SetupStore("HDPVR_AudioInput", PvrSetup.HDPVR_AudioInput = newPvrSetup.HDPVR_AudioInput);

  SetupStore("SliceVBI", PvrSetup.SliceVBI = newPvrSetup.SliceVBI);
     
  SetupStore("UseExternChannelSwitchScript", PvrSetup.UseExternChannelSwitchScript = newPvrSetup.UseExternChannelSwitchScript);

  /* The following code makes sure that we don't write a zero value
     into setup.conf, which would happen if it is still INVALID_VALUE
     because no device is present.
     The macro SETUPSTORE(n,v) is a abbreviation for the 'if .. then' comparison. */
  #define SETUPSTORE(n, v) if (v.query_isvalid) SetupStore(n, v.value)

  PvrSetup.Brightness.value = Percent2IntVal(newPvrSetup.Brightness.value,
                                             PvrSetup.Brightness.queryctrl.minimum,
                                             PvrSetup.Brightness.queryctrl.maximum);
  SETUPSTORE("Brightness", PvrSetup.Brightness);

  PvrSetup.Contrast.value = Percent2IntVal(newPvrSetup.Contrast.value,
                                           PvrSetup.Contrast.queryctrl.minimum,
                                           PvrSetup.Contrast.queryctrl.maximum);
  SETUPSTORE("Contrast", PvrSetup.Contrast);

  PvrSetup.Saturation.value = Percent2IntVal(newPvrSetup.Saturation.value,
                                             PvrSetup.Saturation.queryctrl.minimum,
                                             PvrSetup.Saturation.queryctrl.maximum);
  SETUPSTORE("Saturation", PvrSetup.Saturation);

  PvrSetup.Hue.value = Percent2IntVal(newPvrSetup.Hue.value,
                                      PvrSetup.Hue.queryctrl.minimum,
                                      PvrSetup.Hue.queryctrl.maximum);
  SETUPSTORE("Hue", PvrSetup.Hue);

  PvrSetup.AudioVolumeTVCommon.value = Percent2IntVal(newPvrSetup.AudioVolumeTVCommon.value,
                                                      PvrSetup.AudioVolumeTVCommon.queryctrl.minimum,
                                                      PvrSetup.AudioVolumeTVCommon.queryctrl.maximum);
  SETUPSTORE("AudioVolumeTVCommon", PvrSetup.AudioVolumeTVCommon);

  PvrSetup.AudioVolumeTVException.value = Percent2IntVal(newPvrSetup.AudioVolumeTVException.value,
                                                         PvrSetup.AudioVolumeTVException.queryctrl.minimum,
                                                         PvrSetup.AudioVolumeTVException.queryctrl.maximum);
  SETUPSTORE("AudioVolumeTVException", PvrSetup.AudioVolumeTVException);

  PvrSetup.AudioVolumeFM.value = Percent2IntVal(newPvrSetup.AudioVolumeFM.value,
                                                PvrSetup.AudioVolumeFM.queryctrl.minimum,
                                                PvrSetup.AudioVolumeFM.queryctrl.maximum);
  SETUPSTORE("AudioVolumeFM", PvrSetup.AudioVolumeFM);

  // videobitrate in setup.conf is 0..27000, but ivtv 0..27000000
  PvrSetup.VideoBitrateTV.value = newPvrSetup.VideoBitrateTV.value * 1000;
  SETUPSTORE("VideoBitrateTV", newPvrSetup.VideoBitrateTV);

  PvrSetup.AudioBitrate.value = newPvrSetup.AudioBitrate.value;
  SETUPSTORE("AudioBitrate", PvrSetup.AudioBitrate);

  PvrSetup.AudioSampling.value = newPvrSetup.AudioSampling.value;
  SETUPSTORE("AudioSampling", PvrSetup.AudioSampling);

  // HDPVR audio encoding in setup.conf is 0..1, but driver needs 3..4
  // if audio input SPDIF is selected, audio encoding must be AAC
  if (newPvrSetup.HDPVR_AudioInput == 2)
     newPvrSetup.HDPVR_AudioEncoding.value = 0;
  PvrSetup.HDPVR_AudioEncoding.value = newPvrSetup.HDPVR_AudioEncoding.value + 3;
  SETUPSTORE("HDPVR_AudioEncoding", newPvrSetup.HDPVR_AudioEncoding);

  PvrSetup.BitrateMode.value = newPvrSetup.BitrateMode.value;
  SETUPSTORE("BitrateMode", PvrSetup.BitrateMode);

  PvrSetup.AspectRatio.value = newPvrSetup.AspectRatio.value;
  SETUPSTORE("AspectRatio", PvrSetup.AspectRatio);

  PvrSetup.FilterSpatialMode.value = newPvrSetup.FilterSpatialMode.value;
  SETUPSTORE("FilterSpatialMode", PvrSetup.FilterSpatialMode);

  PvrSetup.FilterSpatial.value = newPvrSetup.FilterSpatial.value;
  SETUPSTORE("FilterSpatial", PvrSetup.FilterSpatial);

  PvrSetup.FilterLumaSpatialType.value = newPvrSetup.FilterLumaSpatialType.value;
  SETUPSTORE("FilterLumaSpatialType", PvrSetup.FilterLumaSpatialType);

  PvrSetup.FilterChromaSpatialType.value = newPvrSetup.FilterChromaSpatialType.value;
  SETUPSTORE("FilterChromaSpatialType", PvrSetup.FilterChromaSpatialType);

  PvrSetup.FilterTemporalMode.value = newPvrSetup.FilterTemporalMode.value;
  SETUPSTORE("FilterTemporalMode", PvrSetup.FilterTemporalMode);

  PvrSetup.FilterTemporal.value = newPvrSetup.FilterTemporal.value;
  SETUPSTORE("FilterTemporal", PvrSetup.FilterTemporal);

  PvrSetup.FilterMedianType.value = newPvrSetup.FilterMedianType.value;
  SETUPSTORE("FilterMedianType", PvrSetup.FilterMedianType);

  PvrSetup.FilterLumaMedianBottom.value = newPvrSetup.FilterLumaMedianBottom.value;
  SETUPSTORE("FilterLumaMedianBottom", PvrSetup.FilterLumaMedianBottom);

  PvrSetup.FilterLumaMedianTop.value = newPvrSetup.FilterLumaMedianTop.value;
  SETUPSTORE("FilterLumaMedianTop", PvrSetup.FilterLumaMedianTop);

  PvrSetup.FilterChromaMedianBottom.value = newPvrSetup.FilterChromaMedianBottom.value;
  SETUPSTORE("FilterChromaMedianBottom", PvrSetup.FilterChromaMedianBottom);

  PvrSetup.FilterChromaMedianTop.value = newPvrSetup.FilterChromaMedianTop.value;
  SETUPSTORE("FilterChromaMedianTop", PvrSetup.FilterChromaMedianTop);

  cPvrDevice::ReInitAll();
  PvrSetup.repeat_ReInitAll_after_next_encoderstop = true;
}

cPvrMenuMain::cPvrMenuMain(void)
: cOsdObject(),
  border(2),
  margin(4),
  mode(ePicPropBrightness)
{
  font = cFont::GetFont(fontOsd);
  width = Setup.OSDWidth;
  height = 2 * font->Height() + 3 * border + 4 * margin;
}

cPvrMenuMain::~cPvrMenuMain()
{
  delete osd;
}

void cPvrMenuMain::Draw(void)
{
  static const char * pictureProperties[4] = {
    tr("Setup.pvrinput$Brightness"),
    tr("Setup.pvrinput$Contrast"),
    tr("Setup.pvrinput$Saturation"),
    tr("Setup.pvrinput$Hue")
    };

  int titleWidth   = font->Width(tr(pictureProperties[mode])) + 2 * border + 2 * margin;
  int titleHeight  = font->Height() + border + 2 * margin;
  int titleStart   = 50;
  int localvalue   = PvrSetup.Brightness.value;
  int localminimum = PvrSetup.Brightness.queryctrl.minimum;
  int localmaximum = PvrSetup.Brightness.queryctrl.maximum;
  
  switch (mode) {
    case ePicPropBrightness:
      localvalue = PvrSetup.Brightness.value;
      localminimum = PvrSetup.Brightness.queryctrl.minimum;
      localmaximum = PvrSetup.Brightness.queryctrl.maximum;
      break;
    case ePicPropContrast:
      localvalue = PvrSetup.Contrast.value;
      localminimum = PvrSetup.Contrast.queryctrl.minimum;
      localmaximum = PvrSetup.Contrast.queryctrl.maximum;
      break;
    case ePicPropSaturation:
      localvalue = PvrSetup.Saturation.value;
      localminimum = PvrSetup.Saturation.queryctrl.minimum;
      localmaximum = PvrSetup.Saturation.queryctrl.maximum;
      break;
    case ePicPropHue:
      localvalue = PvrSetup.Hue.value;
      localminimum = PvrSetup.Hue.queryctrl.minimum;
      localmaximum = PvrSetup.Hue.queryctrl.maximum;
      break;
    }
  int barWidth = (localvalue - localminimum) * (width - font->Width("100%") - 2 * border - 3 * margin) / (localmaximum - localminimum);

  osd->DrawRectangle(0, 0, width - 1, height - 1, clrTransparent);
  osd->DrawRectangle(0, titleHeight, width - 1, height - 1, clrBlack);
  osd->DrawRectangle(titleStart, 0, titleStart + titleWidth - 1, titleHeight - 1, clrBlack);
  osd->DrawText(titleStart + border + margin, border + margin, tr(pictureProperties[mode]), clrWhite, clrBlack, font);
  osd->DrawRectangle(titleStart, 0, titleStart + titleWidth - 1, border - 1, clrWhite);
  osd->DrawRectangle(titleStart, 0, titleStart + border - 1, titleHeight - 1, clrWhite);
  osd->DrawRectangle(titleStart + titleWidth - border, 0, titleStart + titleWidth - 1, titleHeight - 1, clrWhite);
  osd->DrawRectangle(0, titleHeight, width - 1, titleHeight + border - 1, clrWhite);
  osd->DrawRectangle(0, titleHeight, border - 1, height - 1, clrWhite);
  osd->DrawRectangle(0, height - border, width - 1, height - 1, clrWhite);
  osd->DrawRectangle(width - border, titleHeight, width - 1, height - 1, clrWhite);
  char strpercentvalue[4];
  sprintf(strpercentvalue, "%d%%", IntVal2Percent(localvalue, localminimum, localmaximum));
  osd->DrawText(width - font->Width("100%") - border - margin - 1,
                border + margin + titleHeight,
                strpercentvalue,
                clrWhite,
                clrBlack,
                font,
                font->Width("100%"),
                0,
                taRight);
  osd->DrawRectangle(border + margin, border + margin + titleHeight, border + margin + barWidth - 1, height - border - margin - 1, clrWhite);
  osd->Flush();
}

void cPvrMenuMain::Show(void) {
  osd = cOsdProvider::NewOsd(Setup.OSDLeft, Setup.OSDTop + Setup.OSDHeight - height);
  tArea area = { 0, 0, width - 1, height - 1, 4 };
  if (osd->CanHandleAreas(&area, 1) == oeOk)
     osd->SetAreas(&area, 1);
  Draw();
}

eOSState cPvrMenuMain::ProcessKey(eKeys Key)
{
  eOSState state = cOsdObject::ProcessKey(Key);
  if (state == osUnknown) {
    switch (Key & ~k_Repeat) {
      case kUp:
        mode--;
        if (mode < ePicPropBrightness)
          mode = ePicPropHue;
        Draw();
        break;
      case kDown:
        mode++;
        if (mode > ePicPropHue)
          mode = ePicPropBrightness;
        Draw();
        break;
      case kLeft:
        switch (mode) {
          case ePicPropBrightness:
            if (PvrSetup.Brightness.value > PvrSetup.Brightness.queryctrl.minimum) {
              PvrSetup.Brightness.value--;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Brightness, PvrSetup.Brightness.value);
                  }
              }
            break;
          case ePicPropContrast:
            if (PvrSetup.Contrast.value > PvrSetup.Contrast.queryctrl.minimum) {
              PvrSetup.Contrast.value--;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Contrast, PvrSetup.Contrast.value);
                  }
              }
            break;
          case ePicPropSaturation:
            if (PvrSetup.Saturation.value > PvrSetup.Saturation.queryctrl.minimum) {
              PvrSetup.Saturation.value--;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Saturation, PvrSetup.Saturation.value);
                  }
              }
            break;
          case ePicPropHue:
            if (PvrSetup.Hue.value > PvrSetup.Hue.queryctrl.minimum) {
              PvrSetup.Hue.value--;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Hue, PvrSetup.Hue.value);
                  }
              }
            break;
        }
        Draw();
        break;
      case kRight:
        switch (mode)
        {
          case ePicPropBrightness:
            if (PvrSetup.Brightness.value < PvrSetup.Brightness.queryctrl.maximum) {
              PvrSetup.Brightness.value++;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Brightness, PvrSetup.Brightness.value);
                  }
              }
            break;
          case ePicPropContrast:
            if (PvrSetup.Contrast.value < PvrSetup.Contrast.queryctrl.maximum) {
              PvrSetup.Contrast.value++;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                      pvr->SetControlValue(&PvrSetup.Contrast, PvrSetup.Contrast.value);
                  }
              }
            break;
          case ePicPropSaturation:
            if (PvrSetup.Saturation.value < PvrSetup.Saturation.queryctrl.maximum) {
              PvrSetup.Saturation.value++;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                      pvr->SetControlValue(&PvrSetup.Saturation, PvrSetup.Saturation.value);
                  }
              }
            break;
          case ePicPropHue:
            if (PvrSetup.Hue.value < PvrSetup.Hue.queryctrl.maximum) {
              PvrSetup.Hue.value++;
              for (int i = 0; i < cPvrDevice::Count(); i++) {
                  cPvrDevice *pvr = cPvrDevice::Get(i);
                  if (pvr)
                     pvr->SetControlValue(&PvrSetup.Hue, PvrSetup.Hue.value);
                  }
              }
            break;
        }
        Draw();
        break;
      case kOk:
        PluginPvrInput->SetupStore("Brightness", PvrSetup.Brightness.value);
        PluginPvrInput->SetupStore("Contrast",   PvrSetup.Contrast.value);
        PluginPvrInput->SetupStore("Saturation", PvrSetup.Saturation.value);
        PluginPvrInput->SetupStore("Hue",        PvrSetup.Hue.value);
        return osEnd;
      case kBack:
        return osEnd;
      default:
        return state;
    }
    state = osContinue;
  }
  return state;
}
