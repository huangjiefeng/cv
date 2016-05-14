#include "memberlistwidget.h"

#include "terminal-controller.h"
#include "util/MediaManager.h"

#include <QApplication>
#include <QMessageBox>
#include <QDesktopWidget>
#include <log/Log.h>
#include "util/ini/CVIniConfig.h"

MemberListWidget::MemberListWidget(QString &confname,ConferenceListWidget *parent)
        : p(parent)
{
    this->setupUi(this);

	m_bRec = CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_TX;

    /*******************************1.16 dhx***********************************/
    mwidth = p->confwidth;
    mheight = p->confheight;
    /*******************************1.16 dhx end***********************************/

    username = p->getUsername();           //2.25

    this->IMButton->hide();
	//this->handupButton->hide();

    layout = new QGridLayout(this->scrollWidget);
    layout->setMargin(0);
    layout->setSpacing(1);
    this->scrollWidget->setLayout(layout);


    tcontroller = new TerminalController( parent);
   // terminalname = "";

	terminalListCheckTimer = new QTimer(this);
	connect( terminalListCheckTimer , SIGNAL(timeout()) , this , SLOT(checkTerminalListSlot()));

    connect(this,SIGNAL(ReceiveVideoRequestSignal(QString,int,int)),tcontroller,SLOT(onReceiveVideo(QString,int,int)));
    connect(this,SIGNAL(StopVideoRequestSignal(QString,int)),tcontroller,SLOT(onStopVideo(QString,int)));
    connect(this,SIGNAL(changeVideoRequestSignal(QString,int,int)),tcontroller,SLOT(onChangeVideoSeet(QString,int,int)));
    connect(tcontroller,SIGNAL(ConnectedTerminalChanged(QString)),this,SLOT(deviceChangedSlot(QString)));
    connect(this,SIGNAL(ReceiveDocumentRequestSignal(QString,int)),tcontroller,SLOT(onReceiveDocument(QString,int)));
    connect(this,SIGNAL(StopDocumentRequestSignal(QString,int)),tcontroller,SLOT(onStopDocument(QString,int)));

    connect(handupButton,SIGNAL(clicked(bool)),this,SIGNAL(handupSIGNAL(bool)));
    connect(quitConferenceButton,SIGNAL(clicked()),SLOT(closeConference()));
    connect(this->IMButton,SIGNAL(clicked()),SLOT(onIMButtonClicked()));
    connect(this->memberButton,SIGNAL(clicked(bool)),this,SLOT(showTreeWidget(bool)));

    handupButton->setCheckable(true);
    this->memberButton->setCheckable(true);

    memberTable = new QTableWidget();
    memberTable->setColumnCount(7);
    memberTable->setHorizontalHeaderLabels(QStringList()<<QString::fromLocal8Bit("帐号")<<QString::fromLocal8Bit("名称")
                                           <<QString::fromLocal8Bit("视频")<<QString::fromLocal8Bit("音频")<<QString::fromLocal8Bit("举手"));
    memberTable->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    memberTable->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    memberTable->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
    memberTable->horizontalHeader()->setSectionResizeMode(3,QHeaderView::ResizeToContents);
    memberTable->setEditTriggers(QTableWidget::NoEditTriggers);
 //   memberTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    memberTable->verticalHeader()->setHidden(true);
    memberTable->setGeometry(10,10,800,300);
   // memberTable->show();

    treeWidget = new QWidget(this);
    treeWidget->setMaximumSize(400,300);
    treeWidget->setMinimumSize(400,300);
    memberTreeWidget = new QTreeWidget();
    memberTreeWidget->setColumnCount(3);
    memberTreeWidget->header()->hide();
    memberTreeWidget->header()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
    memberTreeWidget->header()->setSectionResizeMode(1,QHeaderView::ResizeToContents);
    QGridLayout *treelayout = new QGridLayout(treeWidget);
    treelayout->addWidget(memberTreeWidget);
    treelayout->setMargin(2);
    treeWidget->setLayout(treelayout);
    treeWidget->hide();

  /*  terminalTable = new QTableWidget();
    terminalTable->setColumnCount(13);
    terminalTable->setGeometry(10,10,800,300);
    terminalTable->show();
*/


	isChangeControlScreen=false;

    screenWidget0 = new ShowWidget(QString::fromLocal8Bit("第1屏幕"),0,this);
    screenWidget1 = new ShowWidget(QString::fromLocal8Bit("第2屏幕"),1,this);
	screenWidget2 = new ShowWidget(QString::fromLocal8Bit("第3屏幕"),2,this);
	screenWidget3 = new ShowWidget(QString::fromLocal8Bit("第4屏幕"),3,this);
	
	lwidget = screenWidget0;
	rwidget = screenWidget1;

    llayout = new QGridLayout(this->showWidget_L);
    rlayout = new QGridLayout(this->showWidget_R);

    llayout->addWidget(lwidget);
    llayout->setMargin(0);
    this->showWidget_L->setLayout(llayout);

    rlayout->addWidget(rwidget);
    rlayout->setMargin(0);
    this->showWidget_R->setLayout(rlayout);

	this->ChangeScreenButton->hide();
	this->ChangeScreenButton->setStyleSheet("border-image:url(:/image/front.png)");

	this->ScreenChangeButton->setEnabled(false);
	
	connect(this->ChangeScreenButton,SIGNAL(clicked()),this,SLOT(controlScreenChangeSlot()));
	connect(this->ScreenChangeButton,SIGNAL(clicked()),this,SLOT(ChangeScreenTo1And2Slot()));
	connect(this->ScreenChangeButton2,SIGNAL(clicked()),this,SLOT(ChangeScreenTo3And4Slot()));

    /*************************************2.21**************************************/
    lmenu = new QMenu("m");
   // lmenu->setContextMenuPolicy();;
    lmenu->addAction(lwidget->oneAndOneAction);
    lmenu->addAction(lwidget->twoAndTwoAction);
    lmenu->addAction(lwidget->threeAndThreeAction);
    lmenu->addAction(lwidget->onePlusFiveLAction);
   // lmenu->addAction(lwidget->onePlusTwentyLAction);
    lmenu->addAction(lwidget->showRTCPAction);
    lmenu->addAction(lwidget->documentAction);
	lmenu->addAction(lwidget->autoAction);
    this->lToolButton->setMenu(lmenu);
    connect(this->lToolButton,SIGNAL(clicked()),this->lToolButton,SLOT(showMenu()));

    rmenu = new QMenu("m");
    rmenu->addAction(rwidget->oneAndOneAction);
    rmenu->addAction(rwidget->twoAndTwoAction);
    rmenu->addAction(rwidget->threeAndThreeAction);
    rmenu->addAction(rwidget->onePlusFiveLAction);
 //   rmenu->addAction(rwidget->onePlusTwentyLAction);
    rmenu->addAction(rwidget->showRTCPAction);
    rmenu->addAction(rwidget->documentAction);
	rmenu->addAction(rwidget->autoAction);
    this->rToolButton->setMenu(rmenu);
    connect(this->rToolButton,SIGNAL(clicked()),this->rToolButton,SLOT(showMenu()));
    /*************************************2.21  end**************************************/
	

	connect(lwidget,SIGNAL(oneAndOneSignal(int)),tcontroller,SLOT(onOneAndOneClicked(int)));
	connect(lwidget,SIGNAL(twoAndTwoSignal(int)),tcontroller,SLOT(onTwoAndTwoClicked(int)));
	connect(lwidget,SIGNAL(threeAndThreeSignal(int)),tcontroller,SLOT(onThreeAndThreeClicked(int)));
	connect(lwidget,SIGNAL(onePlusFiveLSignal(int)),tcontroller,SLOT(onLClicked(int)));
	connect(lwidget,SIGNAL(receiveVideoSignal(QString,int,int)),this,SLOT(receiveVideoSlot(QString,int,int)));
	connect(lwidget,SIGNAL(stopVideoSignal(QString,int)),this,SLOT(stopVideoSlot(QString,int)));
	connect(lwidget,SIGNAL(changeVideoSeetSignal(QString,int,int)),this,SLOT(changeVideoSeetSlot(QString,int,int)));
	connect(lwidget,SIGNAL(showRTCPMessageSignal(bool,int)),this,SLOT(onShowRTCPMessage(bool,int)));
	connect(lwidget,SIGNAL(documentSignal(int)),tcontroller,SLOT(onDocumentClicked(int)));
	connect(lwidget,SIGNAL(autoLayoutSignal(int)),tcontroller,SLOT(onAutoLayoutClicked(int)));


    connect(rwidget,SIGNAL(oneAndOneSignal(int)),tcontroller,SLOT(onOneAndOneClicked(int)));
    connect(rwidget,SIGNAL(twoAndTwoSignal(int)),tcontroller,SLOT(onTwoAndTwoClicked(int)));
    connect(rwidget,SIGNAL(threeAndThreeSignal(int)),tcontroller,SLOT(onThreeAndThreeClicked(int)));
    connect(rwidget,SIGNAL(onePlusFiveLSignal(int)),tcontroller,SLOT(onLClicked(int)));
    connect(rwidget,SIGNAL(receiveVideoSignal(QString,int,int)),this,SLOT(receiveVideoSlot(QString,int,int)));
    connect(rwidget,SIGNAL(stopVideoSignal(QString,int)),this,SLOT(stopVideoSlot(QString,int)));
    connect(rwidget,SIGNAL(changeVideoSeetSignal(QString,int,int)),this,SLOT(changeVideoSeetSlot(QString,int,int)));
    connect(rwidget,SIGNAL(showRTCPMessageSignal(bool,int)),this,SLOT(onShowRTCPMessage(bool,int)));
    connect(rwidget,SIGNAL(documentSignal(int)),tcontroller,SLOT(onDocumentClicked(int)));
	connect(rwidget,SIGNAL(autoLayoutSignal(int)),tcontroller,SLOT(onAutoLayoutClicked(int)));

	connect(screenWidget2,SIGNAL(oneAndOneSignal(int)),tcontroller,SLOT(onOneAndOneClicked(int)));
	connect(screenWidget2,SIGNAL(twoAndTwoSignal(int)),tcontroller,SLOT(onTwoAndTwoClicked(int)));
	connect(screenWidget2,SIGNAL(threeAndThreeSignal(int)),tcontroller,SLOT(onThreeAndThreeClicked(int)));
	connect(screenWidget2,SIGNAL(onePlusFiveLSignal(int)),tcontroller,SLOT(onLClicked(int)));
	connect(screenWidget2,SIGNAL(receiveVideoSignal(QString,int,int)),this,SLOT(receiveVideoSlot(QString,int,int)));
	connect(screenWidget2,SIGNAL(stopVideoSignal(QString,int)),this,SLOT(stopVideoSlot(QString,int)));
	connect(screenWidget2,SIGNAL(changeVideoSeetSignal(QString,int,int)),this,SLOT(changeVideoSeetSlot(QString,int,int)));
	connect(screenWidget2,SIGNAL(showRTCPMessageSignal(bool,int)),this,SLOT(onShowRTCPMessage(bool,int)));
	connect(screenWidget2,SIGNAL(documentSignal(int)),tcontroller,SLOT(onDocumentClicked(int)));
	connect(screenWidget2,SIGNAL(autoLayoutSignal(int)),tcontroller,SLOT(onAutoLayoutClicked(int)));

	connect(screenWidget3,SIGNAL(oneAndOneSignal(int)),tcontroller,SLOT(onOneAndOneClicked(int)));
	connect(screenWidget3,SIGNAL(twoAndTwoSignal(int)),tcontroller,SLOT(onTwoAndTwoClicked(int)));
	connect(screenWidget3,SIGNAL(threeAndThreeSignal(int)),tcontroller,SLOT(onThreeAndThreeClicked(int)));
	connect(screenWidget3,SIGNAL(onePlusFiveLSignal(int)),tcontroller,SLOT(onLClicked(int)));
	connect(screenWidget3,SIGNAL(receiveVideoSignal(QString,int,int)),this,SLOT(receiveVideoSlot(QString,int,int)));
	connect(screenWidget3,SIGNAL(stopVideoSignal(QString,int)),this,SLOT(stopVideoSlot(QString,int)));
	connect(screenWidget3,SIGNAL(changeVideoSeetSignal(QString,int,int)),this,SLOT(changeVideoSeetSlot(QString,int,int)));
	connect(screenWidget3,SIGNAL(showRTCPMessageSignal(bool,int)),this,SLOT(onShowRTCPMessage(bool,int)));
	connect(screenWidget3,SIGNAL(documentSignal(int)),tcontroller,SLOT(onDocumentClicked(int)));
	connect(screenWidget3,SIGNAL(autoLayoutSignal(int)),tcontroller,SLOT(onAutoLayoutClicked(int)));

    AdjustScreenLayout();



    voicewidget = new VoiceWidget();
    voicewidget->setParent(this);
    voicewidget->hide();

    connect(voicewidget,SIGNAL(changeVoiceSignal(QString,int)),tcontroller,SLOT(onVoiceChange(QString,int)));
    QPoint pos = this->mapToGlobal(this->voiceToolButton->pos());
    voicewidget->setGeometry(pos.x(),pos.y()-120,80,120);
    connect(this->voiceToolButton,SIGNAL(clicked(bool)),this,SLOT(showVoiceSliderWidget(bool)));

    //connect(tcontroller,SIGNAL(GetMyTerminalSignal()),this,SLOT(refreshMyTerminals()));
    //connect(tcontroller,SIGNAL(GetExitProcessSignal()),this,SLOT(GetExitProcessSlot()));
    //connect(tcontroller,SIGNAL(GetLayoutChangeSignal(ScreenInfo)),this,SLOT(GetLayoutChangeSlot(ScreenInfo)));
    //connect(tcontroller,SIGNAL(GetMemberLocationSignal(QVector<MemberLocation>)),this,SLOT(GetMemberLocationSlot(QVector<MemberLocation>)));


    timer = new QTimer();
    timer->setSingleShot(true);
    connect(this->timer,SIGNAL(timeout()),this,SLOT(setupMemberVideo()));//(setupTerminalLayout()));

    //this->InitializeMWidget();
    QString stopvoice = QString::fromLocal8Bit("停止发送音频");
    this->selfSoundOffAction = new QAction(QString::fromLocal8Bit("停止发送音频"),this);
    this->selfVideoOffAction = new QAction(QString::fromLocal8Bit("停止发送视频"),this);
	this->allRecvAudioOffAction = new QAction(QString::fromLocal8Bit("停止接收音频"),this);
	this->allowSpeakAction = new QAction(QString :: fromLocal8Bit("允许发言"),this);
	this->forbidSpeakAction = new QAction(QString :: fromLocal8Bit("禁止发言"),this);

    this->selfSoundOffAction->setCheckable(true);
    this->selfVideoOffAction->setCheckable(true);
	this->allRecvAudioOffAction->setCheckable(true);
	this->allowSpeakAction->setChecked(true);
	this->forbidSpeakAction->setChecked(true);

    stopToolButton->addAction(this->selfSoundOffAction);
    stopToolButton->addAction(this->selfVideoOffAction);
	stopToolButton->addAction(this->allRecvAudioOffAction);
	SpeakingToolButton->addAction(this->allowSpeakAction);
	SpeakingToolButton->addAction(this->forbidSpeakAction);
	
    connect(this->stopToolButton,SIGNAL(clicked()),this->stopToolButton,SLOT(showMenu()));
    connect(this->selfSoundOffAction,SIGNAL(triggered(bool)),tcontroller,SLOT(StopMyAudio(bool)));
    connect(this->selfVideoOffAction,SIGNAL(triggered(bool)),tcontroller,SLOT(StopMyVideo(bool)));
	connect(this->allRecvAudioOffAction , SIGNAL(triggered(bool) ) , this , SLOT(stopAllRecvAudio(bool) ) );

	connect(this->SpeakingToolButton,SIGNAL(clicked()),this->SpeakingToolButton,SLOT(showMenu()));
	connect(this->allowSpeakAction,SIGNAL(triggered(bool)),this,SLOT(MemberAllowSpeakHandler()));
	connect(this->forbidSpeakAction,SIGNAL(triggered(bool)),this,SLOT(MemberForbidSpeakHandler()));
	
	currentTermianlNum=0;
	totalTermianlNum =999;

	this->m_isAutoRecv = true;
}

MemberListWidget::~MemberListWidget()
{
    //delete lwidget;
    //delete rwidget;
	if(screenWidget0 != NULL)
	delete screenWidget0;
	if(screenWidget1 != NULL)
	delete screenWidget1;
	if(screenWidget2 != NULL)
	delete screenWidget2;
	if(screenWidget3 != NULL)
	delete screenWidget3;
    delete memberTable;
 //   delete tcontroller;
   // delete terminalTable;
    delete voicewidget;
    delete memberTreeWidget;
    delete treeWidget;
    foreach(HeadWidget *widget,terminalheadlist)
    {
        delete widget;
    }
    foreach(HeadWidget *widget,memberheadlist)
    {
        delete widget;
    }
    foreach(TerminalDetail *terminal,this->onterminallist)
    {
        delete terminal;
    }
    foreach(TerminalDetail *terminal,this->offterminallist)
    {
        delete terminal;
    }
    this->onterminallist.clear();
    this->offterminallist.clear();
	terminalsMap.clear();
    delete lmenu;
    delete rmenu;
    delete timer;
	
	disconnect( terminalListCheckTimer , SIGNAL(timeout()) , this , SLOT(checkTerminalListSlot()));
	if(terminalListCheckTimer) 
	{
		delete terminalListCheckTimer ;  
		terminalListCheckTimer=NULL ;
	}
	

    qDebug()<<"delete memberlistwidget";
}

bool MemberListWidget::checkForDuplicate(const QVariantMap &variantMap)
{
    //QVariant uri = variantMap["uri"]; // TODO
	QVariant uri = variantMap["virtualuri"];
    QString uriStr = uri.toString();
    QVariant status = variantMap["status"];
    QString statusStr = status.toString();
    int l = memberTable->rowCount();
    for(int i = 0; i<l; i++)
    {
        if(uriStr.compare(memberTable->item(i,0)->text()) == 0&&statusStr.compare(memberTable->item(i,6)->text()))
        {
            return true;
        }
    }
    return false;

}

bool MemberListWidget::tCheckForDuplicate(TerminalDetail *terminal)
{
   // if(this->onterminallist.empty())
  //if(!this->onterminallist.contains(*terminal))
    if(terminal->statusStr == "Not Available")
    {
        if(this->searchTerminal(terminal->virtualUriStr, this->offterminallist)<0)
        {
            return false;
        }
    }else
    {
        if(this->searchTerminal(terminal->virtualUriStr, this->onterminallist)<0)
        {
            return false;
        }
    }

    //delete terminal;
    return true;
}

void MemberListWidget::addMembers(const QList<QVariantMap> &memberList)
{
  //  qDebug()<<"member-widget:addMembers"<<memberList.at(0);
    //memberTable->clearContents();
    if(memberList.isEmpty())
    {
        return;
    }
    QStringList idlist;
    if(this->checkForDuplicate(memberList.at(0)))
    {
        return;
    }
    int tablelength = memberTable->rowCount();
    //qDebug()<<"tlength:"<<tablelength;
    int length = (memberList.length()+tablelength);
   //qDebug()<<"tlength:"<<length;
    memberTable->setRowCount(length);
    for (int i=tablelength; i<length; i++)
    {
        QVariantMap variantMap = memberList.at(i-tablelength);
        QVariant uri = variantMap["uri"];
        QString uriStr = uri.toString();
        QTableWidgetItem *uriitem = new QTableWidgetItem(uriStr);
        memberTable->setItem(i,0,uriitem);

        QString imName = uriStr.left(uriStr.indexOf("@"));
        idlist.append(imName.toLower());
        QVariant member = variantMap["name"];
        QString memberName = member.toString();
        QTableWidgetItem *nameitem = new QTableWidgetItem(memberName);
        memberTable->setItem(i,1,nameitem);
//        qDebug()<<"name:"<<memberName;

        QVariant floor = variantMap["floor"];
        QString floorStr = floor.toString();
        QTableWidgetItem *flooritem = new QTableWidgetItem(floorStr);
        memberTable->setItem(i,2,flooritem);

        QVariant muladdress = variantMap["multicastAddress"];
        QString muladdressStr = muladdress.toString();
        QTableWidgetItem *muladdressitem = new QTableWidgetItem(muladdressStr);
        memberTable->setItem(i,3,muladdressitem);


        QVariant location = variantMap["location"];
        QString locationStr = location.toString();
        QTableWidgetItem *locationitem = new QTableWidgetItem(locationStr);
        memberTable->setItem(i,4,locationitem);

        QTableWidgetItem *handupitem = new QTableWidgetItem("");
        memberTable->setItem(i,5,handupitem);

        QVariant status = variantMap["status"];
        QString statusStr = status.toString();
        QTableWidgetItem *statusitem = new QTableWidgetItem(statusStr);
        memberTable->setItem(i,6,statusitem);

        if(this->meetingroomlist.count()>0)
        {
            for(int i = 0 ; i < this->meetingroomlist.count();i++)
            {
                if(this->meetingroomlist.at(i) == locationStr)
                {
                    break;
                }
            }
            this->meetingroomlist.append(locationStr);
        }else
        {
            this->meetingroomlist.append(locationStr);
        }
        QString temp = uriStr;
        if(temp.left(temp.indexOf("@")) == username)
        {
            QVariant myterminal = variantMap["AllowTerminal"];
            QString myterminalStr = myterminal.toString();

            myterminallist = myterminalStr.split("\t");
            if(myterminallist.last() == "")
            {
                myterminallist.removeLast();
            }
            qDebug()<<"MyTerminalsList:"<<myterminallist;
            this->refreshMyTerminals();
        }

        this->addMemberTreeWidget(locationStr,memberName,statusStr);
    }
   // this->deviceChangedSlot(this->terminalname);

}

void MemberListWidget::addMemberTreeWidget(const QString &roomname,const QString &membername,const QString &status)
{
    int count = this->memberTreeWidget->invisibleRootItem()->childCount();

        for(int k = 0; k<count;k++)
        {
            //qDebug()<<"memberTreeWidget!!!:"<<k<<count<<roomname;
            if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(0) ==roomname)
            {
                QTreeWidgetItem *item = this->memberTreeWidget->invisibleRootItem()->child(k);
                for(int i = 0 ; i<item->childCount();i++)
                {
                    if(item->child(i)->text(0) ==membername)
                    {
                        QTreeWidgetItem *childitem =item->child(i);
                        childitem->setText(1,status);
                        return;
                    }
                }
                QTreeWidgetItem *childitem = new QTreeWidgetItem(this->memberTreeWidget->invisibleRootItem()->child(k));
                childitem->setText(0,membername);
                childitem->setText(2,"");
                childitem->setText(1,status);
                return;
            }
        }
        QTreeWidgetItem *item= new QTreeWidgetItem(this->memberTreeWidget->invisibleRootItem());
        item->setText(0,roomname);
        //qDebug()<<"%%%%%%%%%%%%%%0"<<roomname;
        QTreeWidgetItem *childitem = new QTreeWidgetItem(item);
        childitem->setText(0,membername);
        childitem->setText(2,"");
        childitem->setText(1,status);

}

void MemberListWidget::removeMemberTreeWidget(const QString &roomname,const QString &membername)
{
    int count = this->memberTreeWidget->invisibleRootItem()->childCount();

        for(int k = 0; k<count;k++)
        {
            if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(0) ==roomname)
            {        
                QTreeWidgetItem *item =this->memberTreeWidget->invisibleRootItem()->child(k);

                for(int i = 0 ; i<item->childCount();i++)
                {
                    if(item->child(i)->text(0) ==membername)
                    {

                        QTreeWidgetItem *childitem =item->child(i);
                        childitem->setText(1,"Not Available");
                        childitem->setText(2,"");
                        return;
                    }
                }
            }
        }
}

void MemberListWidget::handMemberTreeWidget(const QString &roomname,const QString &membername,bool check)
{

    int count = this->memberTreeWidget->invisibleRootItem()->childCount();
    qDebug()<<"handMemberTreeWidget"<<count<<roomname<<membername<<check;
        for(int k = 0; k<count;k++)
        {
            if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(0) ==roomname)
            {
                QTreeWidgetItem *item =this->memberTreeWidget->invisibleRootItem()->child(k);
                for(int i = 0 ; i<item->childCount();i++)
                {
                    if(item->child(i)->text(0) ==membername)
                    {
                        QTreeWidgetItem *childitem =item->child(i);
                        if(check)
                        {
                            childitem->setText(2,QString::fromLocal8Bit("举手"));
                        }else
                        {
                            childitem->setText(2,"");
                        }
                        return;
                    }
                }
            }
        }
}

void MemberListWidget::SetAutoRecv(bool isauto)
{
	this->m_isAutoRecv = isauto;
}


void MemberListWidget::addMember(const QVariantMap& variantMap)
{
    if(variantMap.isEmpty())
    {
      //  qDebug()<<"member is empty!";
        return;
    }

    QVariant uri = variantMap["uri"];
    QString uriStr = uri.toString();
    QVariant member = variantMap["name"];
    QString memberName = member.toString();
    QVariant floor = variantMap["floor"];
    QString floorStr = floor.toString();
    QVariant muladdress = variantMap["multicastAddress"];
    QString muladdressStr = muladdress.toString();
    QVariant location = variantMap["location"];
    QString locationStr = location.toString();
    QVariant status = variantMap["status"];
    QString statusStr = status.toString();
   // QString memberName =  uriStr.left( uriStr.indexOf("@") );

    for(int i = 0; i < this->memberTable->rowCount();i++)
    {
        if(this->memberTable->item(i,0)->text() == uriStr)
        {
            this->memberTable->item(i,2)->setText(floorStr);
            this->memberTable->item(i,3)->setText(muladdressStr);
            this->memberTable->item(i,4)->setText(locationStr);
            this->memberTable->item(i,6)->setText(statusStr);
            this->addMemberTreeWidget(locationStr,memberName,statusStr);
          //  qDebug()<<"member add!"<<statusStr;
            return;
        }
    }

    int j = memberTable->rowCount();
    memberTable->setRowCount(j+1);

    QTableWidgetItem *item = new QTableWidgetItem(uriStr);
    memberTable->setItem(j,0,item);


    QTableWidgetItem *nameitem = new QTableWidgetItem(memberName);
    memberTable->setItem(j,1,nameitem);

    QTableWidgetItem *flooritem = new QTableWidgetItem(floorStr);
    memberTable->setItem(j,2,flooritem);

    QTableWidgetItem *muladdressitem = new QTableWidgetItem(muladdressStr);
    memberTable->setItem(j,3,muladdressitem);

    QTableWidgetItem *locationitem = new QTableWidgetItem(locationStr);
    memberTable->setItem(j,4,locationitem);

    QTableWidgetItem *statusitem = new QTableWidgetItem(statusStr);
    memberTable->setItem(j,6,statusitem);

    this->addMemberTreeWidget(locationStr,memberName,statusStr);
}

void MemberListWidget::terminalLogout(const QString&terminalURI)
{
	/*for(int i = 0; i<memberTable->rowCount();i++)
	{
	if(sipuri == memberTable->item(i,0)->text())
	{
	this->removeMemberTreeWidget(memberTable->item(i,4)->text(),memberTable->item(i,1)->text());

	if( memberTable->item( i,6 ) )
	{
	memberTable->item(i,6)->setText("Not Available");
	}
	if( memberTable->item( i , 5 ) )
	{
	memberTable->item(i,5)->setText("");
	}
	return;
	}
	}*/

	TerminalVector & virtualList = terminalsMap[terminalURI];
	for (TerminalVector::iterator it = virtualList.begin(); it != virtualList.end(); ++it)
	{
		QString virtualURI = (*it)->virtualUriStr;
		int index = this->searchTerminal(virtualURI, this->onterminallist);
		if(index < 0)
		{
			continue;
		}

		qDebug()<<"MemberLeft:"<<virtualURI<<index;
		TerminalDetail *terminal = this->onterminallist.at(index);
		terminal->isSpeaking="false";
		terminal->isHandUp="false";
		terminal->isChairmanTerminal="false";
		terminal->isMainSpeaker="false";
		this->insertTerminal(terminal,this->offterminallist);
		terminal->statusStr = "Not Available";
		terminal->isShown = false;
		terminal->isReceivedAudio = false;

		this->onterminallist.remove(index);
		this->setupMemberVideo();

		//if(sipuri == RunningProfile::getInstance()->getUserURI().c_str())
		//{
		//    tcontroller->QueryTeminal();
		//}
		this->refreshMyTerminals();

		this->_uiUpdateForMemberLogout( virtualURI );
	}

}

void MemberListWidget::memberHandUp(const QString &sipuri)
{
 
	for(int i=0; i<terminalheadlist.size();i++)
	{
		 qDebug()<<"compare"<<i<<sipuri;
		if(sipuri == terminalheadlist[i]->getUri())
		{
			qDebug()<<"hand up success";
			QIcon icon;
			icon.addFile(QString::fromUtf8(":/image/hire-me.png"), QSize(), QIcon::Normal, QIcon::Off);
			terminalheadlist[i]->handUpMarkButton->setIcon(icon);
			terminalheadlist[i]->setHandUpMarkHidden(false);
			int index = this->searchTerminal(sipuri,this->onterminallist);
			if(index >0)
			{
				onterminallist.at(index)->isHandUp="true";
			}			
			break;
		}
	}

}

void MemberListWidget::memberHandDown(const QString& sipuri)
{


	for(int i=0; i<terminalheadlist.size();i++)
	{
		qDebug()<<"compare"<<i<<sipuri;
		if(sipuri == terminalheadlist[i]->getUri())
		{
			 qDebug()<<"hand down success";
			 QIcon icon;
			 icon.addFile(QString::fromUtf8(":/image/hire-me.png"), QSize(), QIcon::Normal, QIcon::Off);
			 terminalheadlist[i]->handUpMarkButton->setIcon(icon);
			 terminalheadlist[i]->setHandUpMarkHidden(true);
			 int index = this->searchTerminal(sipuri,this->onterminallist);
			 if(index >0)
			 {
				 onterminallist.at(index)->isHandUp="false";
			 }			
			 break;

		}
	}
}

void MemberListWidget::closeConference()
{
        int reply = QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("确认退出会议吗？"),QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes)
        {
            this->releaseResouce();

       //     this->changeButtonNameSlot(false);                                                 //2.21

            ChangeScreenTo1And2Slot();
			InitialConferenceRoom();
			emit quitConfSIGNAL();
        }
        else
        {
            return ;
        }

}





void MemberListWidget::receiveVideoSlot(const QString& name, int screenIndex, int seet)
{
    if(seet==9)
    {
		
        emit this->ReceiveDocumentRequestSignal(name,screenIndex);
        return;
    }

    emit ReceiveVideoRequestSignal(name,screenIndex,seet+1);
    int onindex = this->searchTerminal(name,this->onterminallist);
    if(onindex>=0)
    {//head
        //HeadWidget *video = this->terminalheadlist[name];
        //video->isShown = true;
        this->onterminallist.at(onindex)->isShown = true;
        this->onterminallist.at(onindex)->isReceivedAudio = true;
        this->terminalheadlist[onindex]->shownChange(true);
        this->terminalheadlist[onindex]->receiveAudioMarkChange(this->terminalheadlist[onindex]->getUri(),true);
        emit CheckMarkCleanSignal(true);
        return;
    }else
    {
        int offindex = this->searchTerminal(name,this->offterminallist);
        if(offindex>=0)
        {
            //head
           // HeadWidget *video = this->terminalheadlist[name];
           // video->isShown = true;
            emit CheckMarkCleanSignal(true);
            return;
        }
    }



}


void MemberListWidget::stopVideoSlot(const QString& name, int screenIndex)
{
    emit StopVideoRequestSignal(name, screenIndex);
    int onindex = this->searchTerminal(name,this->onterminallist);
    if(onindex>=0)
    {//head
        //HeadWidget *video = this->terminalheadlist[name];
        //video->isShown = false;
        this->onterminallist.at(onindex)->isShown = false;
        this->onterminallist.at(onindex)->isReceivedAudio = false;
        this->terminalheadlist[onindex]->shownChange(false);
        this->terminalheadlist[onindex]->receiveAudioMarkChange(this->terminalheadlist[onindex]->getUri(),false);
        return;
    }else
    {
        int offindex = this->searchTerminal(name,this->offterminallist);
        if(offindex>=0)
        {
            this->offterminallist.at(offindex)->isShown = false;
            this->terminalheadlist[this->onterminallist.size()+offindex]->shownChange(false);
            //head
            //HeadWidget *video = this->terminalheadlist[name];
           // video->isShown = false;
            return;
        }
    }
   /* for(int i = 0; i < this->terminalTable->rowCount() ; i++)
    {
        if(terminalTable->item(i,0)->text()== name)
        {
            qDebug()<<"name"<<name;

            //widget->videoMarkButton->hide();
            terminalTable->item(i,2)->setBackgroundColor(QColor(255, 255, 255));
       //     if(lwidget->videoIsInThisScreen(name)<0&&rwidget->videoIsInThisScreen(name)<0)
       //     {
                terminalheadlist.at(i)->isShown = false;
               // this->changeButtonNameSlot(false);              //2.21
      //      }
            return;
         }
    }*/
}

void MemberListWidget::changeVideoSeetSlot(const QString& name, int screenIndex, int seet)
{
    emit this->changeVideoRequestSignal(name,screenIndex,seet);
}

void MemberListWidget::onIMButtonClicked()
{
    QStringList list;
    for(int i = 0; i < memberheadlist.count() ; i++)
    {
        if(memberheadlist.at(i)->isChosen)
        {
            QString name = memberheadlist.at(i)-> memberNameLabel->text().toLower();
            list.append(name);
         }
    }
    emit CheckMarkCleanSignal(true);
   // emit SWCheckMarkCleanSignal(true);
    emit addContactSignal(list);
   // emit startIMSignal(list);
}

void MemberListWidget::deviceChangedSlot(const QString& deviceName)
{

//    if(deviceName =="")
//    {
//        lwidget->showSelfHead("","","");
//        lwidget->clearVideo();
//        rwidget->clearVideo();
//   //     tcontroller->onDisconnectClicked();
//
//      //  terminalname = deviceName;
//        return;
//    }
//
//  //  terminalname = deviceName;
//    qDebug()<<"deviceChangedSlot:"<<deviceName;//<<terminalname;
//    //QString tname =tcontroller->getConnectedTerminal();
//    QString name;
//    QString uri;
//
////head
//    for(int i = 0 ; i<this->onterminallist.size();i++)
//    {
//        TerminalDetail *terminal = this->onterminallist.at(i);
//        uri =  terminal->uriStr;
//
//        if(uri == tname)
//        {
//            //HeadWidget *video = this->terminalheadlist[uri];
//            name = terminal->nameStr;
//            //video->isShown = true;
//            break;
//        }
//    }
//    lwidget->showSelfHead(":/image/users_group.png",uri,name);
//    /*for(int i = 0 ; i<this->terminalTable->rowCount();i++)
//    {
//        uri =  this->terminalTable->item(i,0)->text();
//        QString temp = this->terminalTable->item(i,0)->text();
//        QString temp1 = temp.left(temp.indexOf("@"));
//       if( temp1 == tname)
//        {
//           name= this->terminalTable->item(i,2)->text();
//           this->terminalheadlist.at(i)->isShown = true;
//           break;
//       }
//    }*/
//
//   // this->changeButtonNameSlot(true);           //2.21
}

void MemberListWidget::onShowRTCPMessage(bool check, int index)
{
    //if(tcontroller->getConnectedTerminal() == "")
    //{
    //    QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("没有选择终端，请返回终端选择界面进行终端选择。"),QMessageBox::Ok);
    //    //lwidget->showRTCPCheckBox->setChecked(false);             //2.21
    //    //rwidget->showRTCPCheckBox->setChecked(false);             //2.21
    //    return ;
    //}
    tcontroller->onShowRTCPMessage(check,index);
 //   emit showRTCPMessageSignal(check,index);
}

void MemberListWidget::checkMarkClean()
{
    emit CheckMarkCleanSignal(true);
}

/*void MemberListWidget::sWCheckMarkClean()
{
    emit SWCheckMarkCleanSignal(true);
}*/

/*void MemberListWidget::changeButtonNameSlot(bool check)
{
    if(check  == true)
    {
        this->receiveButton->setText(QString::fromLocal8Bit("音频开"));
        this->receiveButton_2->setText(QString::fromLocal8Bit("视频开"));
    }
    else
    {
        this->receiveButton->setText(QString::fromLocal8Bit("音频关"));
         this->receiveButton_2->setText(QString::fromLocal8Bit("视频关"));
    }
}*/                                //2.21

void MemberListWidget::showVoiceSliderWidget(bool check)
{
    QPoint pos = this->voiceToolButton->pos();

	voicewidget->loadSystemVolumn();
    voicewidget->setGeometry(pos.x(),pos.y()-200,95,200);
    this->voicewidget->setVisible(check);
}

void MemberListWidget::dragEnterEvent(QDragEnterEvent *event)
{

        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }

}

void MemberListWidget::dragMoveEvent(QDragMoveEvent *event)
{

        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }

}

void MemberListWidget::dropEvent(QDropEvent *event)
{
        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
            /*for(int i=0; i<this->terminalheadlist.size();i++)
            {
                if(this->terminalheadlist[i]->getUri()==source2->getMemberUri())
                {
                    this->terminalheadlist[i]->frame->setStyleSheet("QFrame{border-width:2px;border-color:darkGray;border-style:solid;border-radius: 5px;}");
                    break;
                }
            }*/
            //this->setHeadShow(event->mimeData()->text());
           // this->headWidget->show();
            if(source2->getIndex()==9)
            {
                emit this->StopDocumentRequestSignal(source2->getMemberUri(),source2->getScreenIndex());
            }else
            {
                this->stopVideoSlot(source2->getMemberUri(),source2->getScreenIndex());
            }

            source2->setHeadHide();

          //  qDebug()<<"(**************"<<source2->getMemberUri();
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }

}

QString MemberListWidget::getTerminalName()
{
    //return tcontroller->getConnectedTerminal();
	return QString("");
}

PeerShowWidget* MemberListWidget::searchPeerForWidget(const QString& name)
{
	
	int screen0 = screenWidget0->videoIsInThisScreen(name);
	int screen1 = screenWidget1->videoIsInThisScreen(name);
	int screen2 = screenWidget2->videoIsInThisScreen(name);
	int screen3 = screenWidget3->videoIsInThisScreen(name);
    if(screen0>=0)
    {
        return screenWidget0->getPeerShowWidget(screen0);
    }else
		if(screen1>=0)
		{
			return screenWidget1->getPeerShowWidget(screen1);
		}else
			if(screen2>=0)
			{
				return screenWidget2->getPeerShowWidget(screen2);
			}else
				if(screen3>=0)
				{
					return screenWidget3->getPeerShowWidget(screen3);
				}else
					{
						return NULL;
					}
}


/*******************************1.16 dhx***********************************/
void MemberListWidget::resizeEvent(QResizeEvent *event)       //1.16 dhx
{
/*    int w = event->size().width();
    int h = event->size().height();
    int width = (w-2)/2;
    int height = width*9/16;
    this->showWidget_L->resize(width,height);
    this->showWidget_R->resize(width,height);

    lwidget->resize(width,height);
    rwidget->resize(width,height);

    if(mwidth<800&&mheight<550)
    {
        //int swidth = w-2;
        int sheight = h-height-68;
        this->memberVideoScrollArea->setMaximumHeight(sheight);
        this->memberVideoScrollArea->setMinimumHeight(sheight);
    }
    else
    {
        this->memberVideoScrollArea->setMaximumHeight(160);
        this->memberVideoScrollArea->setMinimumHeight(160);
    }*/

    //this->memberVideoScrollArea->resize(w-2,h-height-this->quitConferenceButton->height()-3-this->ongoingConferenceLabel->height());

}
/*******************************1.16 dhx end***********************************/

void MemberListWidget::addTerminals(const MapList & tlist)
{
    int length = tlist.length();

    for (int i=0; i<length; i++)
    {
        TerminalDetail *terminal = new TerminalDetail(tlist.at(i));

		if(this->tCheckForDuplicate(terminal))
		{
			//目前SipWrapper存在Bug，会不定时向服务器重复发送终端的Qos_Login消息，导致接收终端误认为
			//发送Login消息的终端重复登录，并将正在接收的音视频流清空。由于SipWrapper的Bug比较难找，
			//目前在终端屏蔽重复登录时的音视频清空动作。本想在屏蔽条件里增加对目标终端当前通信状态的
			//判断，不过通信Qos信息存储在ConferenceRoom进程，改起来比较麻烦，以后有时间再改进，目前一
			//律屏蔽处理。即注释掉下面一行代码。		by Ruan 2013/1/14

			//如果终端重复登录，那么该终端的音视频流等信息应该清空
			//this->_uiUpdateForMemberLogout( t->uriStr );

			delete terminal;
			continue;
		}
        
		this->checkBeforeInsertTerminal(terminal);
		
		//自动接收对方媒体流
		this->autoRecvTerminalMedia(terminal);
    }
    //this->setupMemberVideo();
    //this->refreshMyTerminals();

}

void MemberListWidget::checkBeforeInsertTerminal(TerminalDetail *terminal)
{
	TerminalVector & virtualList = terminalsMap[terminal->uriStr]; // Map不会溢出，如果不存在将返回空Vector
	TerminalVector::iterator it = virtualList.begin();
	while (it != virtualList.end())
	{
		if ((*it)->virtualUriStr == terminal->virtualUriStr)
		{
			// Map中的数据指向与offterminallist和onoffterminallist相同，留给相关逻辑去删除
			(*it) = terminal;
			break;
		}
		++it;
	}
	if (it == virtualList.end())
	{
		virtualList.push_back(terminal);
	}

	terminal->uriStr = terminal->virtualUriStr; // TODO:应修改每个方法向控制器传virtualUri，目前是临时方案

    if(terminal->statusStr == "Not Available")
    {
        this->insertTerminal(terminal,offterminallist);
        this->insertTHead(terminal);
		//currentTermianlNum++;

    }else
    {
		// 可能是终端登录调用的插入操作
        int index = this->searchTerminal(terminal->uriStr,this->offterminallist);
        if(index<0)
        {
            
			this->insertTerminal(terminal,onterminallist);
            this->insertTHead(terminal);

			//如果是主讲人，主动接收其视频
			if(terminal->isMainSpeaker =="true")
				SetMemberWindowToFirstPosition(terminal->uriStr);
			
			//currentTermianlNum++;

        }else
        {
			TerminalDetail *temp = this->offterminallist.at(index);
			delete temp;
			this->offterminallist.remove(index);
            this->insertTerminal(terminal,onterminallist);
        }
    }

	currentTermianlNum = terminalsMap.size();
	if(currentTermianlNum >= totalTermianlNum)
	{
		this->setupMemberVideo();
		if(!isFirstFinishTerminalLIst)
		{
			isFirstFinishTerminalLIst=true;
			onMemberFirstFinishTerminalLIstSignal();
			this->AutoRecvMedia();
		}
	}

}

void MemberListWidget::AutoRecvMedia()
{
	if(m_isAutoRecv == false || !isFirstFinishTerminalLIst || onterminallist.isEmpty())
		return;

	for(int i=0; i<onterminallist.size(); i++)
	{
		autoRecvTerminalMedia(onterminallist.at(i));
	}
}

void MemberListWidget::autoRecvTerminalMedia(TerminalDetail *terminal)
{
	if(m_isAutoRecv == false || !isFirstFinishTerminalLIst)
		return;

	if(terminal->statusStr == "Not Available")
		return;

	//如果对应的uri已经在接收，那么直接返回
	PeerShowWidget* peer = this->searchPeerForWidget( terminal->uriStr );
	if( peer )
		return;

	//int maxScreenIndex = QApplication::desktop()->numScreens() - 1;
	int freeScreenIndex = -1;
	int freeSeet = -1;

	if((freeSeet = screenWidget0->getFreeSeet()) != -1)
		freeScreenIndex = 0;
	else if((freeSeet = screenWidget1->getFreeSeet()) != -1)
		freeScreenIndex = 1;
	else if((freeSeet = screenWidget2->getFreeSeet()) != -1)
		freeScreenIndex = 2;
	else if((freeSeet = screenWidget3->getFreeSeet()) != -1)
		freeScreenIndex = 3;

	if(freeScreenIndex == -1)
		return;

	startRecvRemoteMember(terminal->uriStr, freeScreenIndex, freeSeet + 1);
}

void MemberListWidget::insertTerminal(TerminalDetail *terminal, QVector<TerminalDetail *> & terminallist)
{
	int i = 0 ;
	while( i<terminallist.size())
	{
		TerminalDetail *temp = terminallist.at(i);
		if(*temp>=*terminal)
		{
			break;
		}
		i++;
	}

	if(i == terminallist.size())
	{
		terminallist.append(terminal);
	}else
	{
		terminallist.insert(i,terminal);
	}
	
	/*if( terminal&& terminal->uriStr.toStdString()==RunningProfile::getInstance()->user_uri()  )*/
	if( terminal && terminal->uriStr.toStdString().find(RunningProfile::getInstance()->user_uri()) != std::string::npos )
	{
		int bIndex = terminal->virtualUriStr.indexOf('v') + 1;
		int eIndex = terminal->virtualUriStr.indexOf('#');
		int virtualIndex = 6; // 从自动布局右下角开始往前分派本地回显窗格
		if (bIndex > -1 && eIndex > bIndex)
		{
			virtualIndex = 6 - terminal->virtualUriStr.mid(bIndex, eIndex - bIndex).toInt() + 1;
		}
		//通知控制器创建本地回显接收流
		startRecvRemoteMember(terminal->virtualUriStr, MediaManager::getInstance()->GetLocalPreviewScreen(), virtualIndex);
	}
}

void MemberListWidget::insertTHead(TerminalDetail *terminal)
{
    this->addTtoMemberTreeWidget(terminal->locationStr,terminal->nameStr);
   // if((this->onterminallist.size()+this->offterminallist.size())>40)
    //{qDebug()<<"1111";
        HeadWidget *video = new HeadWidget(this);
        int count = this->onterminallist.size()+this->offterminallist.size()-1;
        this->terminalheadlist.insert(count,video);
        this->setupTerminalLayout();
    //}
    if(timer->isActive())
    {
        timer->stop();
    }
    timer->start(3000);

   // this->setupMemberVideo();
}

void MemberListWidget::setupMemberVideo()
{
   // if(this->terminalheadlist.count()>24)
   // {

    if(timer->isActive())
    {
        timer->stop();
    }

    qDebug()<<"setupMemberVideo start:"<<QTime::currentTime();
    for(int i = 0 ; i<this->onterminallist.size();i++)
    {
       // this->terminalheadlist[i]->frame->show();
        this->terminalheadlist[i]->setHead(onterminallist.at(i),":/image/users_group.png");
    }
    for(int j = this->onterminallist.size();j<(this->onterminallist.size()+this->offterminallist.size());j++)
    {
       // this->terminalheadlist[j]->frame->show();
        this->terminalheadlist[j]->setHead(this->offterminallist.at(j-this->onterminallist.size()),":/image/users_group.png");
    }
   // int last = this->onterminallist.size()+this->offterminallist.size();
 /*   for(int last=this->onterminallist.size()+this->offterminallist.size();last<this->terminalheadlist.size();last++)
    {
        this->terminalheadlist[last]->frame->hide();
    }*/
    qDebug()<<"setupMemberVideo end:"<<QTime::currentTime();
//}

}

void MemberListWidget::setupTerminalLayout()
{


   // int tsize = this->terminalheadlist.size();
   // int total = this->onterminallist.size()+this->offterminallist.size();
/*    if(tsize<total)
    {
        for(int k = tsize;k<total;k++)
        {
            HeadWidget *video = new HeadWidget(this);
            this->terminalheadlist.insert(k,video);
        }
    }*/
   // int count = this->onterminallist.size()+this->offterminallist.size();
   // for(int i = 0; i< this->terminalheadlist.size(); i++ )
    //{
    int i = this->terminalheadlist.size()-1;
        int row = i/7;
        int column = i-7*row;
        layout->addWidget(this->terminalheadlist[i],row,column);
/*    }
    for(int j = this->onterminallist.size();j<count;j++)
    {
        int row = j/8;
        int column = j-8*row;
        layout->addWidget(this->terminalheadlist[j],row,column);*/
    //}
    
  //  this->setupMemberVideo();
}

int MemberListWidget::searchTerminal(const QString& uri,const QVector <TerminalDetail *>&terminallist)
{
    if(terminallist.isEmpty())
    {
        return -1;
	}
	for(int i = 0; i < terminallist.size() ; i ++)
	{
		if (uri.contains("#"))
		{
			if(terminallist.at(i)->virtualUriStr == uri)
			{
				return i;
			}
		}
		else if(uri.contains("@"))
        {
            if(terminallist.at(i)->virtualUriStr.contains(uri))
            {
                return i;
            }
        }
		else
        {
            QString temp = terminallist.at(i)->uriStr;
            //tempuri
            QString temp1 = temp.left(temp.indexOf("@"));
            if(temp1 == uri)
            {
                return i;
            }
        }
    }

    return -1;
}

TerminalDetail* MemberListWidget::returnTermianl(const QString& uri,const QVector <TerminalDetail *>&terminallist)
{
    if(terminallist.isEmpty())
    {
        return NULL;
    }
    if(uri.contains("@"))
    {
        for(int i = 0; i < terminallist.size() ; i ++)
        {
            if(terminallist.at(i)->uriStr == uri)
            {
                return terminallist.at(i);
            }
        }
    }
    else
    {
        for(int j = 0; j < terminallist.size(); j ++)
        {
            QString temp = terminallist.at(j)->uriStr;
            //tempuri
            QString temp1 = temp.left(temp.indexOf("@"));qDebug()<<"d"<<temp<<temp1<<uri;
            if(temp1 == uri)
            {
                return terminallist.at(j);
            }
        }
    }

    return NULL;
}

void MemberListWidget::terminalIn(const MapList& terminalList)
{
   /* QVariant uri = terminal["uri"];
    QString uriStr = uri.toString();

    for(int i = 0 ; i < this->terminalTable->rowCount(); i ++)
    {
        if(this->terminalTable->item(i,0)->text() == uriStr&&this->terminalTable->item(i,4)->text()=="Available")
        {
            QIcon icon;
            icon.addFile(":/image/green.png");
            this->terminalheadlist.at(i)->statusButton->setIcon(icon);
            this->terminalTable->item(i,4)->setText("Available");
            return;
        }
    }*/

    this->addTerminals(terminalList);
}

void MemberListWidget::addTtoMemberTreeWidget(const QString& roomname, const QString& terminal)
{
    int count = this->memberTreeWidget->invisibleRootItem()->childCount();

        for(int k = 0; k<count;k++)
        {
            if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(0) ==roomname)
            {
                QTreeWidgetItem *item = this->memberTreeWidget->invisibleRootItem()->child(k);
                QString t = item->text(1);
                item->setText(1,t+terminal+"/");
                return;
            }
        }
        QTreeWidgetItem *item= new QTreeWidgetItem(this->memberTreeWidget->invisibleRootItem());
        item->setText(0,roomname);
        item->setText(1,terminal);
}

void MemberListWidget::removeTtoMemberTreeWidget(const QString& roomname, const QString& terminal)
{
    int count = this->memberTreeWidget->invisibleRootItem()->childCount();

        for(int k = 0; k<count;k++)
        {
            if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(0) ==roomname)
            {
                QTreeWidgetItem *item =this->memberTreeWidget->invisibleRootItem()->child(k);
                QString t = item->text(1);
                item->setText(1,t.remove(terminal+"/"));
            }
        }
}

void MemberListWidget::showTreeWidget(bool check)
{
    if(check)
    {
        int c = 0;
        for(int i = 0; i< this->terminalheadlist.size();i++)
        {
            if(this->terminalheadlist[i]->isChosen)
            {
                c = 1;
                QString terminalname = this->terminalheadlist[i]->getName();
                int count = this->memberTreeWidget->invisibleRootItem()->childCount();
                for(int k = 0; k<count;k++)
                {
                    if(this->memberTreeWidget->invisibleRootItem()->child(k)->text(1).contains(terminalname))
                    {
                        this->memberTreeWidget->invisibleRootItem()->child(k)->setHidden(false);
                    }else
                    {
                        this->memberTreeWidget->invisibleRootItem()->child(k)->setHidden(true);
                    }
                }
                break;
            }
        }
        if(c == 0)
        {
            int count = this->memberTreeWidget->invisibleRootItem()->childCount();
            for(int k = 0; k<count;k++)
            {
                this->memberTreeWidget->invisibleRootItem()->child(k)->setHidden(false);
            }
        }


        QPoint pos = this->memberButton->pos();
        this->treeWidget->setGeometry(pos.x(),pos.y()-300,200,300);
        this->treeWidget->show();
    }else
    {
        this->treeWidget->hide();
    }
}

void MemberListWidget::refreshMyTerminals()
{
    MapList onlist;
    MapList offlist;
    for(int i = 0 ; i<this->myterminallist.size();i++)
    {
        int onindex = this->searchTerminal(myterminallist.at(i),this->onterminallist);
        if(onindex>=0)
        {
            QVariantMap variantMap ;qDebug()<<"R1";
            variantMap.insert("uri",myterminallist.at(i));qDebug()<<"R2";
            variantMap.insert("name",this->onterminallist.at(onindex)->nameStr);qDebug()<<"R3";
            variantMap.insert("location",this->onterminallist.at(onindex)->locationStr);qDebug()<<"R6";
            onlist.append(variantMap);
        }else
        {
            int offindex = this->searchTerminal(myterminallist.at(i),this->offterminallist);
            if(offindex>=0)
            {
                QVariantMap variantMap ;qDebug()<<"R11";
                variantMap.insert("uri",myterminallist.at(i));qDebug()<<"R12";
                variantMap.insert("name",this->offterminallist.at(offindex)->nameStr);qDebug()<<"R13";
                variantMap.insert("location",this->offterminallist.at(offindex)->locationStr);qDebug()<<"R16";
                offlist.append(variantMap);
            }
        }
    }
    //tcontroller->setMyTerminal(onlist,offlist);
}

void MemberListWidget::releaseResouce()
{
    foreach(HeadWidget *widget,terminalheadlist)
    {
        delete widget;
    }
    terminalheadlist.clear();
   /* for(int i= 40;i<this->terminalheadlist.size();i++)
    {
        HeadWidget *widget = this->terminalheadlist[i];
        delete widget;
        //this->terminalheadlist.remove(i);
    }*/
    foreach(TerminalDetail *terminal,this->onterminallist)
    {
        delete terminal;
    }
    this->onterminallist.clear();
    foreach(TerminalDetail *terminal,this->offterminallist)
    {
        delete terminal;
    }
    this->offterminallist.clear();

	terminalsMap.clear(); // 资源已在上两步中释放

    memberTable->setRowCount(0);
    myterminallist.clear();
  //  this->deviceChangedSlot("");

    //tcontroller->onDisconnectClicked();
    //lwidget->clearVideo();
    //rwidget->clearVideo();
	screenWidget0->clearVideo();
	screenWidget1->clearVideo();
	screenWidget2->clearVideo();
	screenWidget3->clearVideo();
    this->refreshMyTerminals();



    while(this->memberTreeWidget->invisibleRootItem()->childCount()!=0)
    {
        QTreeWidgetItem *item = this->memberTreeWidget->invisibleRootItem()->child(0);
        delete item;
    }

	this->selfSoundOffAction->setChecked(false);
	this->selfVideoOffAction->setChecked(false);
	this->allRecvAudioOffAction->setChecked(false);
	this->handupButton->setChecked(false);

	this->terminalListCheckTimer->stop();
}

void MemberListWidget::startRecvRemoteMember( const QString& uri , const int& screenIndex , const int& seet )
{

	if( screenIndex>3 || seet >9 || seet<1)
		return;
	
	//如果对应的uri已经在接收，那么直接返回
	PeerShowWidget* peer = this->searchPeerForWidget( uri );
	if( peer )
	{
		return;
	}

	//获取peer头像
	if( screenIndex>0 )
	{
		peer = rwidget->getPeerShowWidget( seet -1 );
	}else
	{
		peer = lwidget->getPeerShowWidget( seet -1 );
	}

	if( peer->getMemberUri()!="" )
	{
		//如果对应的peer已经在接收其他用户，那么应该释放掉它
		this->stopRecvRemoteMember( peer->getMemberUri() );
	}

	int terminalIndex = this->searchTerminal( uri , this->onterminallist );
	if( peer && terminalIndex>-1 )
	{
		peer->setHeadShow( ":/image/users_group.png", onterminallist.at(terminalIndex)->uriStr , onterminallist.at(terminalIndex)->nameStr );
		//接收媒体流
		tcontroller->onReceiveVideo( onterminallist.at(terminalIndex)->uriStr, screenIndex , seet );
	}
}

void MemberListWidget::stopRecvRemoteMember( const QString& uri )
{
	//先查找对应的位置上是不是在接收用户
	PeerShowWidget* peer = this->searchPeerForWidget( uri );
	if( peer )
	{
		//先隐藏掉该头像，并停止媒体流
		peer->setHeadHide();
		tcontroller->onStopVideo( uri ,-1 );
	}
}
//void MemberListWidget::showDeviceList()
//{
//    p->showDeviceList();
//}

void MemberListWidget::changeRemoteMemberSeet( const QString& uri , const int& screenIndex , const int& seet )
{
	
	if( screenIndex>3 || seet >9 || seet<1)
		return;

	//先查找对应的位置上是不是在接收用户
	PeerShowWidget* sourcPeer = this->searchPeerForWidget( uri );
	if( sourcPeer )
	{
		//先隐藏掉该头像
		sourcPeer->setHeadHide();
	}
	
	PeerShowWidget* peer;
	//获取并显示peer头像
	if( screenIndex>0 )
	{
		peer = rwidget->getPeerShowWidget( seet -1 );
	}else
	{
		peer = lwidget->getPeerShowWidget( seet -1 );
	}
	int terminalIndex = this->searchTerminal( uri , this->onterminallist );
	if( peer && terminalIndex>-1 && sourcPeer )
	{
		
		peer->setHeadShow( ":/image/users_group.png", onterminallist.at(terminalIndex)->uriStr , onterminallist.at(terminalIndex)->nameStr );
		
		if( peer->getMemberUri()!="" && 
			peer->getMemberUri()!= uri )
		{
			//如果原来的位置上有在接收终端，那么向其发送命令停止发送音视频
			peer->stopVideo( sourcPeer->getMemberUri(), sourcPeer->getIndex() );
		}

		//如果开启了小流，且终端的视频窗口位置发生了改变，需要以下判断
		//1.如果是从大窗口到小窗口（即从第一个窗口到其他窗口）
		//2.如果是从小窗口到大窗口（即从非第一个窗口到第一个窗口）
		//这两种情况都需要关闭原来的媒体流并重新接收
		if( !m_bRec && 
			((sourcPeer->getIndex()==0&&(seet-1)>0) ||
			(sourcPeer->getIndex()>0&&(seet-1)==0) ))
		{
			 sourcPeer->stopVideo( uri, sourcPeer->getIndex());
			 peer->receiveVideo(uri,peer->getIndex() );

		}
		else
		{
			//接收媒体流
			tcontroller->onChangeVideoSeet( onterminallist.at(terminalIndex)->uriStr, screenIndex , seet );
		}
	}

}
//void MemberListWidget::showDeviceList()
//{
//    p->showDeviceList();
//}

void MemberListWidget::InitializeMWidget()
{
    for(int i = 0; i<40;i++)
    {
        HeadWidget *video = new HeadWidget(this);
        video->frame->hide();

        this->terminalheadlist.insert(i,video);
        int row = i/7;
        int column = i-7*row;
        layout->addWidget(video,row,column);
        this->scrollWidget->setLayout(layout);
    }

}
/*
void MemberListWidget::GetMemberLocationSlot(QVector<MemberLocation> locationlist)
{
    qDebug()<<"GetMemberLocationSlot";
    lwidget->clearVideo();
    rwidget->clearVideo();
    for(int i = 0 ; i<locationlist.count();i++)
    {
        QString uri = locationlist.at(i)._memberName;
        int screen = locationlist.at(i)._screenIndex;
        int seet = locationlist.at(i)._seet;
        if(seet>=0)
        {
            switch(screen)
            {
            case 0:
                {qDebug()<<"GetMemberLocationSlot0"<<screen<<uri<<seet;
                    TerminalDetail *terminal;
                    if(uri == "LocalPreview")
                    {
                        terminal = this->returnTermianl(tcontroller->getConnectedTerminal(),this->onterminallist);
                    }else
                    {
                        terminal = this->returnTermianl(uri,this->onterminallist);
                    }

                    if(terminal)
                    {
                        PeerShowWidget *pwidget = lwidget->getPeerShowWidget(seet-1);
                        pwidget->setHeadShow(":/image/users_group.png",terminal->uriStr,terminal->nameStr);
                        qDebug()<<"!@!@@@@@@@@@@2";
                        terminal->isReceivedAudio = true;
                        qDebug()<<"!@!@@@@@@@@@@3";
                        emit this->receiveAudioChangeSignal(terminal->uriStr,true);
                        qDebug()<<"!@!@@@@@@@@@@4";

                    }
                    break;
                }
            case 1:
                {qDebug()<<"GetMemberLocationSlot1"<<screen<<uri<<seet;
                    TerminalDetail *terminal;
                    if(uri == "LocalPreview")
                    {
                        terminal = this->returnTermianl(tcontroller->getConnectedTerminal(),this->onterminallist);
                    }else
                    {
                        terminal = this->returnTermianl(uri,this->onterminallist);
                    }
                    if(terminal)
                    {
                        PeerShowWidget *pwidget = rwidget->getPeerShowWidget(seet-1);
                        pwidget->setHeadShow(":/image/users_group.png",terminal->uriStr,terminal->nameStr);
                        terminal->isReceivedAudio = true;
                        emit this->receiveAudioChangeSignal(terminal->uriStr,true);
                    }

                    break;
                }
            default:
                break;
            }
        }

    }
}
*/
void MemberListWidget::remoteTerminalAskForMediaSlot( const QString& terminalURI , const QString& terminalName )
{
	////如果已经在接收terminalURI，那么不给用户任何提示
	//PeerShowWidget* peer = this->searchPeerForWidget( terminalURI );
	//if( peer )
	//{
	//	return;
	//}

	//QString msg = QString::fromLocal8Bit("远程终端[") + terminalName + 
	//	QString::fromLocal8Bit("]接收了你的视频，是否接收对方的视频流？");
	//int result = QMessageBox::information( this , QString::fromLocal8Bit("提示"),
	//	msg, QMessageBox::Ok|QMessageBox::Cancel 
	//	);
	//if( result==QMessageBox::Ok )
	//{
	//	//再次检查接收的终端面板上面板上是否存在该URI
	//	PeerShowWidget* peer = this->searchPeerForWidget( terminalURI );
	//	if( peer )
	//		return;

	//	this->startRecvRemoteMember( terminalURI , 0 , 6 );
	//}
}
/*
void MemberListWidget::GetLayoutChangeSlot(ScreenInfo screen)
{
    qDebug()<<"GetLayoutChangeSlot";
    switch(screen._screenIndex)
    {
    case 0:
        {qDebug()<<"GetLayoutChangeSlot"<<screen._layout<<screen._screenIndex;
            if(screen._layout == "1")
            {
                lwidget->oneAndOne();
                //lwidget->oneAndOneAction->setChecked(true);
                return;
            }
            if(screen._layout == "2")
            {
                lwidget->twoAndTwo();
                //lwidget->twoAndTwoAction->setChecked(true);
                return;
            }
            if(screen._layout == "3")
            {
                lwidget->threeAndThree();
                //lwidget->threeAndThreeAction->setChecked(true);
                return;
            }
            if(screen._layout == "L1_5")
            {
                lwidget->onePlusFiveL();
                //lwidget->onePlusFiveLAction->setChecked(true);
                return;
            }
            break;
        }
    case 1:
        {qDebug()<<"GetLayoutChangeSlot"<<screen._layout<<screen._screenIndex;
            if(screen._layout == "1")
            {
                rwidget->oneAndOne();
                //rwidget->oneAndOneAction->setChecked(true);
                return;
            }
            if(screen._layout == "2")
            {
                rwidget->twoAndTwo();
                //rwidget->twoAndTwoAction->setChecked(true);
                return;
            }
            if(screen._layout == "3")
            {
                rwidget->threeAndThree();
                //rwidget->threeAndThreeAction->setChecked(true);
                return;
            }
            if(screen._layout == "L1_5")
            {
                rwidget->onePlusFiveL();
                //rwidget->onePlusFiveLAction->setChecked(true);
                return;
            }
            break;
        }
    default:
        break;
    }
}

void MemberListWidget::GetExitProcessSlot()
{
    qDebug()<<"GetExitProcessSlot";
}
*/
void MemberListWidget::receiveAudioSlot(QString uri, bool check)
{
    TerminalDetail *terminal = this->returnTermianl(uri,this->onterminallist);
    terminal->isReceivedAudio = check;
    tcontroller->ReceiveAudio(uri,check);
}

void MemberListWidget::stopAllRecvAudio( const bool checked)
{
	for( int i=0 ; i<this->onterminallist.size() ; i++ )
	{
		//对本终端的账号不进行处理
		if(  onterminallist.at(i)->isShown && 
			onterminallist.at(i)->uriStr.toStdString()!=RunningProfile::getInstance()->user_uri() )
		{
			_uiUpdateForAudioControl(onterminallist.at(i)->uriStr , !checked);
			this->tcontroller->ReceiveAudio( onterminallist.at(i)->uriStr , !checked);
		}
	}
}

void MemberListWidget::_uiUpdateForAudioControl( const QString&terminalURI , const bool enable )
{	
	for(int i = 0; i< this->terminalheadlist.size();i++)
	{
		if(this->terminalheadlist[i]->getUri()==terminalURI)
		{
			this->terminalheadlist[i]->receiveAudioMarkChange( terminalURI , enable );
			break;
		}
	}
}

void MemberListWidget::_uiUpdateForMemberLogout( const QString&terminalURI )
{
	//避免同时接受了屏幕共享和音视频流，所以需要加个循环进行判断
	PeerShowWidget* peer = this->searchPeerForWidget( terminalURI );
	if( peer!=NULL )
	{
		//处理的对应的媒体流
		//关闭接收媒体流
		QString username = terminalURI;
		int index = username.indexOf( "@" );
		if( index > 0 )
			username = username.left( index );
		MediaManager::getInstance()->RemoveRecvMedia( username );

		//更新布局
		peer->setHeadHide();
	}

	lwidget->setDocumentOutThisScreen( terminalURI );
	rwidget->setDocumentOutThisScreen( terminalURI );

	//关闭接收的屏幕流
	MediaManager::getInstance()->RemoveRecvScreenMedia( terminalURI );

}

void MemberListWidget:: MemberAllowSpeakHandler()
{
	for (int i=0; i<terminalheadlist.size();i++)
	{
		if(terminalheadlist[i]->isChosen==true)
		{
			QString MyUri= QString ::fromStdString( RunningProfile::getInstance()->user_uri());
			if(terminalheadlist[i]->TerminalStatus == "Available" && MyUri !=terminalheadlist[i]->getUri())
			{
				AllowSpeakSignal(terminalheadlist[i]->getUri());
				return;
			}	
		}
		break;
	}

}

void MemberListWidget :: MemberForbidSpeakHandler()
{
	for (int i=0; i<terminalheadlist.size();i++)
	{
		if(terminalheadlist[i]->isChosen==true)
		{
			QString MyUri= QString ::fromStdString( RunningProfile::getInstance()->user_uri());
			if(terminalheadlist[i]->TerminalStatus == "Available" && MyUri !=terminalheadlist[i]->getUri())
			{
				ForbidSpeakSignal(terminalheadlist[i]->getUri());
				return;
			}	
		}
		break;
	}
}




void MemberListWidget:: SetMemberWindowToFirstPosition(QString Uri)
{
	startRecvRemoteMember(Uri,0,1);
	changeRemoteMemberSeet(Uri,0,1);
}


void MemberListWidget::UpgradeTerminalInfo(const QVariantMap& terminalMap)
{
	TerminalDetail terminal(terminalMap);
	terminal.uriStr = terminal.virtualUriStr; // TODO

	int index = this->searchTerminal(terminal.uriStr,this->onterminallist);
	if(index<0)
	{
		index = this->searchTerminal(terminal.uriStr,this->offterminallist);
		if(index<0)
		{
			return;
		}
		TerminalDetail *temp = this->offterminallist.at(index);
		*temp=terminal;
		
	}else
	{
		TerminalDetail *temp = this->onterminallist.at(index);
		*temp=terminal;
	}
	this->setupMemberVideo();
}

void MemberListWidget:: ModifyStopMyAudioState(bool enable)
{
	this->selfSoundOffAction->setChecked(enable);
}

void MemberListWidget:: controlScreenChangeSlot()
{
	if(!isChangeControlScreen)
	{
		this->ChangeScreenButton->setStyleSheet("border-image:url(:/image/back.png)");
		
		screenWidget0->hide();
		lwidget=screenWidget2;
		llayout->addWidget(lwidget);
		llayout->setMargin(0);
		this->showWidget_L->setLayout(llayout);
		screenWidget2->show();

		
		screenWidget1->hide();
		rwidget=screenWidget3;
		rlayout->addWidget(rwidget);
		rlayout->setMargin(0);
		this->showWidget_R->setLayout(rlayout);
		screenWidget3->show();

	}
	else
	{
		this->ChangeScreenButton->setStyleSheet("border-image:url(:/image/front.png)");

		screenWidget2->hide();
		lwidget=screenWidget0;
		llayout->addWidget(lwidget);
		llayout->setMargin(0);
		this->showWidget_L->setLayout(llayout);
		screenWidget0->show();

		screenWidget3->hide();
		rwidget=screenWidget1;
		rlayout->addWidget(rwidget);
		rlayout->setMargin(0);
		this->showWidget_R->setLayout(rlayout);
		screenWidget1->show();
	}

	lmenu->clear();
	lmenu->addAction(lwidget->oneAndOneAction);
	lmenu->addAction(lwidget->twoAndTwoAction);
	lmenu->addAction(lwidget->threeAndThreeAction);
	lmenu->addAction(lwidget->onePlusFiveLAction);
	lmenu->addAction(lwidget->showRTCPAction);
	lmenu->addAction(lwidget->documentAction);
	lmenu->addAction(lwidget->autoAction);
	this->lToolButton->setMenu(lmenu);
	
	rmenu->clear();
	rmenu->addAction(rwidget->oneAndOneAction);
	rmenu->addAction(rwidget->twoAndTwoAction);
	rmenu->addAction(rwidget->threeAndThreeAction);
	rmenu->addAction(rwidget->onePlusFiveLAction);
	rmenu->addAction(rwidget->showRTCPAction);
	rmenu->addAction(rwidget->documentAction);
	rmenu->addAction(rwidget->autoAction);
	this->rToolButton->setMenu(rmenu);

	isChangeControlScreen=!isChangeControlScreen;
}

void MemberListWidget:: ChangeScreenTo1And2Slot()
{

	this->ScreenChangeButton->setEnabled(false);
	this->ScreenChangeButton2->setEnabled(true);
	
	screenWidget2->hide();
	lwidget=screenWidget0;
	llayout->addWidget(lwidget);
	llayout->setMargin(0);
	this->showWidget_L->setLayout(llayout);
	screenWidget0->show();

	screenWidget3->hide();
	rwidget=screenWidget1;
	rlayout->addWidget(rwidget);
	rlayout->setMargin(0);
	this->showWidget_R->setLayout(rlayout);

	if (m_bRec)
	{
		screenWidget1->show();
	} 
	else
	{
		int screenNum = QApplication::desktop()->numScreens();
		if(screenNum>=2) screenWidget1->show();
	}

	lmenu->clear();
	lmenu->addAction(lwidget->oneAndOneAction);
	lmenu->addAction(lwidget->twoAndTwoAction);
	lmenu->addAction(lwidget->threeAndThreeAction);
	lmenu->addAction(lwidget->onePlusFiveLAction);
	lmenu->addAction(lwidget->showRTCPAction);
	lmenu->addAction(lwidget->documentAction);
	lmenu->addAction(lwidget->autoAction);
	this->lToolButton->setMenu(lmenu);

	rmenu->clear();
	rmenu->addAction(rwidget->oneAndOneAction);
	rmenu->addAction(rwidget->twoAndTwoAction);
	rmenu->addAction(rwidget->threeAndThreeAction);
	rmenu->addAction(rwidget->onePlusFiveLAction);
	rmenu->addAction(rwidget->showRTCPAction);
	rmenu->addAction(rwidget->documentAction);
	rmenu->addAction(rwidget->autoAction);
	this->rToolButton->setMenu(rmenu);

}

void MemberListWidget:: ChangeScreenTo3And4Slot()
{
	this->ScreenChangeButton->setEnabled(true);
	this->ScreenChangeButton2->setEnabled(false);

	screenWidget0->hide();
	lwidget=screenWidget2;
	llayout->addWidget(lwidget);
	llayout->setMargin(0);
	this->showWidget_L->setLayout(llayout);
	screenWidget2->show();


	screenWidget1->hide();
	rwidget=screenWidget3;
	rlayout->addWidget(rwidget);
	rlayout->setMargin(0);
	this->showWidget_R->setLayout(rlayout);

	if (m_bRec)
	{
		screenWidget3->show();
	}
	else
	{
		int screenNum = QApplication::desktop()->numScreens();
		if(screenNum>=4) screenWidget3->show();
	}

	lmenu->clear();
	lmenu->addAction(lwidget->oneAndOneAction);
	lmenu->addAction(lwidget->twoAndTwoAction);
	lmenu->addAction(lwidget->threeAndThreeAction);
	lmenu->addAction(lwidget->onePlusFiveLAction);
	lmenu->addAction(lwidget->showRTCPAction);
	lmenu->addAction(lwidget->documentAction);
	lmenu->addAction(lwidget->autoAction);
	this->lToolButton->setMenu(lmenu);

	rmenu->clear();
	rmenu->addAction(rwidget->oneAndOneAction);
	rmenu->addAction(rwidget->twoAndTwoAction);
	rmenu->addAction(rwidget->threeAndThreeAction);
	rmenu->addAction(rwidget->onePlusFiveLAction);
	rmenu->addAction(rwidget->showRTCPAction);
	rmenu->addAction(rwidget->documentAction);
	rmenu->addAction(rwidget->autoAction);
	this->rToolButton->setMenu(rmenu);

}

void MemberListWidget:: AdjustScreenLayout()
{
	int screenNum = 4;

	if (!m_bRec)
	{
		screenNum = QApplication::desktop()->numScreens();
	}
	
	if(screenNum<=2)
	{
		this->ScreenChangeButton->hide();
		this->ScreenChangeButton2->hide();

		if(screenNum<=1)
		screenWidget1->hide();
		else
		screenWidget1->show();
	}
	else
	{
		this->ScreenChangeButton->show();
		this->ScreenChangeButton2->show();
	}

}

void MemberListWidget::InitialConferenceRoom()
{
	//重新初始化会议室UI的布局
	if (m_bRec)
	{
		screenWidget0->threeAndThree();
		screenWidget1->threeAndThree();
		screenWidget2->threeAndThree();
		screenWidget3->threeAndThree();
	}
	else
	{
		lwidget->autoLayout();
		rwidget->documentSlot();
		AdjustScreenLayout();
	}

	SpeakingToolButton->hide();

	handupButton->show();

	//mWidget->stopToolButton->hide();
	currentTermianlNum=0;
	totalTermianlNum=999;
	isFirstFinishTerminalLIst=false;
}


void MemberListWidget:: checkTerminalListSlot()
{	
	//服务器发送的终端列表丢包了
	if(currentTermianlNum<totalTermianlNum && totalTermianlNum!=999)
	{
		_snprintf(__global_msg , sizeof(__global_msg), "接收到的终端列表数目%d，终端列表总数:%d" ,currentTermianlNum ,totalTermianlNum);
		CV_LOG_ERROR(__global_msg);
		onMemberGetTermialListSignal();
		return;
	}

	terminalListCheckTimer->stop();
	
	_snprintf(__global_msg , sizeof(__global_msg), "接收到的终端列表数目%d，终端列表总数:%d" ,currentTermianlNum ,totalTermianlNum);
	CV_LOG_INFO(__global_msg);
}

void MemberListWidget::cutFile( QString uri)
{
	tcontroller->SegmentMedia(uri);
}
