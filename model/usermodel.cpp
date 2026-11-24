#include "usermodel.h"

UserModel::UserModel()
{

}

bool UserModel::queryUsernameAndPassword(QString &username, QString &password,int &userId)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QSqlQuery query(db);
    QString sql="select * from admin_user where username=:username and password=:password";
    query.prepare(sql);
    query.bindValue(":username",username);
    query.bindValue(":password",DbConnect::getInstence()->getMD5(password));
    if(query.exec()){
        if(query.next()){
            userId=query.value("id").toInt();
            qDebug()<<"userId"<<userId;
            return true;
        }
    }
    qDebug()<<"登陆失败";
    return false;
}

bool UserModel::insertUser(QString &username, QString &password)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QSqlQuery query(db);
    QString sql="select * from admin_user where username=:username";
    query.prepare(sql);
    query.bindValue(":username",username);
    if(query.exec()){
        while(query.next()){
            QString name=query.value("username").toString();
            if(name==username) return false;
        }
    }
    QString sql1="insert into admin_user(username,password,create_time,status) values"
                   "(:name,:pwd,:time,:status)";
    query.prepare(sql1);
    query.bindValue(":name",username);
    query.bindValue(":pwd",DbConnect::getInstence()->getMD5(password));
    query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":status",1);
    if(query.exec()){
        qDebug()<<"新用户插入成功";
        return query.numRowsAffected();
    }
    qDebug()<<"新用户插入失败";
    return false;
}

int UserModel::updateLoginTime(int userId)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return -1;
    }
    QSqlQuery query(db);
    QString sql="update admin_user set last_login_time=:time where id=:userId";
    query.prepare(sql);
    query.bindValue(":time",QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":userId",userId);
    if(query.exec()){
        qDebug()<<"受影响的行数："<<query.numRowsAffected();
        return query.numRowsAffected();
    }
    return -1;
}

bool UserModel::queryVideo(QList<QString> &list, int &pageCount, int pageNum, int nowPage,int &total_page)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    int offset=(nowPage-1)*pageNum;
    QSqlQuery queryData(db);
    QString sql="select video_name from video limit :offset,:pageNum";
    queryData.prepare(sql);
    queryData.bindValue(":offset",offset);
    queryData.bindValue(":pageNum",pageNum);
    if(!queryData.exec()){
        qDebug()<<"查询指定记录数视频失败！";
        return false;
    }
    while(queryData.next()){
        QString name=queryData.value("video_name").toString();
        list.append(name);
    }
    //查询总记录数
    QSqlQuery queryCount(db);
    QString sql1="select count(*) from video ";
    if(!queryCount.exec(sql1)){
        qDebug()<<"查询失败"<<queryCount.lastError().text();
        return false;
    }
    queryCount.next();
    pageCount=queryCount.value(0).toInt();
    total_page=(pageCount+pageNum-1)/pageNum;
    return true;
}


