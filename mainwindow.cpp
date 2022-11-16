#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket=new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,socket,&QTcpSocket::deleteLater);
    nextBlockSize=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_connect_clicked()
{
    socket->connectToHost("127.0.0.1",2323);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_1);
    if(in.status()==QDataStream::Ok)
    {
//        QString str;
//        in>>str;
//        ui->textBrowser->append(str);
        for (;;)
        {
          if(nextBlockSize==0)
          {
            if(socket->bytesAvailable()<2)
            {
                break;
            }
            in >> nextBlockSize;
          }
          if(socket->bytesAvailable()<nextBlockSize)
          {
              break;
          }
          QTime time;
          QString str;
          in >>time>> str;
          nextBlockSize=0;
          ui->textBrowser->append(time.toString() + " " + str);
        }
    }
    else
    {
        ui->statusbar->showMessage("error reading data");
    }
}

void MainWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out<<quint16(0)<<QTime::currentTime()<<str;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof (quint16));
    socket->write(Data);
    ui->lineEdit->clear();
}

void MainWindow::on_pushButton_send_clicked()
{
    SendToServer(ui->lineEdit->text());
}

void MainWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}
