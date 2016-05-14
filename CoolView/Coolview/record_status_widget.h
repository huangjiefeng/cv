#ifndef RECORDSTATUSWIDGET_H
#define RECORDSTATUSWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMap>
#include "ui_Record_Status_Widget.h"
#include "util/CVMsgParser.h"
#include "util/report/RtpStat.h"
#include "util/report/RecordStat.h"

class IControllerManager;

#include "conference_controller_interface.h"


class RecordStatusWidget : public QWidget
{
	Q_OBJECT

public:
	RecordStatusWidget(QWidget *parent = 0);
	~RecordStatusWidget();

	void Initialize(IControllerManager *controller_manager);

	//void setTerminalLogout(QString uri);

	/**
	 * @brief 改变vuri的状态为启动
	 */
	/**
	 * @brief 改变vuri的状态为停止
	 */
	void recordStopped(QString vuri);

public slots:
	/*void startRecSlot();
	void stopRecSlot();
	void cutFileSlot();*/


private Q_SLOTS:
    // 处理从conference controller接收到的信号
    void HandleConferenceStateChangedNotifySlot(
        const QString &conference_uri, 
        IConferenceController::ConferenceState state);

    void HandleTerminalReceivedNotifySlot(
        ConstTerminalPointer terminal,
        const QString &conference_uri);

    void HandleTerminalUpdateNotifySlot(
        ConstTerminalPointer terminal, 
        const QString &conference_uri);

    void HandleNotifyRTPStatReceivedSlot(
        const RtpStatItem &rtpstat);

    void HandleNotifyRecStatReceivedSlot(
      const RecStatItem &recstat);

    void HandleNotifyStreamMediaReceiveStartedSlot(const QString &vuri);
    void HandleNotifyStreamMediaReceiveStoppedSlot(const QString &vuri);
    void HandleNotifyStreamMediaReceiveReplySlot(const QString &vuri, 
      bool permission);
    void HandleNotifyRecordRequesterSlot(const QString &vuri, 
      const QString &requester_uri);


private:
    void Reset();
    void ResetRow(int row);
	int GetRow(QString id);
	void UpdateFileColumn(const int row, const QString text);
	void UpdateStatusColumn(const int row, const QString text);

private:
    // UI
	Ui::RecordStatusWidgetClass ui;

    // Controllers
    IControllerManager *controller_manager_;

    // Data Member
	QMap<QString, QString> mediaID_name_map_;     // 流标识-流名称映射
	QMap<QString, QString> name_uri_map;    // 流名称-终端Uri(无@部分)映射
};

#endif // RECORDSTATUSWIDGET_H
