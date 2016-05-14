/************************************************************************/
/* Declaration of template class                                        */
/************************************************************************/

#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <memory.h>

template<typename T>
class CCircularBuffer
{
public:
	CCircularBuffer(void);
	virtual ~CCircularBuffer(void);

	int  Allocate(unsigned int size);
	void Clear();
	void Destroy();

	//删除头部数据
	void Pop();

	//返回内部数组长度
	unsigned int BufSize() const;

	//[]运算符返回以开始位置作为起始下标0的数据引用，实现随机访问，超过BufSize()将抛出异常
	T & operator[](const unsigned int index);

protected:

	unsigned int GetPos(unsigned int base, unsigned int offset);

private:

	T * _buf;
	unsigned int _size;

	unsigned int _headPos; //第一个数据所在位置
	//unsigned int _tailPos; //最后一个数据所在位置，注意：中间可能不连续
	//bool _empty;

};


/************************************************************************/
/* Definition of the class members                                      */
/************************************************************************/
template<typename T>
CCircularBuffer<T>::CCircularBuffer(void)
: _buf(nullptr)
, _size(0)
{
}

template<typename T>
CCircularBuffer<T>::~CCircularBuffer(void)
{
	Destroy();
}

template<typename T>
int CCircularBuffer<T>::Allocate( unsigned int size )
{
	Destroy();
	_buf = new T[size];
	if (!_buf)
	{
		return 0;
	}
	_size = size;

	Clear();

	return _size;
}

template<typename T>
void CCircularBuffer<T>::Clear()
{
	memset(_buf, 0, sizeof(T) * _size);
	_headPos = /*_tailPos =*/ 0;
	//_empty = true;
}

template<typename T>
void CCircularBuffer<T>::Destroy()
{
	if (!_buf)
	{
		delete []_buf;
	}
	_size = 0;
}

template<typename T>
void CCircularBuffer<T>::Pop()
{
	memset(_buf + _headPos, 0, sizeof(T));
	_headPos = GetPos(_headPos, 1);
}

template<typename T>
unsigned int CCircularBuffer<T>::BufSize() const
{
	return _size;
}


template<typename T>
T & CCircularBuffer<T>::operator[]( const unsigned int index )
{
	//计算位置
	unsigned int pos = GetPos(_headPos, index);

	return _buf[pos];

}

template<typename T>
unsigned int CCircularBuffer<T>::GetPos( unsigned int base, unsigned int offset )
{
	if (offset > _size)
	{
		throw "Index overflow.";
	}
	base += offset;
	while (base >= _size)
	{
		base -= _size;
	}
	return base;
}

#endif
