#include "tx_display_controller.h"

#include <QDebug>
#include "util/ini/TxConfig.h"

TXDisplayController::TXDisplayController(QObject *parent, MediaManager *media_manager)
  : DisplayController(parent, media_manager)
{
  //重新初始化屏幕数，以及布局
  CTxConfig &tx_config = CTxConfig::getInstance();
  screen_count_ = tx_config.GetRecUiScreenCount();
  if (screen_count_ < 1 || screen_count_ > 10) {
    screen_count_ = 1;
  }
  screen_type_ = ValidateScreenLayout(tx_config.GetRecUiScreenType());

  screen_layout_.clear();
  for (int i = 0; i < screen_count_; ++i) {
    screen_layout_.push_back(screen_type_);
  }
}

TXDisplayController::~TXDisplayController()
{

}

void TXDisplayController::HandleReceiveVideoRequestSlot( 
  const QString &vuri, 
  int screen_index, 
  int seat_index, 
  bool use_small_video)
{
  if (seat_index < 1 || screen_index < 0 || screen_index >= screen_count_) {
    return;
  }

  //TX要求同时接收和停止对同一终端所有虚拟终端的录像，因此为简化逻辑，UI操作不允许指定位置，
  //接收时将由系统统一分配位置，并且期间不允许调整位置
  if (!conference_controller_) return;

  TerminalManager &terminal_manager = conference_controller_->GetTerminalManager();
  QString uri = TerminalHelper::GetTerminalURI(vuri);

  //使用自动接收处理
  TerminalList virtual_terminals = terminal_manager[uri];
  for (auto &vterminal : virtual_terminals) {
    HandleRequestStreamMediaAutoReceiveSlot(vterminal->virtual_uri);
  }
}

void TXDisplayController::HandleStopVideoRequestSlot(const QString &vuri)
{
  if (!conference_controller_) return;

  TerminalManager &terminal_manager = conference_controller_->GetTerminalManager();
  QString uri = TerminalHelper::GetTerminalURI(vuri);

  TerminalList virtual_terminals = terminal_manager[uri];
  for (auto &vterminal : virtual_terminals) {
    __super::HandleStopVideoRequestSlot(vterminal->virtual_uri);
  }
}

void TXDisplayController::HandleReceiveAudioRequestSlot( 
  const QString &uri, 
  bool enable)
{
  //Do nothing
}

void TXDisplayController::HandleReceiveDocumentRequestSlot( 
  const QString &uri, 
  int screen_index)
{
  //Do nothing
}

void TXDisplayController::HandleOpenDocumentRequestSlot( 
  const QString &filename, 
  int screen_index)
{
  //Do nothing
}

void TXDisplayController::HandleScreenLayoutRequestSlot( 
  int screen_index, 
  IDisplayController::ScreenLayout layout)
{
  //目前总是设为3X3
  __super::HandleScreenLayoutRequestSlot(screen_index, screen_type_);
}

TXDisplayController::ScreenLayout TXDisplayController::ValidateScreenLayout( int layout )
{
  switch (layout) {
  case IDisplayController::kLayoutOf1x1:
  case IDisplayController::kLayoutOf2x2:
  case IDisplayController::kLayoutOf3x3:
  case IDisplayController::kLayoutOf1plus5:
  case IDisplayController::kLayoutOfAuto:
  case IDisplayController::kLayoutOfTop1x2:
  case IDisplayController::kLayoutOf4x4:
    return ConvertToScreenLayout(layout);

  default:
    return kLayoutOf3x3;
    break;
  }
}
