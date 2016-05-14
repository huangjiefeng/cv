/********************************************************************************
** Form generated from reading UI file 'conference_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFERENCE_WIDGET_H
#define UI_CONFERENCE_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConferenceWidget
{
public:
    QVBoxLayout *verticalLayout;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QLabel *label_1;
    QComboBox *conferenceComboBox;
    QPushButton *enterConferenceButton;
    QPushButton *leaveConferenceButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *ConferenceWidget)
    {
        if (ConferenceWidget->objectName().isEmpty())
            ConferenceWidget->setObjectName(QStringLiteral("ConferenceWidget"));
        ConferenceWidget->resize(1118, 30);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConferenceWidget->sizePolicy().hasHeightForWidth());
        ConferenceWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(ConferenceWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        widget = new QWidget(ConferenceWidget);
        widget->setObjectName(QStringLiteral("widget"));
        sizePolicy.setHeightForWidth(widget->sizePolicy().hasHeightForWidth());
        widget->setSizePolicy(sizePolicy);
        widget->setMinimumSize(QSize(0, 30));
        widget->setMaximumSize(QSize(16777215, 30));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush);
        QBrush brush1(QColor(170, 170, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
        widget->setPalette(palette);
        widget->setAutoFillBackground(true);
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(5, 5, 5, 5);
        label_1 = new QLabel(widget);
        label_1->setObjectName(QStringLiteral("label_1"));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\256\213\344\275\223"));
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        label_1->setFont(font);

        horizontalLayout->addWidget(label_1);

        conferenceComboBox = new QComboBox(widget);
        conferenceComboBox->setObjectName(QStringLiteral("conferenceComboBox"));
        conferenceComboBox->setMinimumSize(QSize(270, 0));
        conferenceComboBox->setStyleSheet(QLatin1String("QComboBox QAbstractItemView {\n"
"     font-size: 30px;\n"
" }"));

        horizontalLayout->addWidget(conferenceComboBox);

        enterConferenceButton = new QPushButton(widget);
        enterConferenceButton->setObjectName(QStringLiteral("enterConferenceButton"));

        horizontalLayout->addWidget(enterConferenceButton);

        leaveConferenceButton = new QPushButton(widget);
        leaveConferenceButton->setObjectName(QStringLiteral("leaveConferenceButton"));

        horizontalLayout->addWidget(leaveConferenceButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addWidget(widget);


        retranslateUi(ConferenceWidget);

        QMetaObject::connectSlotsByName(ConferenceWidget);
    } // setupUi

    void retranslateUi(QWidget *ConferenceWidget)
    {
        ConferenceWidget->setWindowTitle(QApplication::translate("ConferenceWidget", "ConferenceWidget", 0));
        label_1->setText(QApplication::translate("ConferenceWidget", "\344\274\232\350\256\256\357\274\232", 0));
#ifndef QT_NO_TOOLTIP
        conferenceComboBox->setToolTip(QApplication::translate("ConferenceWidget", "\344\274\232\350\256\256\345\210\227\350\241\250", 0));
#endif // QT_NO_TOOLTIP
        enterConferenceButton->setText(QApplication::translate("ConferenceWidget", "\350\277\233\345\205\245", 0));
        leaveConferenceButton->setText(QApplication::translate("ConferenceWidget", "\351\200\200\345\207\272", 0));
    } // retranslateUi

};

namespace Ui {
    class ConferenceWidget: public Ui_ConferenceWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFERENCE_WIDGET_H
