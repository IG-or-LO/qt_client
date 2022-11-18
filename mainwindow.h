#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>

#include "/home/y/gcc_progrs/klient-server/server/myclient.h"
class MyClient;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
     void slotReadyRead();
     void slotRegistrationW();
     void slotSokConnected();
     void slotSokDisconnected();
     void slotSokDisplayError(QAbstractSocket::SocketError socketError);

     void on_pbConnect_clicked();
     void on_pbDisconnect_clicked();
     void on_cbToAll_clicked();
     void on_pbSend_clicked();


private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString _name;
//    QByteArray Data;
    quint16 nextBlockSize;

    void AddToLog(QString text, QColor color = Qt::black);


};
#endif // MAINWINDOW_H
