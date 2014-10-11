#ifndef BUILDSVRDIALOG_H
#define BUILDSVRDIALOG_H

#include <QDialog>

namespace Ui {
class BuildSvrDialog;
}

class BuildSvrDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit BuildSvrDialog(QWidget *parent = 0);
    ~BuildSvrDialog();
    
private slots:
    void on_pushButton_clicked();

    void on_buildButton_clicked();

    void on_resoveModeCheck_clicked(bool checked);

private:
    Ui::BuildSvrDialog *ui;
};

#endif // BUILDSVRDIALOG_H
