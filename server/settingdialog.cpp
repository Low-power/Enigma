#include "common.h"
#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QPushButton>
SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
    ui->ftpAddrEdit->setText(st_FtpAddress);
    ui->ftpPortEdit->setValue(st_FtpPort.toInt());
    ui->ftpUserNameEdit->setText(st_Username);
    ui->ftpPassWordEdit->setText(st_Password);
    ui->ftpFileNameEdit->setText(st_FtpFileName);
    ui->connAddrComboBox->addItems(hostIpList);
    ui->connAddrComboBox->setEditText(st_ConnAddrToFtp);
    ui->connPortEdit->setValue(st_ConnPortToFtp.toInt());
    getHostIpList();


    ui->hostPortEdit->setValue(HostPort);
    ui->maxConnectionsEdit->setValue(MaxConnections);
    //ui->foreColorCombo->setCurrentIndex(st_ForegroundColor.toInt());
    //ui->backColorCombo->setCurrentIndex(st_BackgroundColor.toInt());
	ui->foreColorCombo->setCurrentIndex(ForegroundColor);
	ui->backColorCombo->setCurrentIndex(BackgroundColor);

    //ui->fileBodyEdit->setText(QString("%当前监听Ip%:")+this->ui->hostPortEdit->text());
}


void SettingDialog::on_hostPortEdit_valueChanged(int value)
{
    //ui->fileBodyEdit->setText("%当前选定Ip%:"+this->ui->hostPortEdit->text());
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_connAddrComboBox_textChanged(const QString &arg1)
{
    ui->fileBodyEdit->setText(arg1 + ":" + ui->connPortEdit->text());
}

void SettingDialog::on_connPortEdit_valueChanged(const QString &arg1)
{
    ui->fileBodyEdit->setText(ui->connAddrComboBox->currentText() + ":" + arg1);
}
