#include "kinectinfowidget.h"

KinectInfoWidget::KinectInfoWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

KinectInfoWidget::~KinectInfoWidget()
{

}

void KinectInfoWidget::SetInfoText( const QString& message )
{
	ui.KinectInfoText->setText(message );
}