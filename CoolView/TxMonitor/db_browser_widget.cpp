#include "db_browser_widget.h"

#include <QStandardItemModel>

#include "record_sqlite_db.h"
#include "dbus/txMonitor/common/TxMonitorServiceCommon.h"
#include "util/ini/TxConfig.h"
#include "util.h"


enum MyItemDataRole { 
  kTypeRole = Qt::UserRole+1,
  kIdRole,
  kDataRole
};

enum ConferenceItemType {
  kUnknown = 0,
  kConferenceType,
  kConferenceMonthType,
  kConferenceDayType,
  kTerminalType,
  kEpisodeType,
  kViewType
};

inline QString DateTimeStringFromMSecs(qint64 msec)
{
  QDateTime start_date = QDateTime::fromMSecsSinceEpoch(msec);
  return start_date.toString("yyyy-MM-dd hh:mm:ss");
}

inline QString DurationStringFromSecs(qint64 sec)
{
  QString duration = QString("%1:%2:%3").
    arg((int)sec/3600).
    arg((sec%3600)/60, 2, 10, QChar('0')).
    arg((sec%60), 2, 10, QChar('0'));
  return duration;
}

struct RecordMediaFormat
{
  int video_width;
  int video_height;
  int video_fps;
  int audio_sample_rate;
  int audio_channel;
  int audio_bits_count;
};

int MediaFormatFromJson(const QString &json, RecordMediaFormat &format)
{
  memset(&format, 0, sizeof(RecordMediaFormat));
  QJsonParseError json_error;
  QJsonDocument parse_doucment = QJsonDocument::fromJson(
    json.toUtf8(), &json_error); 
  if(json_error.error == QJsonParseError::NoError && parse_doucment.isObject()) {
    QJsonObject format_obj = parse_doucment.object();
    if (format_obj.contains("video")) {
      QJsonObject video_obj = format_obj["video"].toObject();
      if (video_obj.contains("width")) 
        format.video_width = video_obj["width"].toDouble();
      if (video_obj.contains("height")) 
        format.video_height = video_obj["height"].toDouble();
      if (video_obj.contains("fps")) 
        format.video_fps = video_obj["fps"].toDouble();
    }
    if (format_obj.contains("audio")) {
      QJsonObject audio_obj = format_obj["audio"].toObject();
      if (audio_obj.contains("rate")) 
        format.audio_sample_rate = audio_obj["rate"].toDouble();
      if (audio_obj.contains("channel")) 
        format.audio_channel = audio_obj["channel"].toDouble();
      if (audio_obj.contains("bit")) 
        format.audio_bits_count = audio_obj["bit"].toDouble();
    }
  }
  return 0;
}

QString RecordedFileStatusToString(int status)
{
  QString msg;
  if (status & RecFileStat_Deleted) {
    msg = QString::fromLocal8Bit("已删除");
  } else {
    msg = QString::fromLocal8Bit("正常");
  }
  if (status & RecFileStat_Uploaded) {
    msg += QString::fromLocal8Bit(",已备份");
  }
  return msg;
}


//Definition of DBBrowserWidget

DBBrowserWidget::DBBrowserWidget(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);
}

DBBrowserWidget::~DBBrowserWidget()
{

}

void DBBrowserWidget::Initialize()
{
  db_.reset(new RecordSqliteDB);

  //设置UI
  QStandardItemModel *conf_model = new QStandardItemModel;
  conf_model->setHorizontalHeaderLabels(
    QStringList() << QString::fromLocal8Bit("会议列表"));
  ui.conferenceView->setModel(conf_model);
  ui.conferenceView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui.conferenceView, &QTreeView::clicked,
    this, &DBBrowserWidget::HandleViewClickedSlot);

  QStandardItemModel *episode_model = new QStandardItemModel;
  episode_model->setHorizontalHeaderLabels(
    QStringList() << QString::fromLocal8Bit("标题")
    << QString::fromLocal8Bit("开始时间")
    << QString::fromLocal8Bit("时长")
    << QString::fromLocal8Bit("参与者")
    << QString::fromLocal8Bit("关键词"));
  ui.episodeView->header()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  ui.episodeView->setModel(episode_model);
  ui.episodeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  connect(ui.episodeView, &QTreeView::clicked,
    this, &DBBrowserWidget::HandleViewClickedSlot);

  QStandardItemModel *view_model = new QStandardItemModel;
  view_model->setHorizontalHeaderLabels(
    QStringList() << QString::fromLocal8Bit("开始时间")
    << QString::fromLocal8Bit("时长")
    << QString::fromLocal8Bit("格式")
    << QString::fromLocal8Bit("状态")
    << QString::fromLocal8Bit("物理路径"));
  ui.fileView->setModel(view_model);
  ui.fileView->horizontalHeader()->setSectionResizeMode(
    QHeaderView::ResizeToContents);
  /*ui.fileView->setColumnWidth(0, 150);
  ui.fileView->setColumnWidth(1, 60);
  ui.fileView->setColumnWidth(2, 250);
  ui.fileView->setColumnWidth(3, 100);
  ui.fileView->setColumnWidth(4, 500);*/
  ui.fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  QHeaderView *vertical_header = ui.fileView->verticalHeader();
  vertical_header->sectionResizeMode(QHeaderView::Fixed);
  vertical_header->setDefaultSectionSize(20);
  connect(ui.fileView, &QTreeView::doubleClicked,
    this, &DBBrowserWidget::HandleViewClickedSlot);

  //连接其他组件信号槽
  connect(ui.conferenceRefreshButton, &QPushButton::clicked,
    this, &DBBrowserWidget::HandleConferenceRefreshButtonClickedSlot);

  //初始化成员
  rec_path_prefix_ = QString::fromLocal8Bit(
    CTxConfig::getInstance().GetUploadPath());

  //初始加载会议列表
  RefreshConference();
}

void DBBrowserWidget::HandleConferenceRefreshButtonClickedSlot()
{
  RefreshConference();
}

void DBBrowserWidget::RefreshConference()
{
  QStandardItemModel *conf_model = 
    dynamic_cast<QStandardItemModel *>(ui.conferenceView->model());
  if (!conf_model) {
    return;
  }

  //清除原来的条目
  ClearConferenceView();

  IRecordDB::ConferenceEntityList conferences; 
  db_->SelectConferences(0, 0, conferences);
  for (auto conference : conferences) {
    QStandardItem* item = new QStandardItem(conference->title);  
    item->setData(kConferenceType, kTypeRole); //触发clicked信号时会用到
    item->setData(conference->cid, kIdRole);
    item->setToolTip(QString::fromLocal8Bit("创建于%1\n%2").
      arg(conference->start_time).
      arg(conference->description));
    conf_model->appendRow(item);
  }
}

void DBBrowserWidget::HandleViewClickedSlot( 
  const QModelIndex &index0)
{
  const QModelIndex &index = (
    index0.column() == 0 ? index0 : index0.sibling(index0.row(), 0));
  int item_type = index.data(kTypeRole).toInt();
  switch (item_type)
  {
  case kConferenceType:
    GetEpisodeDates(index);
    break;
  case kConferenceMonthType:
    ui.conferenceView->expand(index);
    break;
  case kConferenceDayType:
    GetEpisodesByDate(index);
    break;
  case kTerminalType:
    ui.episodeView->expand(index);
    break;
  case kEpisodeType:
    GetViewByEpisode(index);
    break;
  case kViewType:
    SendFileItemDoubleClickedSignal(index);
    break;
  default:
    break;
  }
}

void DBBrowserWidget::GetEpisodeDates( const QModelIndex &index )
{
  QStandardItem *conference_item = 
    dynamic_cast<const QStandardItemModel*>(index.model())->itemFromIndex(index);
  //有排序proxy时记得要这样mysort->mapToSource(index)获取原来的index，不然item不对
  if (conference_item->hasChildren()) {
    //已有子对象则不更新
    return;
  }

  //int cid = conference_item->data().toInt();
  int cid = index.data(kIdRole).toInt();
  IRecordDB::EpisodeDateList dates;
  db_->SelectEpisodeDate(cid, "", dates);

  QHash<QString, QStandardItem*> month_item_dict;
  for (short &d : dates) {
    QDateTime datetime = QDateTime::fromMSecsSinceEpoch(d * 86400000LL); //day to ms
    QString parent_title = datetime.toString("yyyy-MM");
    if (month_item_dict[parent_title] == nullptr) {
      //创建月份节点
      QStandardItem *parent = new QStandardItem(parent_title);
      parent->setData(kConferenceMonthType, kTypeRole);
      month_item_dict[parent_title] = parent;
      conference_item->appendRow(parent);
    }
    QStandardItem *item = new QStandardItem(datetime.toString("dd"));
    item->setData(kConferenceDayType, kTypeRole);
    item->setData(((int)d)*86400, kDataRole); // in s
    month_item_dict[parent_title]->appendRow(item);
  }
  ui.conferenceView->expand(index);
}

void DBBrowserWidget::GetEpisodesByDate( const QModelIndex &index )
{
  QStandardItemModel *episode_model = 
    dynamic_cast<QStandardItemModel *>(ui.episodeView->model());
  if (!episode_model) {
    return;
  }

  ClearEpisodeView();

  long long start_time = index.data(kDataRole).toLongLong(); // in s
  long long end_time = start_time + 86400;
  int cid = 0;

  //get conference item index and get cid
  for (QModelIndex parent_index = index.parent();
       parent_index.isValid();
       parent_index = parent_index.parent()) {
    if (parent_index.data(kTypeRole).toInt() == kConferenceType) {
      cid = parent_index.data(kIdRole).toInt();
    }
  }

  //获得终端列表
  IRecordDB::ConferenceEntityPointer conference = db_->SelectConference(cid);
  IRecordDB::TerminalDict &terminals_dict = conference->terminal_list;
  QHash<QString, QStandardItem*> terminal_items_dict;

  //获取分集
  IRecordDB::EpisodeEntityList episodes;
  db_->SelectEpisodesByInterval(cid, "", start_time, end_time, episodes);
  for (auto e : episodes) {
    QStandardItem* item = new QStandardItem(e->title);  
    item->setData(kEpisodeType, kTypeRole); //触发clicked信号时会用到
    item->setData(e->id, kIdRole);
    item->setToolTip(e->description);

    QString uri = e->terminal_uri;
    if (terminal_items_dict[uri] == nullptr) {
      //终端节点
      QString terminal_name = (terminals_dict.contains(uri) ?
        terminals_dict[uri]->name : uri);
      QStandardItem* terminal_item = new QStandardItem(terminal_name);  
      terminal_item->setData(kTerminalType, kTypeRole); //触发clicked信号时会用到
      terminal_item->setData(uri, kIdRole);
      terminal_items_dict[uri] = terminal_item;
      episode_model->appendRow(terminal_item);
    }
    QStandardItem *terminal_item = terminal_items_dict[uri];
    terminal_item->appendRow(item);
    //start time column
    terminal_item->setChild(item->index().row(), 1, 
      new QStandardItem(DateTimeStringFromMSecs(e->start_time * 1000)));
    //duration column
    terminal_item->setChild(item->index().row(), 2, 
      new QStandardItem(DurationStringFromSecs(e->duration)));
    //other column
    terminal_item->setChild(item->index().row(), 3, 
      new QStandardItem(e->participants));
    terminal_item->setChild(item->index().row(), 4, 
      new QStandardItem(e->keywords));
  }
}

void DBBrowserWidget::GetViewByEpisode( const QModelIndex &index )
{
  QStandardItemModel *view_model = 
    dynamic_cast<QStandardItemModel *>(ui.fileView->model());
  if (!view_model) {
    return;
  }

  ClearFileView();

  long long eid = index.data(kIdRole).toLongLong();

  IRecordDB::ViewEntityList views;
  db_->SelectViewsByEpisode(eid, views);
  for (auto v : views) {
    //start time column
    QStandardItem* item = new QStandardItem(
      DateTimeStringFromMSecs(v->start_time));  
    item->setData(kViewType, kTypeRole); //触发clicked信号时会用到
    item->setData(v->vid, kIdRole);
    item->setData(v->file, kDataRole);
    view_model->appendRow(item);
    //duration column
    view_model->setItem(item->index().row(), 1, 
      new QStandardItem(DurationStringFromSecs(v->duration/1000)));
    //format column
    RecordMediaFormat format = {0};
    MediaFormatFromJson(v->media_format, format);
    QString format_string = QString::fromLocal8Bit("V:%1x%2@%3, A:%4Hz %5ch").
      arg(format.video_width).arg(format.video_height).arg(format.video_fps).
      arg(format.audio_sample_rate).arg(format.audio_channel);
    view_model->setItem(item->index().row(), 2, 
      new QStandardItem(format_string));
    //status column
    view_model->setItem(item->index().row(), 3, 
      new QStandardItem(RecordedFileStatusToString(v->status)));
    //path
    view_model->setItem(item->index().row(), 4, 
      new QStandardItem(v->file));
  }
}

void DBBrowserWidget::ClearConferenceView()
{
  QStandardItemModel *conf_model = 
    dynamic_cast<QStandardItemModel *>(ui.conferenceView->model());
  if (!conf_model) {
    return;
  }

  //清除原来的条目
  //conf_view_model->clear(); //这个会清除表头
  int count = conf_model->rowCount();
  conf_model->removeRows(0, count);

  ClearEpisodeView();
}

void DBBrowserWidget::ClearEpisodeView()
{
  QStandardItemModel *episode_model = 
    dynamic_cast<QStandardItemModel *>(ui.episodeView->model());
  if (!episode_model) {
    return;
  }

  int count = episode_model->rowCount();
  episode_model->removeRows(0, count);

  ClearFileView();
}

void DBBrowserWidget::ClearFileView()
{
  QStandardItemModel *view_model = 
    dynamic_cast<QStandardItemModel *>(ui.fileView->model());
  if (!view_model) {
    return;
  }

  int count = view_model->rowCount();
  view_model->removeRows(0, count);
}

void DBBrowserWidget::SendFileItemDoubleClickedSignal( 
  const QModelIndex &index )
{
  int vid = index.data(kIdRole).toInt();
  QString file = index.data(kDataRole).toString();
  emit NotifyViewSelectedSignal(vid, file);
}
