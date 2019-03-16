/*
 * This file is part of the "bluetoothheater" distribution 
 * (https://gitlab.com/mrjones.id.au/bluetoothheater) 
 *
 * Copyright (C) 2018  Ray Jones <ray@mrjones.id.au>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 */

#ifndef __SCREEN_MANAGER_H__
#define __SCREEN_MANAGER_H__

#include <Arduino.h>
#include <vector>

class CProtocol;
class C128x64_OLED;
class CScreen;
class CRebootScreen;

class CScreenManager {
  std::vector<std::vector<CScreen*>> _Screens;
  CRebootScreen* _pRebootScreen;
  C128x64_OLED* _pDisplay;
  int _menu;
  int _subMenu;
  int _rootMenu;
  unsigned long _DimTime;
  bool _bReqUpdate;
  void _enterScreen();
  void _leaveScreen();
  void _changeSubMenu(int dir);
public:
  enum eUIMenuSets { RootMenuLoop, TimerMenuLoop, TuningMenuLoop, BranchMenu };
  enum eUIRootMenus { DetailedControlUI, BasicControlUI, ClockUI, ModeUI, CommsUI, SettingsUI };
  enum eUITimerMenus { TimerOverviewUI, Timer1UI, Timer2UI, Timer3UI, Timer4UI, Timer5UI, Timer6UI, Timer7UI,
                       Timer8UI, Timer9UI, Timer10UI, Timer11UI, Timer12UI, Timer13UI, Timer14UI };
  enum eUITuningMenus { MixtureUI, HeaterSettingsUI };
  enum eUIBranchMenus { SetClockUI, InheritSettingsUI, ExperimentalUI, FontDumpUI };
public:
  CScreenManager();
  ~CScreenManager();
  void begin(bool bNoClock);
  bool checkUpdate();
  bool animate();
  void keyHandler(uint8_t event);
  void refresh();
  void nextMenu();
  void prevMenu();
  void reqUpdate();
  void selectMenu(eUIMenuSets menuset, int specific = -1);   // use to select loop menus, including the root or branches
  void showRebootMsg(const char* content[2], long delayTime);
};

#endif // __SCREEN_MANAGER_H__
