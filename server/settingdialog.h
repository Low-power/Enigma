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
