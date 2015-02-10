/*	A part of Enigma
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

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
