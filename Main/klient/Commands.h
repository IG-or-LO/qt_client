#ifndef COMMANDS_H
#define COMMANDS_H

static const QString constNameUnknown;
static const quint8 comRegReq=0;
static const quint8 comAutchReq = 1;
static const quint8 comUsersOnline = 2;
static const quint8 comUserJoin = 3;
static const quint8 comUserLeft = 4;
static const quint8 comMessageToAll = 5;
static const quint8 comMessageToUsers = 6;
static const quint8 comPublicServerMessage = 7;
static const quint8 comPrivateServerMessage = 8;
static const quint8 comAuthSuccess = 9;
static const quint8 comLoadArchive=10;
static const quint8 comSavePersonalInfo = 11;
static const quint8 comUpdatePassword = 12;
static const quint8 comGetPersonalInfo = 13;
static const quint8 comSaveImagelUser = 14;
static const quint8 comRegSuccess=15;
static const quint8 comGetNewMessages=16;
static const quint8 comUpdateStatusMessages=17;
static const quint8 comSearchUsers=19;
static const quint8 comErrNameUsed = 201;
static const quint8 comErrNameOrPassFalse = 202;


#endif // COMMANDS_H
