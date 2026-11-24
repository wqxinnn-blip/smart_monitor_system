#ifndef USERSERVICE_H
#define USERSERVICE_H

#include <QString>
#include <QList>
#include "../model/usermodel.h"

class UserService
{
public:
    UserService();
    bool login(QString username,QString password,int &user_id);
    bool regsiter(QString username,QString password);
    bool videoPlayBack(QList<QString> &list,int &pageCount,int pageNum,int nowPage,int &total_page);

    UserModel usermodel;
private:

};

#endif // USERSERVICE_H
