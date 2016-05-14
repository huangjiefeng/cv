#pragma once
//2014.4.2.gmlan-增加云台控制功能
#include <QMainWindow>

class PROTOCOL
{
public:
	PROTOCOL(void);
	~PROTOCOL(void);

	virtual void left(){};//左
	virtual void right(){};//右
	virtual void up(){};//上
	virtual void down(){};//下
	virtual void stop(){};//停止
	virtual void focusFar(){};//聚焦远
	virtual void focusNear(){};//聚焦近
	virtual void zoomIn(){};//放大
	virtual void zoomOut(){};//缩小
	virtual void callPreset1(){};//调用预置位1
	//virtual void ();

//private:
	PROTOCOL* protocol;
	QByteArray command;

};

class PELCO_D:public PROTOCOL
{
public:
	PELCO_D(){ command.resize(7); command[0] = 0xff; }
	~PELCO_D(){}

	 void left();//左
	 void right();
	 void up();
	 void down();
	 void stop();
	 void focusFar();
	 void focusNear();
	 void zoomIn();//放大
	 void zoomOut();//缩小
	 void callPreset1();//调用预置位1

private:
	
};

class PELCO_P:public PROTOCOL
{
public:
	PELCO_P(){ command.resize(8); command[0] = 0xa0; }
	~PELCO_P(){}

	//void left();//左
	//void right();
	//void up();
	//void down();
	//void stop();
	//void focusFar();
	//void focusNear();
	//void zoomIn();//放大
	//void zoomOut();//缩小
	//void callPreset1();//调用预置位1

private:

};

class VISCA:public PROTOCOL
{
public:
	VISCA(){};
	~VISCA(){};

	//void left();//左
	//void right();
	//void up();
	//void down();
	//void stop();
	//void focusFar();
	//void focusNear();
	//void zoomIn();//放大
	//void zoomOut();//缩小
	//void callPreset1();//调用预置位1

private:

};