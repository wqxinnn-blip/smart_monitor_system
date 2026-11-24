#include "dbconnect.h"
DbConnect *DbConnect::s_instence=nullptr;
QMutex DbConnect::mutex;
QString DbConnect::host="";
int DbConnect::port=0;
QString DbConnect::user="";
QString DbConnect::pwd="";
QString DbConnect::dbname="";

DbConnect *DbConnect::getInstence()
{
    if(DbConnect::s_instence==nullptr){
        QMutexLocker locker(&mutex);
        if(DbConnect::s_instence==nullptr){
            DbConnect::s_instence=new DbConnect;
        }
    }
    return DbConnect::s_instence;
}

void DbConnect::releaseInstence()
{
    if(DbConnect::s_instence!=nullptr){
        delete DbConnect::s_instence;
        DbConnect::s_instence=nullptr;
    }
}

DbConnect::DbConnect()
{
    //1.注册mysql驱动
    db=QSqlDatabase::addDatabase("QMYSQL");
    //2.设置数据库连接参数
    db.setHostName(DbConnect::host);
    db.setPort(DbConnect::port); //默认
    db.setUserName(DbConnect::user);
    db.setPassword(DbConnect::pwd);
    db.setDatabaseName(DbConnect::dbname);
    //3.打开数据库即链接
    if(!db.open()){ //打开失败
        qDebug()<<"连接数据库失败！"<<db.lastError().text();
        return;
    }
    qDebug()<<"连接数据库成功！";
}

DbConnect::~DbConnect()
{
    //释放连接
    qDebug()<<"释放数据库连接";
    if(db.open()){
        db.close();//断连
        QSqlDatabase::removeDatabase(QSqlDatabase::defaultConnection); //移除默认连接
    }
}

//实现md5加密
QString DbConnect::getMD5(const QString &str){
    //1.将字符串转为utf-8编码字节数组（避免中文乱码）
    QByteArray byteArr=str.toUtf8();
    //计算MD5值
    QByteArray md5Arr = QCryptographicHash::hash(byteArr,QCryptographicHash::Md5);
    //3.转为16进制字符串
    QString md5Str = md5Arr.toHex();
    return md5Str;
}

QSqlDatabase DbConnect::getDb() const
{
    return db;
}

void DbConnect::setPwd(const QString &newPwd)
{
    pwd = newPwd;
}

void DbConnect::setUser(const QString &newUser)
{
    user = newUser;
}

void DbConnect::setDbname(const QString &newDbname)
{
    dbname = newDbname;
}

void DbConnect::setPort(const int &newPort)
{
    port = newPort;
}

void DbConnect::setHost(const QString &newHost)
{
    host = newHost;
}

QString DbConnect::getPwd()
{
    return pwd;
}

QString DbConnect::getUser()
{
    return user;
}

QString DbConnect::getDbname()
{
    return dbname;
}

int DbConnect::getPort()
{
    return port;
}

QString DbConnect::getHost()
{
    return host;
}
