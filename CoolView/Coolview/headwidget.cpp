#include "headwidget.h"
#include "peershowwidget.h"
#include <config/RunningConfig.h>

HeadWidget::HeadWidget( MemberListWidget *parent)
    :p(parent)
{
    this->setupUi(this);

    this->n = "";
    this->uri = "";
    this->imageurl = "";
    this->status = false;

    this->isChosen = false;
	this->isMainSpeakerTermianl=false;
  //  this->isShown = false;

    this->videoMarkButton->hide();
    this->audioMarkButton->hide();
	this->handUpMarkButton->hide();
	
	



    this->checkwidget = new CheckedWidget();

    this->isAudioReceived = false;

    menu = new QMenu(this);
    receiveAudio = new QAction(QString::fromLocal8Bit("接收音频"),this);
    menu->addAction(receiveAudio);
    connect(receiveAudio,SIGNAL(triggered()),this,SLOT(receiveAudioSlot()));

	if (CVIniConfig::getInstance().getModelCategory() == CVCFG_VALUE_MODEL_CATEGORY_TX)
	{
		cutFile = new QAction(QString::fromLocal8Bit("录制分段"),this);
		menu->addAction(cutFile);
		connect(cutFile,SIGNAL(triggered()),this,SLOT(cutFileSlot()));
	}

    checkwidget->setMaximumSize(this->headFrame->width(),this->headFrame->height());
    checkwidget->setMinimumSize(this->headFrame->width(),this->headFrame->height());
    QGridLayout *layout = new QGridLayout(this->headFrame);
    layout->addWidget(checkwidget);
    layout->setMargin(0);
    layout->setSpacing(0);
    this->headFrame->setLayout(layout);
    this->checkwidget->hide();
    connect(p,SIGNAL(CheckMarkCleanSignal(bool)),this,SLOT(onCheckMarkCleanSlot(bool)));
    connect(p,SIGNAL(receiveAudioChangeSignal(QString,bool)),this,SLOT(receiveAudioMarkChange(QString,bool)));


    this->headFrame->setObjectName("headframe");


}

HeadWidget::~HeadWidget()
{
    delete checkwidget;
    qDebug()<<"delete headwidget";
}

void HeadWidget::mousePressEvent(QMouseEvent *event)
{


    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<"clicked left button";

        if(checkwidget->isVisible())
       {
             qDebug()<<"clicked left button hide";
            checkwidget->hide();
            this->isChosen = false;
        }
        else
        {
            qDebug()<<"clicked left button show"<<checkwidget->width()<<checkwidget->height();
            p->checkMarkClean();
            checkwidget->show();
            this->isChosen = true;

         //   p->changeButtonNameSlot(this->isShown);                                    //2.21
        }
    }
    QWidget::mousePressEvent(event);
}

void HeadWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons()& Qt::LeftButton)
    {
        int distance = (event->pos()-startPos).manhattanLength();
        if(distance >= QApplication::startDragDistance())
        {
            performDrag();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void HeadWidget::performDrag()
{
    QString u = this->imageurl+"\t"+this->uri+"\t"+this->n;

    if(!u.isNull())
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(u);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(u));
        drag->exec(Qt::CopyAction);
    }
}

void HeadWidget::onCheckMarkCleanSlot(bool check)
{

    checkwidget->hide();
    isChosen = false;

}

QString HeadWidget::getHeadImageUrl()
{
   return imageurl;
}

QString HeadWidget::getName()
{
    return n;
}

QString HeadWidget::getUri()
{
    return uri;
}

void HeadWidget::setVideoMarkHidden(bool check)
{
    if(check)
    {
        this->videoMarkButton->hide();
    }
    else
    {
        this->videoMarkButton->show();
    }
}

void HeadWidget::setAudioMarkHidden(bool check)
{
    if(check)
    {
        this->audioMarkButton->hide();
    }
    else
    {
        this->audioMarkButton->show();
    }
}

void HeadWidget::setHandUpMarkHidden(bool check)
{
    if(check)
    {
        this->handUpMarkButton->hide();
    }
    else
    {
        this->handUpMarkButton->show();
    }
}

void HeadWidget::statusChange(bool check)
{
    if(check)
    {
        QIcon icon;
        icon.addFile(":/image/green.png");
        this->statusButton->setIcon(icon);
        this->status = check;
    }else
    {
        QIcon icon;
        icon.addFile(":/image/red.png");
        this->statusButton->setIcon(icon);
        this->status = check;
    }
}
void HeadWidget::setHead(TerminalDetail *detail, QString imagepath)
{
    this->uri = detail->uriStr;
    this->n = detail->nameStr;
    this->imageurl = imagepath;
	this->TerminalStatus=detail->statusStr;

    if(detail->statusStr == "Available")
    {
        this->statusChange(true);
        this->receiveAudioMarkChange(this->uri,detail->isReceivedAudio);
        this->menu->setEnabled(true);
    }else
    {
        this->statusChange(false);
        this->receiveAudioMarkChange(this->uri,false);
        this->menu->setEnabled(false);
    }
    if(detail->hasVideo)
    {
        this->videoMarkButton->hide();
    }else
    {
        this->videoMarkButton->show();
    }
	/*
    if(detail->audioStr == "")
    {
        this->audioMarkButton->hide();
    }else
    {
        this->audioMarkButton->show();
    }
	*/

	

	

    QString temp = n;
	int i=0;
	while(i<n.length())
	{
		i++;
		if(temp[i]=='_')
		{
			temp[i]='\n';
			break;
		}
	}

	
	QIcon icon;
	if(!detail)
	{	
		icon.addFile(QString::fromUtf8(":/image/hire-me.png"), QSize(), QIcon::Normal, QIcon::Off);
		handUpMarkButton->setIcon(icon);
		handUpMarkButton->hide();
	}
	else
	{

		if(detail->isSpeaking == "true")
		{
			icon.addFile(QString::fromUtf8(":/image/speaking.png"), QSize(), QIcon::Normal, QIcon::Off);
			handUpMarkButton->setIcon(icon);
			handUpMarkButton->show();
		}
		else
		{
			
			icon.addFile(QString::fromUtf8(":/image/hire-me.png"), QSize(), QIcon::Normal, QIcon::Off);
			handUpMarkButton->setIcon(icon);

			if(detail->isHandUp == "true")
			handUpMarkButton->show();
			else
			handUpMarkButton->hide();
		}

		if(detail->isMainSpeaker == "true")
		{
			audioMarkButton->show();
			this->isMainSpeakerTermianl=true;
		}
		else
		{
			audioMarkButton->hide();
			this->isMainSpeakerTermianl=false;
		}

		if(detail->isChairmanTerminal == "true")
		{
			this->setStyleSheet("background-color:rgb(123,170,249)");
			
		}
		else
		{
			
			if(this->uri.indexOf(QString::fromStdString(RunningProfile::getInstance()->user_uri())) != -1)
			{
				this->setStyleSheet("background-color:rgb(210,159,149)");		
			}
			else
			{
				this->setStyleSheet("background-color:rgb(240,240,240)");
			}
		}

	}

	
    this->memberNameLabel->setText(temp);
    this->headFrame->setStyleSheet("QFrame#headframe{border-image:url("+imageurl+")}");
    if(detail->isShown)
    {
        this->frame->setStyleSheet("QFrame{border-width:2px;border-color:red;border-style:solid;border-radius: 5px;}");
    }else
    {
        this->frame->setStyleSheet("QFrame{border-width:2px;border-color:darkGray;border-style:solid;border-radius: 5px;}");
    }



}

bool HeadWidget::getStatus()
{
    return status;
}

void HeadWidget::shownChange(bool check)
{
  //  if(turi == uri)
    {
        if(check)
        {
            this->frame->setStyleSheet("QFrame{border-width:2px;border-color:darkGreen;border-style:solid;border-radius: 5px;}");
        }else
        {
            this->frame->setStyleSheet("QFrame{border-width:2px;border-color:darkGray;border-style:solid;border-radius: 5px;}");
        }
    }
}

void HeadWidget::mouseReleaseEvent(QMouseEvent *event)
{
	//不对本终端进行操作
	if( this->getUri()!=RunningProfile::getInstance()->user_uri().c_str() )
	{
		startPos = event->globalPos();
		menu->exec(startPos);
	}
}

void HeadWidget::receiveAudioSlot()
{
    if(this->isAudioReceived)
    {
        p->receiveAudioSlot(this->uri,false);
        this->receiveAudioMarkChange(this->uri,false);
    }else
    {
        p->receiveAudioSlot(this->uri,true);
        this->receiveAudioMarkChange(this->uri,true);
    }

}

void HeadWidget::receiveAudioMarkChange(QString uriStr,bool check)
{
    if(uriStr!=this->uri)
    {
        return;
    }
    if(check)
    {
        this->isAudioReceived=true;
        this->receiveAudio->setText(QString::fromLocal8Bit("停止接收音频"));
        //this->audioMarkButton->setIcon(QIcon(":/image/audio.png"));
    }else
    {
        this->isAudioReceived=false;
        this->receiveAudio->setText(QString::fromLocal8Bit("接收音频"));
        //this->audioMarkButton->setIcon(QIcon(":/image/audio-volume-muted.png"));
    }
}

void HeadWidget::cutFileSlot()
{
	p->cutFile(this->uri);
}
