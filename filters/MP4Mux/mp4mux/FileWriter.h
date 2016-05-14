#pragma once

#include <queue>


class BufferBlock;
typedef std::queue<BufferBlock *> BufferQueue;


class FileWriter
{
public:
	FileWriter(void);
	~FileWriter(void);

	void SetFile(HANDLE hFile);

	void Start();
	void Stop();
	bool IsStop()
	{
		return m_bStop;
	}

	void Add(LONGLONG pos, const BYTE* pBuffer, long cBytes);
	void Flush();

private:

	// methods of writer thread
	static unsigned __stdcall _WriterProc(void *);
	void _WriterLoop();

	// private method
	void _WriteBufToFile(BufferBlock * pBlock);
	void _WriteToFile(LONGLONG pos, const BYTE* pBuffer, long cBytes);
	void _GetBufferBlock();


private:

	HANDLE     m_hFile;

	HANDLE     m_hProc;
	bool       m_bStop;

	CCritSec      m_csQueue;
	BufferQueue   m_queueFree;
	BufferQueue   m_queueFull;
	BufferQueue   m_queuePending;
	BufferBlock * m_pCurrent;

	static const long BUF_SIZE = 1000 * 4 * 1024; 

	LONGLONG m_llWritenBytes;
	long     m_lBufCount;

};
