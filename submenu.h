/**
 *  pvrinput plugin for the Video Disk Recorder
 *
 *  submenu.h  -  submenu of the plugin configuration
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
#include <vdr/osdbase.h>


class cPvrMenuGeneral : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuGeneral(cPvrSetup *setupObject);
};

class cPvrMenuVideo : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuVideo(cPvrSetup *setupObject);
};

class cPvrMenuAudio : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuAudio(cPvrSetup *setupObject);
};

class cPvrMenuMpegFilter : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuMpegFilter(cPvrSetup *setupObject);
};

class cPvrMenuExperts : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuExperts(cPvrSetup *setupObject);
};

class cPvrMenuHdPvr : public cOsdMenu {
private:
  /* Reference to the setup data object, mustn't be deleted within this Object */
  cPvrSetup *setup;
public:
  cPvrMenuHdPvr(cPvrSetup *setupObject);
};
