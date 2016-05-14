#include <QtWidgets/QApplication>
#include <QtCore/QTextCodec>
#include "ConfigurationFile.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	/*QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
	QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));*/

	ConfigurationFile file;
	file.saveCameraConfig("diei","ÖÚ¶à", "eii", 1280, 800);

	return a.exec();
}
