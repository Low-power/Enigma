/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "querydialog.h"
#include "ui_querydialog.h"
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>

QueryDialog::QueryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);
}

void QueryDialog::setQueryButtonsText(QString button0Text,
                         QString button1Text,
                         QString button2Text,
                         QString button3Text)
{
    this->setWindowFlags(Qt::WindowMinimizeButtonHint);
    if(button0Text.isEmpty())
        this->ui->no0Button->setVisible(false);
    else
        this->ui->no0Button->setText(button0Text);

    if(button1Text.isEmpty())
        this->ui->no1Button->setVisible(false);
    else
        this->ui->no1Button->setText(button1Text);

    if(button2Text.isEmpty())
        this->ui->no2Button->setVisible(false);
    else
        this->ui->no2Button->setText(button2Text);

    if(button3Text.isEmpty())
        this->ui->no3Button->setVisible(false);
    else
        this->ui->no3Button->setText(button3Text);
}

void QueryDialog::on_no0Button_clicked()
{
    done(0);
}

void QueryDialog::on_no1Button_clicked()
{
    done(1);
}

void QueryDialog::on_no2Button_clicked()
{
    //this->setResult(2);
    done(2);
    //this->close();
}

void QueryDialog::on_no3Button_clicked()
{
    done(3);
}

void QueryDialog::setQueryText(QString text)
{
    this->ui->messageLabel->setText(text);
}

void QueryDialog::setQueryCaption(QString caption)
{
    this->setWindowTitle(caption);
}

QueryDialog::~QueryDialog()
{
    delete ui;
}

QPoint QueryDialog::center() {
	return QPoint((QApplication::desktop()->width() - width()) / 2, (QApplication::desktop()->height() - height()) / 2);
}
