/********************************************************************************
** Form generated from reading UI file 'fileplay_ctrl_widget.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEPLAY_CTRL_WIDGET_H
#define UI_FILEPLAY_CTRL_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FilePlayCtrlWidget
{
public:
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pause_button;
    QSlider *seek_slider;
    QLabel *time_label;

    void setupUi(QWidget *FilePlayCtrlWidget)
    {
        if (FilePlayCtrlWidget->objectName().isEmpty())
            FilePlayCtrlWidget->setObjectName(QStringLiteral("FilePlayCtrlWidget"));
        FilePlayCtrlWidget->resize(400, 41);
        horizontalLayout_2 = new QHBoxLayout(FilePlayCtrlWidget);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pause_button = new QPushButton(FilePlayCtrlWidget);
        pause_button->setObjectName(QStringLiteral("pause_button"));

        horizontalLayout_2->addWidget(pause_button);

        seek_slider = new QSlider(FilePlayCtrlWidget);
        seek_slider->setObjectName(QStringLiteral("seek_slider"));
        seek_slider->setMaximum(999);
        seek_slider->setOrientation(Qt::Horizontal);

        horizontalLayout_2->addWidget(seek_slider);

        time_label = new QLabel(FilePlayCtrlWidget);
        time_label->setObjectName(QStringLiteral("time_label"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(120, 120, 120, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        time_label->setPalette(palette);

        horizontalLayout_2->addWidget(time_label);


        retranslateUi(FilePlayCtrlWidget);

        QMetaObject::connectSlotsByName(FilePlayCtrlWidget);
    } // setupUi

    void retranslateUi(QWidget *FilePlayCtrlWidget)
    {
        FilePlayCtrlWidget->setWindowTitle(QApplication::translate("FilePlayCtrlWidget", "FilePlayCtrlWidget", 0));
        pause_button->setText(QApplication::translate("FilePlayCtrlWidget", "\346\232\202\345\201\234/\346\201\242\345\244\215", 0));
        time_label->setText(QApplication::translate("FilePlayCtrlWidget", "00:00:00/00:00:00", 0));
    } // retranslateUi

};

namespace Ui {
    class FilePlayCtrlWidget: public Ui_FilePlayCtrlWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEPLAY_CTRL_WIDGET_H
