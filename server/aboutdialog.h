/*	A part of Enigma
	Copyright 2014 Ixil

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class AboutDialog;
}

class AboutDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AboutDialog(QWidget *parent = 0);

    ~AboutDialog();
    
private slots:
    void on_closeButton_clicked();

private:
    Ui::AboutDialog *ui;
};

#endif // ABOUTDIALOG_H
