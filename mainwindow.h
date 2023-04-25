#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTime>
#include <QMessageBox>
#include <QPainter>

#include <QTextEdit>
#include<QListWidget>
#include <emodjis.h>
#include <Commands.h>
#include <klientdatabase.h>


//tests
#include <QLayout>

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

protected:
    void paintEvent(QPaintEvent *event);

public slots:
     void loadArchiveMessages(QString user_name,QPixmap user_pic);

private slots:
     void updateSizes(int index);
     void slotReadyRead();
     void slotSokConnected();
     void slotSokDisconnected();
     void slotSokDisplayError(QAbstractSocket::SocketError socketError);
     void slotListWidgetClicked();



     //slots for qtoolbutton actions
     void toolButtonInfo();
     void toolButtonAbout();
     void toolButtonProfileInfo();
     void toolButtonEmodgiClicked();

     //
     void on_pbSend_clicked();

     void on_pb_enter_enter_clicked();

     void on_pb_reg_enter_clicked();

     void on_pb_saveChanges_clicked();

     void on_pbPersonInfo_clicked();

     void on_pb_back_personal_info_clicked();

     void on_pb_back_user_info_clicked();

     void on_pb_back_about_clicked();

   //  void on_lwUsers_currentRowChanged(int currentRow);

     void on_pb_reg_Reg_clicked();

     void on_pb_pi_image_clicked();

private:

     //переменные для отображения сообщений
     QPixmap  _scrollAreaBg;
     QBoxLayout *messLayout;
  //   QBoxLayout *listLayout;

    //переменные класса mainwindow
    Ui::MainWindow *ui;
    QTcpSocket *socket;
    QString _currentUserLogin;
    QString _lastDateMessAfterLogin;
    quint16 nextBlockSize;
    bool _already_connected;
    QByteArray inByteArrayImage;
    QListWidget* emodjiListWidget;
    QPixmap image_personal;
    QPixmap image_user;

    klientDataBase *dataBase;

    void SetInterfaceStyle();
    void SetInterfaceStyleAfterConnect();
    void GetListOfChats();
    void SetTabWidgetStyle();
    void SetMenuToolButtonActions();
    void SetEmodjiButtonStyle();
    void SendCommandToServer(quint8 command);
    void OpenDataBase(QString userLogin);

    void SetPersonalInfo(QVariantList nameSurnameAboutImage);
    bool isNameValid( QString name);
    bool isPassValid( QString pass);
    void showMessageBox(QString title,QString text);
    QPixmap getPixmapFromQByteArray(QByteArray byteArray);

    QByteArray getShablonImage();




    void addMessageGui(QString mess,QString user, QDateTime dateTime, QString status, QString title="");

    void addUserGui(QPixmap pixIcon, const QString &title, const QString &mess, QDateTime date, const int &countUnRead);

};
#endif // MAINWINDOW_H
