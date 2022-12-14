#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _name = "";

    socket=new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::connected, this, &MainWindow::slotSokConnected);
    connect(socket,&QTcpSocket::disconnected,this,&MainWindow::slotSokDisconnected);
    connect(socket , SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(slotSokDisplayError(QAbstractSocket::SocketError)));

    nextBlockSize=0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    //���� ��������� ����� ���� ������ 2 ����� ��� ��� ������
    if (nextBlockSize == 0) {
        //���� ������ ������ 2 ���� ���� ���� ����� 2 �����
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        //��������� ������ (2 �����)
        in >> nextBlockSize;
        qDebug() << "nextBlockSize now " << nextBlockSize;
    }
    //���� ���� ���� ������� ���������
    if (socket->bytesAvailable() < nextBlockSize)
        return;
    else
        //����� ��������� ����� ����
        nextBlockSize = 0;
    //3 ���� - ������� �������
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case MyClient::comAutchSuccess:
        {
            ui->pbSend->setEnabled(true);
            AddToLog("Enter as "+_name,Qt::green);
        }
        break;
        case MyClient::comUsersOnline:
        {
            AddToLog("Received user list "+_name,Qt::green);
            ui->pbSend->setEnabled(true);
            QString users;
            in >> users;
            if (users == "")
                return;
            QStringList l =  users.split(",");
            ui->lwUsers->addItems(l);
        }
        break;
        case MyClient::comPublicServerMessage:
        {
            QString message;
            in >> message;
            AddToLog("[PublicServerMessage]: "+message, Qt::red);
        }
        break;
        case MyClient::comMessageToAll:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            AddToLog("["+user+"]: "+message);
        }
        break;
        case MyClient::comMessageToUsers:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            AddToLog("["+user+"](private): "+message, Qt::blue);
        }
        break;
        case MyClient::comPrivateServerMessage:
        {
            QString message;
            in >> message;
            AddToLog("[PrivateServerMessage]: "+message, Qt::red);
        }
        break;
        case MyClient::comUserJoin:
        {
            QString name;
            in >> name;
            ui->lwUsers->addItem(name);
            AddToLog(name+" joined", Qt::green);
        }
        break;
        case MyClient::comUserLeft:
        {
            QString name;
            in >> name;
            for (int i = 0; i < ui->lwUsers->count(); ++i)
                if (ui->lwUsers->item(i)->text() == name)
                {
                    ui->lwUsers->takeItem(i);
                    AddToLog(name+" left", Qt::green);
                    break;
                }
        }
        break;
        case MyClient::comErrNameInvalid:
        {
            QMessageBox::information(this, "Error", "This name is invalid.");
            socket->disconnectFromHost();
        }
        break;
        case MyClient::comErrNameUsed:
        {
            QMessageBox::information(this, "Error", "This name is already used.");
            socket->disconnectFromHost();
        }
        break;
    }
}

void MainWindow::slotRegistrationW()
{
    this->show();
}

void MainWindow::slotSokConnected()
{
    ui->pbConnect->setEnabled(false);
    ui->pbDisconnect->setEnabled(true);
    nextBlockSize = 0;
    AddToLog("Connected to"+socket->peerAddress().toString()+":"+QString::number(socket->peerPort()),Qt::green);

    //try autch
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)MyClient::comAutchReq;
    out << ui->leName->text();
    out << ui->lePass->text();//new line pass
    _name = ui->leName->text();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}

void MainWindow::slotSokDisconnected()
{
    ui->pbConnect->setEnabled(true);
    ui->pbDisconnect->setEnabled(false);
    ui->pbSend->setEnabled(false);
    ui->lwUsers->clear();
    AddToLog("Disconnected from"+socket->peerAddress().toString()+":"+QString::number(socket->peerPort()), Qt::green);
}

void MainWindow::slotSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(this, "Error", "The following error occurred: "+socket->errorString());
    }
}

void MainWindow::AddToLog(QString text, QColor color)
{
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+" "+text);
    ui->lwLog->item(0)->setTextColor(color);
}

void MainWindow::on_pbConnect_clicked()
{
    socket->connectToHost(ui->leHost->text(), ui->sbPort->value());
}

void MainWindow::on_pbDisconnect_clicked()
{
    socket->disconnectFromHost();
}

void MainWindow::on_cbToAll_clicked()
{
    if (ui->cbToAll->isChecked())
        ui->pbSend->setText("Send To All");
    else
        ui->pbSend->setText("Send To Selected");
}

void MainWindow::on_pbSend_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    if (ui->cbToAll->isChecked())
        out << (quint8)MyClient::comMessageToAll;
    else
    {
        out << (quint8)MyClient::comMessageToUsers;
        QString s;
        foreach (QListWidgetItem *i, ui->lwUsers->selectedItems())
            s += i->text()+",";
        s.remove(s.length()-1, 1);
        out << s;
    }

    out << ui->pteMessage->document()->toPlainText();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    ui->pteMessage->clear();
}
