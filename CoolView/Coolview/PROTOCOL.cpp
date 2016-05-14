#include "PROTOCOL.h"


PROTOCOL::PROTOCOL(void)
{

}


PROTOCOL::~PROTOCOL(void)
{

}

///////////////////////////////////////////////////////////////////////
//PELCO_D协议
///////////////////////////////////////////////////////////////////////

//左
void PELCO_D::left()
{
	command[2]=0x00;
	command[3]=0x04;
	command[4]=0x20;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//右
void PELCO_D::right()
{
	command[2]=0x00;
	command[3]=0x02;
	command[4]=0x20;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//上
void PELCO_D::up()
{
	command[2]=0x00;
	command[3]=0x08;
	command[4]=0x00;
	command[5]=0x20;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//下
void PELCO_D::down()
{
	command[2]=0x00;
	command[3]=0x10;
	command[4]=0x00;
	command[5]=0x20;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//停止
void PELCO_D::stop()
{
	command[2]=0x00;
	command[3]=0x00;
	command[4]=0x00;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//聚焦远
void PELCO_D::focusFar()
{
	command[2]=0x00;
	command[3]=0x80;
	command[4]=0x00;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//聚焦近
void PELCO_D::focusNear()
{
	command[2]=0x01;
	command[3]=0x00;
	command[4]=0x00;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//放大
void PELCO_D::zoomIn()
{
	command[2]=0x00;
	command[3]=0x20;
	command[4]=0x00;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//缩小
void PELCO_D::zoomOut()
{
	command[2]=0x00;
	command[3]=0x40;
	command[4]=0x00;
	command[5]=0x00;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}

//调用预置位1
void PELCO_D::callPreset1()
{
	command[2]=0x00;
	command[3]=0x07;
	command[4]=0x00;
	command[5]=0x01;

	command[6]=0x00;//计算1-5的校验和
	for(int i=1;i<6;i++)
	{
		command[6]=command[i]+command[6];
	}

}







///////////////////////////////////////////////////////////////////////
//PELCO_P协议
///////////////////////////////////////////////////////////////////////