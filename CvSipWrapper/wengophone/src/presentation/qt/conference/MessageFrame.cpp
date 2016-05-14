#include "stdafx.h"
#include ".\messageframe.h"

CVMessageFrame::CVMessageFrame(void)
{
	m_nLength = 0;
	m_nReadIndex = 0;
}

CVMessageFrame::CVMessageFrame(const char *msg)
{
	m_nReadIndex = 0;
	m_sMessageBox.append(msg);
	m_nLength = m_sMessageBox.length();
}

CVMessageFrame::~CVMessageFrame(void)
{
}

//�����Ϣ����
void CVMessageFrame::Append(string str)
{
	m_sMessageBox = m_sMessageBox + str + "\r\n";
	//�޸���Ϣ����
	m_nLength = m_sMessageBox.length();
	
}

void CVMessageFrame::Append(int nt)
{
	char buffer[20];          
	itoa(nt, buffer, 10);    
	string str(buffer); 
	m_sMessageBox = m_sMessageBox + str + "\r\n";
	m_nLength = m_sMessageBox.length();
}

//��ȡ��Ϣ����
int CVMessageFrame::getLength()
{
	return m_nLength;
}

string &CVMessageFrame::getMessage()
{
	return m_sMessageBox;
}

//ת����UTF-8��ʽ��ת��m_sMessageBox���ص�, ���ش��ĳ��ȵ�nLen
char* CVMessageFrame::toString(int &nLen)
{
	/*
	nLen = MultiByteToWideChar(CP_ACP, 0, m_sMessageBox, m_nLength, NULL, 0);

	nLen = nLen*2 + 1;
	char *strUTF = new char[nLen];
//	wchar_t *te = new wchar_t[nLen];
//	MultiByteToWideChar(CP_ACP, 0, m_sMessageBox, m_nLength, te, nLen);	
	MultiByteToWideChar(CP_ACP, 0, m_sMessageBox, m_nLength, (LPWSTR)strUTF, nLen);
	strUTF[nLen-1] = '\0';
	*/
	/*��ȡת������ִ�����
	char *p = m_sMessageBox.GetBuffer();

	nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)_T(p), -1, NULL, 0, NULL, NULL);
	char *strUTF = new char[nLen];


	nLen = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)&m_sMessageBox, m_nLength, strUTF, nLen, NULL, NULL);
	strUTF[nLen] = '\0';	
//	*/

//	return (char*)&te;
	nLen = m_nLength + 1;
	char *strUTF = new char[nLen];
	strcpy(strUTF, m_sMessageBox.c_str());
	strUTF[nLen -1] = '\0';
	return strUTF;
}


//��ȡһ��
string CVMessageFrame::ReadLine()
{
	int index = m_sMessageBox.find("\r\n", m_nReadIndex);
	if (index == -1)
		return "";
	//char *str = new char[index-m_nReadIndex +1];
	string subStr = m_sMessageBox.substr(m_nReadIndex, index-m_nReadIndex);
	//strcpy(str, subStr.c_str());
	m_nReadIndex = index + 2;
	return subStr;
}

//��ȡָ��ǰ�Ƽ���
void CVMessageFrame::SetReadIndex(int rowsCount)
{
	int index = 0;
	for (int i=0;i<rowsCount;i++)
	{
		index = m_sMessageBox.find("\r\n", m_nReadIndex);
		if (index == -1)
			return;
		m_nReadIndex = index + 2;
	}
}

//���ض�ȡλ�ú�ʣ�����Ϣ����
int CVMessageFrame::GetLines()
{
	int lines = 0;
	int index = m_nReadIndex;  //����Ϣ��ȡλ�ÿ�ʼ����ʣ�������
	int rc = m_sMessageBox.find("\r\n", index);
	while (rc>=0)
	{
		lines++;
		index = rc + 2;
		rc = m_sMessageBox.find("\r\n", index);
	}
	return lines;
}

//��ȡ����ָ����С���ڴ�
void *CVMessageFrame::read(int size)
{
	char *str = new char[size];
	string subStr = m_sMessageBox.substr(m_nReadIndex, size);
	memcpy(str,subStr.c_str(),size);
	m_nReadIndex += size;
	return str;
}
