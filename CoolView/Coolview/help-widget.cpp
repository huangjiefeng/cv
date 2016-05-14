#include "help-widget.h"
#include "util/GetFileVersion.h"
#include <tchar.h>

QString getVersionByName(const TCHAR * cName)
{
	char strVersion[20];

	memset(strVersion, 0, sizeof(strVersion));
	//使用CGetFileVersion类读取rtpware.dll的版本信息
	CGetFileVersion verReader;
	verReader.Open(cName);	//打开文件，读取版本信息
	if (!verReader.IsValid())
	{
		//读取错误
		printf("error to read the version of %s \n", cName);
	}
	else
	{
		//输出版本号信息
		sprintf(strVersion, "%d.%d.%d.%d", 
			verReader.GetFileVersionMajor(),
			verReader.GetFileVersionMinor(), 
			verReader.GetFileVersionBuild(), 
			verReader.GetFileVersionQFE());
	}
	return QString::fromStdString(std::string(strVersion));
}


HelpWidget::HelpWidget(QWidget *parent) : QWidget(parent) {
    this->setupUi(this);

	this->CoolviewVersionStringLabel->setText("<b>" + 
		QString(QObject::tr("CoolView")) + " " +
		getVersionByName(L"coolview.exe")
		);


	this->DependenciesLabel->setText(
		"MSDX: "		+getVersionByName(TEXT("msdx.dll")) + "<BR>" +
		"Video-Codec: "	+getVersionByName(TEXT("filter\\SCUT_H264_Encoder.dll")) + "<BR>" +
		"Audio-Codec: "	+getVersionByName(TEXT("filter\\SpeexEnc.dll")) + "<BR>" +
		"RTP:"			+getVersionByName(TEXT("filter\\RtpWare3.dll")) + "<BR>" +//原RTP（RTPout_Video.dll）时间戳算法有问题，结构复杂，难以改进，已弃用gmlan20150714
		"R-CTRL:"		+getVersionByName(TEXT("CvTelecontrollerSocket.exe")) + "<BR>"
		);

    this->show();
}

HelpWidget::~HelpWidget()
{

}
