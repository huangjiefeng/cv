#include "stdafx.h"
#include "tchar.h"

#include ".\qtaboutdialog.h"
#include "ui_AboutDialog.h"
#include "GetFileVersion.h"//hf.qiu
#include <qtutil/WidgetBackgroundImage.h>

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


QtAboutDialog::QtAboutDialog(void)
{
	_ui = new Ui::AboutDialog;
	_ui->setupUi(this);

	WidgetBackgroundImage::setBackgroundImage(_ui->LogoLabel, ":/pics/conference/setup_banner.jpg", WidgetBackgroundImage::AdjustSize);
	/* Set a formatted text to the build id label. */
	_ui->CoolviewVersionStringLabel->setText("<b>" + 
					QString(QObject::tr("@product@")) + " " +
					getVersionByName("coolview.exe")
					);
	
	//QFile file(":/AUTHORS");
	//if (file.open(QFile::ReadOnly)) {
	//	QString authors = file.readAll();
	//	file.close();
	//	_ui->authorsTextEdit->setPlainText(authors);
	//} else {
	//	LOG_ERROR("couldn't locate file=" + file.fileName().toStdString());
	//}

	/* Set a formatted text to dependencies version label. */
	_ui->DependenciesLabel->setText(
				"MSDX: "		+getVersionByName("msdx.dll") + "<BR>" +
				"Video-Codec: "	+getVersionByName("filter\\SCUT_H264_Encoder.dll") + "<BR>" +
				"Audio-Codec: "	+getVersionByName("filter\\SpeexEnc.dll") + "<BR>" +
				"RTP:"			+getVersionByName("filter\\RTPout_Video.dll") + "<BR>" +
				"R-CTRL:"		+getVersionByName("teleController.exe") + "<BR>"
				);
}

QtAboutDialog::~QtAboutDialog(void)
{
	if (_ui!=NULL)
	{
		delete _ui;
	}
}
