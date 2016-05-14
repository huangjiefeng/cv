#include "peershowwidget.h"

#include <QMessageBox>

#include "headwidget.h"
#include "profile/RunningProfile.h"
#include "Util/MediaManager.h"
#include "config/RunningConfig.h"

PeerShowWidget::PeerShowWidget(ShowWidget *parent, int seetIndex)
    :p(parent),
    index(seetIndex),
    screenIndex(p->getScreenIndex())                   //2.22
{
    this->setupUi(this);
    this->documentLabel->hide();

    int w = this->width();
    int h = this->height();
    int height = h-10;
    int width = height;
    int x = (w-width)/2;
    int y = 5;
    this->headWidget->setGeometry(x,y,width,height);
    int cheight = 16;
    int cwidth = 16;
    int cx = w-20;
    int cy = 8;
    this->vCheckFrame->setGeometry(cx-18,cy,cwidth,cheight);
    this->aCheckFrame->setGeometry(cx,cy,cwidth,cheight);
    this->vCheckFrame->hide();
    this->aCheckFrame->hide();
    int nheight = 20;
    int nwidth = w-4;
    int nx = 2;
    int ny = h-22;
    this->nameLabel->setGeometry(nx,ny,nwidth,nheight);
    this->nameLabel->hide();
    this->nameLabel->setAlignment(Qt::AlignCenter);


    this->headWidget->setObjectName("headframe");
    nuri = "";
    n = "";
    iurl = "";

    if(index==9)
    {
        this->documentLabel->show();
        iurl = ":/image/folder-with-contents.png";
    }

    QPixmap fp(iurl);
    this->headLabel->setPixmap(fp);
   /* this->vCheckFrame = new CheckedWidget();
    this->vCheckFrame->setGeometry(0,0,this->width(),this->height());
    this->vCheckFrame->hide();*/

    connect(this,SIGNAL(receiveVideoSignal(QString,int)),p,SLOT(receiveVideoSlot(QString,int)));
    connect(this,SIGNAL(stopVideoSignal(QString,int)),p,SLOT(stopVideoSlot(QString,int)));
    connect(this,SIGNAL(changeVideoSeetSignal(QString,int)),p,SLOT(changeVideoSeetSlot(QString,int)));
}

PeerShowWidget::~PeerShowWidget()
{
    //delete vCheckFrame;
    qDebug()<<"delete peershowwidget";
}

bool PeerShowWidget::headIsShown()
{
    if(this->nameLabel->isVisible())
    {
        return true;
    }
    return false;
}

void PeerShowWidget::setHeadHide()
{
    this->headWidget->hide();
    this->nameLabel->hide();
//    QString text = nuri;
    nuri = "";
    n = "";
    if(index!=9)
    {
        iurl = "";
    }

//    emit stopVideoSignal(text,index);
}

void PeerShowWidget::setHeadShow(QString imageurl,QString memberuri,QString membername)
{
    if(index!=9)
    {
        iurl = imageurl;
    }
    nuri = memberuri;
    n = membername;
    QPixmap p(iurl);
    QPixmap fixp = p.scaled(this->headWidget->width(),this->headWidget->height());
    this->headLabel->setPixmap(fixp);
    this->headWidget->show();
	int i=0;
	while(i<membername.length())
	{
		i++;
		if(membername[i]=='_')
		{
			membername[i]='\n';
			break;
		}
	}
    this->nameLabel->setText(membername);
    this->nameLabel->show();
    this->setToolTip(n);
}


void PeerShowWidget::showSelfHead(QString imageurl, QString memberuri, QString membername)
{
    if(index!=9)
    {
        iurl = imageurl;
    }
    nuri = memberuri;
    n = membername;
    if(iurl == "")
    {
        this->headWidget->hide();
        this->nameLabel->setText(membername);
        this->nameLabel->hide();
        this->setToolTip(membername);
    }

    QPixmap p(iurl);
    QPixmap fixp = p.scaled(this->headWidget->width(),this->headWidget->height());
    this->headLabel->setPixmap(fixp);
    this->headWidget->show();
	QString n =membername;

	int i=0;
	while(i<membername.length())
	{
		i++;
		if(membername[i]=='_')
		{
			membername[i]='\n';
			break;
		}
	}
    this->nameLabel->setText(membername);
    this->nameLabel->show();
    this->setToolTip(n);
}

void PeerShowWidget::mousePressEvent(QMouseEvent *event)
{
     qDebug()<<"mousePressEvent()";
   /* if(event->button() == Qt::LeftButton)
    {

        if(vCheckFrame->isVisible())
        {
            vCheckFrame->hide();
            this->isChosen = false;
        //    qDebug()<<this->isChosen;
        }
        else
        {
            p->checkMarkClean();
            vCheckFrame->show();
            this->isChosen = true;
         //   qDebug()<<this->isChosen;
        }
    }*/
    if(this->nameLabel->isVisible())
    {

            if(event->button() == Qt::LeftButton)
        {
            startPos = event->pos();

        }

    }
    QWidget::mousePressEvent(event);
}

void PeerShowWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"mouseMoveEvent()";
    if(this->nameLabel->isVisible())
    {
        if(event->buttons()& Qt::LeftButton)
        {
            int distance = (event->pos()-startPos).manhattanLength();
            if(distance >= QApplication::startDragDistance())
                performDrag();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void PeerShowWidget::performDrag()
{
    QString u = this->iurl+"\t"+this->nuri+"\t"+this->n;
    qDebug()<<"performDrag()"<<u;
    if(!u.isNull())
    {
        QMimeData *mimeData = new QMimeData;
        mimeData->setText(u);

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimeData);
        drag->setPixmap(QPixmap(u));
        drag->exec(Qt::MoveAction);
    }
}

void PeerShowWidget::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();
    int h = event->size().height();
    int height = h-27;
    int width = height;
    int x = (w-width)/2;
    int y = 5;
    this->headWidget->setGeometry(x,y,width,height);


    int cheight = 16;
    int cwidth = 16;
    int cx = w-20;
    int cy = 8;
    this->vCheckFrame->setGeometry(cx-18,cy,cwidth,cheight);
    this->aCheckFrame->setGeometry(cx,cy,cwidth,cheight);
    if(w>60)
    {
        int nheight =20;
        int nwidth = w-4;//80;
        int nx = 2;//(w-80)/2;
        int ny = h-22;
        this->nameLabel->setGeometry(nx,ny,nwidth,nheight);
        QFont font;
        font.setPointSize(7);
        this->nameLabel->setFont(font);
        this->nameLabel->setAlignment(Qt::AlignCenter);
        if(this->nameLabel->isVisible())
        {
            qDebug()<<"OOOOOOOOOOOOO"<<width<<height;
            this->headWidget->show();
            QPixmap temp(iurl);
            QPixmap fixtemp = temp.scaled(width,height);
            this->headLabel->setPixmap(fixtemp);
            this->headLabel->show();
        }

    }else
    {
        int nheight = h*4/5;
        int nwidth = w-4;
        int nx = 2;
        int ny = (h-nheight)/2;                   //1.16 dhx
        this->nameLabel->setGeometry(nx,ny,nwidth,nheight);
        QFont font;
        font.setPointSize(7);
        this->nameLabel->setFont(font);
        //this->nameLabel->setAlignment(Qt::AlignLeft);
        //this->nameLabel->setAlignment(Qt::AlignVCenter);
		 this->nameLabel->setAlignment(Qt::AlignCenter);
        this->headWidget->hide();

    }
}

/*void PeerShowWidget::onCheckMarkCleanSlot(bool check)
{
    vCheckFrame->hide();
    isChosen = false;
}*/

void PeerShowWidget::dragEnterEvent(QDragEnterEvent *event)
{
    HeadWidget *source = qobject_cast<HeadWidget *>(event->source());
    if(source)
    {
        event->setDropAction(Qt::CopyAction);
        event->accept();
    }else
    {
        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }

}

void PeerShowWidget::dragMoveEvent(QDragMoveEvent *event)
{
    HeadWidget *source = qobject_cast<HeadWidget *>(event->source());
    if(source)
    {

            event->setDropAction(Qt::CopyAction);
            event->accept();

    }else
    {
        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }
}

void PeerShowWidget::dropEvent(QDropEvent *event)
{
    HeadWidget *source = qobject_cast<HeadWidget *>(event->source());

    if(source)
    {
		QString currentTerminalUri = QString::fromStdString(RunningProfile::getInstance()->user_uri() );
		/*if( this->screenIndex!=MediaManager::getInstance()->GetLocalPreviewScreen()
		&& source->getUri()==currentTerminalUri )
		{
		int screenNum = MediaManager::getInstance()->GetLocalPreviewScreen() + 1;
		QString message = QString::fromLocal8Bit("对不起，本地回显只能在第") +
		QString::number(screenNum , 10 ) + QString::fromLocal8Bit("块屏幕显示，不能拖动到其他屏幕。");
		return;
		}*/
        if(!source->getStatus())
        {
            QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("您选择的成员已离线，请重新选择！"));
            return;
        }
        QString mimetext  = event->mimeData()->text();
        QStringList mimelist = mimetext.split("\t");
        QString iu = mimelist.at(0);
        QString nu = mimelist.at(1);
        QString n = mimelist.at(2);
        if(this->index==9)
        {
			if(!source->isMainSpeakerTermianl) //只允许接收主讲人的屏幕共享
			return;

			this->setHeadShow(iu,nu,n);
            emit this->receiveVideoSignal(nu,index);
        }else
        {
            if(source->videoMarkButton->isHidden())
            {
                QMessageBox::warning(this,tr("CoolView"),QString::fromLocal8Bit("您选择的成员没有音视频输出，请重新选择！"));
                return;
            }

			if( this->getMemberUri()!="" && 
				this->getMemberUri()!= nu )
			{
				//如果原来的位置上有在接收终端，那么向其发送命令停止发送音视频
				emit this->stopVideoSignal( this->getMemberUri(), this->getIndex() );
			}

           // source->frame->setStyleSheet("QFrame{border-width:2px;border-color:red;border-style:solid;border-radius: 5px;}");
                //p->videoIsInThisScreen(nu);
            PeerShowWidget *peer = p->searchPeerForWidget(nu);
            if(peer !=NULL)
            {
                if(peer == this)
                {
                    return;
                }

                peer->setHeadHide();
                this->setHeadShow(iu,nu,n);

				//如果开启了小流，且终端的视频窗口位置发生了改变，需要以下判断
				//1.如果是从大窗口到小窗口（即从第一个窗口到其他窗口）
				//2.如果是从小窗口到大窗口（即从非第一个窗口到第一个窗口）
				//这两种情况都需要关闭原来的媒体流并重新接收
				if( RunningConfig::Instance()->isEnableSmallVideo() &&
					((peer->getIndex()==0 && index>0) ||
					(peer->getIndex()>0 && index==0)) )
				{
					emit this->stopVideoSignal( nu, peer->getIndex());
					emit this->receiveVideoSignal(nu,index );
				}
				else if(peer->getScreenIndex()!=this->screenIndex)
				{
					//将远程终端在另一个屏幕上接收
					emit peer->stopVideoSignal(nu,peer->getIndex());
					emit this->receiveVideoSignal(nu,index);
				}
				else
				{
					emit this->changeVideoSeetSignal(nu,index);
				}

                event->setDropAction(Qt::CopyAction);
                event->accept();
            }
			else
            {
                this->setHeadShow(iu,nu,n);
                emit this->receiveVideoSignal(nu,index);
                event->setDropAction(Qt::CopyAction);
                event->accept();
            }
            //delete peer;
         }

    }
    else
    {
        PeerShowWidget *source2 = qobject_cast<PeerShowWidget *>(event->source());
        if(source2)
        {
			QString currentTerminalUri = QString::fromStdString(RunningProfile::getInstance()->user_uri() );
			/*if( this->screenIndex!=MediaManager::getInstance()->GetLocalPreviewScreen()
				&&source2->getMemberUri()==currentTerminalUri )
            {
				int screenNum = MediaManager::getInstance()->GetLocalPreviewScreen() + 1;
				QString message = QString::fromLocal8Bit("对不起，本地回显只能在第") +
					QString::number(screenNum , 10 ) + QString::fromLocal8Bit("块屏幕显示，不能拖动到其他屏幕。");
				QMessageBox::warning(this,tr("CoolView"),message );
                return;
            }*/
            if(source2 == this)
            {
                return;
            }
    /*        if(source2->getScreenIndex()!= this->screenIndex)
            {
                return;
            }*/
            if(source2->getIndex()==9||this->index==9)
            {
                return;
            }


            QString mimetext  = event->mimeData()->text();
            QStringList mimelist = mimetext.split("\t");
            QString iu = mimelist.at(0);
            QString nu = mimelist.at(1);
            QString n = mimelist.at(2);
			
			if( this->getMemberUri()!="" && 
				this->getMemberUri()!= nu )
			{
				//如果原来的位置上有在接收终端，那么向其发送命令停止发送音视频
				emit this->stopVideoSignal( this->getMemberUri(), this->getIndex() );
			}

            source2->setHeadHide();
            this->setHeadShow(iu,nu,n);


			//如果开启了小流，且终端的视频窗口位置发生了改变，需要以下判断
			//1.如果是从大窗口到小窗口（即从第一个窗口到其他窗口）
			//2.如果是从小窗口到大窗口（即从非第一个窗口到第一个窗口）
			//这两种情况都需要关闭原来的媒体流并重新接收
			if(  RunningConfig::Instance()->isEnableSmallVideo() &&
				((source2->getIndex()==0&&index>0) ||
				 (source2->getIndex()>0&&index==0)) )
			{
				emit this->stopVideoSignal( nu, source2->getIndex());
				emit this->receiveVideoSignal(nu,index );
			}
			else
			{
				//如果侦测到视频窗口跨屏幕拖动，关闭原来的媒体流，等渲染器的窗口信息设置好了，再重新接收
				//这样更安全。尤其在硬编码的模式下，不这样做很容易出错。
				if (source2->getScreenIndex() != screenIndex)
				{
					emit this->stopVideoSignal( nu, source2->getIndex());
					emit this->receiveVideoSignal(nu,index );
				}
				else
					emit this->changeVideoSeetSignal(nu,index);
			}
            //this->headWidget->show();

            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
    }
}

QString PeerShowWidget::getMemberUri()
{
    return nuri;
}



int PeerShowWidget::getIndex()
{
    return this->index;
}

/************************2.22***************************/
int PeerShowWidget::getScreenIndex()
{
    return this->screenIndex;
}
/************************2.22 end***************************/
void PeerShowWidget:: clearScrenn()
{
	
	this->documentLabel->show();
	iurl = ":/image/folder-with-contents.png";
	QPixmap fp(iurl);
	this->headLabel->setPixmap(fp);
	this->nameLabel->hide();
}


void PeerShowWidget:: stopVideo(QString uri,int seet)
{
	this->stopVideoSignal( uri,seet);
}


void PeerShowWidget:: receiveVideo(QString uri ,int seet)
{
	this->receiveVideoSignal( uri,seet);
}
