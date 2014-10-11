#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <QtCore/QString>

#ifndef __GNUC__
#define __attribute__(E)
#endif

namespace Tr {
	enum {
		Request,
		Trunc,
		Append,
		Data,
		Terminate
	};
}

extern QString st_FtpAddress;           //ftp
extern QString st_FtpPort;              //port
extern QString st_Username;             //username
extern QString st_Password;             //password
extern QString st_FtpFileName;          //ftpfile
extern QString st_ConnAddrToFtp;
extern QString st_ConnPortToFtp;
//extern QString st_HostPort;             //hostport
extern uint16_t HostPort;
//extern QString st_MaxConnectionCount;   //maxconncount
extern unsigned int MaxConnections;
extern QString st_ForegroundColor;      //foreground		// DEPRECATED
extern QString st_BackgroundColor;      // background		// DEPRECATED
extern int ForegroundColor;
extern int BackgroundColor;

extern QStringList hostIpList;

extern void saveSettings();
extern void readSettings();
extern void getHostIpList();
#endif // COMMON_H
