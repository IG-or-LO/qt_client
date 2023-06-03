#include "mainwindow.h"
#include <QDialog>
#include <QVariantList>
#include <QTcpServer>

namespace Ui {
class registration;
}

class registration : public QDialog
{
    Q_OBJECT

public:
    explicit registration(QWidget *parent = nullptr);
    ~registration();


private slots:

    void on_ok_button_clicked();

    void on_close_button_clicked();

    void on_pushButton_Connect_clicked();
    
signals:
    void signal_for_mainwindow();

private:
    Ui::registration *ui;
    MainWindow *form_main;
    quint16 nextBlockSize;
    QByteArray Data;
    QTcpServer* server;

    void SendToServer(QVariantList list);
    void setInterfaceStyle();
    bool checkLogAndPass(QString login,QString password);
    QString mem_log="razor";
    QString mem_pass="12345";

public slots:
    void  slotReadyRead();
};


