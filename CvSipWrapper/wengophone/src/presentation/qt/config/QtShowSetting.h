/**
 * Coolview 3.0 界面更新  
 * zhenHua.sun 2010-08-14
 */

#ifndef SHOWSETTING_H
#define SHOWSETTING_H

#include "ui_qtShowSetting.h"
class QtToolWidget;

class QtShowSetting : public QWidget 
{
	friend class QtToolWidget;
    Q_OBJECT

    public:
        QtShowSetting(QtToolWidget* qtToolWidget);
		~QtShowSetting();

		Ui::showSettingForm *_ui;

	public Q_SLOTS:
		/**
		 * @brief 更新会议室布局
		 */
		void updateConfRoomDisplay1X1( );
		void updateConfRoomDisplay2X2( );
		void updateConfRoomDisplay3X3( );
		void updateConfRoomDisplayL3X3( );
		void updateConfRoomDisplayL4X4();
		void updateConfRoomDisplayL5X5();
		void updateConfRoomDisplayAll();
		void teleSetLayoutTypeCommand(QString LayoutType);//Add By LZY 2010-11-03 处理遥控器SetLayoutType动作Slot

	private:
		
		QtToolWidget *_qtTool;
};

#endif // SHOWSETTING_H
