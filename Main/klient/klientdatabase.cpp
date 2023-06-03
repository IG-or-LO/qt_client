#include "klientdatabase.h"

klientDataBase::klientDataBase(const QString &dataBaseName, QObject *parent) : QObject(parent)
{
    this->dataBaseName=dataBaseName;
}

klientDataBase::~klientDataBase()
{

}

//внутренние методы бд

bool klientDataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("./"+dataBaseName+".db").exists()){
        this->restoreDataBase();
        return false;
    } else {
        this->openDataBase();
        return true;
    }
}

bool klientDataBase::restoreDataBase()
{
    if(this->openDataBase())
    {
        if(createUsersTable() && createChatGroupsTable() && createGroupParticipantsTable()
                && createArchiveMessageTable() && createStatusMessageTable() && createAccTable()
                && createTriggerOnMessageTable() &&createTriggerOnAccTable())
        {
            return true;
        } else {
            qDebug() << "Не удалось создать одну из таблиц";
            return false;
        }
    }
    else
    {
        qDebug() << "Не удалось восстановить/создать базу данных";
        return false;
    }
}

bool klientDataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");//QSQLITE
    db.setHostName(dataBaseName);
    db.setDatabaseName("./"+dataBaseName+".db");

    if(db.open()){
        return true;
    } else {
        return false;
    }
}


void klientDataBase::closeDataBase()
{
    db.close();
}

bool klientDataBase::createAccTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " ACC_TABLE  " ("
                            ACC_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                            ACC_NICKNAME     " VARCHAR   NOT NULL,"
                            ACC_PASS         " VARCHAR   NOT NULL,"
                            ACC_AUTO         " INTEGER      NOT NULL,"
                            ACC_IMAGE           " BLOB NOT NULL,"
                            ACC_NAME             " VARCHAR ,"
                            ACC_SURNAME          " VARCHAR ,"
                            ACC_DESCRIPTION      " VARCHAR ,"
                            ACC_PHONE            " VARCHAR ,"

                            "UNIQUE (" ACC_NICKNAME ")"
                             " )"
                    )){
        qDebug() << "DataBase: error of create " <<ACC_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Метод для создания таблицы пользователей в базе данных
 * */
bool klientDataBase::createUsersTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " USERS_TABLE  " ("
                            USER_ID " INTEGER PRIMARY KEY AUTOINCREMENT, "
                            USER_NICKNAME     " VARCHAR   NOT NULL,"
                            USER_IMAGE        " BLOB NOT NULL, "
                            "UNIQUE (" USER_NICKNAME ")"
                             " )"
                    )){
        qDebug() << "DataBase: error of create " <<USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool klientDataBase::createChatGroupsTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " CHAT_TABLE " ("
                                                 " " CHAT_ID " INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                 " " CHAT_NAME " VARCHAR NOT NULL,"
                                                 " " CHAT_USER_CREATE_ID " INTEGER NOT NULL, "
                                                 "FOREIGN KEY (" CHAT_USER_CREATE_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )");
    qDebug()<< "CREATE DATABASE " CHAT_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createNewChat(const QString &name_user_create, const QString &chatname)
{
    QSqlQuery query;
    QString str=QString("INSERT INTO " CHAT_TABLE " "
                                                        "(" CHAT_NAME ", " CHAT_USER_CREATE_ID ")"
                                                        "VALUES("
                                                        "('"+chatname+"'), "
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_ID " = '"+name_user_create+"') "//дописать
                                                        " ");
    qDebug()<< "try insert into table- "<<CHAT_TABLE<<" this chatname= "<<chatname;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createGroupParticipantsTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " PARTICIPANTS_TABLE " ("
                                                 " " CHAT_ID " INTEGER NOT NULL,"
                                                 " " USER_ID " INTEGER NOT NULL,"
                                                 "FOREIGN KEY (" CHAT_ID ") REFERENCES " CHAT_TABLE " (" CHAT_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" USER_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )"  );
    qDebug()<< "CREATE DATABASE " PARTICIPANTS_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createNewGroupParticipants(const QString chatname, const QString &userID)
{
    QSqlQuery query;
    QString str=QString("INSERT INTO " PARTICIPANTS_TABLE " "
                                                        "(" CHAT_ID ", " USER_ID ")"
                                                        "VALUES("
                                                        "(SELECT " CHAT_ID " FROM " CHAT_TABLE " WHERE  " CHAT_NAME " = '"+chatname+"' ), "
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_ID " = '"+userID+"') "//дописать
                                                        " ");
    qDebug()<< "try insert into table- "<<PARTICIPANTS_TABLE<<" this chatname= "<<chatname;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createArchiveMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " MESSAGE_TABLE " ("
                                                 " " MESSAGE_ID " INTEGER PRIMARY KEY AUTOINCREMENT,"
                                                 " " MESSAGE_TOID " INTEGER NOT NULL,"
                                                 " " MESSAGE_FROMID " INTEGER NOT NULL,"
                                                 " " CHAT_ID " INTEGER ,"
                                                 " " MESSAGE_CONTENT " VARCHAR ,"
                                                 " " MESSAGE_DATA " VARCHAR NOT NULL, "
                                                 "FOREIGN KEY (" MESSAGE_TOID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" MESSAGE_FROMID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" CHAT_ID ") REFERENCES " CHAT_TABLE " (" CHAT_ID ") ON DELETE CASCADE )" );
    qDebug()<< "CREATE DATABASE " MESSAGE_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createStatusMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TABLE " STATUS_MESS_TABLE " ("
                                                 " " MESSAGE_ID " INTEGER NOT NULL,"
                                                 " " USER_ID " INTEGER NOT NULL,"
                                                 " " STATUS_MESSAGE " VARCHAR NOT NULL,"
                                                 "FOREIGN KEY (" MESSAGE_ID ") REFERENCES " MESSAGE_TABLE " (" MESSAGE_ID ") ON DELETE CASCADE "
                                                 "FOREIGN KEY (" USER_ID ") REFERENCES " USERS_TABLE " (" USER_ID ") ON DELETE CASCADE )" );
    qDebug()<< "CREATE DATABASE " STATUS_MESS_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createTriggerOnMessageTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TRIGGER add_status_message AFTER INSERT"
                        " ON " MESSAGE_TABLE " BEGIN"
                        " INSERT INTO " STATUS_MESS_TABLE " "
                        " (" MESSAGE_ID ", " USER_ID ", " STATUS_MESSAGE ")"
                        " VALUES (NEW." MESSAGE_ID ", NEW." MESSAGE_FROMID ", 'send'),"
                        " (NEW." MESSAGE_ID ", NEW." MESSAGE_TOID ", 'notread');"
                        " END; ");
    qDebug()<< "CREATE TRIGGER ON " MESSAGE_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::createTriggerOnAccTable()
{
    QSqlQuery query;
    QString str=QString("CREATE TRIGGER add_new_user AFTER INSERT"
                        " ON " ACC_TABLE " BEGIN"
                        " INSERT INTO " USERS_TABLE " "
                        " (" USER_NICKNAME ", " USER_IMAGE ")"
                        " VALUES (NEW." ACC_NICKNAME ", NEW." ACC_IMAGE ") ;"
                        " END; ");
    qDebug()<< "CREATE TRIGGER ON " ACC_TABLE "";
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

int klientDataBase::getUserId(const QString &login_user)
{
    int _id;

    QSqlQuery query;
    QString str = QString("SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+login_user+"'  ");
    query.prepare(str);
    query.exec();

    if(query.first())
        _id=query.value(0).toInt();
    else
        qDebug()<<query.lastError();

    return _id;
}


//public методы бд

bool klientDataBase::insertIntoMessageTable(const QString &user_from, const QString &user_to, const QString &message, const QString &data,const QByteArray &shablonImage)
{
    //проверка на наличие пользователя в бд
    if(!checkNameExist(user_from))
        insertIntoUsersTable(user_from,shablonImage);
    if(!checkNameExist(user_to))
        insertIntoUsersTable(user_to,shablonImage);

    QSqlQuery query;
    QString str=QString("INSERT INTO " MESSAGE_TABLE " "
                                                        "(" MESSAGE_FROMID ", " MESSAGE_TOID ", " MESSAGE_CONTENT ", " MESSAGE_DATA ")"
                                                        "VALUES("
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"'),"//дописать
                                                        "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"'),"
                                                        "('"+message+"'),"
                                                        "('"+data+"')"
                                                        ") ");
    qDebug()<< "try insert into table- "<<MESSAGE_TABLE<<" this meesage= "<<message;
    if(query.exec(str))
        return true;
    else
    {
        qDebug()<<query.lastError();
        return false;
    }
}

bool klientDataBase::insertIntoAccTable(const QVariantList &nickPassAutoImage )
{
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " ACC_TABLE " ( " ACC_NICKNAME ", "
                                             ACC_PASS ", "
                                             ACC_AUTO ","
                                            // ACC_MAIL ","
                                             ACC_IMAGE " ) "
                  "VALUES (:LOGIN, :PASS, :AUTO, :IMAGE)");
    query.bindValue(":LOGIN",       nickPassAutoImage[0].toString());
    query.bindValue(":PASS",        nickPassAutoImage[1].toString());
    query.bindValue(":AUTO",        nickPassAutoImage[2].toInt());
    query.bindValue(":IMAGE",       nickPassAutoImage[3].toByteArray());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << ACC_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


QStringList klientDataBase::loadArchiveMessages(const QString &user_from, const QString &user_to)
{
    QSqlQuery query;
    QStringList malesDataStatusFrom;

    QString str = QString("SELECT " MESSAGE_TABLE "." MESSAGE_CONTENT ","
                          " " MESSAGE_TABLE "." MESSAGE_DATA ", "
                          " " STATUS_MESS_TABLE "." STATUS_MESSAGE ", "
                          "(SELECT " USERS_TABLE "." USER_NICKNAME " FROM " USERS_TABLE " "
                          " WHERE " USER_ID " = " MESSAGE_TABLE "." MESSAGE_FROMID " ) AS '_from'"
                          " FROM " MESSAGE_TABLE " "
                          " JOIN " STATUS_MESS_TABLE " "
                          " ON " STATUS_MESS_TABLE "." MESSAGE_ID " = " MESSAGE_TABLE "." MESSAGE_ID " "
                          " AND " STATUS_MESS_TABLE "." USER_ID " = " MESSAGE_TABLE "." MESSAGE_TOID " "
                          " WHERE " MESSAGE_TABLE "." MESSAGE_FROMID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"') "
                          " AND " MESSAGE_TABLE "." MESSAGE_TOID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"') "
                          " OR " MESSAGE_TABLE "." MESSAGE_FROMID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_to+"') "
                          " AND " MESSAGE_TABLE "." MESSAGE_TOID " = (SELECT " USER_ID " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+user_from+"') "
                          " ORDER BY " MESSAGE_TABLE "." MESSAGE_DATA "; ");
          query.prepare(str);
          query.exec();

         if(query.isActive()){
             qDebug()<<"query is active now";
             while(query.next())
             {
                 malesDataStatusFrom.append(query.value(0).toString());
                 malesDataStatusFrom.append(query.value(1).toString());
                 malesDataStatusFrom.append(query.value(2).toString());
                 malesDataStatusFrom.append(query.value(3).toString());
             }
         }
         else
             qDebug()<<"ERROR:"<<query.lastError();

         return malesDataStatusFrom;
}
//переписать
QVariantList klientDataBase::getPersonalInfo(const QString &name_user_to_load)
{
    QVariantList nameSurnameAboutImage;

    QSqlQuery query;
    QString str = QString("SELECT " ACC_NAME ", " ACC_SURNAME ", " ACC_DESCRIPTION ", " ACC_IMAGE " "
                          " FROM " ACC_TABLE " WHERE " ACC_NICKNAME " = '"+name_user_to_load+"'  ");
    query.prepare(str);
    query.exec();

    if(query.isActive()){
        qDebug()<<"query is active now";
        while(query.next())
        {
            nameSurnameAboutImage.append(query.value(0).toString());
            nameSurnameAboutImage.append(query.value(1).toString());
            nameSurnameAboutImage.append(query.value(2).toString());
            nameSurnameAboutImage.append(query.value(3).toByteArray());
        }
    }
    else
        qDebug()<<"ERROR get personal info:"<<query.lastError();

    return nameSurnameAboutImage;
}

QVariantList klientDataBase::getChatsList(const QString &login_user)
{
    QVariantList loginMessDateImageUnRead;

    int _id=getUserId(login_user);

    QSqlQuery query;
    query.prepare(QString("SELECT " USERS_TABLE "." USER_NICKNAME ", " MESSAGE_TABLE "." MESSAGE_CONTENT ", "
                          "MAX( " MESSAGE_DATA " ), " USERS_TABLE "." USER_IMAGE ", "
                          "COUNT(CASE WHEN " STATUS_MESSAGE "= 'notread' THEN 1 ELSE NULL END) "
                          "FROM " USERS_TABLE " "
                          "JOIN " MESSAGE_TABLE ", " STATUS_MESS_TABLE " "
                          "ON " MESSAGE_TABLE "." MESSAGE_ID "=" STATUS_MESS_TABLE "." MESSAGE_ID " "
                          "WHERE (" USERS_TABLE "." USER_ID "=(SELECT " MESSAGE_FROMID " WHERE " MESSAGE_TOID "= :ID ) "
                          "OR " USERS_TABLE "." USER_ID "=(SELECT " MESSAGE_TOID " WHERE " MESSAGE_FROMID "= :ID ))"
                          "AND " STATUS_MESS_TABLE "." USER_ID "= :ID "
                          "GROUP BY " USER_NICKNAME " "
                          "ORDER BY MAX( " MESSAGE_DATA " )DESC "));
    query.bindValue(":ID",_id);
    query.bindValue(":NOTREAD", "notread");
    query.exec();

    if(query.isActive()){
        qDebug()<<"query is active now";
        while(query.next())
        {
            loginMessDateImageUnRead.append(query.value(0).toString());
            loginMessDateImageUnRead.append(query.value(1).toString());
            loginMessDateImageUnRead.append(query.value(2).toDateTime());
            loginMessDateImageUnRead.append(query.value(3).toByteArray());
            loginMessDateImageUnRead.append(query.value(4).toInt());

        }
    }
    else
        qDebug()<<"ERROR get chatsLists:"<<query.lastError();

    return loginMessDateImageUnRead;
}

QByteArray klientDataBase::getImageUser(const QString &name_user_to_load)
{
    QByteArray image;

    QSqlQuery query;
    QString str = QString("SELECT " USER_IMAGE " FROM " USERS_TABLE " WHERE " USER_NICKNAME " = '"+name_user_to_load+"'  ");
    query.prepare(str);
    query.exec();

    if(query.first())
        image=query.value(0).toByteArray();
    else
        qDebug()<<query.lastError();

    return image;
}

bool klientDataBase::setImageUser(const QString &name_user_to_load, const QByteArray &inByteArray)
{
    QSqlQuery query;

    query.prepare(" UPDATE " USERS_TABLE " SET " USER_IMAGE " = :IMAGESET "
                                            "WHERE " USER_NICKNAME " = :NICKNAMESET ");
    query.bindValue(":IMAGESET",  inByteArray);
    query.bindValue(":NICKNAMESET",  name_user_to_load);

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert image in to " << USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool klientDataBase::setStatusAutoLogin(const QString &user_login, int boolOnOff)
{
    QSqlQuery query;


    query.prepare(" UPDATE " ACC_TABLE " SET " ACC_AUTO " = :STATUS"
                                            "WHERE " USER_NICKNAME " = :NICKNAMESET ");
    query.bindValue(":STATUS",  boolOnOff);
    query.bindValue(":NICKNAMESET",  user_login);

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert status in to " << ACC_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool klientDataBase::updateStatusMessageTable( const QString user, const QString status)
{
    qDebug()<<"TRY TO update status for user"<<user;
    QSqlQuery query;
    query.prepare(" UPDATE  " STATUS_MESS_TABLE " SET " STATUS_MESSAGE " = '"+status+"' "
                                            "WHERE " USER_ID " ="
                                            "(SELECT " USER_ID " FROM " USERS_TABLE " WHERE "
                                            " " USER_NICKNAME " =  '"+user+"' ) "
                                            " AND " STATUS_MESSAGE " !='send' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE INFO in " << STATUS_MESS_TABLE<<"to user:"<<user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

QString klientDataBase::getStatusMessage(const QString mess_id, const QString user)
{


}

QString klientDataBase::getLastDateChat(const QString &currentUser)
{
    qDebug()<<"TRY TO GETLASTDATE FOR"<<currentUser;
    QSqlQuery query;
    QString lastDate;

    QString str = QString("SELECT " MESSAGE_TABLE "." MESSAGE_DATA " "
                          "FROM " MESSAGE_TABLE " "
                          "JOIN " USERS_TABLE " ON " MESSAGE_TABLE "." MESSAGE_FROMID "= "
                          "(SELECT " USERS_TABLE "." USER_ID " WHERE " USER_NICKNAME " = '"+currentUser+"' )"
                          "OR " MESSAGE_TABLE "." MESSAGE_TOID " = "
                          "(SELECT " USERS_TABLE "." USER_ID " WHERE " USER_NICKNAME " = '"+currentUser+"' )"
                          "ORDER BY " MESSAGE_TABLE "." MESSAGE_DATA "; "); //DESC"); //LIMIT %1 ").arg(1);
          query.prepare(str);
          query.exec();
          if(query.isActive()){
              qDebug()<<"query is active now";
              while(query.next())
              {
                  lastDate=query.value(0).toString();
              }
          }
          else
              qDebug()<<"ERROR get lastDateChat:"<<query.lastError();


         return lastDate;

}

bool klientDataBase::insertIntoUsersTable(const QString &nickname, const QByteArray &shablonImage)
{
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " USERS_TABLE " ( " USER_NICKNAME ", "
                                              USER_IMAGE " ) "
                  "VALUES (:NICKNAME, :IMAGE )");
    query.bindValue(":NICKNAME",  nickname);
    query.bindValue(":IMAGE",  shablonImage);
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << USERS_TABLE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}



bool klientDataBase::updatePasswordIntoAccTable(const QString &name_user, const QString &newpass)
{
    QSqlQuery query;
    query.prepare("UPDATE  " ACC_TABLE " SET " ACC_PASS " = '"+newpass+"' "
                                   "WHERE " ACC_NICKNAME " = '"+name_user+"' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE PASS in " << ACC_TABLE<<"to user:"<<name_user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool klientDataBase::updatePersonalInfoIntoAccTable(const QString &name_user, const QString &name_add, const QString &surname_add, const QString &about)
{
    QSqlQuery query;
    query.prepare(" UPDATE  " ACC_TABLE " SET " ACC_NAME " = '"+name_add+"' ,"
                                            ACC_SURNAME " = '"+surname_add+"' ,"
                                            ACC_DESCRIPTION " = '"+about+"'   "
                                            "WHERE " ACC_NICKNAME " = '"+name_user+"' ");

    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error UPDATE INFO in " << ACC_TABLE<<"to user:"<<name_user;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

bool klientDataBase::checkNameExist(const QString &nickname)
{
    QSqlQuery query;
    QString names=nullptr;//для проверки совпадения логина
    QString str = QString("SELECT * FROM " USERS_TABLE " WHERE EXISTS "
                          "(SELECT " USER_NICKNAME " FROM " USERS_TABLE " WHERE " USER_NICKNAME "= '"+nickname+"')");

    query.prepare(str);
    query.exec();
   qDebug()<< query.lastError();
    if(query.isActive()){
        qDebug()<<"query is active now";
        while(query.next())
        {
            names=query.value(0).toString();
        }
    }
    else
        qDebug()<<"ERROR:"<<query.lastError();

    return names==nullptr?false:true;
}

bool klientDataBase::checkLogIn(const QString &nickname, const QString &password)
{
    QSqlQuery query;
    QString names=nullptr;//проверка наличия логина и пароля
    QString str = QString("SELECT * FROM " ACC_TABLE " WHERE EXISTS "
                          "(SELECT " ACC_NICKNAME " FROM " ACC_TABLE " WHERE " ACC_NICKNAME "= '"+nickname+"'"
                          "AND " ACC_PASS "= '"+password+"' )");

    query.prepare(str);
    query.exec();
    if(query.isActive()){
        qDebug()<<"query is active now";
        while(query.next())
        {
            names=query.value(0).toString();
        }
    }
    else
        qDebug()<<"ERROR:"<<query.lastError();

    return names==nullptr?false:true;
}



