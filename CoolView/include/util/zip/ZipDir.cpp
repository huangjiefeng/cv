
//#include <tchar.h>
//#include "windows.h"
#include "ZipDir.h"
#include <QDir>
#include <QtConcurrent/QtConcurrentRun>
#include <QMetaType>

#include "zip.h"

#define MAX_PATH 260;

#ifdef UNICODE
#	define _tqPrintable(string) (const TCHAR *)(string).constData()
#else
#	define _tqPrintable qPrintable
#endif


CQZip::CQZip( CZipDir * pCaller, QString szFile, CZipDir::SrcList list )
{
	m_pCaller = pCaller;
	m_szFile = szFile;
	m_list = list;
}

void CQZip::DoZip()
{
	HZIP hz = CreateZip(_tqPrintable(m_szFile), NULL);
	if (NULL == hz)
	{
		return;
	}

	int nCount = 0;
	for (CZipDir::SrcList::iterator it = m_list.begin(); it != m_list.end(); ++it)
	{
		QMetaObject::invokeMethod(m_pCaller, "ZipStateChanged", Qt::QueuedConnection
			, Q_ARG(CZipDir::ZipState, CZipDir::ZIP_STATE_PROGRESS)
			, Q_ARG(QString, QString("%1%(%2)").arg(QString::number(100 * nCount / m_list.size()), it->second)));

		ZRESULT zr = ZipAdd(hz, _tqPrintable(it->first), _tqPrintable(it->second));
		if (ZR_OK != zr)
		{
			char szMsg[255] = {0};
			FormatZipMessage(zr, szMsg, sizeof(szMsg));
			QString qMsg = szMsg;

			QMetaObject::invokeMethod(m_pCaller, "ZipStateChanged", Qt::QueuedConnection
				, Q_ARG(CZipDir::ZipState, CZipDir::ZIP_STATE_ERROR)
				, Q_ARG(QString, QString("[error] %1: %2").arg(it->second, qMsg)));
		}

		++nCount;
	}

	CloseZip(hz);

	QThread::currentThread()->quit();

}


CZipDir::CZipDir()
{
	qRegisterMetaType<CZipDir::ZipState>("CZipDir::ZipState");
	m_pthrZip = NULL;
	m_pobjZip = NULL;
}

CZipDir::~CZipDir()
{
	Close();
}

int CZipDir::Create( QString szFile )
{
	if (m_pthrZip && m_pthrZip->isRunning()) return -1;
	m_szFile = szFile;
	return 0;
}

int CZipDir::Close()
{
	if (m_szFile.isEmpty())
	{
		return -1;
	}

	if (m_pthrZip)
	{
		m_pthrZip->terminate();
		while (m_pthrZip && m_pthrZip->isRunning());
		if (m_pthrZip) m_pthrZip->deleteLater(); //确保删除
		m_pthrZip = NULL;
	}
	m_pthrZip = new QThread;

	CQZip * pZip = new CQZip(this, m_szFile, m_vtSrcList);
	pZip->moveToThread(m_pthrZip);
	connect(m_pthrZip, SIGNAL(started()), pZip, SLOT(DoZip()));
	connect(m_pthrZip, SIGNAL(finished()), this, SLOT(OnDone()));
	connect(m_pthrZip, SIGNAL(terminated()), this, SLOT(OnDone()));
	m_pthrZip->start();

	if (m_pobjZip) m_pobjZip->deleteLater(); //确保删除
	m_pobjZip = (QObject *)pZip;

	m_vtSrcList.clear();
	m_szFile = "";

	return 0;
}

int CZipDir::Add( QString szDst, QString szSrc )
{
	QDir dir(szSrc);
	if (m_szFile.isEmpty() || !dir.exists())
	{
		return -1;
	}

	dir.setFilter(QDir::Dirs|QDir::Files);
	dir.setSorting(QDir::DirsFirst);
	QFileInfoList list = dir.entryInfoList();

	for (int i = 0; i < list.size(); ++i)
	{
		QFileInfo fileInfo = list.at(i);

		// Dir
		if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")
		{
			continue;
		}

		if(fileInfo.isDir())
		{
			QString szDirDst = szDst + "\\" + fileInfo.fileName();
			QString szDirSrc = szSrc + "\\" + fileInfo.fileName();
			Add(szDirDst, szDirSrc); // ignore error
		}
		// File
		else
		{
			QString szFileDst = szDst + "\\" + fileInfo.fileName();
			QString szFileSrc = szSrc + "\\" + fileInfo.fileName();

			QPair<QString, QString> p = qMakePair(szFileDst, szFileSrc);
			m_vtSrcList.push_back(p);
		}
	}

	return 0;
}


void CZipDir::OnDone()
{
	emit ZipStateChanged(ZIP_STATE_DONE, "");
	disconnect(m_pthrZip, SIGNAL(started()), m_pobjZip, SLOT(DoZip()));
	disconnect(m_pthrZip, SIGNAL(finished()), this, SLOT(OnDone()));
	disconnect(m_pthrZip, SIGNAL(terminated()), this, SLOT(OnDone()));
	m_pthrZip->deleteLater();
	m_pthrZip = NULL;
	m_pobjZip->deleteLater();
	m_pobjZip = NULL;
}
