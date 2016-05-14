#include "uploadfile.h"
#include "weblibsetting.h"
#include <QDebug>

UploadFile::UploadFile(const QString &name) : m_info(name)
{
	m_file = new QFile(name);
	m_position = 0;
	m_httphead = NULL;
	m_head = NULL;
	m_tail = NULL;
}

UploadFile::~UploadFile()
{
	if(m_tail != NULL)
		delete m_tail;
	if(m_head != NULL)
		delete m_head;
	if(m_file != NULL)
	{
		m_file->close();
		delete m_file;
	}
	if(m_httphead != NULL)
		delete m_httphead;
}

QString UploadFile::getFileName()
{
	return m_info.fileName();
}

bool UploadFile::isFinished()
{
	return m_position >= sizefull();
}

void UploadFile::setHttpHeader(const QByteArray &head)
{
	m_httphead = new QByteArray(head);
}

void UploadFile::setHead(const QByteArray &head)
{
	m_head = new QByteArray(head);
}

void UploadFile::setTail(const QByteArray &tail)
{
	m_tail = new QByteArray(tail);
}

void UploadFile::closeFile()
{
	m_file->close();
}

bool UploadFile::isOpen()
{
	return m_file->isOpen();
}

qint64 UploadFile::readData(char *data, qint64 maxlen)
{
	if (!m_file->isOpen())
	{
		return -1;
	}

	char *pointer = data;
	qint64 atAll = 0;

	if((m_position < m_httphead->size()) && (maxlen > 0))
	{
		qint64 count = qMin(maxlen, (qint64)m_httphead->size());
		memcpy(pointer, m_httphead->data(), count);
		pointer += count;
		m_position += count;
		atAll += count;
		maxlen -= count;
	}

	if ((m_position < (m_httphead->size() + m_head->size())) && (maxlen > 0))
	{
		qint64 count = qMin(maxlen, (qint64)m_head->size());
		memcpy(pointer, m_head->data(), count);
		pointer += count;
		m_position += count;
		atAll += count;
		maxlen -= count;
	}

	if ((m_position < (m_httphead->size() + m_head->size() + m_file->size())) && (maxlen > 0))
	{
		qint64 count = qMin(maxlen, m_file->bytesAvailable());
		int s = m_file->read(pointer, count);
		pointer += s;
		maxlen -= s;
		m_position += s;
		atAll += s;
	}

	if (m_position >= (m_httphead->size() + m_head->size() + m_file->size()) && (maxlen > 0) && (m_position < sizefull()))
	{
		qint64 count = qMin(maxlen, (qint64)m_tail->size());
		memcpy(pointer, m_tail->data(), count);
		m_position += count;
		atAll += count;
	}

	return atAll;
}

qint64 UploadFile::filesize()
{
	return m_file->size();
}

bool UploadFile::openFile()
{
	if (m_file->open(QIODevice::ReadOnly))
		return true;
	return false;
}

qint64 UploadFile::sizepart()
{
	return m_file->size() + m_head->size() + m_tail->size();
}

qint64 UploadFile::sizefull()
{
	return m_httphead->size() + m_file->size() + m_head->size() + m_tail->size();
}