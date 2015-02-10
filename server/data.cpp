/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "common.h"
#include <stdint.h>
#include <sys/stat.h>
#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QSettings>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QNetworkInterface>
#include <QtGui/QApplication>

#define CONFIG_FILE_NAME "server.cfg"

bool settingLoaded=false;
QString st_FtpAddress="";       //ftp
QString st_FtpPort;             //ftpport
QString st_Username;            //username
QString st_Password;            //password
QString st_FtpFileName;         //ftpfile
QString st_ConnAddrToFtp;
QString st_ConnPortToFtp;

//QString st_HostPort;            //hostport
uint16_t HostPort;
//QString st_MaxConnectionCount;  //maxconncount
unsigned int MaxConnections;
QString st_ForegroundColor;     //foreground		// DEPRECATED
QString st_BackgroundColor;     // background		// DEPRECATED
int ForegroundColor;
int BackgroundColor;
QStringList hostIpList;

#ifdef Q_OS_WIN
#define mkdir(N, M) _mkdir(N)
extern "C" int _mkdir(const char *);
#endif

QString config_dir() {
	QString appath = QApplication::applicationDirPath();
#ifndef Q_OS_WINCE
	if(QFile::exists(appath + "/" CONFIG_FILE_NAME)) {
#endif
		return appath;
#ifndef Q_OS_WINCE
	}
	QString inhome = QDir::homePath() + "/.enigma";
	if(!QFile::exists(inhome)) mkdir(inhome.toLocal8Bit().data(), 0755);
	return inhome;
#endif
}

void readSettings()
{

    //QFileInfo configFile(config_dir() + "/" CONFIG_FILE_NAME);
    //if(!configFile.exists()) return false;
    //if(!QFile::exists(config_dir() + "/" CONFIG_FILE_NAME)) return false;
    QSettings configIniRead(config_dir() + "/" CONFIG_FILE_NAME, QSettings::IniFormat);
    configIniRead.setIniCodec("UTF8");
    st_FtpAddress=configIniRead.value("FTP/FtpAddress").toString();
    st_FtpPort=configIniRead.value("FTP/FtpPort","21").toString();
    st_Username=configIniRead.value("FTP/FtpUserName").toString();
    st_Password=configIniRead.value("FTP/FtpPassword").toString();
    st_FtpFileName=configIniRead.value("FTP/FtpFileName","conn.inf").toString();
    st_ConnAddrToFtp=configIniRead.value("FTP/FtpData_Address").toString();
    st_ConnPortToFtp=configIniRead.value("FTP/FtpData_Port").toString();

    HostPort = configIniRead.value("Common/ListenPort", 2133).toInt();
    MaxConnections = configIniRead.value("Common/MaxConnections", 100).toInt();
    //st_ForegroundColor=configIniRead.value("Common/ForegroundColor").toString();
    //st_BackgroundColor=configIniRead.value("Common/BackgroundColor").toString();
    ForegroundColor = configIniRead.value("Common/ForegroundColor", 0).toInt();
    BackgroundColor = configIniRead.value("Common/BackgroundColor", 0).toInt();

    //delete configIniRead;
    //return true;
}

void saveSettings()
{
    QSettings configIniWrite(config_dir() + "/" CONFIG_FILE_NAME, QSettings::IniFormat, NULL);
    configIniWrite.setIniCodec("UTF8");
    configIniWrite.setValue("FTP/FtpAddress",st_FtpAddress);
    configIniWrite.setValue("FTP/FtpPort",st_FtpPort);
    configIniWrite.setValue("FTP/FtpUserName",st_Username);
    configIniWrite.setValue("FTP/FtpPassword",st_Password);
    configIniWrite.setValue("FTP/FtpFileName",st_FtpFileName);
    configIniWrite.setValue("FTP/FtpData_Address",st_ConnAddrToFtp);
    configIniWrite.setValue("FTP/FtpData_Port",st_ConnPortToFtp);
    configIniWrite.setValue("Common/ListenPort", HostPort);
    configIniWrite.setValue("Common/MaxConnections", MaxConnections);
    //configIniWrite.setValue("Common/ForegroundColor",st_ForegroundColor);
    //configIniWrite.setValue("Common/BackgroundColor",st_BackgroundColor);
    configIniWrite.setValue("Common/ForegroundColor", ForegroundColor);
    configIniWrite.setValue("Common/BackgroundColor", BackgroundColor);
    //return true;
}

void getHostIpList()
{
    hostIpList.clear();
    //QHostInfo host = QHostInfo::fromName(QHostInfo::localHostName());
    foreach(const QHostAddress &address, QNetworkInterface::allAddresses())
    {
        if(address.protocol() == QAbstractSocket::IPv4Protocol)
        {
            QString buffer=address.toString();
            hostIpList << buffer;
        }
    }
}
