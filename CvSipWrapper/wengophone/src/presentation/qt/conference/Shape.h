#pragma once
#include <QtGui/QtGui>

class CShape
{
public:
	CShape(int shapetype,int startx,int starty,int endx,int endy);
	CShape(CShape& sourceShape);
	void Draw(QPainter *pDC);
	void UnDraw(QPainter *pDC);
	int GetType();

	//返回点
	QPoint GetStartPoint();
	QPoint GetEndPoint();
protected:
	int m_type;
	//起点
	QPoint m_start;
	//终点
	QPoint m_end;

};