#ifndef ZIPDIR_H
#define ZIPDIR_H


#include <QString>
#include <QVector>
#include <QThread>
#include <QPair>



class CZipDir : public QObject {
	Q_OBJECT;

public:

	enum ZipState {
		ZIP_STATE_DONE,
		ZIP_STATE_ERROR,
		ZIP_STATE_PROGRESS,
	};

	typedef QVector<QPair<QString, QString>> SrcList;

	CZipDir();
	~CZipDir();

	int Create(QString szFile);
	int Close();

	int Add(QString szDst, QString szSrc);

signals:

	Q_INVOKABLE void ZipStateChanged(CZipDir::ZipState state, QString msg);

protected:

	int AddInternal(QString szDst, QString szSrc);

private slots:

	void OnDone();

private:

	QString m_szFile;
	SrcList m_vtSrcList;

	QThread * m_pthrZip;
	QObject * m_pobjZip;

};


class CQZip : public QObject
{
	Q_OBJECT;

public:
	CQZip(CZipDir * pCaller, QString szFile, CZipDir::SrcList list);

public slots:
	void DoZip();

private:
	CZipDir * m_pCaller;
	QString m_szFile;
	CZipDir::SrcList m_list;

};


#endif
