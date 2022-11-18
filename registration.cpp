#include "registration.h"
#include "ui_registration.h"
#include <QMessageBox>


registration::registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::registration)
{
    ui->setupUi(this);
    setInterfaceStyle();
    form_main= new MainWindow;
    connect(form_main->socket,&QTcpSocket::readyRead,this,&registration::slotReadyRead);
    connect(this, &registration::signal_for_mainwindow,form_main,&MainWindow::slotRegistrationW);
    //signal to connect
    connect(form_main->socket,&QTcpSocket::connected,this,&registration::on_ok_button_clicked);
}

registration::~registration()
{
    delete ui;
}

void registration::setInterfaceStyle()
{
}




void registration::SendToServer(QVariantList list)
{
     qDebug()<<"try to send pass log";
    Data.clear();
    QDataStream out(&Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_1);
    out<<quint16(0)<<QTime::currentTime()<<list;
    out.device()->seek(0);
    out<<quint16(Data.size()-sizeof (quint16));
    form_main->socket->write(Data);
    ui->login_edit->clear();
    ui->paassword_edit->clear();
}

void registration::slotReadyRead()
{
    QDataStream in(form_main->socket);
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
            if(form_main->socket->bytesAvailable()<2)
            {
                break;
            }
            in >> nextBlockSize;
          }
          if(form_main->socket->bytesAvailable()<nextBlockSize)
          {
              break;
          }
          QStringList str;
          in >> str;
          nextBlockSize=0;
        }
    }
    else {
        qDebug()<<"error riding data in registration";
    }
}


void registration::on_ok_button_clicked()
{
    QString login=ui->login_edit->text();
    QString password=ui->paassword_edit->text();
    QVariantList list;
    list.push_back(login);
    list.push_back(password);

    //после подключения следует отправить запрос на авторизацию
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
    //резервируем 2 байта для размера блока. Класс MyClient используется в реализации сервера, но тут используем статические члены этого класса - константы команд
    //третий байт - команда





}

void registration::on_close_button_clicked()
{
    this->close();
}

void registration::on_pushButton_Connect_clicked()
{
    form_main->socket->connectToHost("127.0.0.1",2323);
}
