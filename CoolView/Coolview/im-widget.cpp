#include "im-widget.h"


#include <QDebug>
#include <QtGui/QtGui>
#include <QtDBus/QtDBus>

IMWidget::IMWidget(MainWindow *parent, QString id)
    : p(parent),
    cid(id)
{
    setupUi(this);
   // parent->showIMSlot();
    //setAttribute(Qt::WA_DeleteOnClose);

    connect(sendButton,SIGNAL(clicked()),this,SLOT(sendTextSlot()));
    connect(this->textBrowser,SIGNAL(textChanged()),this,SLOT(autoScroll()));                        //12.31
}

IMWidget::~IMWidget()
{
  //  tConns->requestDisconnect();
 //   textchannel->requestClose();
  // textchannel->;
    //~textchannel;
    qDebug()<<"delete imwidget";

}

void IMWidget::showIMWidgetSlot(QString text)
{
    if(this->isHidden())
    {
        //createIMWidget();
        this->show();
        QString name = cid.left( cid.indexOf("@") );       //12.31
        p->imTabWidget->addTab(this,name);               //12.31
       // qDebug()<<"addTab!!!!!!!";
        p->actionIM->setEnabled(true);
        p->showIMSlot();
        p->imTabWidget->setCurrentWidget(this);
    }
    QString currentText = this->textBrowser->toPlainText();
    currentText.append(text);
    textBrowser->setPlainText(currentText);
    textEdit->clear();
}

void IMWidget::sendTextSlot()
{
    if(textEdit->toPlainText().isEmpty())
    {
        return;
    }
    emit sendTextSignal(textEdit->toPlainText());
}

void IMWidget::showIMMessage( const QString&from , const QString& subject , const QString& body , const QString&time )
{
	QString imTime = time;
	if( time=="" )
		imTime = QTime::currentTime().toString("hh:mm:ss");

	QString title  = QString::fromLocal8Bit("<font size=\"4\" color=\"green\">") + from + " " + imTime+ "</font>";
	//½«body×Ö·û´®×ªÎªHTMLÓïÑÔ
	QString messageBody = body;
	messageBody.replace( "<" , "&lt;");
	messageBody.replace( ">" , "&gt;");
	messageBody.replace( " " , "&nbsp;");
	messageBody.replace( "\n" , "<br>" );
	QString message = QString::fromLocal8Bit("<font size=\"4\" color=\"black\">") + messageBody + "</font>";
	QString text = title + "<br>" + message;
	this->textBrowser->append( text );
}


void IMWidget::SendErrorSlot(uint error, uint timestamp, uint type, QString text)
{
    QDateTime time = QDateTime::fromTime_t(timestamp);
    qDebug()<<error<<time.toString("hh:mm:ss")<<type<<text;
}

void IMWidget::showErrorMessage( const QString& errorMessge )
{
	QString message = QString::fromLocal8Bit("<font size=\"3\" color=\"red\">") + errorMessge+ QString::fromLocal8Bit("</font><br>");
	this->textBrowser->append( message );
}
/*******************************textchannel*******************************/
/*********************************12.31***********************************/
void IMWidget::autoScroll()
{
    QTextCursor cursor = this->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->textBrowser->setTextCursor(cursor);
}
/*********************************12.31***********************************/
