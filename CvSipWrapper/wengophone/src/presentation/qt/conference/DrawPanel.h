#pragma once
#include "Shape.h"
#include <string>
using namespace std;
class QtWhiteBoardWidget;

class CDrawPanel:public QWidget
{
public:
	CDrawPanel(QtWhiteBoardWidget *_pWhiteBoard);
	~CDrawPanel(void);
public:
	QVector <CShape*>m_Shape;//用来记录所画图像的
	QVector <CShape*>m_UndoShape;//用于记录撤销的图象
	// Operations
public:
	//添加一个shape
	void AddShape(int type,int x1,int y1,int x2,int y2);
	void AddShape(CShape* doShape);
	CShape* GetShape(int index);
	void DeleteShape();
	//添加一个撤销的shape
	void AddUndoShape(CShape* undoShape);
	CShape* GetUndoShape(int index);
	void DeleteUndoShape();

	//发送手绘图形
	void SendHandShape();
	//更新画板
	void OnNetDrawMsg(const char *message);
	void OnDrawPanelBarMSG(int type);

	void Save(QString fileName);
	void Open(QString fileName);

protected:	
	bool m_bLine;//是否在直线
	bool m_bRect;//是否在画方框
	bool m_bEill;//是否在画圈
	bool m_bHand;//是否手动
	bool m_bDrawing;
	QPoint m_Start,m_End;//记录开始和结束的点


	//下面的变量用于同步手绘
	int m_nHandPoints; //多少个点同步发送一次
	int m_nCount; //记录画了多少个点




public:
	void ClearPanel();
	
	// Generated message map functions

	void SetDrawLine();
	void SetDrawRect();
	void SetDrawEill();
	void SetDrawHand();
	void SetDrawPict();
	void RedoDraw();
	void UndoDraw();
	void Draw(QPainter *paint);


protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void paintEvent(QPaintEvent*e );

private:
	QtWhiteBoardWidget *pWhiteBoard;
};
