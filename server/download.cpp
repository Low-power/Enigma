#include "download.h"
#include "server.h"
#include "model.h"
#include <assert.h>
#include <QtCore/QFileInfo>
#include <QtCore/QDebug>

Download::Download(Server *server) {
	this->server = server;
}

bool Download::requestDownload(QString client_name, const QString &path_name, const QString &save_file_name) {
	qDebug("function: requestDownload(QString &, QString &)");
	qDebug("&this->client_name = %p", &this->client_name);
	if(isOpen()) return false;
	this->client_name = client_name;
	//QFileInfo local_file(path_name);
	//setFileName(local_file.fileName());
	setFileName(save_file_name);
	QByteArray pathname = path_name.toUtf8();
	QByteArray *data = DataAnalysis::modulateData(DataAnalysis::ToClient_DownloadFile, &pathname);
	assert(data);
	bool r = server->sendToClient(data, client_name);
	delete data;
	return r && open(QIODevice::WriteOnly | QIODevice::Truncate);
}

bool Download::requestTerminate() {
	QByteArray tmp("TERMINATE");
	QByteArray *data = DataAnalysis::modulateData(DataAnalysis::ToClient_DownloadFile, &tmp);
	assert(data);
	bool r = server->sendToClient(data, client_name);
	delete data;
	close();
	return r;
}

//bool Download::saveData(
