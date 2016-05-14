#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include "qtconferenceroom.h"
#include "ConferenceRoomService.h"
#include "MediaWindowManager.h"
#include <log/Log.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	strcpy(Log::_logFileName , "CvConferenceRoom");
	QTranslator qtTralator;
	qtTralator.load(":/CvConferenceRoom/Resources/qt_zh_CN.qm");
	a.installTranslator( &qtTralator );

	QFont font  = a.font();
	font.setPointSize(9);
  //font.setStyleStrategy (QFont::PreferAntialias);
	a.setFont(font);

	//设置插件。支持图片格式
	QString appDir = QCoreApplication::applicationDirPath();
	appDir.replace("/" , "\\" );
	QString pluginsDir = appDir + "\\plugins";
	a.addLibraryPath( pluginsDir );

  a.setAutoSipEnabled(true);
  QApplication::setQuitOnLastWindowClosed(false); //因主窗口隐藏，这一设置防止QMessageBox关闭时程序退出

	MediaWindowManager::getInstance();

	ConferenceRoomService::getInstance();

	return a.exec();
}