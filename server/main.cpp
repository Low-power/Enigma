/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "mainwindow.h"
#include "model.h"
#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QIODevice>
int main(int argc, char *argv[])
{
	//QApplication::setStyle("cleanlooks");
	//QApplication::setStyle("plastique");
	//QApplication::setStyle("windows");
	for(int i=1; i<argc; i++) if(strcmp(argv[i], "--style") == 0) {
		QApplication::setStyle(argv[i+1]);
		for(int j=i; j<argc-1; j++) argv[j] = argv[j+2];
		argc -= 2;
	}

	QTextCodec *codec = QTextCodec::codecForName("GB18030");
	QTextCodec::setCodecForLocale(codec);
	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
