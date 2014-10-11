#ifndef CREATECLIENTAPPDIALOG_H
#define CREATECLIENTAPPDIALOG_H

#include <QDialog>

namespace Ui {
class CreateClientAppDialog;
}

class CreateClientAppDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit CreateClientAppDialog(QWidget *parent = 0);
    ~CreateClientAppDialog();
    
private slots:
    void on_buttonBox_accepted();

private:
    Ui::CreateClientAppDialog *ui;
};

#endif // CREATECLIENTAPPDIALOG_H
