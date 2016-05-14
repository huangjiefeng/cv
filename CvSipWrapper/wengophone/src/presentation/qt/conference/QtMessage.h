/**
 * Coolview 3.0 ½çÃæÔªËØ
 * zhenHua.sun 2010-08-14
 */
#ifndef MESSAGE_H
#define MESSAGE_H

#include "ui_qtMessage.h"
class QtToolWidget;
class QtMessage : public QWidget , public Ui::messageForm
{
    Q_OBJECT

    public:
        QtMessage(QtToolWidget * qtToolWidget);

	private:
		QtToolWidget* _qtTool;

};


#endif // MESSAGE_H
