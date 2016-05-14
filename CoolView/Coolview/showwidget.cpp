#include "showwidget.h"

ShowWidget::ShowWidget(QString name, int sIndex,MemberListWidget *parent)
    :screenIndex(sIndex),
    p(parent)
{
    this->setupUi(this);
    //this->showWidgetLabel->setText(name);               //2.21
    for(int i=0;i<10;i++)
    {
        PeerShowWidget *widget = new PeerShowWidget(this,i);
        widgetlist.append(widget);
    }
    createActions();
    layout = new QGridLayout(this->showFrameWidget);
    layout->setMargin(1);
    layout->setSpacing(1);

	if(screenIndex==1)
	this->documentSlot();
	else
	this->autoLayout();
//	onePlusFiveL();


   // this->threeAndThree();
    this->isRtcpShown = false;
    //layout->setSizeConstraint(QLayout::SetFixedSize);
  /*  QMenu *menu = new QMenu("m");
    menu->addAction(oneAndOneAction);
    menu->addAction(twoAndTwoAction);
    menu->addAction(threeAndThreeAction);
    menu->addAction(onePlusFiveLAction);
    menu->addAction(onePlusTwentyLAction);
    this->toolButton->setMenu(menu);
    connect(this->toolButton,SIGNAL(clicked()),this->toolButton,SLOT(showMenu()));*/       //2.21


  //  connect(this->showRTCPCheckBox,SIGNAL(clicked(bool)),this,SLOT(onShowRTCPMessage(bool)));      //2.21

}

ShowWidget::~ShowWidget()
{
    foreach(PeerShowWidget *widget, widgetlist)
    {
        delete widget;
    }
    delete layout;
    delete oneAndOneAction;
    delete twoAndTwoAction;
    delete threeAndThreeAction;
    delete onePlusFiveLAction;
    delete showRTCPAction;
    delete documentAction;
	delete autoAction;
  //  delete videomaptable;
    //delete onePlusTwentyLAction;
    qDebug()<<"delete showwidget";
}

void ShowWidget::createActions()
{
    this->oneAndOneAction = new QAction(tr("1*1"),this);
    this->twoAndTwoAction = new QAction(tr("2*2"),this);
    this->threeAndThreeAction = new QAction(tr("3*3"),this);
    this->onePlusFiveLAction = new QAction(tr("L(1+5)"),this);
    this->showRTCPAction = new QAction(QString::fromLocal8Bit("Qos实时信息"),this);
    this->documentAction = new QAction(QString::fromLocal8Bit("屏幕共享"),this);
	this->autoAction=new QAction(QString::fromLocal8Bit("自动布局"),this);
	

    this->showRTCPAction->setCheckable(true);
    this->oneAndOneAction->setCheckable(true);
    this->twoAndTwoAction->setCheckable(true);
    this->threeAndThreeAction->setCheckable(true);
    this->onePlusFiveLAction->setCheckable(true);
    this->documentAction->setCheckable(true);
	this->autoAction->setCheckable(true);

    connect(this->oneAndOneAction,SIGNAL(triggered()),this,SLOT(oneAndOne()));
    connect(this->twoAndTwoAction,SIGNAL(triggered()),this,SLOT(twoAndTwo()));
    connect(this->threeAndThreeAction,SIGNAL(triggered()),this,SLOT(threeAndThree()));
    connect(this->onePlusFiveLAction,SIGNAL(triggered()),this,SLOT(onePlusFiveL()));
    connect(this->showRTCPAction,SIGNAL(triggered()),this,SLOT(onShowRTCPMessage()));
    connect(this->documentAction,SIGNAL(triggered()),this,SLOT(documentSlot()));
	connect(this->autoAction,SIGNAL(triggered()),this,SLOT(autoLayout()));


  //  connect(this->oneAndOneAction,SIGNAL(triggered()),this,SLOT(oneAndOneSend()));
  //  connect(this->twoAndTwoAction,SIGNAL(triggered()),this,SLOT(twoAndTwoSend()));
  //  connect(this->threeAndThreeAction,SIGNAL(triggered()),this,SLOT(threeAndThreeSend()));
  //  connect(this->onePlusFiveLAction,SIGNAL(triggered()),this,SLOT(onePlusFiveLSend()));
   // connect(this->showRTCPAction,SIGNAL(triggered()),this,SLOT(onShowRTCPMessage()));
  //  connect(this->documentAction,SIGNAL(triggered()),this,SLOT(documentSend()));
}

void ShowWidget::clearMenuMarks()
{
    this->oneAndOneAction->setChecked(false);
    this->twoAndTwoAction->setChecked(false);
    this->threeAndThreeAction->setChecked(false);
    this->onePlusFiveLAction->setChecked(false);
    this->documentAction->setChecked(false);
	this->autoAction->setChecked(false);

}

void ShowWidget::oneAndOne()
{
    this->clearMenuMarks();
    //if(p->getTerminalName() == "")
    //{
    //    p->showDeviceList();
    //    return ;
    //}    
    this->oneAndOneAction->setChecked(true);
    for(int i = 1; i< widgetlist.count(); i++)
    {
        widgetlist.at(i)->hide();
        /*if(widgetlist.at(i)->headIsShown())
        {
            layout->addWidget(widgetlist.at(i));
            return;
        }*/
    }
    layout->addWidget(widgetlist.at(0),0,0,2,2);
    widgetlist.at(0)->show();
    emit oneAndOneSignal(screenIndex);
}

void ShowWidget::twoAndTwo()
{
    this->clearMenuMarks();
    //if(p->getTerminalName() == "")
    //{
    //    p->showDeviceList();
    //    return ;
    //}   
    this->twoAndTwoAction->setChecked(true);
    for(int j = 4 ; j<widgetlist.count(); j++)
    {
        widgetlist.at(j)->hide();
    }
    for(int i = 0 ;i<4;i++)
    {
        int y = i/2;
        int x = i-y*2;

        layout->addWidget(widgetlist.at(i),y,x);
        widgetlist.at(i)->show();
    }
    emit twoAndTwoSignal(screenIndex);

}

void ShowWidget::threeAndThree()
{
    //if(p->getTerminalName() == "")
    //{
    //    p->showDeviceList();
    //    return ;
    //}
    this->clearMenuMarks();
    this->threeAndThreeAction->setChecked(true);
    for(int j = 9 ; j<widgetlist.count(); j++)
    {
        widgetlist.at(j)->hide();
    }
    for(int i = 0 ;i<9;i++)
    {
        int y = i/3;
        int x = i-y*3;

        layout->addWidget(widgetlist.at(i),y,x);
        widgetlist.at(i)->show();
    }
    emit threeAndThreeSignal(screenIndex);
}

void ShowWidget::onePlusFiveL()
{
    this->clearMenuMarks();
   /* if(p->getTerminalName() == "")
    {
        p->showDeviceList();
        return ;
    }*/
    this->onePlusFiveLAction->setChecked(true);
    for(int j = 6 ; j<widgetlist.count(); j++)
    {
        widgetlist.at(j)->hide();
    }

    layout->addWidget(widgetlist.at(0),0,0,2,2);
    layout->addWidget(widgetlist.at(1),0,2,1,1);
    layout->addWidget(widgetlist.at(2),1,2,1,1);
    layout->addWidget(widgetlist.at(3),2,0,1,1);
    layout->addWidget(widgetlist.at(4),2,1,1,1);
    layout->addWidget(widgetlist.at(5),2,2,1,1);
    for(int i = 0 ;i<6;i++)
    {
        widgetlist.at(i)->show();
    }
    emit onePlusFiveLSignal(screenIndex);


}


void ShowWidget:: autoLayout()
{
	this->clearMenuMarks();
	this->autoAction->setChecked(true);

	for (int j=6;j<widgetlist.count();j++)
	{
		widgetlist.at(j)->hide();
	}

	layout->addWidget(widgetlist.at(0),0,1,2,3);
	layout->addWidget(widgetlist.at(1),2,0,1,1);
	layout->addWidget(widgetlist.at(2),2,1,1,1);
	layout->addWidget(widgetlist.at(3),2,2,1,1);
	layout->addWidget(widgetlist.at(4),2,3,1,1);
	layout->addWidget(widgetlist.at(5),2,4,1,1);
	for(int i = 0 ;i<6;i++)
	{
		widgetlist.at(i)->show();
	}
	emit autoLayoutSignal(screenIndex);
}



void ShowWidget::documentSlot()
{
    this->clearMenuMarks();
    //if(p->getTerminalName() == "")
    //{
    //    p->showDeviceList();
    //    return ;
    //}
	
    this->documentAction->setChecked(true);
	widgetlist.at(9)->show();
    for(int i = 0; i<9; i++)
    {
        widgetlist.at(i)->hide();
    }

     layout->addWidget(widgetlist.at(9),0,0,2,2);
	 widgetlist.at(9)->show();

	emit documentSignal(screenIndex);


}

void ShowWidget::oneAndOneSend()
{
    emit this->oneAndOneSignal(screenIndex);
}

void ShowWidget::twoAndTwoSend()
{
    emit this->twoAndTwoSignal(screenIndex);
}

void ShowWidget::threeAndThreeSend()
{
    emit this->threeAndThreeSignal(screenIndex);
}

void ShowWidget::onePlusFiveLSend()
{
    emit this->onePlusFiveLSignal(screenIndex);
}

void ShowWidget::documentSend()
{
    emit this->documentSignal(screenIndex);
}

void ShowWidget::showSelfHead(QString imageurl, QString memberuri, QString membername, int pos /*= 5*/)
{
    this->widgetlist.at(pos)->showSelfHead(imageurl,memberuri,membername);
  //  this->videomaptable->item(2,0)->setText(memberuri);
}

int ShowWidget::videoIsInThisScreen(QString membername)
{
    /*for(int i = 0 ; i<videomaptable->rowCount();i++)
    {
        if(videomaptable->item(i,0)->text() == membername)
        {
            qDebug()<<"videoisinthisscreen"<<i;
            return i;
        }
    }*/
    for(int i = 0 ; i<this->widgetlist.count()-1;i++)
        {
            if(widgetlist.at(i)->getMemberUri() == membername)
            {
                qDebug()<<"videoisinthisscreen"<<i;
                return i;
            }
        }
    qDebug()<<"videoisnotinthisscreen";
    return -1;
}

void ShowWidget::setVideoOutThisScreen(QString membername)
{
    qDebug()<<"setVideoOutThisScreen"<<membername;
 //   videomaptable->item(seet,0)->setText("");
    int seet = this->videoIsInThisScreen(membername);
    if(seet<0)
    {
        return;
    }
    widgetlist.at(seet)->setHeadHide();
}

QString ShowWidget::getScreenVideoName(int seet)
{
   /* if(seet<0)
    {
        return "";
    }*/
    return this->widgetlist.at(seet)->getMemberUri();
}

/*void ShowWidget::checkMarkClean()
{
    p->sWCheckMarkClean();
}*/

void ShowWidget::receiveVideoSlot(QString name,int index)
{
    //if(p->getTerminalName() == "")
    //{
    //   // QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("没有选择终端，请返回终端选择界面进行终端选择。"),QMessageBox::Ok);
    //    p->showDeviceList();
    //    int i = index;
    //    this->widgetlist.at(i)->setHeadHide();
    //    return ;
    //}
    int seet = index;
    //this->videomaptable->item(seet,0)->setText(name);
    qDebug()<<"receiveVideoSlot:"<<seet<<name;
    emit receiveVideoSignal(name,this->screenIndex,seet);
}

void ShowWidget::stopVideoSlot(QString name, int index)
{
    int seet = index;
   // this->videomaptable->item(seet,0)->setText("");
    qDebug()<<"stopVideoSlot:"<<seet<<name;
    emit stopVideoSignal(name,this->screenIndex);
}

void ShowWidget::changeVideoSeetSlot(QString name, int index)
{

    emit this->changeVideoSeetSignal(name,this->screenIndex,index+1);

}

/*void ShowWidget::setPeerInTable(int index, QString name)
{
    this->videomaptable->item(index,0)->setText(name);
}*/

PeerShowWidget* ShowWidget::getPeerShowWidget(int index)
{
    return this->widgetlist.at(index);
}

PeerShowWidget* ShowWidget::searchPeerForWidget(QString name)
{
    return p->searchPeerForWidget(name);
}

void ShowWidget::onShowRTCPMessage()
{
    qDebug()<<"hi";
    if(this->isRtcpShown == false)
    {
        emit this->showRTCPMessageSignal(true,this->screenIndex);
        this->isRtcpShown = true;
    }else
    {
        emit this->showRTCPMessageSignal(false,this->screenIndex);
        this->isRtcpShown = false;
    }

}
/**************************2.22************************/
int ShowWidget::getScreenIndex()
{
    return this->screenIndex;
}
/**************************2.22 end************************/

void ShowWidget::clearVideo()
{
    for(int i = 0 ; i<this->widgetlist.size(); i++)
    {
        this->widgetlist.at(i)->setHeadHide();
    }
    //this->threeAndThree();
}

void ShowWidget::setDocumentOutThisScreen(const QString& memberURI )
{
	
	if(widgetlist.at(9)->getMemberUri()==memberURI )
	{
		widgetlist.at(9)->setHeadHide();
	}
}

void ShowWidget::clearDocumentScreen()
{
	widgetlist.at(9)->clearScrenn();
}

int ShowWidget::getFreeSeet()
{
	for(int i=0; i<this->widgetlist.count()-1; i++)
	{
		if(widgetlist.at(i)->isVisible() && widgetlist.at(i)->getMemberUri().length() == 0)
			return i;
	}
	return -1;
}