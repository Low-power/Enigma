/*	A part of Enigma
	Copyright 2014 Ixil

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}
class QMainWindow;
class SettingDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit SettingDialog(QWidget *parent = 0);

    Ui::SettingDialog *ui;

    ~SettingDialog();

private:
    QMainWindow *parent;

private slots:
    void on_hostPortEdit_valueChanged(int value);
    void on_connAddrComboBox_textChanged(const QString &arg1);
    void on_connPortEdit_valueChanged(const QString &arg1);
};

#endif // SETTINGDIALOG_H
