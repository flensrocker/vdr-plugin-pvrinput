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

#include "common.h"

cPvrMenuSetup::cPvrMenuSetup()
{
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

  newPvrSetup.StreamType.value = (newPvrSetup.StreamType.value == 0 ? 0 : 1);

  cachedPvrSetup = newPvrSetup;
  Add(new cOsdItem(tr("Setup.pvrinput$General Parameters")));
  Add(new cOsdItem(tr("Setup.pvrinput$Video Parameters")));
  Add(new cOsdItem(tr("Setup.pvrinput$Audio Parameters")));
  Add(new cOsdItem(tr("Setup.pvrinput$MPEG Filter Parameters")));
  Add(new cOsdItem(tr("Setup.pvrinput$Expert Parameters")));
  Add(new cOsdItem(tr("Setup.pvrinput$HDPVR Parameters")));

}

eOSState cPvrMenuSetup::ProcessKey(eKeys Key) {
  eOSState state = osUnknown;
  if (Key == kOk && HasSubMenu()) {
    Store();
  }
  if (Key == kBack && HasSubMenu()) {
    newPvrSetup = cachedPvrSetup;
  }
  if (!HasSubMenu()) {
    if (Key == kOk) {
      const char* ItemText = Get(Current())->Text();
      if (strstr(ItemText, tr(tr("Setup.pvrinput$General Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuGeneral(&newPvrSetup));
      } else if (strstr(ItemText, tr(tr("Setup.pvrinput$Video Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuVideo(&newPvrSetup));
      } else if (strstr(ItemText, tr(tr("Setup.pvrinput$Audio Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuAudio(&newPvrSetup));
      } else if (strstr(ItemText, tr(tr("Setup.pvrinput$MPEG Filter Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuMpegFilter(&newPvrSetup));
      } else if (strstr(ItemText, tr(tr("Setup.pvrinput$Expert Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuExperts(&newPvrSetup));
      } else if (strstr(ItemText, tr(tr("Setup.pvrinput$HDPVR Parameters"))) == ItemText) {
        state = AddSubMenu(new cPvrMenuHdPvr(&newPvrSetup));
      } else {
        state = cOsdMenu::ProcessKey(Key);
      }
    } else {
      state = cOsdMenu::ProcessKey(Key);
    }
  } else {
    state = cOsdMenu::ProcessKey(Key);
  }
  return state;
}

void cPvrMenuSetup::Store()
{
  cachedPvrSetup = newPvrSetup;
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
  cString strpercentvalue = cString::sprintf("%d%%", IntVal2Percent(localvalue, localminimum, localmaximum));
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
