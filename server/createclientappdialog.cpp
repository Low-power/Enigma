#include "createclientappdialog.h"
#include "ui_createclientappdialog.h"

CreateClientAppDialog::CreateClientAppDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateClientAppDialog)
{
    ui->setupUi(this);
}

CreateClientAppDialog::~CreateClientAppDialog()
{
    delete ui;
}

void CreateClientAppDialog::on_buttonBox_accepted()
{

}
