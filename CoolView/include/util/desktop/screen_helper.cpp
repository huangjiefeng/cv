#include "screen_helper.h"

#include <algorithm>
#include <cassert>

#include <QApplication>
#include <QDesktopWidget>

#include "util/ini/CVIniConfig.h"

#define COMPILE_MULTIMON_STUBS

ScreenHelper::ScreenHelper() {
  UpdateDesktopInfo();
  UpdateMapper();
}

ScreenHelper::~ScreenHelper() {
}

void ScreenHelper::Update()
{
  int primary_index = GetPrimaryScreen();
  int screen_count = GetScreenCount();
  if (UpdateDesktopInfo()) {
    //UpdateMapper();
    if (screen_count != GetScreenCount()) {
      emit ScreenCountChanged(screen_count);
    }
    if (primary_index != GetPrimaryScreen()) {
      emit PrimaryScreenChanged(GetPrimaryScreen());
    }
  }
}

bool ScreenHelper::UpdateDesktopInfo()
{
  desktop_.primary_index = -1;
  desktop_.screens.clear();
  return EnumDisplayMonitors(
    NULL, NULL, &MonitorEnumProc, reinterpret_cast<LPARAM>(&desktop_)) == TRUE;
}

BOOL CALLBACK ScreenHelper::MonitorEnumProc(
  HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
  Desktop *desktop = reinterpret_cast<Desktop*>(dwData);

  // 保存显示器信息
  MONITORINFOEX monitor_info;  
  monitor_info.cbSize = sizeof(MONITORINFOEX);
  // 获得显示器信息，将信息保存到monitorinfo中
  GetMonitorInfo(hMonitor, &monitor_info);

  //若检测到主屏
  if(monitor_info.dwFlags == MONITORINFOF_PRIMARY) {
    if(desktop->primary_index == -1) {
      //第一次检测到主屏
      desktop->primary_index = desktop->screens.size();
    } else {
      // 第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
      return FALSE;    //结束检测
    }
  }

  Screen screen;
  screen.device_name = monitor_info.szDevice;
  screen.monitor_rect = monitor_info.rcMonitor;
  screen.work_rect = monitor_info.rcWork;

  DISPLAY_DEVICE monitor;
  ZeroMemory(&monitor, sizeof monitor);
  monitor.cb = sizeof monitor;
  if (EnumDisplayDevices(monitor_info.szDevice, 0, &monitor, NULL)) {
    screen.device_id = monitor.DeviceID;
  }

  desktop->screens.push_back(screen);

  return TRUE;
}

void ScreenHelper::UpdateMapper() {
  bool is_model_tx = CVIniConfig::getInstance().IsModelTX();
  const int screen_count = is_model_tx ? 4 : GetScreenCount();
  pos_to_screen_list_.clear();
  pos_to_screen_list_.reserve(screen_count);
  ScreenPos screen_pos;
  for (int i = 0; i < screen_count; ++i) {
    screen_pos.screen_index = i;
    if (is_model_tx) {
      screen_pos.pos_x = i;
    } 
    else {
      screen_pos.pos_x = desktop_.GetScreen(i).monitor_rect.left;
    }
    pos_to_screen_list_.push_back(screen_pos);
  }
  std::sort(pos_to_screen_list_.begin(), pos_to_screen_list_.end());

  screen_to_pos_list_.clear();
  screen_to_pos_list_.reserve(screen_count);
  for (int i = 0; i < screen_count; ++i) {
    screen_to_pos_list_.push_back(-1);
  }

  for (int i = 0; i < screen_count; ++i) {
    screen_to_pos_list_[pos_to_screen_list_[i].screen_index] = i;
  }
}

int ScreenHelper::PosIndexToScreenIndex( int pos_index ) {
  assert(0 <= pos_index && pos_index < pos_to_screen_list_.size());
  return pos_to_screen_list_[pos_index].screen_index;
}

int ScreenHelper::ScreenIndexToPosIndex( int screen_index ) {
  assert(0 <= screen_index && screen_index < screen_to_pos_list_.size());
  return screen_to_pos_list_[screen_index];
}

QRect ScreenHelper::GetAvailableGeometry(int screen_index /* = -1 */)
{
  if (screen_index == -1) {
    screen_index = desktop_.primary_index;
  }
  return ConvertTo(desktop_.screens[screen_index].work_rect);
}

bool ScreenHelper::SetPrimaryScreen( int screen_index )
{
  if (desktop_.primary_index == screen_index) {
    return false;
  }
  
  auto &old_primary_screen = desktop_.GetPrimaryScreen();
  auto &new_primary_screen = desktop_.GetScreen(screen_index);
  POINTL direct = old_primary_screen.GetDirectTo(new_primary_screen);

  DEVMODE device_mode;
  ZeroMemory(&device_mode, sizeof device_mode);
  device_mode.dmSize = sizeof device_mode;
  device_mode.dmFields = DM_POSITION;

  for (int i = 0; i < desktop_.screens.size(); ++i) {
    auto &screen = desktop_.screens[i];
    device_mode.dmPosition.x = screen.monitor_rect.left - direct.x;
    device_mode.dmPosition.y = screen.monitor_rect.top - direct.y;
    DWORD flag = CDS_UPDATEREGISTRY;
    if (i == screen_index) flag |= CDS_SET_PRIMARY;
    ChangeDisplaySettingsEx(
      screen.device_name.c_str(), &device_mode, NULL, flag, NULL);
  }
  Update();
  return true;
}

bool ScreenHelper::IsAvailableScreen( int screen_index )
{
  return 0 <= screen_index && screen_index < desktop_.screens.size();
}




