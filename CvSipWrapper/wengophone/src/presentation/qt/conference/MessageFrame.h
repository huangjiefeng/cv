#pragma once
/*
消息格式专用消息基类，所有消息将全部派生于这个基类
m_sMessageBox就是用来传输的message
length  保存Message的长度
*/
#include <string>
using namespace std;
class  CVMessageFrame
{
public:
	CVMessageFrame(void);
	CVMessageFrame(const char *msg);
	~CVMessageFrame(void);

	//通用函数
public:
	//获取消息长度
	int getLength();

	//转换成UTF-8格式，转换m_sMessageBox返回到, 返回串的长度到nLen
	char* toString(int &nLen);
	//获去消息内容
	std::string &getMessage();

	//添加消息内容串
	void Append(string str);
	void Append(int nt);

	//读取一行内容以，行定义为"\r\n"结束
	string ReadLine();

	//获取消息行数
	int GetLines();

	//读取指针前移几行
	void SetReadIndex(int rowsCount);

	//读取多少指定大小的内存
	void *read(int size);



protected:
	//读取位置
	int m_nReadIndex;
	//串长度
	int m_nLength;
	//串
	std::string m_sMessageBox;
	

};
