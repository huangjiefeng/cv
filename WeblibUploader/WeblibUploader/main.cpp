#include "mainwindow.h"
#include <QApplication>

//程序输出流重定向至了日志文件，因此在项目里通过qDebug及qCritical等输出信息不会在
//调试窗口显示，需要查看日志文件：log.h
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	MainWindow mainwindow;
	mainwindow.show();
	return a.exec();
}
