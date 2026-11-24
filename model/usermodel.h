#ifndef USERMODEL_H
#define USERMODEL_H

#include <QString>
#include <QDebug>
#include <QList>
#include <QDateTime>
#include "../util/dbconnect.h"

class UserModel
{
public:
    UserModel();
    bool queryUsernameAndPassword(QString &username,QString &password,int &userId);
    bool insertUser(QString &username,QString &password);
    int updateLoginTime(int userId);
    bool queryVideo(QList<QString> &list,int &pageCount,int pageNum,int nowPage,int &total_page);
};

#endif // USERMODEL_H
