#ifndef _WEBLIBUPLOADER_LOG_H_
#define _WEBLIBUPLOADER_LOG_H_

#include <QFile>
#include <QTextStream>
#include <QDateTime>

class LogClass
{
public:
	~LogClass()
	{
		m_logfile.close();
		delete m_stream;
	}

	static void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
		switch (type)
		{
		case QtCriticalMsg:
			{
				LogClass::getInstance().logInfo("Error",
					QString("File:%1\r\nFunction:%2\tLine:%3").arg(context.file).arg(context.function).arg(context.line),
					msg);
				break;
			}
		case QtFatalMsg:
			{
				LogClass::getInstance().logInfo("Failure",
					QString("File:%1\r\nFunction:%2\tLine:%3").arg(context.file).arg(context.function).arg(context.line),
					msg);
				abort();
			}
		case QtDebugMsg:
		case QtWarningMsg:
			break;
		}
	}
private:
	static LogClass &getInstance()
	{
		static LogClass g_log("weblibuploader.log");
		return g_log;
	}

	LogClass(QString filename) : m_logfile(filename)
	{
		if(!m_logfile.open(QIODevice::WriteOnly | QIODevice::Append))
			return;

		m_stream = new QTextStream(&m_logfile);
	}

	LogClass(LogClass const &param);
	LogClass &operator = (LogClass const &param);

	void logInfo(const QString &type, const QString &posinfo, const QString &info)
	{
		(*m_stream) << type << "#  " << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << "\r\n"
			<< posinfo << "\r\n" << info << "\r\n===========================\r\n\r\n";
	}

private:
	QTextStream *m_stream;
	QFile m_logfile;
};

#endif