#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QAction>
#include<QDirIterator>
#include <QFileDialog>
#include <QBuffer>

#include <QPainter>
#include <stylehelper.h>
#include <messegeswidget.h>
#include <mylistwidgetitem.h>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //определние классовых перменных
    _scrollAreaBg.load(":/new/img/fon_information.jpg");
    image_personal=QPixmap();
    nextBlockSize=0;
    _currentUserLogin = "";
    _already_connected=false;
    ui->pbPersonInfo->setEnabled(false);
    socket=new QTcpSocket(this);



    //connections
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this, SLOT(updateSizes(int)));
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::connected, this, &MainWindow::slotSokConnected);
    connect(socket,&QTcpSocket::disconnected,this,&MainWindow::slotSokDisconnected);
    connect(socket , SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(slotSokDisplayError(QAbstractSocket::SocketError)));
   // connect(ui->listWidget,&QListWidget::itemClicked,this,&MainWindow::slotListWidgetClicked);
    //методы обработки интерфейса
    SetTabWidgetStyle();
    SetMenuToolButtonActions();
    SetInterfaceStyle();
    SetEmodjiButtonStyle();

    //tests

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OpenDataBase(QString userLogin)
{
    //база клиента
    dataBase=new klientDataBase(userLogin);
    if(!dataBase->connectToDataBase());
    //если бд не было,то сохраняем новый аккаунт
    dataBase->insertIntoAccTable(QVariantList() <<ui->le_login_enter->text()
                                               <<ui->le_pass_enter->text()
                                               <<0//auto_login=false
                                             //  <<ui->le_reg_Mail->text()
                                               <<getShablonImage());
}


void MainWindow::SetInterfaceStyle()
{
    //chat area
    messLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    messLayout->addStretch(-1);
    ui->mess_frame->setLayout(messLayout);
    ui->mess_frame->setStyleSheet("padding:5px 5px 0px 5px;");

    ui->frame_messEdit->hide();
    ui->frame_userInfo->hide();

    //buttonsStyle
    ui->pbSend->setEnabled(true);
    ui->pbAdd->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbMenu->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbSave->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbSend->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbSearch->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbPersonInfo->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_personal_info->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_user_info->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_about->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_reg_Reg->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());

    ui->pb_saveChanges->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_reg_enter->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_enter_enter->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    //lablesStyle
    ui->label_enter_or_reg->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_login->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_pass->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_settings->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info_name->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info_pass->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info_pass2->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info_about->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_per_info_surname->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_user_info->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_user_info_name->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_user_info_about->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_user_info_surname->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_about_program->setStyleSheet(stylehelper::getLabelsStyle());
    ui->la_reg_login->setStyleSheet(stylehelper::getLabelsStyle());
    ui->la_reg_Mail->setStyleSheet(stylehelper::getLabelsStyle());
    ui->la_reg_Pass->setStyleSheet(stylehelper::getLabelsStyle());
    ui->la_reg_Phone->setStyleSheet(stylehelper::getLabelsStyle());
    ui->la_reg_Title->setStyleSheet(stylehelper::getLabelsStyle());

    ui->label_about->setStyleSheet(stylehelper::getLabelsStyle());

}

void MainWindow::SetTabWidgetStyle()
{
    //  ui->tabWidget->tabBar()->hide();
      ui->tab->setObjectName("tabChat");
      ui->tab_enter->setObjectName("enter");
      ui->tab_reg->setObjectName("reg");
      ui->tab_info->setObjectName("info");
      ui->tab_personalInfo->setObjectName("perinfo");
      ui->tab_about->setObjectName("about");

      ui->tabWidget->setStyleSheet(stylehelper::getTabWidgetStyle());
      ui->tab_enter->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_reg->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_personalInfo->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_info->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_about->setStyleSheet(stylehelper::getTabsStyle());

      ui->tabWidget->setCurrentIndex(1);

      ui->le_login_enter->setFocus();
}

void MainWindow::SetMenuToolButtonActions()
{
    QAction *btn_Profileinfo=new QAction(this);
    QAction *btn_About=new QAction(this);
    QFont font("Times",15,QFont::Bold);
    btn_Profileinfo->setFont(font);
    btn_About->setFont(font);

    QIcon icon_info(":/new/img/person.svg");
    QIcon icon_about(":/new/img/about.svg");
    btn_Profileinfo->setIcon(icon_info);
    btn_About->setIcon(icon_about);

    btn_Profileinfo->setText("Profile");
    btn_About->setText("About Programm");

    connect(btn_Profileinfo,&QAction::triggered,this,&MainWindow::toolButtonProfileInfo);
    connect(btn_About,&QAction::triggered,this,&MainWindow::toolButtonAbout);

    ui->pbMenu->addAction(btn_Profileinfo);
    ui->pbMenu->addAction(btn_About);
    ui->pbMenu->setArrowType(Qt::NoArrow);
}
void MainWindow::SetInterfaceStyleAfterConnect()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->textEdit->setFocus();
    ui->textEdit->setPlaceholderText("Написать сообщение");
}

void MainWindow::GetListOfChats()
{
    qDebug()<<"in getlistofchats";
   QVariantList loginMessDateImageUnRead=dataBase->getChatsList(_currentUserLogin);
   for (int i = 0; i < loginMessDateImageUnRead.size(); i=i+5) {
       addUserGui(getPixmapFromQByteArray(loginMessDateImageUnRead[i+3].toByteArray()),loginMessDateImageUnRead[i].toString(),
               loginMessDateImageUnRead[i+1].toString(),loginMessDateImageUnRead[i+2].toDateTime(),loginMessDateImageUnRead[i+4].toInt());
   }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(this->width()<600)
    {
        ui->right_frame->hide();
    }

    else
    {
        ui->right_frame->show();
    }


    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter,this);
    QWidget::paintEvent(event);
}

void MainWindow::on_pbPersonInfo_clicked()
{
   ui->tabWidget->setCurrentIndex(4);
   ui->label_user_info->setText(ui->laWithUserChat->text());
   SendCommandToServer(comGetPersonalInfo);
}

void MainWindow::SetEmodjiButtonStyle()
{
    ui->pbEmodgi->setCursor(Qt::PointingHandCursor);
    connect(ui->pbEmodgi,&QToolButton::clicked,this,&MainWindow::toolButtonEmodgiClicked);
    emodjiListWidget=new QListWidget;
    emodjiListWidget->setWindowFlag(Qt::ToolTip);
    emodjiListWidget->setLayoutMode(QListView::Batched);
    emodjiListWidget->setViewMode(QListView::IconMode);
    emodjiListWidget->setGridSize(QSize(24,24));
    emodjiListWidget->setWrapping(true);

    //eventfilter needs

    QDirIterator it(":/new/img/emodji",QDirIterator::NoIteratorFlags);
    QSize iconSize(24,24);
    while(it.hasNext()){
        QString iconPath=it.next();

        QToolButton *emoji=new QToolButton;
        emoji->setIconSize(iconSize);
        emoji->setIcon(QIcon(iconPath));
        emoji->setFixedSize(iconSize);
        emoji->resize(iconSize);
        emoji->setStyleSheet("QToolButton{border: none; padding: 0px;}");
        connect(emoji,&QToolButton::clicked, [=](){

             ui->textEdit->insertHtml(QString("<img src=\"%1\" width=\"24\" height=\"24\">").arg(QString(iconPath)));//.replace("24x24","14x14")));
            });
        QListWidgetItem *item=new QListWidgetItem;
        item->setSizeHint(emoji->sizeHint());
        emodjiListWidget->addItem(item);
        emodjiListWidget->setItemWidget(item,emoji);
    }

}

void MainWindow::SendCommandToServer(quint8 command)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << (quint8)command;

    switch (command) {
    case comAutchReq:
    {
        out << ui->le_login_enter->text();
        out << ui->le_pass_enter->text();//new line pass
    }
        break;
    case comRegReq:
    {
        out<<ui->le_reg_Login->text();
        out<<ui->le_reg_Pass->text();
        out<<ui->le_reg_Mail->text();
        out<<ui->le_reg_Phone->text();
    }
        break;

    case comMessageToUsers:
    {
       QString user_to;
       user_to= ui->laWithUserChat->text();
       // foreach (QListWidgetItem *i, ui->lwUsers->selectedItems())
       //     s += i->text()+",";
       // s.remove(s.length()-1, 1);
        out << user_to;
        out << ui->textEdit->toPlainText();
    }
        break;

    case comGetNewMessages:
    {
        QString lastDateMess= dataBase->getLastDateChat(_currentUserLogin);//последняя дата
        qDebug()<<"lastDateMess: "<<lastDateMess;
        out<<lastDateMess;
    }
        break;

    case comLoadArchive:
    {
        out << _currentUserLogin;
        out << ui->laWithUserChat->text();
    }
        break;
    case comGetPersonalInfo:
    {
        if( ui->tabWidget->currentIndex()==4)
        {
            out << _currentUserLogin;
            out << ui->laWithUserChat->text();

        }
        else
        {
            out << _currentUserLogin;
            out << _currentUserLogin;
        }
    }
        break;
    case comSavePersonalInfo:
    {
        out << _currentUserLogin;
        out << ui->le_pi_name_3->toPlainText();
        out << ui->le_pi_surname_3->toPlainText();
        out << ui->le_pi_aboutme_3->toPlainText();
    }
        break;
    case comUpdatePassword:
    {
        out<<_currentUserLogin;
        out<<ui->le_pi_newpass_3->toPlainText();
    }
        break;
    case comSaveImagelUser:
    {
        out<<_currentUserLogin;
        out<<inByteArrayImage;
    }
        break;

    default:
        break;
    }

     out.device()->seek(0);
     out << (quint16)(block.size() - sizeof(quint16));
     socket->write(block);
}


void MainWindow::updateSizes(int index)
{
    for(int i=0;i<ui->tabWidget->count();i++)
        if(i!=index)
            ui->tabWidget->widget(i)->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    ui->tabWidget->widget(index)->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    ui->tabWidget->widget(index)->resize(ui->tabWidget->widget(index)->minimumSizeHint());
    ui->tabWidget->widget(index)->adjustSize();
    resize(minimumSizeHint());
    adjustSize();
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    if (nextBlockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
       //      if (socket->bytesAvailable() < (int)sizeof(unsigned int))
            return;
        in >> nextBlockSize;
        qDebug() << "nextBlockSize now " << nextBlockSize;
    }
    if (socket->bytesAvailable() < nextBlockSize)
        return;
    else
        nextBlockSize = 0;
    quint8 _command;
    in >> _command;
    qDebug() << "Received command " << _command;

    switch (_command)
    {
    case comRegSuccess:
    {
        showMessageBox("Регистрация пройдена","Теперь можно выполнить вход");
        ui->tabWidget->setCurrentIndex(1);
        _already_connected=true;
    }
        break;
   case comAuthSuccess:
    {
        _currentUserLogin=ui->le_login_enter->text();
        SetInterfaceStyleAfterConnect();
        OpenDataBase(_currentUserLogin);//подключение к бд пользователя
        SetPersonalInfo(dataBase->getPersonalInfo(_currentUserLogin));
        SendCommandToServer(comGetNewMessages);//запрос на получение новых сообщений
    }
        break;
    case comGetNewMessages:
    {
        qDebug()<<"TRY TO GetNewMessages";
        QStringList malesDataStatusFrom;
        in>>malesDataStatusFrom;
        for (int i = 0; i < malesDataStatusFrom.size(); i=i+4) {//добавляем новые сообщения в бд
            dataBase->insertIntoMessageTable(malesDataStatusFrom[i+3],
                    _currentUserLogin,malesDataStatusFrom[i],malesDataStatusFrom[i+1]);
        }
        GetListOfChats();//получение списка чатов и вывод их в гуи
    }
        break;
    case comGetPersonalInfo:
        {
            QString name;
            QString surname;
            QString about;
            QByteArray inByteArray;
            in>>name;
            in>>surname;
            in>>about;
            in>>inByteArray;
            SetPersonalInfo(QVariantList()<<name<<surname<<about<<inByteArray);
        }
        break;

    case comLoadArchive:
        {
           QStringList malesDataStatusFromTo;
           in>>malesDataStatusFromTo;

           for (int i = 0; i < malesDataStatusFromTo.size(); i=i+5) {
               addMessageGui(malesDataStatusFromTo[i],malesDataStatusFromTo[i+3],
                            QDateTime::fromString(malesDataStatusFromTo[i+1],"ddd MMM d hh:mm:ss yyyy"),
                            malesDataStatusFromTo[i+2]);
               dataBase->insertIntoMessageTable(malesDataStatusFromTo[i+3],malesDataStatusFromTo[i+4],
                       malesDataStatusFromTo[i],malesDataStatusFromTo[i+1]);
           }
        }
        break;
        //доработать получение статуса онлайн
    case comUsersOnline:
        {   

            QString users;
            in >> users;
            if (users == "")
                return;
            QStringList l =  users.split(",");
            QByteArray byteImageArray;

            for (int i = 0; i < l.size(); ++i) {
                in >> byteImageArray;
 //               addUserGui(getPixmapFromQByteArray(byteImageArray),l[i]);
//                ui->lwUsers->addItem(l[i]);
//                ui->lwUsers->item(i)->setIcon(getIconFromQByteArray(byteImageArray));
            }
        }
        break;
    case comPublicServerMessage:
        {
            QString message;
            in >> message;
        }
        break;
    case comMessageToAll:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
        }
        break;
//add from db date
    case comMessageToUsers:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            QDateTime date;
            in >> date;
            addMessageGui(message, user ,date,"must be done");
            dataBase->insertIntoMessageTable(user,_currentUserLogin,message,date.toString("ddd MMM d hh:mm:ss yyyy"));
        }
        break;
    case comPrivateServerMessage:
        {
            QString message;
            in >> message;
        }
        break;
        //доработать получение юзера
    case comUserJoin:
        {
            QString name;
            QByteArray imageByteArray;
            in >> name;
            in >> imageByteArray;
  //          addUserGui(getPixmapFromQByteArray(imageByteArray),name);
//            ui->lwUsers->addItem(name);
//            ui->lwUsers->item(ui->lwUsers->count()-1)->setIcon(getIconFromQByteArray(imageByteArray));
        }
        break;

    case comUserLeft:
        {
            QString name;
            in >> name;
//            for (int i = 0; i < listLayout->count(); ++i) {
//                 MyListWidgetItem *listWidget=qobject_cast<MyListWidgetItem*>(listLayout->itemAt(i)->widget());
//                 if(listWidget->getUserName()==name)
//                 {
//                     listWidget->hide();
//                     delete listWidget;
//                 }
//            }

        }
        break;
    case comErrNameUsed:
        {
            QMessageBox::information(this, "Error", "This name is already used.");
            socket->disconnectFromHost();
        }
        break;
    case comErrNameOrPassFalse:
        {
            QMessageBox::information(this, "Error", "Name or Pass False.");
            socket->disconnectFromHost();
        }
        break;
        }
}

void MainWindow::slotSokConnected()
{
    _already_connected=true;
}

 //удалять виджеты из layout!!!
void MainWindow::slotSokDisconnected()
{
    ui->tabWidget->setCurrentIndex(1);
    ui->frame_userInfo->hide();
    ui->frame_messEdit->hide();
    _already_connected=false;
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

void MainWindow::slotListWidgetClicked()
{
    //очищаем список сообщений в messLayout
    for (int i = 1; i < messLayout->count(); ++i) {
        QLayoutItem *messItem=messLayout->takeAt(i);
        delete messItem->widget();
        delete messItem;
    }

//    for (int i = 0; i < messLayout->count(); ++i) {
//        MessegesWidget *messWidget=qobject_cast<MessegesWidget*>(messLayout->itemAt(i)->widget());
//        messWidget->hide();
//        delete messWidget;
//    }
    //открываем фреймы чата
    ui->frame_userInfo->show();
    ui->frame_userInfo->show();

    //получаем имя пользователя и его картинку в листвиджете
    if(MyListWidgetItem *item= qobject_cast<MyListWidgetItem *>(ui->listWidget->itemWidget(ui->listWidget->currentItem())))
    {
        ui->laWithUserChat->setText(item->getUserName());
        image_user=item->getUserPixmap();
    }

    //получаем список сообщений из бд
    QStringList malesDataStatusFromTo;
    malesDataStatusFromTo=dataBase->loadArchiveMessages(_currentUserLogin,ui->laWithUserChat->text());

    //создаем виджеты для каждого сообщения
    for (int i = 0; i < malesDataStatusFromTo.size(); i=i+5)
    {
        addMessageGui(malesDataStatusFromTo[i],malesDataStatusFromTo[i+3],
                     QDateTime::fromString(malesDataStatusFromTo[i+1],"ddd MMM d hh:mm:ss yyyy"),
                     malesDataStatusFromTo[i+2]);
    }
}

//может быть добавить команду сендто сервер
void MainWindow::toolButtonInfo()
{
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::toolButtonAbout()
{
    ui->tabWidget->setCurrentIndex(5);
}

void MainWindow::toolButtonProfileInfo()
{
    ui->tabWidget->setCurrentIndex(3);
   // SendCommandToServer(comGetPersonalInfo);
}

void MainWindow::toolButtonEmodgiClicked()
{
    if (emodjiListWidget->isVisible()) {
        emodjiListWidget->hide();
         ui->textEdit->setFocus();
         QApplication::setActiveWindow(ui->textEdit);
     }
     else {
         emodjiListWidget->show();
         QPoint point = QPoint(0,-emodjiListWidget->height());
         QPoint pos = ui->textEdit->mapToGlobal(point);
         emodjiListWidget->move(pos);
         QApplication::setActiveWindow(emodjiListWidget);
         emodjiListWidget->setFocus();
     }
}


void MainWindow::on_pbSend_clicked()
{
    SendCommandToServer(comMessageToUsers);
            ///
    addMessageGui(ui->textEdit->toPlainText(),_currentUserLogin,QDateTime::currentDateTime(),"must be done");
    dataBase->insertIntoMessageTable(_currentUserLogin,ui->laWithUserChat->text(),ui->textEdit->toPlainText(),QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy"));
    ui->textEdit->clear();
}

void MainWindow::on_pb_enter_enter_clicked()
{
    if(!isNameValid(ui->le_login_enter->text()))
       showMessageBox("Ошибка логина","от 5 до 20");
    else if(!isPassValid(ui->le_pass_enter->text()))
       showMessageBox("Ошибка пароля","от 5 до 20");
    else
    {
        if(!_already_connected)
        {
            socket->connectToHost(ui->leHost->text(), ui->sbPort->value());
            SendCommandToServer(comAutchReq);
        }
        else
            SendCommandToServer(comAutchReq);
    }
}

void MainWindow::on_pb_reg_enter_clicked()
{
    ui->tabWidget->setCurrentIndex(2);
}

//убрал из коннекта
void MainWindow::loadArchiveMessages(QString user_name, QPixmap user_pic)
{
   qDebug()<<"Колличество обьектов в meessLayouyt"<<messLayout->count();
//очищаем список сообщений в messLayout
   //очищаем список сообщений в messLayout
   for (int i = messLayout->count()-1; i > 0; --i) {
       QLayoutItem *messItem=messLayout->takeAt(i);
       delete messItem->widget();
       delete messItem;
   }

//    for (int i = 0; i < messLayout->count(); ++i) {
//        MessegesWidget *messWidget=qobject_cast<MessegesWidget*>(messLayout->itemAt(i)->widget());
//        messWidget->hide();
//        delete messWidget;
//    }
    //открываем фреймы чата
    ui->frame_userInfo->show();
    ui->frame_messEdit->show();

    //получаем имя пользователя и его картинку в листвиджете
    if(MyListWidgetItem *item= qobject_cast<MyListWidgetItem *>(sender()))
    {
        ui->laWithUserChat->setText(item->getUserName());
        image_user=item->getUserPixmap();
    }

    //получаем список сообщений из бд
    QStringList malesDataStatusFrom;
    malesDataStatusFrom=dataBase->loadArchiveMessages(_currentUserLogin,ui->laWithUserChat->text());

    //создаем виджеты для каждого сообщения
    for (int i = 0; i < malesDataStatusFrom.size(); i=i+4)
    {
        addMessageGui(malesDataStatusFrom[i],malesDataStatusFrom[i+3],
                     QDateTime::fromString(malesDataStatusFrom[i+1],"ddd MMM d hh:mm:ss yyyy"),
                     malesDataStatusFrom[i+2]);
    }
     qDebug()<<"Колличество обьектов в meessLayouyt после добавления"<<messLayout->count();
//  ui->laWithUserChat->setText(user_name);
//  image_user=user_pic;
//  SendCommandToServer(comLoadArchive);
}

//добавить label к 4 индексу для отображения фото
void MainWindow::SetPersonalInfo(QVariantList nameSurnameAboutImage)
{
    if(nameSurnameAboutImage.empty())
        return;

    if(ui->tabWidget->currentIndex()==4)
    {
        ui->le_user_name->setText(nameSurnameAboutImage[0].toString());
        ui->le_user_surname->setText(nameSurnameAboutImage[1].toString());
        ui->le_user_aboutme->setText(nameSurnameAboutImage[2].toString());
        QPixmap image=QPixmap();
        image.loadFromData(nameSurnameAboutImage[3].toByteArray());
    }
    else
    {
        ui->le_pi_name_3->setText(nameSurnameAboutImage[0].toString());
        ui->le_pi_surname_3->setText(nameSurnameAboutImage[1].toString());
        ui->le_pi_aboutme_3->setText(nameSurnameAboutImage[2].toString());

        image_personal.loadFromData(nameSurnameAboutImage[3].toByteArray());
        ui->la_pi_image->setPixmap(image_personal.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}


void MainWindow::addMessageGui(QString mess, QString user, QDateTime dateTime, QString status, QString title /* ="" */)
{
    MessegesWidget *messWidget=new MessegesWidget(this);
    if(_currentUserLogin==user)
    {
        messWidget->setWidgetDisign(false);
        messWidget->addWidgetMessIconData(mess,
                                        image_personal.scaled(40,40, Qt::KeepAspectRatio,Qt::SmoothTransformation),dateTime);
        messWidget->setWidgetStatus(status=="read"?true:false);
    }
    else
    {
        messWidget->setWidgetDisign(true);
        messWidget->addWidgetMessIconData(mess,image_user,dateTime);

     }

    if(title!="")
        messWidget->addWidgetTitle(title);

    messLayout->addWidget(messWidget);

}

void MainWindow::addUserGui(QPixmap pixIcon, const QString &title, const QString &mess,
                            QDateTime date, const int &countUnRead)
{
    MyListWidgetItem *listItem=new MyListWidgetItem(pixIcon,title,this);
    listItem->setMessCountUnread(countUnRead);
    if(mess!="")
    listItem->setMessShort(mess,date);



    connect(listItem,SIGNAL(onWidgetClicked(QString,QPixmap)),this,SLOT(loadArchiveMessages(QString,QPixmap)));

    QListWidgetItem *item=new QListWidgetItem(ui->listWidget);
    item->setSizeHint(listItem->sizeHint());
    ui->listWidget->setItemWidget(item,listItem);

}


void MainWindow::on_pb_saveChanges_clicked()
{
    //if no value personal info
    if(ui->le_pi_name_3->toPlainText()=="" && ui->le_pi_aboutme_3->toPlainText()=="" && ui->le_pi_surname_3->toPlainText()=="" )
        QMessageBox::information(this, "Error", "Need to fill some fields...");
    else
        SendCommandToServer(comSavePersonalInfo);

    //if no pass
    if(ui->le_pi_newpass_3->toPlainText()!="" && ui->le_pi_newrepeatpass_3->toPlainText()=="")
         QMessageBox::information(this, "Error", "Need to fill newpassrepeat field...");
    else if(ui->le_pi_newpass_3->toPlainText()!=ui->le_pi_newrepeatpass_3->toPlainText())
        QMessageBox::information(this, "Error", "PASS AND REPEAT PASS NOT EQUAL...");
    else if(ui->le_pi_newpass_3->toPlainText()!="" && ui->le_pi_newrepeatpass_3->toPlainText()!="")
        SendCommandToServer(comUpdatePassword);

}

void MainWindow::on_pb_back_personal_info_clicked()
{
    ui->tabWidget->setCurrentIndex(0);

    //test
    ui->frame_messEdit->show();
    ui->frame_userInfo->show();
    //
}

void MainWindow::on_pb_back_user_info_clicked()
{
     ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::on_pb_back_about_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
}



void MainWindow::on_pb_reg_Reg_clicked()
{
    if(!isNameValid(ui->le_reg_Login->text()))
        showMessageBox("Ошибка логина","от 5 до 20");
    else if(!isPassValid(ui->le_reg_Pass->text()))
        showMessageBox("Ошибка пароля","от 5 до 20");
    else
    {
        if(!_already_connected)
        {
            socket->connectToHost(ui->leHost->text(),ui->sbPort->value());
            SendCommandToServer(comRegReq);
        }
        else
            SendCommandToServer(comRegReq);
    }
}

bool MainWindow::isNameValid(QString name)
{
    if (name.length() > 20 || name.length() < 5)
        return false;
    QRegExp r("[A-Za-z0-9_]+");
    if(r.exactMatch(name))
        return true;
    else
        return false;
}

bool MainWindow::isPassValid( QString pass)
{
    if (pass.length() > 20 || pass.length() < 5)
        return false;
    QRegExp r("[A-Za-z0-9_]+");
    if(r.exactMatch(pass))
        return true;
    else
            return false;
}

void MainWindow::showMessageBox(QString title, QString text)
{
        QMessageBox mess;
        mess.setWindowTitle(title);
        mess.setText(text);
        mess.exec();
}

QPixmap MainWindow::getPixmapFromQByteArray(QByteArray byteArray)
{
    QPixmap image=QPixmap();
    image.loadFromData(byteArray);
    return image;

}

QByteArray MainWindow::getShablonImage()
{
    QByteArray _shablonImageByteArray;
    QPixmap image=QPixmap(":/new/img/person.svg");
    QBuffer inBuffer( &_shablonImageByteArray );
    inBuffer.open(QIODevice::WriteOnly);
    image.save(&inBuffer, "PNG");

    return _shablonImageByteArray;
}


void MainWindow::on_pb_pi_image_clicked()
{
    QString fileName = QFileDialog::getOpenFileName
            (this, tr("Open File"),"/home",tr("Images (*.png *jpg)"));

    QPixmap image=QPixmap(fileName);
    image=image.scaled(300,300);//,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->la_pi_image->setPixmap(image);

    QBuffer inBuffer( &inByteArrayImage ); // Сохранение изображения производим через буффер

    inBuffer.open( QIODevice::WriteOnly );// Открываем буффер
    image.save( &inBuffer, "PNG" );// Записываем image в inByteArray

    SendCommandToServer(comSaveImagelUser);
}
