#include "im-channel.h"


#include <QtGui/QtGui>

#include "CoolviewCommon.h"
#include "devicelist_widget.h"
#include "addresslist-widget.h"
#include "im-widget.h"
#include "main-window.h"
#include "util/IMManager.h"

IMChannel::IMChannel(MainWindow *p , QString cid):_mainWindow(p), _contactID(cid)
{ 
    _mainWindow->showIMSlot();

    connect(_mainWindow,SIGNAL(closeConfSignal()),this , SLOT(closeTextChannel()));
    connect(this,SIGNAL(clearIMChannelSignal(QString)), _mainWindow ,SLOT(clearIMChannelSlot(QString)));
    
	createIMWidget();
}

IMChannel::~IMChannel()
{
    SAFE_DELETE( _channelWidget );

  // textchannel->;
    //~textchannel;

}
//
//void IMChannel::onNewTextChannels(const Tp::ChannelDetailsList &channels)
//{
//    qDebug() << "CallWindow::onNewChannels";
//    Q_FOREACH (const Tp::ChannelDetails &details, channels) {
//        QString channelType = details.properties.value(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".ChannelType")).toString();
//        bool requested = details.properties.value(QLatin1String(TELEPATHY_INTERFACE_CHANNEL ".Requested")).toBool();
//        QString targetHandleType = details.properties.value(TELEPATHY_INTERFACE_CHANNEL ".TargetHandleType").toString();
//        QString targetHandle = details.properties.value(TELEPATHY_INTERFACE_CHANNEL ".TargetHandle").toString();
//        QString targetId = details.properties.value(TELEPATHY_INTERFACE_CHANNEL ".TargetID").toString();
//        QString channelPath = details.channel.path();
//        qDebug() << " channelType:" << channelType;
//        qDebug() << " requested  :" << requested;
//        qDebug()<<"targethandletype:"<< targetHandleType;
//        qDebug()<<"targethandle:"<<targetHandle;
//        qDebug()<<"targetid:"<<targetId;
//        qDebug()<<"channelpath:"<<channelPath;
//
//
//
//        if (channelType == QLatin1String(TELEPATHY_INTERFACE_CHANNEL_TYPE_TEXT) &&
//            !requested) {
//           // TextChannelPtr channel = TextChannel::create(tConns,
//            textchannel = Tp::TextChannel::create(tConns,
//                        details.channel.path(),
//                        details.properties);
//           //int i =  textchannel->messagesInterface()->PendingMessages().count();
//            qDebug()<<"222222222!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<textchannel->channelType();
//            channeltype = 0;
//           // qDebug()<<"33333333333333333!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<i;
//            if(widget == NULL)
//            {
//                createIMWidget();
//            }
//
//          addIncomingText(textchannel);
//        }
//    }
//}
//
//void IMChannel::addIncomingText(const Tp::TextChannelPtr &chan)
//{
//    tChannels.append(chan);
//    qDebug()<<"222222222!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
//    connect(chan->becomeReady(),
//            SIGNAL(finished(Tp::PendingOperation*)),this,
//            SLOT(onIncomingChannelReady(Tp::PendingOperation*)));
//}
//
//void IMChannel::onIncomingChannelReady(Tp::PendingOperation *op)
//{
//    Tp::PendingReady *pr = qobject_cast<Tp::PendingReady *>(op);
//    Tp::TextChannelPtr chan = Tp::TextChannelPtr(qobject_cast<Tp::TextChannel *>(pr->object()));
//
//    if (op->isError()) {
//        // ignore - channel cannot be ready
//        qWarning() << "CallHandler::onIncomingChannelReady: channel cannot become ready:" <<
//            op->errorName() << "-" << op->errorMessage();
//
//       // mChannels.removeOne(chan);
//        return;
//    }
//    qDebug()<<"1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<textchannel->channelType();
//    Tp::ContactPtr contact = chan->initiatorContact();
//
//
//      QDBusPendingReply<Tp::PendingTextMessageList> reply = textchannel->textInterface()->ListPendingMessages(true);
//      Tp::PendingTextMessageList pendinglist =  reply.value();
//      QString pendingtext = pendinglist.at(0).text;
//      QString pendingsender = contact.data()->id();
//      QString pendingtime = QString::number(pendinglist.at(0).unixTimestamp);
//
//    //this->iWidget->receiverLinEdit->setText(contact.data()->id());
//    widget->textBrowser->setPlainText(pendingsender+" "+pendingtime+"\n"+pendingtext+"\n");
// //   connect(widget->sendButton,SIGNAL(clicked()),this,SLOT(sendTextSlot()));
//    connect(chan->textInterface(),SIGNAL(Sent(uint,uint,QString)),this,SLOT(showSelfText(uint,uint,QString)));
//    connect(chan->textInterface(),SIGNAL(Received(uint,uint,uint,uint,uint,QString)),
//            SLOT(Received(uint,uint,uint,uint,uint,QString)));
//}
//
//void IMChannel::onTConnectionInvalidated(Tp::DBusProxy *proxy,
//        const QString &errorName, const QString &errorMessage)
//{
//    qDebug() << "IMChannel::onConnectionInvalidated: connection became invalid:" <<
//        errorName << "-" << errorMessage;
//  /*  foreach (const ConnectionPtr &conn, tConns) {
//        if (conn.data() == proxy) {
//            //conflistwidget->removeConnection(conn);
//            tConns.removeOne(conn);
//        }
//    }*/
//}
//
//void IMChannel::onNewTChannels(Tp::PendingOperation* op)
//{
//    if (op->isError()) {
//        qWarning() << "CallHandler::onNewTChannels: channel cannot be created:" <<
//            op->errorName() << "-" << op->errorMessage();
//        return;
//    }
//
//    Tp::PendingChannel *pc = qobject_cast<Tp::PendingChannel *>(op);
//    textchannel = Tp::TextChannel::create(pc->connection(),pc->objectPath(),pc->immutableProperties());
//
//    channeltype = 1;
//    if(widget ==NULL)
//    {
//        createIMWidget();
//    }
//    else{
//        return;
//    }
//
//
//    qDebug()<<"!!!!!!!!!!!textchannel-onNewTChannels"<<textchannel->targetHandle();
//    tChannels.append(textchannel);
//    connect(textchannel->becomeReady(),SIGNAL(finished(Tp::PendingOperation*)),SLOT(onOutgoingTChannelReady(Tp::PendingOperation*)));
//
//}
//
//void IMChannel::onOutgoingTChannelReady(Tp::PendingOperation *op)
//{
//    Tp::PendingReady *pr = qobject_cast<Tp::PendingReady *>(op);
//    Tp::TextChannelPtr channel =  Tp::TextChannelPtr(qobject_cast<Tp::TextChannel *>(pr->object()));
//    qDebug()<<"channel2:"<<channel->channelType()<<channel->targetHandle();
//    if (op->isError()) {
//        qWarning() << "CallHandler::onOutgoingTChannelReady: channel cannot become ready:" <<
//            op->errorName() << "-" << op->errorMessage();
//        tChannels.removeOne(channel);
//        return;
//    }
//    qDebug()<<"text channel is ready";
//    qDebug()<<"getting text" ;//<<channel->continitiatorContact();
//    QList <ContactPtr> clist;
//    foreach (const ContactPtr &contact, tConns->contactManager()->allKnownContacts()) {
//        clist.append(contact);
//        qDebug()<<"add to group:"<<contact->id();
//        }
//    channel->groupAddContacts( clist);
//   qDebug()<< "channel featurecore"<<channel->groupCanAddContacts()<<channel->groupCanAcceptContactsWithMessage();
//
//    foreach (const ContactPtr &contact, channel->groupContacts()) {
//        //clist.append(contact);
//        qDebug()<<"group:"<<contact->id();
//        }
////    connect(widget->sendButton,SIGNAL(clicked()),this,SLOT(sendTextSlot()));
//    connect(channel->textInterface(),SIGNAL(Sent(uint,uint,QString)),this,SLOT(showSelfText(uint,uint,QString)));
//   // connect(channel->grogroupInterface(),SIGNAL(Sent(uint,uint,QString)),this,SLOT(showSelfText(uint,uint,QString)));
////    connect(channel->textInterface(),SIGNAL(SendError(uint,uint,uint,QString)),this,SLOT(SendErrorSlot(uint,uint,uint,QString)));
//    connect(channel->textInterface(),SIGNAL(Received(uint,uint,uint,uint,uint,QString)),
//            SLOT(Received(uint,uint,uint,uint,uint,QString)));
//}
//
//void IMChannel::Received(uint id, uint timestamp, uint sender, uint type, uint flags, const QString &text)
//{
//    //this->iWidget->receiverLinEdit->setText(sender);
//  // PendingHandles ph = tConns->referenceHandles(type, sender)->validNames().at(0);
//    if(widget->isHidden())
//    {
//        //createIMWidget();
//        widget->show();
//        _mainWindow->imTabWidget->addTab(widget,_contactID);
//        qDebug()<<"addTab!!!!!!!";
//        _mainWindow->actionIM->setEnabled(true);
//        _mainWindow->showIMSlot();
//        _mainWindow->imTabWidget->setCurrentWidget(widget);
//    }
//
//    Tp::ContactPtr contact = tConns->contactManager()->lookupContactByHandle(sender);
//    //ContactPtr contact = textchannel->initiatorContact();
//    QString currentText = widget->textBrowser->toPlainText();
//     //this->iWidget->receiverLinEdit->setText(contact.data()->id());
//    QString receiverid =contact.data()->id();
//    QString sendtime = QString::number(timestamp);
//    qDebug()<<id<<timestamp<<sender<<type<<flags;
//    currentText.append(receiverid+" "+sendtime+"\n"+text+"\n");
//    widget->textBrowser->setPlainText(currentText);
//    Tp::UIntList ids;
//    ids.append(id);
//    textchannel->textInterface()->AcknowledgePendingMessages(ids);
//}
//
//void IMChannel::showText(const Tp::ReceivedMessage &message)
//{
//       QString currentText = widget->textBrowser->toPlainText();
//       currentText.append(message.text());
//       widget->textBrowser->setPlainText(currentText);
//}
//
//void IMChannel::SendErrorSlot(uint error, uint timestamp, uint type, QString text)
//{
//    qDebug()<<error<<timestamp<<type<<text;
//}
//
void IMChannel::sendTextSlot( const QString& text )
{
	int result = IMManager::getInstance()->SendMessage( _contactID , "" , text );
	if( result<0 )
    {
		_channelWidget->showErrorMessage( QString::fromLocal8Bit("发送失败：没有可用的IM账户"));
    }

}

void IMChannel::createIMWidget()
{
    for(int i = 0 ;i<_mainWindow->imTabWidget->count() ; i++)
    {
        if(_mainWindow->imTabWidget->tabText(i) == _contactID)
        {
         //   widget = qobject_cast<IMWidget *>parent->imTabWidget->widget(i);
            return;
        }
    }

    _channelWidget = new IMWidget( _mainWindow, _contactID);
	QString contactName = _contactID.left( _contactID.indexOf("@") );
    _mainWindow->imTabWidget->addTab(_channelWidget,contactName);
    qDebug()<<"addTab!!!!!!!";
    _mainWindow->actionIM->setEnabled(true);
    _mainWindow->showIMSlot();
    _mainWindow->imTabWidget->setCurrentWidget(_channelWidget);
}

void IMChannel::showIMWidget()
{
    if(_channelWidget && _channelWidget->isHidden())
	{
		_channelWidget->show();
		QString contactName = _contactID.left( _contactID.indexOf("@") );
        _mainWindow->imTabWidget->addTab( _channelWidget ,contactName );
    }
		_mainWindow->showIMSlot();
		_mainWindow->imTabWidget->setCurrentWidget(_channelWidget);
    }


void IMChannel::closeTextChannel()
{
 //   qDebug()<<"close!!";
 //   connect(textchannel->requestClose(),SIGNAL(finished(Tp::PendingOperation*)),this,SLOT(removeIMChannelSlot()));

    //qDebug()<<"close!!2";
	clearIMChannelSignal(_contactID);
}

void IMChannel::receiveMessageSlot( const QString &from, const QString &subject, const QString &body, const QString &time )
{
	_channelWidget->showIMMessage( from , subject , body , time );
}

//void IMChannel::removeIMChannelSlot()
//{
//
//    /*for(int i = 0 ; i<imchannels.count();i++)
//    {
//        if(imchannels.at(i)->cid == cid)
//        {
//            imchannels.removeAt(i);
//            qDebug()<<"clear2......................."<<imchannels.count();
//        }
//    }
//    foreach ( IMChannel* chan, imchannels) {
//        delete chan;
//    }
//    for(int i = 0 ; i<this->imTabWidget->count();i++)
//    {
//        this->imTabWidget->removeTab(i);
//    }*/
//}
