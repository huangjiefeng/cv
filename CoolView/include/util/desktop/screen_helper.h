#ifndef SCREEN_HELPER_H
#define SCREEN_HELPER_H

#include <windows.h>
#include <QObject>
#include <QVector>
#include <QRect>


class ScreenHelper : public QObject
{
  Q_OBJECT
public:
  // not thread safe
  static ScreenHelper* Instance() {
    static ScreenHelper obj;
    return &obj;
  }

  void Update();
  bool SetPrimaryScreen(int screen_index);
    
  int PosIndexToScreenIndex(int pos_index);
  int ScreenIndexToPosIndex(int screen_index);

  int GetPrimaryScreen() const { return desktop_.primary_index; }
  int GetScreenCount() const { return desktop_.screens.size(); }
  QRect GetAvailableGeometry(int screen_index = -1);

  bool IsAvailableScreen(int screen_index);

Q_SIGNALS:
  void ScreenCountChanged(int screen_count);
  void PrimaryScreenChanged(int primary_index);

private:
  struct Screen {
    POINTL GetDirectTo(const Screen &other) const {
      POINTL direct;
      direct.x = other.monitor_rect.left - monitor_rect.left;
      direct.y = other.monitor_rect.top - monitor_rect.top;
      return direct;
    }
#ifdef UNICODE
    std::wstring device_name;
    std::wstring device_id;
#else
    std::string device_name;
    std::string device_id;
#endif
    RECT monitor_rect;
    RECT work_rect;
    
  };

  typedef QVector<Screen> ScreenList;

  struct Desktop {
    Desktop() : primary_index(-1) {}
    
    const Screen& GetPrimaryScreen() const {
      return screens[primary_index];
    }

    const Screen& GetScreen(int index) const {
      return screens[index];
    }

    int primary_index;
    ScreenList screens;
  };

  struct ScreenPos {
    ScreenPos(): screen_index(-1), pos_x(-1) {}

    bool operator<(const ScreenPos &other) {
      return pos_x < other.pos_x;
    }

    int screen_index;
    int pos_x;
  };

  typedef QVector<ScreenPos> ScreenPosList;

  ScreenHelper();
  ~ScreenHelper();

  bool UpdateDesktopInfo();
  void UpdateMapper();

  static BOOL CALLBACK MonitorEnumProc(
    HMONITOR hMonitor,
    HDC hdcMonitor, 
    LPRECT lprcMonitor, 
    LPARAM dwData);

  static QRect ConvertTo(const RECT &rect) {
    return QRect(
      rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
  }

private:
  ScreenPosList pos_to_screen_list_;
  QVector<int> screen_to_pos_list_;
  Desktop desktop_;
};

#endif // SCREEN_HELPER_H
