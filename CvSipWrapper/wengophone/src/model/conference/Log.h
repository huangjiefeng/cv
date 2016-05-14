#pragma once

#ifndef LOG_H
#define LOG_H

#include <fstream>
using namespace std;

class Log
{
public:
	Log(void);
	~Log(void);

	void startWrite();//时间，文件名，函数等

	void write(char *param);
	void write(int param);
	void write(void *p);
	void endWrite();

private:
	ofstream out;
};



#endif