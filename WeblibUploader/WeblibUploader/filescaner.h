#ifndef _FILESCANSER_H_
#define _FILESCANSER_H_

#include <QTimer>
#include <QNetworkAccessManager>
#include <QList>
#include <QMap>
#include <QStack>

struct WebLibDirStruct
{
	QString dirPath;
	int weblibid;
	int parentid;
	int groupid;

	WebLibDirStruct(const QString &path, int pid, int gid, int id)
	{
		dirPath = path;
		weblibid = id;
		parentid = pid;
		groupid = gid;
	}
};

class FileScaner : public QObject
{
	Q_OBJECT
public:
	FileScaner();
	~FileScaner();

	void start();
	void stop();
signals:
	void addUploadFile(const QString &filename, int groupid, int parentid);
	void error(const QString &info);

private:
	QList<WebLibDirStruct *> *getRootDirList(int parentid);
	QMap<QString, QList<int> *> *getLocalDirInfo(QList<WebLibDirStruct *> *rootlist);
	QList<int> *getWeblibIdList(int parentid);
	qint64 getWeblibFreeSpace(int groupid);
	qint64 getWeblibSizeLimit(int groupid);
	int checkAndCreateWeblibDirTree(int groupid, int parentid, const QString &dirpath, int rootid, int rootlen, QMap<QString, int> *idlist);
	int createWeblibDir(int groupid, int parentid, const QString &dirpath);
	int createUserDir(int groupid, const QString &dirname);
	bool checkUserDir(int &dirid);

private slots:
	void slotFileScan();

private:
	bool m_scaning;
	QTimer *m_timer;
	QNetworkAccessManager *m_manager;
};

#endif