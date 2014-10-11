#include "buildsvrdialog.h"
#include "ui_buildsvrdialog.h"
#include "common.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
BuildSvrDialog::BuildSvrDialog(QWidget *parent) :
QDialog(parent), ui(new Ui::BuildSvrDialog)
{
	ui->setupUi(this);
	this->ui->resoveModeCheck->setChecked(false);
	this->ui->serverFileNameEdit->setText(QDir::currentPath() + "/" "Server.exe");
	this->ui->UrlEdit->setText("HTTP://");
	this->ui->portSpinBox->setValue(HostPort);
}

BuildSvrDialog::~BuildSvrDialog()
{
	delete ui;
}


void BuildSvrDialog::on_pushButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this,
							tr("打开"),
							this->ui->serverFileNameEdit->text(),
							tr("应用程序(*.exe);;"));
	if(!fileName.isEmpty())
	{
		this->ui->serverFileNameEdit->setText(fileName);
	}
}

void BuildSvrDialog::on_buildButton_clicked()
{
#pragma pack(push,1)
	struct ConnInfo
	{
		unsigned char flag;
		unsigned short port;
		char param[1024];
	} __attribute__((packed)) connSetting={0b00000000,0,{'\0'}};
#pragma pack(pop)
	//0x6400
	QString serverDataFile(QApplication::applicationDirPath()+"/server.dat");
	bool failed=false;
	QFile file(ui->serverFileNameEdit->text());
	if(file.exists())file.remove();
	if(!QFile::copy(serverDataFile,ui->serverFileNameEdit->text()))
	{
		QMessageBox::critical(this,"错误","无法生成服务端安装文件.");
		return;
	}

	do{

		if(!file.open(QIODevice::ReadWrite))
		{
			QMessageBox::critical(this,"错误","数据源加载失败");
			failed=true;
			break;
		}
		if(ui->resoveModeCheck->checkState()==Qt::Checked)
		{
			QByteArray url = ui->UrlEdit->text().toUtf8();
			if(url.isEmpty()) {
				QMessageBox::critical(this, tr("Error"), tr("未填写 URL"));
				//return;
				failed = true;
				break;
			}
			connSetting.flag=1;
			connSetting.port=0;
			strcpy(connSetting.param, url.constData());
		}else
		{
			QByteArray address = ui->addressEdit->text().toUtf8();
			if(address.isEmpty()) {
				QMessageBox::critical(this, tr("Error"), tr("未填写本地地址"));
				//return;
				failed = true;
				break;
			}
			connSetting.flag=0;
			connSetting.port=ui->portSpinBox->value();
			strcpy(connSetting.param, address.constData());
		}
		if(!file.seek(0x6400))
		{
			file.close();
			QMessageBox::critical(this,"错误","数据源有错误");
			failed=true;
			break;
		}
		if(file.write((const char *)&connSetting, sizeof(ConnInfo)) < 0) {
			QMessageBox::critical(this, tr("Error"), tr("无法写入文件 '%1'").arg(file.fileName()));
			failed = true;
			break;
		}
		file.close();
		//file.write()
	}while(false);
	if(failed)
	{
		file.close();
		file.remove();
	}else
	{
		QMessageBox::information(this,"提示",QString("服务端已经生成."));
	}

}

void BuildSvrDialog::on_resoveModeCheck_clicked(bool checked)
{
	if(checked)
		this->ui->stackedWidget->setCurrentIndex(1);
	else
		this->ui->stackedWidget->setCurrentIndex(0);
}
