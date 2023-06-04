#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>
#include <QAction>
#include<QDirIterator>
#include <QFileDialog>
#include <QBuffer>

#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <stylehelper.h>
#include <messegeswidget.h>
#include <mylistwidgetitem.h>


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //определние классовых перменных
    image_personal=QPixmap();
    encrypt=new Encryption();
    nextBlockSize=0;
    _currentUserLogin = "";
    QRegularExpression regExp( "[a-zA-Z0-9]+@[a-zA-Z0-9.]+" );
    validator=new QRegularExpressionValidator(regExp,this);
    _already_connected=false;
    socket=new QTcpSocket(this);

    //connections
    connect(socket, &QTcpSocket::readyRead,this,&MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::connected, this, &MainWindow::slotSokConnected);
    connect(socket,&QTcpSocket::disconnected,this,&MainWindow::slotSokDisconnected);
    connect(socket , SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(slotSokDisplayError(QAbstractSocket::SocketError)));

    //методы обработки интерфейса
    SetStartObjectStatus();
    SetTabWidgetStyle();
    SetMenuToolButtonActions();
    SetInterfaceStyle();
    SetEmodjiButtonStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetStartObjectStatus()
{
    ui->tabWidget->setCurrentIndex(1);
    //first tab
    ui->le_login_enter->setFocus();
    ui->frame_reg->hide();
    ui->frame_connection->hide();
    //tab chat area
    messLayout=new QBoxLayout(QBoxLayout::TopToBottom);
    messLayout->addStretch(-1);
    ui->mess_frame->setLayout(messLayout);

    ui->frame_messEdit->hide();
    ui->frame_userInfo->hide();
    ui->listWidget_Search->hide();
    ui->listWidget_SearchMess->hide();
    ui->frame_TypeSearch->hide();
    //placeHolders
    ui->textEdit->setPlaceholderText("Написать сообщение");
    ui->leSearch->setPlaceholderText("Поиск");
    ui->le_reg_Mail->setValidator(validator);

    ui->scrollArea->installEventFilter(this);
}

void MainWindow::SetInterfaceStyle()
{
    //tab chat
    ui->mess_frame->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->left_frame->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->right_frame->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_Main->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_messEdit->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    //ui->frame_userInfo->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_connection->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_enter->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_reg->setStyleSheet(stylehelper::getFrameNoBorderStyle());
    ui->frame_userInfo->setStyleSheet(stylehelper::getFrameUserInfoStyle());
    ui->frame_TypeSearch->setStyleSheet(stylehelper::getFrameSearchInfoStyle());

    //buttonsStyle
    ui->pbSend->setEnabled(true);
    ui->pbAdd->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbMenu->setStyleSheet(stylehelper::getButtonMenuStyle());
    ui->pbSave->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbSend->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbSearch->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pbPersonInfo->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_personal_info->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_user_info->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_back_about->setStyleSheet(stylehelper::getButtonsStyle());
    ui->pb_reg_Reg->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_connection->setStyleSheet(stylehelper::getButtonConnectionStyle());
    ui->pb_reg_back->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_saveChanges->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_reg_enter->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_enter_enter->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pb_pi_image->setStyleSheet(stylehelper::getButtonsRegAndEnterStyle());
    ui->pbCloseSearch->setStyleSheet(stylehelper::getButtonsStyle());
    //lablesStyle
    ui->la_logo->setStyleSheet(stylehelper::getLabelLogoStyle());
    ui->label_enter_or_reg->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_login->setStyleSheet(stylehelper::getLabelsStyle());
    ui->label_pass->setStyleSheet(stylehelper::getLabelsStyle());
    //ui->label_settings->setStyleSheet(stylehelper::getLabelsStyle());
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
    //lineeditStyle
    ui->leSearch->setStyleSheet(stylehelper::getLineEditSearchStyle());

    //scrollarea
    ui->scrollArea->setStyleSheet(stylehelper::getScrollAreaStyle());
}

void MainWindow::SetTabWidgetStyle()
{
      ui->tabWidget->tabBar()->hide();
      ui->tab->setObjectName("tabChat");
      ui->tab_auth->setObjectName("auth");
      ui->tab_info->setObjectName("info");
      ui->tab_personalInfo->setObjectName("perinfo");
      ui->tab_about->setObjectName("about");

      ui->tabWidget->setStyleSheet(stylehelper::getTabWidgetStyle());
      ui->tab_auth->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_personalInfo->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_info->setStyleSheet(stylehelper::getTabsStyle());
      ui->tab_about->setStyleSheet(stylehelper::getTabsStyle());
}

void MainWindow::SetMenuToolButtonActions()
{
    QAction *btn_Profileinfo=new QAction(this);
    QAction *btn_About=new QAction(this);
    QAction *btn_Exit=new QAction(this);
    QFont font("Roboto-Medium",13,QFont::Bold);
    btn_Profileinfo->setFont(font);
    btn_About->setFont(font);
    btn_Exit->setFont(font);

    QIcon icon_info(":/new/img/icon-user.png");
    QIcon icon_about(":/new/img/round-chat-interrogation(48x48)@1x.png");
    QIcon icon_exit(":/new/img/logout-2(48x48)@1x.png");
    btn_Profileinfo->setIcon(icon_info);
    btn_About->setIcon(icon_about);
    btn_Exit->setIcon(icon_exit);

    btn_Profileinfo->setText("Личный профиль");
    btn_About->setText("О программе");
    btn_Exit->setText("Выход из аккаунта");

    connect(btn_Profileinfo,&QAction::triggered,this,&MainWindow::toolButtonProfileInfo);
    connect(btn_About,&QAction::triggered,this,&MainWindow::toolButtonAbout);
    connect(btn_Exit,&QAction::triggered,this,&MainWindow::toolButtonExit);

    ui->pbMenu->addAction(btn_Profileinfo);
    ui->pbMenu->addAction(btn_About);
    ui->pbMenu->addAction(btn_Exit);
    ui->pbMenu->setArrowType(Qt::NoArrow);
}

void MainWindow::SetInterfaceStyleAfterConnect()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->textEdit->setFocus();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(this->width()<550)
        ui->right_frame->hide();
    else
        ui->right_frame->show();

    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter,this);
    QWidget::paintEvent(event);
}

//END OF INTERFACE functions

void MainWindow::OpenDataBase(QString userLogin)
{
    //база клиента
    dataBase=new klientDataBase(userLogin);
    if(!dataBase->connectToDataBase()) //если бд не было,то сохраняем новый аккаунт
    dataBase->insertIntoAccTable(QVariantList() <<ui->le_login_enter->text()
                                               <<ui->le_pass_enter->text()
                                               <<0//auto_login=false
                                             //  <<ui->le_reg_Mail->text()
                                 <<getShablonImage());
}


void MainWindow::UpdateListOfChats()
{
    qDebug()<<"in getlistofchats";
    //очищаем список
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        MyListWidgetItem *listWidget=qobject_cast<MyListWidgetItem*>(ui->listWidget->itemWidget(ui->listWidget->item(i)));
        delete listWidget;
    }
    ui->listWidget->clear();
   //получаем обновленный список и отпраляем его в addUserGui
   QVariantList loginMessDateImageUnRead=dataBase->getChatsList(_currentUserLogin);
   for (int i = 0; i < loginMessDateImageUnRead.size(); i=i+5) {
       addUserGui(getPixmapFromQByteArray(loginMessDateImageUnRead[i+3].toByteArray()),loginMessDateImageUnRead[i].toString(),
               loginMessDateImageUnRead[i+2].toDateTime(),false,loginMessDateImageUnRead[i+1].toString(),loginMessDateImageUnRead[i+4].toInt());
   }
}

void MainWindow::UpdateListOfMessages()
{
    for (int i = messLayout->count()-1; i > 0; --i) {
        MessegesWidget *messWidget= qobject_cast<MessegesWidget *>(messLayout->itemAt(i)->widget());
        messWidget->setWidgetStatus(true);
    }
}

void MainWindow::on_pbPersonInfo_clicked()
{
   ui->tabWidget->setCurrentIndex(3);
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
        out << encrypt->encodeMessage(ui->le_login_enter->text());
        out << encrypt->encodeMessage(ui->le_pass_enter->text());
        out << encrypt->getHashKey();
        out << encrypt->getHashIV();
    }
        break;
    case comRegReq:
    {
        out << encrypt->encodeMessage(ui->le_reg_Login->text());
        out << encrypt->encodeMessage(ui->le_reg_Pass->text());
        out << encrypt->encodeMessage(ui->le_reg_Mail->text());
        out << encrypt->encodeMessage(ui->le_reg_Phone->text());
        out << encrypt->getHashKey();
        out << encrypt->getHashIV();
    }
        break;

    case comMessageToUsers:
    {
       out << encrypt->encodeMessage(ui->laWithUserChat->text());
       out << encrypt->encodeMessage(ui->textEdit->toPlainText());
       out << encrypt->getHashKey();
       out << encrypt->getHashIV();
    }
        break;

    case comGetNewMessages:
    {
        QString lastDateMess= dataBase->getLastDateChat(_currentUserLogin);//последняя дата
        qDebug()<<"lastDATEmess:"<<lastDateMess;                           //смс из БД
        if(lastDateMess!="")                                               //если были сообщения
        {
            out<<encrypt->encodeMessage(lastDateMess);
            out << encrypt->getHashKey();
            out << encrypt->getHashIV();
        }
    }
        break;

    case comSearchUsers:
    {
        out << encrypt->encodeMessage(ui->leSearch->text().toLower());
        out << encrypt->getHashKey();
        out << encrypt->getHashIV();
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
        if( ui->tabWidget->currentIndex()==3)
            out << ui->laWithUserChat->text();
        else
            out << _currentUserLogin;
    }
        break;
    case comSavePersonalInfo:
    {
        out << _currentUserLogin;
        out << ui->le_pi_name->text();
        out << ui->le_pi_surname->text();
        out << ui->le_pi_aboutme->toPlainText();
    }
        break;
    case comUpdatePassword:
    {
        out << encrypt->encodeMessage(ui->le_pi_newpass->text());
        out << encrypt->getHashKey();
        out << encrypt->getHashIV();
    }
        break;
    case comSaveImagelUser:
    {
        out << inByteArrayImage;
    }
        break;

    case comUpdateStatusMessages:
    {
        out << encrypt->encodeMessage(ui->laWithUserChat->text());
        out << encrypt->getHashKey();
        out << encrypt->getHashIV();
    }
        break;

    default:
        break;
    }



     out.device()->seek(0);
     out << (quint16)(block.size() - sizeof(quint16));
     socket->write(block);
}

void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    if (nextBlockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
        //    if (socket->bytesAvailable() < (int)sizeof(unsigned int))
            return;
        in >> nextBlockSize;
   //     qDebug() << "_blockSize now " << nextBlockSize<< " bytesAvalible"<<socket->bytesAvailable();
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
        ui->frame_reg->hide();
        ui->frame_enter->show();
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
        QVariantList malesDataStatusFromImage;
        in>>malesDataStatusFromImage;
        for (int i = 0; i < malesDataStatusFromImage.size(); i=i+5) {//добавляем новые сообщения в бд
            dataBase->insertIntoMessageTable(malesDataStatusFromImage[i+3].toString(),
                    _currentUserLogin,malesDataStatusFromImage[i].toString(),malesDataStatusFromImage[i+1].toString(),malesDataStatusFromImage[i+4].toByteArray());
            qDebug()<<"bool image exist or not:"<<malesDataStatusFromImage[i+4].isNull();
        }
        UpdateListOfChats();//получение списка чатов и вывод их в гуи
    }
        break;
    case comGetPersonalInfo:
        {
            QVariantList nameSurnameDescImage;
            in>>nameSurnameDescImage;
            SetPersonalInfo(nameSurnameDescImage);
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
                       malesDataStatusFromTo[i],malesDataStatusFromTo[i+1],getShablonImage());//getshablonimage заменить на реальную картинку при использовании в дальнейшем
           }
        }
        break;

    case comSearchUsers:
    {
        QPixmap image2=QPixmap(":/new/img/icon-user.png");
        int size; QString user_name;
        in>>size;
        qDebug()<<"sizeOfQvariANTlIstUsers:"<<size;
        for (int i = 0; i < size; i=i+2) {
            in>>user_name;
           // in>>image;
            addUserGui(image2, user_name, QDateTime::currentDateTime(),true);
        }
        ui->leSearch->setEnabled(true);
        ui->leSearch->setFocus();
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
            QByteArray userBa, messBa, dateBa, imageBa, hashKey, hashIv;
            in >> userBa >> messBa >> dateBa >> imageBa >> hashKey >> hashIv;
            QString user=encrypt->decodeMessage(hashKey,hashIv, userBa);
            QString mess=encrypt->decodeMessage(hashKey,hashIv, messBa);
            QString date=encrypt->decodeMessage(hashKey,hashIv,dateBa);
            dataBase->insertIntoMessageTable(user,_currentUserLogin,mess,date,imageBa);
            MyListWidgetItem *listWidget=qobject_cast<MyListWidgetItem*>(ui->listWidget->itemWidget(ui->listWidget->currentItem()));
            if(listWidget!=0 && listWidget->getUserName()==user)
            addMessageGui(mess,user,QDateTime::fromString(date,"ddd MMM d hh:mm:ss yyyy"),"must be done");
            else
            UpdateListOfChats();

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

    case comUpdateStatusMessages:
    {
        QString userToUpdate;
        in >> userToUpdate;
        dataBase->updateStatusMessageTable(userToUpdate,"read");
        if(ui->laWithUserChat->text()==userToUpdate)//если открыт чат с пользователем, то обновляем статусы
        UpdateListOfMessages();
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
    //обнуляем статусы виджетов
    ui->le_login_enter->setText("");
    ui->le_pass_enter->setText("");
    ui->frame_userInfo->hide();
    ui->frame_messEdit->hide();
    ui->frame_TypeSearch->hide();
    ui->listWidget->show();
    ui->listWidget_Search->hide();
    ui->listWidget_SearchMess->hide();
    ui->leSearch->setText("");
    ui->pbSearch->setEnabled(true);
    ui->pbSearch->setChecked(false);

    ui->frame_connection->hide();
    ui->cb_autoLogin->setChecked(false);
    ui->tabWidget->setCurrentIndex(1);

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

void MainWindow::toolButtonInfo()
{
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::toolButtonAbout()
{
    ui->tabWidget->setCurrentIndex(4);
}

void MainWindow::toolButtonExit()
{
    dataBase->setStatusAutoLogin(_currentUserLogin,0);//устанавливаем автовход отключенным
    socket->disconnectFromHost();
}
//может быть добавить команду сендто сервер
void MainWindow::toolButtonProfileInfo()
{
    ui->tabWidget->setCurrentIndex(2);
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
    dataBase->insertIntoMessageTable(_currentUserLogin,ui->laWithUserChat->text(),ui->textEdit->toPlainText(),
                                     QDateTime::currentDateTime().toString("ddd MMM d hh:mm:ss yyyy"), getQByteArrayFromPixmap(image_user));
    ui->textEdit->clear();

    if(ui->listWidget_Search->isVisible())
    {
       if(MyListWidgetItem *listItem=qobject_cast<MyListWidgetItem *>(ui->listWidget_Search->itemWidget(ui->listWidget_Search->currentItem())))
           addUserGui(listItem->getUserPixmap(),listItem->getUserName(),QDateTime::currentDateTime(),false);
    }
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
            if(socket->waitForConnected(5000))
            SendCommandToServer(comAutchReq);
        }
        else
            SendCommandToServer(comAutchReq);
    }
}

void MainWindow::on_pb_reg_enter_clicked()
{
    ui->frame_connection->hide();
    ui->frame_enter->hide();
    ui->frame_reg->show();
}

void MainWindow::loadArchiveMessages()
{
   qDebug()<<"Колличество обьектов в meessLayouyt"<<messLayout->count();
//очищаем список сообщений в messLayout
   //очищаем список сообщений в messLayout
   for (int i = messLayout->count()-1; i > 0; --i) {
       QLayoutItem *messItem=messLayout->takeAt(i);
       delete messItem->widget();
       delete messItem;
   }
    //открываем фреймы чата
    ui->frame_userInfo->show();
    ui->frame_messEdit->show();

    //получаем имя пользователя и его картинку в листвиджете
    if(MyListWidgetItem *item= qobject_cast<MyListWidgetItem *>(sender()))
    {
        ui->laWithUserChat->setText(item->getUserName());
        image_user=item->getUserPixmap();
        item->setMessCountUnread(0);
    }
    //обноваляем статус на прочитано
    dataBase->updateStatusMessageTable(ui->laWithUserChat->text(),"read");

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

    SendCommandToServer(comUpdateStatusMessages);
}

void MainWindow::SetPersonalInfo(QVariantList nameSurnameAboutImage)
{
    if(nameSurnameAboutImage.empty())
        return;

    if(ui->tabWidget->currentIndex()==3)
    {
        ui->le_user_name->setText(nameSurnameAboutImage[0].toString());
        ui->le_user_surname->setText(nameSurnameAboutImage[1].toString());
        ui->le_user_aboutme->setText(nameSurnameAboutImage[2].toString());
        QPixmap image=QPixmap();
        image.loadFromData(nameSurnameAboutImage[3].toByteArray());
        ui->la_user_image->setPixmap(image.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
    else
    {
        ui->le_pi_name->setText(nameSurnameAboutImage[0].toString());
        ui->le_pi_surname->setText(nameSurnameAboutImage[1].toString());
        ui->le_pi_aboutme->setText(nameSurnameAboutImage[2].toString());

        image_personal.loadFromData(nameSurnameAboutImage[3].toByteArray());
        ui->la_pi_image->setPixmap(image_personal.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}

void MainWindow::addMessageGui(QString mess, QString user, QDateTime dateTime, QString status, QString title /* ="" */)
{
    MessegesWidget *messWidget=new MessegesWidget(this);
    if(_currentUserLogin==user)
    {
        messWidget->setWidgetDisign(false);
        messWidget->setWidgetStatus(status=="read"?true:false);
    }
    else
        messWidget->setWidgetDisign(true);

     messWidget->addWidgetMessData(mess,dateTime);

    if(title!="")
        messWidget->addWidgetTitle(title);

    messLayout->addWidget(messWidget);

}

void MainWindow::addUserGui(QPixmap pixIcon, const QString &title,
                            QDateTime date, bool listSearch, QString mess, const int &countUnRead)
{
    MyListWidgetItem *listItem=new MyListWidgetItem(pixIcon,title,this);
    listItem->setMessCountUnread(countUnRead);
    if(mess!="")
    listItem->setMessShort(mess,date);

    QListWidgetItem *item;
    if(listSearch && !ui->pbSearch->isChecked())//если используется глобальный поиск
    {
        connect(listItem,SIGNAL(onWidgetClicked()),this,SLOT(loadArchiveMessages()));
        item=new QListWidgetItem(ui->listWidget_Search);
        ui->listWidget_Search->setItemWidget(item,listItem);
    }
    else if(listSearch && ui->pbSearch->isChecked())//если используется локальный поиск
    {
        item=new QListWidgetItem(ui->listWidget_SearchMess);
        ui->listWidget_SearchMess->setItemWidget(item,listItem);
    }
    else//без поиска
    {
        connect(listItem,SIGNAL(onWidgetClicked()),this,SLOT(loadArchiveMessages()));
        item=new QListWidgetItem(ui->listWidget);
        ui->listWidget->setItemWidget(item,listItem);
    }
        item->setSizeHint(listItem->sizeHint());

}

void MainWindow::on_pb_saveChanges_clicked()
{
    //if no value personal info
    if(ui->le_pi_name->text()=="" && ui->le_pi_aboutme->toPlainText()=="" && ui->le_pi_surname->text()=="" )
        QMessageBox::information(this, "Error", "Need to fill some fields...");
    else
        SendCommandToServer(comSavePersonalInfo);

    //if no pass
    if(ui->le_pi_newpass->text()!="" && ui->le_pi_newrepeatpass->text()=="")
         QMessageBox::information(this, "Error", "Need to fill newpassrepeat field...");
    else if(ui->le_pi_newpass->text()!=ui->le_pi_newrepeatpass->text())
        QMessageBox::information(this, "Error", "PASS AND REPEAT PASS NOT EQUAL...");
    else if(ui->le_pi_newpass->text()!="" && ui->le_pi_newrepeatpass->text()!="")
        SendCommandToServer(comUpdatePassword);

}

void MainWindow::on_pb_back_personal_info_clicked()
{
    ui->tabWidget->setCurrentIndex(0);

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

QByteArray MainWindow::getQByteArrayFromPixmap(QPixmap pixmap)
{
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "PNG");
    return bArray;
}

QByteArray MainWindow::getShablonImage()
{
    QByteArray _shablonImageByteArray;
    QPixmap image=QPixmap(":/new/img/icon-user.png");
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
    if(image.isNull())//если не выбрана картинка
        return;
    image=image.scaled(100,100,Qt::KeepAspectRatio,Qt::SmoothTransformation);//,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    ui->la_pi_image->setPixmap(image);

    QBuffer inBuffer( &inByteArrayImage ); // Сохранение изображения производим через буффер

    inBuffer.open( QIODevice::WriteOnly );// Открываем буффер
    image.save( &inBuffer, "PNG" );// Записываем image в inByteArray

    dataBase->setImageUser(_currentUserLogin,inByteArrayImage);//сохраняем фото в бд клиента
    UpdateListOfChats();//обновляем список
    SendCommandToServer(comSaveImagelUser);
}


void MainWindow::on_leSearch_textChanged(const QString &arg1)
{
    ui->listWidget_Search->clear();
    ui->listWidget_SearchMess->clear();

    if(ui->leSearch->text()=="")
    {
        return;
    }
    else if(!ui->pbSearch->isChecked())
    {
        if(ui->listWidget_Search->isHidden())
        {
            ui->listWidget->hide();
            ui->listWidget_SearchMess->hide();
            ui->listWidget_Search->show();
            ui->frame_TypeSearch->show();
            ui->laTypeSearch->setText("Глобальный поиск");
        }
        SendCommandToServer(comSearchUsers);
        ui->leSearch->setEnabled(false);
    }
    else if(ui->pbSearch->isChecked())
    {
        QStringList messData;
        messData=dataBase->getSearchMessWithUser(ui->laWithUserChat->text(),arg1);
        for (int  i= 0; i < messData.size(); i=i+2) {
        addUserGui(getPixmapFromQByteArray(dataBase->getImageUser(ui->laWithUserChat->text()))
                   ,ui->laWithUserChat->text(),QDateTime::fromString(messData[i+1]),true,messData[i]);
        }

    }
}


void MainWindow::on_pb_connection_clicked()
{
    if(ui->frame_connection->isHidden())
        ui->frame_connection->show();
    else
        ui->frame_connection->hide();
}

void MainWindow::on_pb_reg_back_clicked()
{
    if(!ui->frame_connection->isHidden())
        ui->frame_connection->hide();
    ui->frame_reg->hide();
    ui->frame_enter->show();
}

void MainWindow::on_pbSearch_clicked()
{
    //обработка открытия/закрытия листвиджетов
    ui->listWidget->hide();
    ui->listWidget_Search->hide();
    ui->frame_TypeSearch->show();
    ui->laTypeSearch->setText("Поиск сообщений в чате");
    ui->listWidget_SearchMess->show();
    ui->pbSearch->setEnabled(false);
    ui->leSearch->setFocus();
}

void MainWindow::on_pbCloseSearch_clicked()
{
    ui->listWidget_Search->hide();
    ui->listWidget_SearchMess->hide();
    ui->frame_TypeSearch->hide();
    ui->laTypeSearch->setText("");
    ui->leSearch->setText("");
    ui->leSearch->clearFocus();
    //обнуляем кнопку локального поиска
    if(ui->pbSearch->isChecked())
    {
        ui->pbSearch->setChecked(false);
        ui->pbSearch->setEnabled(true);
    }
    ui->listWidget->show();

}
