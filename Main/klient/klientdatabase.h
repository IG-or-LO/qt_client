#ifndef KLIENTDATABASE_H
#define KLIENTDATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "KlientDataBase"
#define DATABASE_NAME       "klient.db"

//ACCOUNTS KLIENT
#define ACC_TABLE               "Accounts"
#define ACC_ID                  "ACC_ID"
#define ACC_NICKNAME            "NICKNAME"
#define ACC_PASS                "PASSWORD"
#define ACC_AUTO                "auto_login"
#define ACC_NAME                "NAME"
#define ACC_SURNAME             "SURNAME"
#define ACC_IMAGE               "IMAGE"
#define ACC_DESCRIPTION         "DESCRIPTION"
//#define ACC_MAIL                "MAIL"
#define ACC_PHONE               "PHONE"
//users table defines
#define USERS_TABLE              "Users"
#define USER_ID                 "User_id"
#define USER_NICKNAME           "NICKNAME"
#define USER_IMAGE              "user_image"
//ArchiveMessage table defines
#define MESSAGE_TABLE           "Messages"
#define MESSAGE_ID              "Message_id"
#define MESSAGE_TOID            "toID"
#define MESSAGE_FROMID          "fromID"
#define MESSAGE_CONTENT         "Content"
#define MESSAGE_DATA            "Data_create"
//StatusMessage table defines
#define STATUS_MESS_TABLE       "StatusMessages"
#define STATUS_MESSAGE          "Status_mess"
//ChatGroups table defines
#define CHAT_TABLE              "Chats"
#define CHAT_ID                 "CHAT_id"
#define CHAT_NAME               "CHATNAME"
#define CHAT_USER_CREATE_ID     "USER_CREATE_id"
//GroupParticipants table defines
#define PARTICIPANTS_TABLE      "ChatParticipants"
//test
#define IMAGESET                "IMAGESET"
#define NICKNAMESET             "NICKNAMESET"

class klientDataBase : public QObject
{
    Q_OBJECT
public:
    explicit klientDataBase(const QString &dataBaseName, QObject *parent = nullptr);
    ~klientDataBase();

    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    bool connectToDataBase();

    bool insertIntoUsersTable(const QString &nickname, const QByteArray &shablonImage);
    bool insertIntoMessageTable(const QString &user_from,const QString &user_to, const QString &message, const QString &data,const QByteArray &shablonImage);
    bool insertIntoAccTable(const QVariantList &nickPassAutoImage );

    bool updatePasswordIntoAccTable(const QString &name_user,const QString &newpass);
    bool updatePersonalInfoIntoAccTable(const QString &name_user,const QString &name_add,const QString &surname_add,const QString &about);
    bool updateStatusMessageTable(const QString user, const QString status);

    QStringList loadArchiveMessages(const QString &user_from,const QString &user_to);
    QVariantList getPersonalInfo(const QString &name_user_to_load);
    QVariantList getChatsList(const QString &login_user);
    QStringList getSearchMessWithUser(const QString &login_user,const QString text);
    QByteArray getImageUser(const QString &name_user_to_load);
    QString getStatusMessage(const QString mess_id,const QString user);
    QString getLastDateChat(const QString &currentUser);


    bool setImageUser(const QString &name_user_to_load, const QByteArray &inByteArray);
    bool setStatusAutoLogin(const QString &user_login, int boolOnOff);

    bool checkNameExist(const QString &nickname);
    bool checkLogIn(const QString &nickname, const QString &password);
private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createAccTable();
    bool createUsersTable();
    bool createChatGroupsTable();
    bool createNewChat(const QString &name_user_create, const QString &chatname);
    bool createGroupParticipantsTable();
    bool createNewGroupParticipants(const QString chatname, const QString &userID);
    bool createArchiveMessageTable();
    bool createStatusMessageTable();
    bool createTriggerOnMessageTable();
    bool createTriggerOnAccTable();

    int getUserId(const QString &login_user);

    QString dataBaseName;

};


#endif // KLIENTDATABASE_H
