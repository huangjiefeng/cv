#ifndef QTOOLWIDGET_H
#define QTOOLWIDGET_H

/*菜单栏，包含了呼叫（拨号、地址簿及其相关）、会议（会议列表、会议成员）、
设备管理（摄像头、音频、视频显示）、监控（主要是一些基本的网络流量信息）、
短信息、帮助（未实现），共6个部分*/

#include "StatusWidget.h"
#include "ui_tool.h"

class ToolTest : public QWidget , public Ui::toolbar
{
    Q_OBJECT

    public:
        ToolTest(QWidget *parent = 0);
        void addItem(QTreeWidgetItem *parent, QStringList *sl);//给Tree列表添加项
        void addConfItem(QTreeWidget *treelist,QTreeWidgetItem *parent, QStringList *sl);//给会议列表添加项
        void addMemberItem(QTreeWidget *treelist,QTreeWidgetItem *parent, QStringList *sl);//给会议成员添加项
        void deleteConference();//取消会议

    private:
        StatusWidget *status;
        QAction *delConfAction;  //取消会议的左键点击action

    public Q_SLOTS:
        void enterConference();
        void exitConference();
        void addcontact();
        void deletecontact();
        void okToContinue();

        //以下的槽都是呼叫功能里的
    private Q_SLOTS:
        void pressNum1();
        void pressNum2();
        void pressNum3();
        void pressNum4();
        void pressNum5();
        void pressNum6();
        void pressNum7();
        void pressNum8();
        void pressNum9();
        void pressNum0();
        void pressNumstar(); //*号
        void pressNumpound();//#号
        void getPhoneNum();  //选中通讯录中的一个项就显示到文本框中

    };

#endif // QTOOLWIDGET_H
