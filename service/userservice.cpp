#include "userservice.h"

UserService::UserService()
{

}

bool UserService::login(QString username, QString password,int &user_id)
{
    int userId=0;
    bool flag=usermodel.queryUsernameAndPassword(username,password,userId);
    user_id=userId;
    if(flag){
        //更新最后一次的登录时间
        int row=usermodel.updateLoginTime(userId);
        if(row>0){
            qDebug()<<"用户验证成功 可登录";
            return true;
        }
    }
    return false;
}

bool UserService::regsiter(QString username, QString password)
{
    bool flag=usermodel.insertUser(username,password);
    if(flag){
        return true;
    }
    return false;
}

bool UserService::videoPlayBack(QList<QString> &list,int &pageCount,int pageNum,int nowPage,int &total_page)
{
    bool flag=usermodel.queryVideo(list,pageCount,pageNum,nowPage,total_page);
    if(flag){
        qDebug()<<"视频记录查询成功";
        return true;
    }
    return false;
}

