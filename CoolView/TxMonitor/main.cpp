#include <QApplication>

#include <Windows.h>

#include "component_manager.h"
#include "monitor_window.h"
#include "log.h"

int main(int argc, char *argv[])
{
  SetConsoleTitleA( "TxMonitor.exe" );

  QApplication a(argc, argv);

  SetLogComponent("tx");

  ComponentManager component_manager;
  component_manager.Initialize();
  
  MonitorWindow monitor_window;
  monitor_window.Initialize(&component_manager);

  return a.exec();
}
