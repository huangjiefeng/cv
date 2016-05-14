#include "WeblibUIComponont.h"

#include "uploadsetting.h"
#include "weblibupload.h"
#include "uploadhistory.h"

QWidget * WeblibUIComponont::getSettingWidget()
{
  return new UploadSetting();
}

QWidget * WeblibUIComponont::getUploadStatusWidget()
{
  return new WeblibUpload();
}

QWidget * WeblibUIComponont::getUploadHistoryWidget()
{
	return new UploadHistory();
}