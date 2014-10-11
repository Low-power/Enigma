#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H

//#include <QtCore/QThread>
#include <QtCore/QFile>
#include <QtCore/QString>
#include <stdint.h>

class Server;

class Download : public QFile {
	Q_OBJECT
public:
	struct DownloadFeedback {
		unsigned char flag;
		int64_t size;
		unsigned char data[0];
	} __attribute__((packed));

	explicit Download(Server *);
/*
	enum DownloadFlag {
		Request,
		Data,
		Terminate
	};*/
	bool requestDownload(QString, const QString &, const QString &);
	bool requestTerminate();
	//bool saveData(const unsigned char *, size_t);
	qint64 fileSize;

private:
	Server *server;
	QString client_name;

};

#endif // _DOWNLOAD_H
