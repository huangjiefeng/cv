#include "stdafx.h"
#include ".\drawpanel.h"
#include "QtWhiteBoardWidget.h"
#include "MessageFrame.h"

CDrawPanel::CDrawPanel(QtWhiteBoardWidget *_pWhiteBoard)
{
	pWhiteBoard = _pWhiteBoard;
	setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
	m_bLine = true;
	m_bRect = false;//�Ƿ��ڻ�����
	m_bEill = false;//�Ƿ��ڻ�Ȧ
	m_bHand = false;//�Ƿ��ֶ�
	m_bDrawing = false;
//	m_hCursor = AfxGetApp()->LoadStandardCursor(IDC_CROSS);//��ȡʮ�ֹ��

	m_nHandPoints = 80;  //80 ����ͬ������һ��
	m_nCount = 0;
}

CDrawPanel::~CDrawPanel(void)
{
}


// CDrawPanel ��Ϣ�������

void CDrawPanel::mousePressEvent(QMouseEvent *e)
{  
	if (e->button() == Qt::LeftButton)
	{
		m_Start = e->pos();
		m_End = e->pos();
		//	SetCapture();
		//	RECT rect;
		//	GetClientRect(&rect);
		//	ClientToScreen(&rect);
		//	ClipCursor(&rect);//�������ֻ����view�����ƶ�
		m_bDrawing = true;
		//	SetCursor(m_hCursor);
	}
}

void CDrawPanel::mouseReleaseEvent(QMouseEvent *e)
{  
	if (e->button() == Qt::LeftButton)
	{
		CVMessageFrame *msg = new CVMessageFrame();
		m_End = e->pos();
		//	ReleaseCapture();
		//	ClipCursor(NULL);
		string content="";
		m_bDrawing = false;
		if (m_bLine)
		{
			AddShape(1, m_Start.x(), m_Start.y(), e->pos().x(), e->pos().y());
			msg->Append(1);
		} 
		else if (m_bRect)
		{
			AddShape(2, m_Start.x(), m_Start.y(), e->pos().x(), e->pos().y());
			msg->Append(2);
		}
		else if (m_bEill)
		{
			AddShape(3, m_Start.x(), m_Start.y(), e->pos().x(), e->pos().y());
			msg->Append(3);
		} 
		//
		if (m_bHand) //�ֻ�ʱ���̧��ŷ�����Ϣ
		{
			msg->Append(4);  //ͼ������

			QPoint start, end; //�洢ûС�ε�ͷβ
			int index = m_Shape.size() - 1;

			CShape *tshape = GetShape(index);
			while (m_nCount>0)
			{ //shape ���� �ֻ�����
				start = tshape->GetStartPoint();
				end = tshape->GetEndPoint();
				msg->Append(start.x());
				msg->Append(start.y());
				msg->Append(end.x());
				msg->Append(end.y());

				index--;
				tshape = GetShape(index);

				m_nCount--;
			}
			AddShape(0,0,0,0,0); //ռλ��
			msg->Append(0); //�����ֻ��߽����ź�

		}
		else //�����ֻ�ʱ�ͼ������ʼ����ս��
		{
			msg->Append(m_Start.x());
			msg->Append(m_Start.y());
			msg->Append(e->pos().x());
			msg->Append(e->pos().y());
	
		}
		//������Ϣ
		pWhiteBoard->sendWhiteBoardMsg(msg->getMessage());
		update();
		delete msg;
	}
}

void CDrawPanel::mouseMoveEvent(QMouseEvent *e) 
{  
	if (!m_bDrawing)  return; 

	//���ڻ���
	//QPainter dc(this);
	//if(m_bLine)
	//{
	//	dc.drawLine(m_Start,m_End);
	//	dc.drawLine(m_Start,e->pos());
	//	m_End = e->pos();//��¼��ǰ���յ�
	//}
	//else if(m_bRect)
	//{
	//	dc.SelectStockObject(NULL_BRUSH);
	//	dc.MoveTo(m_Start);
	//	dc.Rectangle(m_Start.x(),m_Start.y(),m_End.x,m_End.y);
	//	dc.Rectangle(m_Start.x(),m_Start.y(),e->pos().x(),e->pos().y());
	//	m_End = point;//��¼��ǰ���յ�
	//}
	//else if(m_bEill)
	//{
	//	dc.SelectStockObject(NULL_BRUSH);
	//	dc.MoveTo(m_Start);
	//	dc.Ellipse(m_Start.x(),m_Start.y(),m_End.x,m_End.y);
	//	dc.Ellipse(m_Start.x(),m_Start.y(),e->pos().x(),e->pos().y());
	//	m_End = point;//��¼��ǰ���յ�
	//}else if(m_bHand)
	//{//�ֻ�Ƚϸ��ӣ�ÿһ����һ��Shape
	//	dc.MoveTo(m_Start);
	//	dc.LineTo(point);
	//	AddShape(4,m_Start.x(),m_Start.y(),e->pos().x(),e->pos().y());
	//	m_Start = point;

	//	//�ۼƵ�
	//	m_nCount++;
	//	if (m_nCount == m_nHandPoints)
	//	{
	//		//ͬ��
	//		SendHandShape();
	//		m_nCount = 0;
	//	}

	//} 
	//Draw(&dc);
	m_End = e->pos();
	update();
}

void CDrawPanel::paintEvent(QPaintEvent* e)
{
	QPainter paint( this );
	int index = m_Shape.size();
	for (int i=0; i<index; i++)
	{
		//����Shape����Ļ�����
		GetShape(i)->Draw(&paint);
	}
	if(m_bDrawing)
		Draw(&paint);
}


void CDrawPanel::SendHandShape()
{
	CVMessageFrame *msg = new CVMessageFrame();
	//��ʼ����Ϣͷ��
	msg->Append(4);  //ͼ������

	QPoint start, end; //�洢ÿС�ε�ͷβ
	int index = m_Shape.size() - 1;

	CShape *tshape = GetShape(index);
	while (m_nCount>0)
	{ //shape ���� �ֻ�����
		start = tshape->GetStartPoint();
		end = tshape->GetEndPoint();
		msg->Append(start.x());
		msg->Append(start.y());
		msg->Append(end.x());
		msg->Append(end.y());

		index--;
		//��0��ֹ
		tshape = GetShape(index);

		m_nCount--;
	}
	//������Ϣ
	pWhiteBoard->sendWhiteBoardMsg(msg->getMessage());
	delete msg;
}



void CDrawPanel::SetDrawLine()
{//����
	m_bLine = true;
	m_bRect = false;//�Ƿ��ڻ�����
	m_bEill = false;//�Ƿ��ڻ�Ȧ
	m_bHand = false;//�Ƿ��ֶ�
}

void CDrawPanel::SetDrawRect()
{
//	������
	m_bLine = false;
	m_bRect = true;//�Ƿ��ڻ�����
	m_bEill = false;//�Ƿ��ڻ�Ȧ
	m_bHand = false;//�Ƿ��ֶ�
}

void CDrawPanel::SetDrawEill()
{//��ԲȦ

	m_bLine = false;
	m_bRect = false;//�Ƿ��ڻ�����
	m_bEill = true;//�Ƿ��ڻ�Ȧ
	m_bHand = false;//�Ƿ��ֶ�
}

void CDrawPanel::SetDrawHand()
{
//	�ֶ�
	m_bLine = false;
	m_bRect = false;//�Ƿ��ڻ�����
	m_bEill = false;//�Ƿ��ڻ�Ȧ
	m_bHand = true;//�Ƿ��ֶ�
}


void CDrawPanel::AddShape(int type,int x1,int y1,int x2,int y2)
{
	m_Shape.push_back(new CShape(type, x1, y1, x2, y2));
}

void CDrawPanel::AddShape(CShape* doShape) 
{
	m_Shape.push_back(doShape);
}

CShape* CDrawPanel::GetShape(int index)
{
	if(index <0 || index > m_Shape.size())
		return NULL;
	return (CShape*)m_Shape.at(index);
}

CShape* CDrawPanel::GetUndoShape(int index)
{
	if(index <0 || index > m_UndoShape.size())
		return NULL;
	return (CShape*)m_UndoShape.at(index);
}

void CDrawPanel::DeleteShape()
{
	//indexΪ��������һ�����Ѿ�����1 
	int index = m_Shape.size() - 1;
	CShape *tshape = GetShape(index);
	if (tshape == NULL) return;

	if (tshape->GetType() != 0) //ռλ�㣬��ʾ����ĵ����ֻ��
	{ //��ͨͼ��
		//��Shape���Ƴ����һ��
		m_Shape.remove(index);
		//���Shape�����һ����Undo��
		m_UndoShape.push_back(tshape);
		return;
	}
	else
	{
		//�Ƴ�ռλ��
		m_UndoShape.push_back(tshape);
		m_Shape.remove(index);
		index--;
		tshape = GetShape(index);

		//�ֹ�ͼ��
		while (tshape!=NULL && tshape->GetType() == 4)
		{
			m_UndoShape.push_back(tshape);
			m_Shape.remove(index);
			index--;
			//��0��ֹ
			if (index<0)  break;
			tshape = GetShape(index);
		}


	}

}


void CDrawPanel::DeleteUndoShape()
{
	//indexΪ��������һ�����Ѿ�����1 
	int index = m_UndoShape.size() -1;
	CShape *tshape = GetUndoShape(index);
	if (tshape == NULL) return;

	if (tshape->GetType() != 4)
	{ //��ͨͼ��
		m_UndoShape.remove(index);

		m_Shape.push_back(tshape);
	}
	else
	{
		while (tshape!=NULL && tshape->GetType() == 4)
		{
			m_Shape.push_back(tshape);
			m_UndoShape.remove(index);
			index--;
			//��0��ֹ
			tshape = GetUndoShape(index);
			if (index<0) break;
		}
		if (tshape!=NULL && tshape->GetType() == 0) //�Ƴ�ռλ��
		{
			m_UndoShape.remove(index);
			//���Shape�����һ����Undo��
			m_Shape.push_back(tshape);
		}
	}

}




void CDrawPanel::UndoDraw()
{
	// TODO: UnDo
	DeleteShape();
	update();
}

void CDrawPanel::RedoDraw()
{
	// TODO: ReDo
	DeleteUndoShape();
	update();
}

void CDrawPanel::ClearPanel()
{
	// TODO: ����Ļ
	m_Shape.clear();
	m_UndoShape.clear();
	update();
}


void CDrawPanel::Draw(QPainter *paint)
{
	if(m_bLine)
	{
		paint->drawLine(m_Start,m_End);
	}
	else if(m_bRect)
	{
		paint->drawRect(m_Start.x(),m_Start.y(),m_End.x()-m_Start.x(),m_End.y()-m_Start.y());
	}
	else if(m_bEill)
	{
		paint->drawEllipse(m_Start.x(),m_Start.y(),m_End.x()-m_Start.x(),m_End.y()-m_Start.y());
	}else if(m_bHand)
	{//�ֻ�Ƚϸ��ӣ�ÿһ����һ��Shape
		paint->drawLine(m_Start,m_End);
		AddShape(4,m_Start.x(),m_Start.y(),m_End.x(),m_End.y());
		m_Start = m_End;

		//�ۼƵ�
		m_nCount++;
		if (m_nCount == m_nHandPoints)
		{
			//ͬ��
			SendHandShape();
			m_nCount = 0;
		}

	} 
}

void CDrawPanel::OnNetDrawMsg(const char *message)
{
	CVMessageFrame msg(message);
	int lines; //���ڶ�ȡ�ֻ�ʱ������
	int t,x,y,m,n;
	t = atoi(msg.ReadLine().c_str());
	switch(t)
	{
	case 1:
	case 2:
	case 3:
		x = atoi(msg.ReadLine().c_str());
		y = atoi(msg.ReadLine().c_str());
		m = atoi(msg.ReadLine().c_str());
		n = atoi(msg.ReadLine().c_str());
		AddShape(t, x,y,m,n);
		break;
	case 4:
		lines = msg.GetLines();

		//һ�ζ�2���㣬4�г���
		for (int i=0; i<lines-1; i+=4) //�ų����һ����
		{
			x = atoi(msg.ReadLine().c_str());
			y = atoi(msg.ReadLine().c_str());
			m = atoi(msg.ReadLine().c_str());
			n = atoi(msg.ReadLine().c_str());
			AddShape(t, x, y, m, n);
		}
		//��������һ��ռλ��
		if (lines%4 == 1) //��ռλ��
			AddShape(0,0,0,0,0);
		break;
	case 5:
		UndoDraw();
		break;
	case 6:
		RedoDraw();
		break;
	case 7:
		ClearPanel();
		break;
	default:
		break;
	}
	update();
}

void CDrawPanel::OnDrawPanelBarMSG(int type)
{
	CVMessageFrame msg;
	switch(type)
	{
	//case 1://Line
	//	SetDrawLine();
	//	break;
	//case 2://Rect
	//	SetDrawRect();
	//	break;
	//case 3://Eill
	//	SetDrawEill();
	//	break;
	//case 4://Hand
	//	SetDrawHand();
	//	break;
	case 5://Undo
		UndoDraw();
		//���ͳ�����Ϣ
		msg.Append(5);
		break;
	case 6://Redo
		RedoDraw();
		//�����ػ���Ϣ
		msg.Append(6);
		break;
	case 7:
		ClearPanel();
		//����������Ϣ
		msg.Append(7);
		break;
	default:
		break;
	}
	//������Ϣ
	pWhiteBoard->sendWhiteBoardMsg(msg.getMessage());
	update();
}


void CDrawPanel::Save(QString fileName)
{
	//std::ofstream out;
	//out.open("d:\\test.txt",ofstream::out|ofstream::app);
	//int index = m_Shape.size();
	//for (int i=0; i<index; i++)
	//{
	//	//����ͼ����Ϣ
	//	int m_type = GetShape(i)->GetType();
	//	QPoint m_start = GetShape(i)->GetStartPoint();
	//	QPoint m_end = GetShape(i)->GetEndPoint();
	//	out<<m_type<<m_start.x()<<m_start.y()<<m_end.x()<< m_end.y();
	//}
	//out.close();
	QFile file(fileName);
	file.open(QIODevice::ReadWrite);
	QTextStream out(&file);
	int index = m_Shape.size();
	for (int i=0; i<index; i++)
	{
		//����ͼ����Ϣ
		int m_type = GetShape(i)->GetType();
		QPoint m_start = GetShape(i)->GetStartPoint();
		QPoint m_end = GetShape(i)->GetEndPoint();
		out<<m_type<<" "<<m_start.x()<<" "<<m_start.y()<<" "<<m_end.x()<< " "<<m_end.y()<<" ";
	}
	file.close();
}

void CDrawPanel::Open(QString fileName)
{
	QFile file(fileName);
	file.open(QIODevice::ReadWrite);
	QTextStream in(&file);
	while(!in.atEnd())
	{
		//����ͼ����Ϣ
		int m_type;
		int m_start_x;
		int m_start_y;
		int m_end_x;
		int m_end_y;
		in>>m_type>>m_start_x>>m_start_y>>m_end_x>> m_end_y;
		AddShape(m_type, m_start_x, m_start_y, m_end_x, m_end_y);
	}
	update();
	file.close();
}
