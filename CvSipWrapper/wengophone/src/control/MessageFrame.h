#pragma once
#include <string>
/*
消息格式专用消息基类，所有消息将全部派生于这个基类
m_sMessageBox就是用来传输的message
length  保存Message的长度

注意： 构造函数中char*[] 必须是字符串，即以\0结尾，否则生成的MessageFrame会出错。
*/
class  CMessageFrame
{
public:
	CMessageFrame(const char *msg=NULL);//Modify By LZY 2010-09-19
	~CMessageFrame(void);

	//通用函数
public:
	//获取消息长度
	int getLength();

	//转换成UTF-8格式，转换m_sMessage返回到
	char* toUtf8();
	char* getString();

	//添加消息内容串
	//void Append(CString str);
	void Append(const char* str);
	void Append(const int nt);
	void Append(const unsigned int nt);//Add By LZY 2010-09-19 添加uint类型值到数据中
	void Append(const unsigned long nt);//Add By LZY 2010-10-05 添加ulong类型值到数据中

	//读取一行内容以，行定义为"\r\n"结束
	char* readLine(bool ReceiveReturn);//Modify By LZY 2010-09-10 增加传入参数，原因看MessageFrame.cpp中该函数的修改说明

	//获取消息行数
	int getLines();

	//读取指针前移几行
	void moveForward(int rowsCount);

	//-------Add By LZY 2010-09-10-------
	//   -   获取从当前内部访问指针到结尾的所有内容
	char* readLast();
	//-------End of Add By LZY-----------

	////重置读取标记
	void resetIndex(); //Add by hexianfu 09-11-9

protected:
	//读取位置
	int m_nReadIndex;
	//串长度
	int m_nLength;
	//串
	//CString m_sMessageBox;
	//Dimer 11.26 add
	std::string m_sMessage;

	//-------Add By LZY 2010-09-10-------
	//   -   新增行信息返回用的相关变量
	char *LineBuffer;//行信息缓冲区指针
	int BufferSize;//行信息缓冲区大小
	//-------End of Add By LZY----------

	//-------Add By LZY 2010-09-15-------
	//   -   新增转utf8后的信息返回用的相关变量
	char *UTF8_Buffer;//UTF8信息缓冲区指针
	int UTF8_BufferSize;//UTF8信息缓冲区大小
	//-------End of Add By LZY----------
};
