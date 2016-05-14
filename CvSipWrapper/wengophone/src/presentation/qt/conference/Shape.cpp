#include "stdafx.h"
#include ".\shape.h"


// CShape
CShape::CShape(int shapetype,int startx,int starty,int endx,int endy)
{
	m_type = shapetype;
	m_start.setX(startx);
	m_start.setY(starty);
	m_end.setX(endx);
	m_end.setY(endy);
}
CShape::CShape(CShape& sourceShape)
{
	m_type = sourceShape.m_type;
	m_start = sourceShape.m_start;
	m_end = sourceShape.m_end;
}
void CShape::Draw(QPainter *pDC)
{
	switch(m_type)
	{
	case 1:
		pDC->drawLine(m_start,m_end);
		break;
	case 2:
		pDC->drawRect(m_start.x(),m_start.y(),m_end.x()-m_start.x(),m_end.y()-m_start.y());
		break;
	case 3:
		pDC->drawEllipse(m_start.x(),m_start.y(),m_end.x()-m_start.x(),m_end.y()-m_start.y());
		break;
	case 4:
		pDC->drawLine(m_start,m_end);
		break;
	default:
		break;
	}
}
int CShape::GetType()
{
	return m_type;
}
void CShape::UnDraw(QPainter *pDC)
{
	switch(m_type)
	{
	case 1:
		pDC->drawLine(m_start,m_end);
		break;
	case 2:
		pDC->drawRect(m_start.x(), m_start.y(), m_end.x(), m_end.y());
		break;
	case 3:
		pDC->drawEllipse(m_start.x(), m_start.y(), m_end.x(), m_end.y());
		break;
	case 4:
		pDC->drawLine(m_start,m_end);
		break;
	default:
		break;
	}
}

QPoint CShape::GetStartPoint()
{
	return m_start;
}

QPoint CShape::GetEndPoint()
{
	return m_end;
}

// CShape ��Ա����
