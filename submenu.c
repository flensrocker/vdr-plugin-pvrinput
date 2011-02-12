/**
 *  pvrinput plugin for the Video Disk Recorder
 *
 *  submenu.c  -  submenu of the plugin configuration
 *
 *  (c) 2010 Rainer Blickle <rblickle AT gmx.de>
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

#include "common.h"

static const char *aspectRatios[] = {
  "1:1",
  "4:3",
  "16:9",
  "2.21:1"
  };

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

static const char *streamType[] = {
  "MPEG2 PS",
  "MPEG2 DVD"
  };


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

cPvrMenuGeneral::cPvrMenuGeneral(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$General Parameters")),
      setup(setupObject)
{
  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Log level"), &setup->LogLevel, 0, 4));

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$Slice VBI Teletext"), &setup->SliceVBI));

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$Hide main menu entry"), &setup->HideMainMenuEntry));
}


cPvrMenuVideo::cPvrMenuVideo(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$Video Parameters")),
          setup(setupObject)
{
  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Brightness"), &setup->Brightness.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Contrast"), &setup->Contrast.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Saturation"), &setup->Saturation.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Hue"), &setup->Hue.value, 0, 100));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Bitrate mode"),
                            &setup->BitrateMode.value,
                            setup->BitrateMode.queryctrl.maximum + 1,
                            bitrateModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Video bitrate TV (kbit/s)"),
                           &setup->VideoBitrateTV.value,
                           setup->VideoBitrateTV.queryctrl.minimum,
                           setup->VideoBitrateTV.queryctrl.maximum));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Aspect ratio"),
                            &setup->AspectRatio.value,
                            setup->AspectRatio.queryctrl.maximum + 1,
                            aspectRatios));

}

cPvrMenuAudio::cPvrMenuAudio(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$Audio Parameters")),
          setup(setupObject)
{
  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Common Audio volume (TV)"), &setup->AudioVolumeTVCommon.value, 0, 100));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Exception Audio volume (TV)"), &setup->AudioVolumeTVException.value, 0, 100));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Exception TV Volume for Card"), &setup->AudioVolumeTVExceptionCard, 19, exceptionVolumeForCard));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Tuner Audio Mode"), &setup->TunerAudioMode, 5, tunerAudioModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Audio radio volume"), &setup->AudioVolumeFM.value, 0, 100));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Audio bitrate (kbit/s)"),
                            &setup->AudioBitrate.value,
                            setup->AudioBitrate.queryctrl.maximum + 1,
                            audioBitrateValues));

  static const char *SamplingFreqs[3];
  SamplingFreqs[0] = tr("Setup.pvrinput$44.1 kHz");
  SamplingFreqs[1] = tr("Setup.pvrinput$48 kHz");
  SamplingFreqs[2] = tr("Setup.pvrinput$32 kHz");

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Audio sampling rate"),
                            &setup->AudioSampling.value,
                            setup->AudioSampling.queryctrl.maximum + 1,
                            SamplingFreqs));

}

cPvrMenuMpegFilter::cPvrMenuMpegFilter(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$MPEG Filter Parameters")),
          setup(setupObject)
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

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Mode Spatial Video Filter"),
                            &setup->FilterSpatialMode.value,
                            setup->FilterSpatialMode.queryctrl.maximum + 1,
                            FilterModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Strength Spatial Video Filter"),
                           &setup->FilterSpatial.value,
                           setup->FilterSpatial.queryctrl.minimum,
                           setup->FilterSpatial.queryctrl.maximum));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Luma Spatial Filter Type"),
                            &setup->FilterLumaSpatialType.value,
                            setup->FilterLumaSpatialType.queryctrl.maximum + 1,
                            SpatialTypes));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Chroma Spatial Filter Type"),
                            &setup->FilterChromaSpatialType.value,
                            setup->FilterChromaSpatialType.queryctrl.maximum + 1,
                            SpatialTypes));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Mode Temporal Video Filter"),
                            &setup->FilterTemporalMode.value,
                            setup->FilterTemporalMode.queryctrl.maximum + 1,
                            FilterModes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Strength Temporal Video Filter"),
                           &setup->FilterTemporal.value,
                           setup->FilterTemporal.queryctrl.minimum,
                           setup->FilterTemporal.queryctrl.maximum));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Median Filter Type"),
                            &setup->FilterMedianType.value,
                            setup->FilterMedianType.queryctrl.maximum + 1,
                            MedianTypes));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Luma Median Filter Bottom"),
                           &setup->FilterLumaMedianBottom.value,
                           setup->FilterLumaMedianBottom.queryctrl.minimum,
                           setup->FilterLumaMedianBottom.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Luma Median Filter Top"),
                           &setup->FilterLumaMedianTop.value,
                           setup->FilterLumaMedianTop.queryctrl.minimum,
                           setup->FilterLumaMedianTop.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Chroma Median Filter Bottom"),
                           &setup->FilterChromaMedianBottom.value,
                           setup->FilterChromaMedianBottom.queryctrl.minimum,
                           setup->FilterChromaMedianBottom.queryctrl.maximum));

  Add(new cMenuEditIntItem(tr("Setup.pvrinput$Chroma Median Filter Top"),
                           &setup->FilterChromaMedianTop.value,
                           setup->FilterChromaMedianTop.queryctrl.minimum,
                           setup->FilterChromaMedianTop.queryctrl.maximum));
}

cPvrMenuExperts::cPvrMenuExperts(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$Expert Parameters")),
          setup(setupObject)
{
  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Stream type"), &setup->StreamType.value, 2, streamType));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$Use only card"), &setup->UseOnlyCard, 20, useOnlyCard));

  Add(new cMenuEditBoolItem(tr("Setup.pvrinput$use externchannelswitch.sh"), &setup->UseExternChannelSwitchScript));
}

cPvrMenuHdPvr::cPvrMenuHdPvr(cPvrSetup *setupObject) :
    cPvrSubMenu(tr("Setup.pvrinput$HDPVR Parameters")),
      setup(setupObject) {

  static const char *HDPVR_AudioEncodings[2];
  HDPVR_AudioEncodings[0] = tr("Setup.pvrinput$AAC");
  HDPVR_AudioEncodings[1] = tr("Setup.pvrinput$AC3");

  static const char *HDPVR_AudioInputs[3];
  HDPVR_AudioInputs[0] = tr("Setup.pvrinput$RCA back");
  HDPVR_AudioInputs[1] = tr("Setup.pvrinput$RCA front");
  HDPVR_AudioInputs[2] = tr("Setup.pvrinput$SPDIF");

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$HDPVR audio encoding"), &setup->HDPVR_AudioEncoding.value, 2, HDPVR_AudioEncodings));

  Add(new cMenuEditStraItem(tr("Setup.pvrinput$HDPVR audio input"), &setup->HDPVR_AudioInput, 3, HDPVR_AudioInputs));
}

eOSState cPvrSubMenu::ProcessKey(eKeys Key) {
  if (Key == kOk) {
    return osBack;
  }
  return cOsdMenu::ProcessKey(Key);
}
