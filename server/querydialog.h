#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>

namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit QueryDialog(QWidget *parent = 0);
    void setQueryText(QString text);
    void setQueryCaption(QString caption);
    void setQueryButtonsText(QString button0Text,
                             QString button1Text,
                             QString button2Text,
                             QString button3Text);
    ~QueryDialog();
    QPoint center();
    
private slots:
    void on_no0Button_clicked();

    void on_no1Button_clicked();

    void on_no2Button_clicked();

    void on_no3Button_clicked();

private:
    Ui::QueryDialog *ui;
    QString queryText;
    QString queryCaption;
};

#endif // QUERYDIALOG_H
