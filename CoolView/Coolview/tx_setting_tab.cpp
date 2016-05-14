#include "tx_setting_tab.h"

#include "WeblibUploader/WeblibUIComponont.h"

TxSettingTab::TxSettingTab(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);
}

TxSettingTab::~TxSettingTab()
{

}

void TxSettingTab::Initialize()
{
  QWidget * weblib_setting_widget = WeblibUIComponont::getSettingWidget();
  if (!weblib_setting_widget) {
    return;
  }
  //TODO:为了方便以后扩展，新建了TX设置窗口，但目前为空内容，只需要Weblib设置
  layout()->addWidget(weblib_setting_widget);
}
