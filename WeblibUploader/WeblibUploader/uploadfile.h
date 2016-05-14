#ifndef _UPLOADFILE_H_
#define _UPLOADFILE_H_

#include <QIODevice>
#include <QFile>
#include <QDateTime>
#include <QFileInfo>

class UploadFile
{
public:
	UploadFile(const QString & name);
	~UploadFile();

	bool openFile();
	bool isOpen();
	QString getFileName();
	bool isFinished();
	qint64 filesize();
	qint64 sizepart();
	qint64 sizefull();
	void setHttpHeader(const QByteArray &head);
	void setHead(const QByteArray &head);
	void setTail(const QByteArray &tail);
	qint64 readData(char *data, qint64 maxlen);
	void closeFile();

private:
	QFile *m_file;
	QByteArray *m_httphead;
	QByteArray *m_head;
	QByteArray *m_tail;
	qint64 m_position;
	QFileInfo m_info;
};

#endif