#include "stdafx.h"
#include ".\messageframe.h"
#include <windows.h>
#include <WinNls.h>
#include <iostream>
using namespace std;

//-------Edit By LZY 2010-09-19------
CMessageFrame::CMessageFrame(const char *msg)
{
	if (msg != NULL) {
		CMessageFrame::m_sMessage = msg ;
		CMessageFrame::m_sMessage += "\r\n";
	}
	CMessageFrame::m_nReadIndex = 0;

	CMessageFrame::LineBuffer=NULL;
	CMessageFrame::BufferSize=0;
	CMessageFrame::UTF8_Buffer=NULL;
	CMessageFrame::UTF8_BufferSize=0;

	CMessageFrame::m_nLength = CMessageFrame::m_sMessage.length();
}
//-------End of Edit By LZY------

CMessageFrame::~CMessageFrame(void)
{
	//-------Add By LZY 2010-09-19-------
	if (CMessageFrame::LineBuffer!=NULL)
		delete []CMessageFrame::LineBuffer;

	if (CMessageFrame::UTF8_Buffer!=NULL)
		delete []CMessageFrame::UTF8_Buffer;
	//-------End of Add By LZY----------
}

////�����Ϣ����
//void CMessageFrame::Append(CString str)
//{
//	m_sMessageBox = m_sMessageBox + str + "\r\n";
//	//�޸���Ϣ����
//	m_nLength = m_sMessageBox.GetLength();
//
//}

void CMessageFrame::Append(const char* str) 
{
	m_sMessage = m_sMessage + str + "\r\n";
	m_nLength = m_sMessage.length();
}

void CMessageFrame::Append(const int nt)
{
	char temp[20];
	sprintf(temp, "%d", nt);
	m_sMessage = m_sMessage + temp + "\r\n";
	m_nLength = m_sMessage.length();
}

//-------Add By LZY 2010-09-19-------
/* 添加uint类型值到数据中
 * @param const unsigned int nt,要添加的值
 * @return void
 */
void CMessageFrame::Append(const unsigned int nt)
{
	char temp[8];
	sprintf(temp, "%u", nt);
	m_sMessage = m_sMessage + temp + "\r\n";
	m_nLength = m_sMessage.length();
}
//-------End of Add By LZY-----------

//-------Add By LZY 2010-10-05-------
/* 添加ulong类型值到数据中
 * @param const unsigned long nt,要添加的值
 * @return void
 */
void CMessageFrame::Append(const unsigned long nt)
{
	char temp[20];
	sprintf(temp, "%lu", nt);
	m_sMessage = m_sMessage + temp + "\r\n";
	m_nLength = m_sMessage.length();
}
//-------End of Add By LZY----------

int CMessageFrame::getLength()
{
	return m_nLength;
}

//-------Modify By LZY 2010-09-19-------
char* CMessageFrame::toUtf8()
{
	char *pbuffer= (char*) CMessageFrame::m_sMessage.c_str();

	int len = ::MultiByteToWideChar(CP_ACP,0, pbuffer, -1, NULL, 0 );
	wchar_t *pwc = new wchar_t[len + 1];
	::MultiByteToWideChar(CP_ACP, 0, pbuffer, -1, pwc, len + 1);
	
	int len2 = ::WideCharToMultiByte(CP_UTF8, 0, pwc, -1, NULL, 0, NULL, NULL);
	
	if (CMessageFrame::UTF8_BufferSize<=len2)
	{
		if (CMessageFrame::UTF8_Buffer!=NULL)
			delete []CMessageFrame::UTF8_Buffer;
		CMessageFrame::UTF8_BufferSize=(len2<3 ? 3 : len2+1);
		CMessageFrame::UTF8_Buffer=new char[CMessageFrame::UTF8_BufferSize];
	}
	::WideCharToMultiByte(CP_UTF8, 0, pwc, -1, CMessageFrame::UTF8_Buffer, len2+1, NULL, NULL);

	delete []pwc;
	return CMessageFrame::UTF8_Buffer;

	//int len = ::MultiByteToWideChar(CP_UTF8,0, pbuffer, -1, NULL, 0 );
	//wchar_t *pwc = new wchar_t[len + 1];
	//::MultiByteToWideChar(CP_UTF8, 0, pbuffer, -1, pwc, len + 1);

	////len = ::WideCharToMultiByte(CP_ACP, 0, pwc, -1, NULL, 0, NULL, NULL);
	//::WideCharToMultiByte(CP_ACP, 0, pwc, -1, pbuffer, 4096, NULL, NULL);
}
//-------End of Modify By LZY-----------

char* CMessageFrame::getString()
{
	
	return (char*) (m_sMessage.c_str());
}

//-------Modify By LZY 2010-09-19-------
char *CMessageFrame::readLine(bool ReceiveReturn)
{
	int index = m_sMessage.find("\r\n", CMessageFrame::m_nReadIndex);
	if (index == -1 || ReceiveReturn==false)
	{
		CMessageFrame::m_nReadIndex = index + 2;
		return NULL;
	}
	if (CMessageFrame::BufferSize<index-CMessageFrame::m_nReadIndex +1)
	{
		if (CMessageFrame::LineBuffer!=NULL)
			delete []CMessageFrame::LineBuffer;
		CMessageFrame::BufferSize=(index-CMessageFrame::m_nReadIndex +1 > 2) ? (index-CMessageFrame::m_nReadIndex +1) : 3;
		CMessageFrame::LineBuffer=new char [CMessageFrame::BufferSize];
	}
	memcpy(CMessageFrame::LineBuffer, CMessageFrame::m_sMessage.substr(CMessageFrame::m_nReadIndex, index-CMessageFrame::m_nReadIndex).c_str(), index-CMessageFrame::m_nReadIndex);
	CMessageFrame::LineBuffer[index-CMessageFrame::m_nReadIndex] = '\0';
	CMessageFrame::m_nReadIndex = index + 2;
	return CMessageFrame::LineBuffer;
}
//-------End Modify By LZY------------

//-------Add By LZY 2010-09-19-------
char* CMessageFrame::readLast()
{
	if (CMessageFrame::BufferSize<CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex+1)
	{
		if (CMessageFrame::LineBuffer!=NULL)
			delete []CMessageFrame::LineBuffer;
		CMessageFrame::BufferSize=CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex+1;
		if (CMessageFrame::BufferSize<3)
			CMessageFrame::BufferSize=3;
		CMessageFrame::LineBuffer=new char [CMessageFrame::BufferSize];
	}
	if (CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex>0)
	{
		memcpy(CMessageFrame::LineBuffer, CMessageFrame::m_sMessage.substr(CMessageFrame::m_nReadIndex, CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex).c_str(), CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex);
		CMessageFrame::LineBuffer[CMessageFrame::m_nLength-CMessageFrame::m_nReadIndex] = '\0';
	}
	else
		CMessageFrame::LineBuffer[0] = '\0';
	return CMessageFrame::LineBuffer;
}
//-------End of Add By LZY-------

//��ȡָ��ǰ�Ƽ���
void CMessageFrame::moveForward(int rowsCount)  //change name setReadIndex to moveForward
{
	int index = 0;
	for (int i=0;i<rowsCount;i++)
	{
		index = m_sMessage.find("\r\n", m_nReadIndex);
		if (index == -1)
			return;
		m_nReadIndex = index + 2;
	}
}

//���ض�ȡλ�ú�ʣ�����Ϣ����
int CMessageFrame::getLines()
{
	int lines = 0;
	int index = m_nReadIndex;  //����Ϣ��ȡλ�ÿ�ʼ����ʣ�������
	int rc = m_sMessage.find("\r\n", index);
	while (rc>=0)
	{
		lines++;
		index = rc + 2;
		rc = m_sMessage.find("\r\n", index);
	}
	return lines;
}


void CMessageFrame::resetIndex()
{
	m_nReadIndex=0;
}
////��ȡ����ָ����С���ڴ�
//void *CMessageFrame::read(int size)
//{
//	char *str = new char[size];
//	memcpy(str,m_sMessage.substr(m_nReadIndex, size).c_str(),size);
//	m_nReadIndex += size;
//	return str;
//}
