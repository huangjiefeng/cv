/************************************************************************/
/*Coolview 3.0 界面
 *zhenHua.sun 2010-08-14
 */
/************************************************************************/
#ifndef SIPCALL_H
#define SIPCALL_H

#include "ui_qtSipCall.h"
class QtToolWidget;
class QtContactList;
class CWengoPhone;
class QtSipCall : public QWidget , public Ui::sipCallForm
{
    Q_OBJECT

    public:
        QtSipCall(QtToolWidget * qtTool , CWengoPhone &cWengoPhone );
//        void addItem(QTreeWidgetItem *parent, QStringList *sl);//给Tree列表添加项

		HWND getSipCallPreviewWnd();

	private:
		QtToolWidget* _qtTool;
		QtContactList* _qtContactList;
		CWengoPhone& _cWengoPhone;


        //以下的槽都是呼叫功能里的
    private Q_SLOTS:
        void pressNum1();
        void pressNum2();
        void pressNum3();
        void pressNum4();
        void pressNum5();
        void pressNum6();
        void pressNum7();
        void pressNum8();
        void pressNum9();
        void pressNum0();
        void pressNumstar(); //*号
        void pressNumpound();//#号
        void getPhoneNum();  //选中通讯录中的一个项就显示到文本框中

		/// @brief 添加联系人
		void addContactClicked();

		/// @brief 删除联系人
		void deleteContactClicked();



		/**
		 * @brief Sip呼叫
		 */
		void sipCallClicked();	

		/** 
		 * @brief 更新联系人列表
		 */
		void updateContactList();

		/**
		 * @brief 隐藏拨号栏
		 */
		void hideDiapadSlot( bool checked);


};


#endif // SIPCALL_H
