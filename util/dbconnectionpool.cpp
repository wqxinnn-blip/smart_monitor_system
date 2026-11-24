#include "dbconnectionpool.h"
DBConnectionPool *DBConnectionPool::instence=nullptr;

DBConnectionPool::DBConnectionPool() {
    //配置数据库信息
    //文件名--格式类型（指定文件格式为标准的 INI 格式）
    QSettings settings("db_config.ini",QSettings::IniFormat);
    //使用节名/键名来获取值 赋值给成员变量
    this->hostName=settings.value("Database/host").toString();
    this->port=settings.value("Database/port").toInt();
    this->databaseName=settings.value("Database/dbname").toString();
    this->userName=settings.value("Database/user").toString();
    this->password=settings.value("Database/password").toString();
}

DBConnectionPool::~DBConnectionPool()
{
    QMutexLocker locker(&mutex);
    for(Qt::HANDLE tid:this->threadCurrentConn.keys()){
        QString connName=this->threadCurrentConn[tid];
        if(QSqlDatabase::contains(connName)){ //如果连接池中右这个连接名 再去释放
            QSqlDatabase::removeDatabase(connName);
        }
    }
    //释放所有连接
    this->threadCurrentConn.clear();
}

//校验连接是否有效
bool DBConnectionPool::isConnectionCalid(const QString &connName)
{
    if(!QSqlDatabase::contains(connName)){
        return false;
    }
    //获取指定连接名的连接对象
    QSqlDatabase db=QSqlDatabase::database(connName);
    if(!db.isOpen()){
        return db.open();
    }
    //尝试执行一条最简单的sql语句
    QSqlQuery query(db);
    return query.exec("select 1");
}

DBConnectionPool *DBConnectionPool::getInstance()
{
    static QMutex mutex;
    if(DBConnectionPool::instence==nullptr){
        mutex.lock();
        if(DBConnectionPool::instence==nullptr){
            DBConnectionPool::instence=new DBConnectionPool;
        }
        mutex.unlock();
    }
    return DBConnectionPool::instence;
}

QSqlDatabase DBConnectionPool::getConnection()
{
    QMutexLocker locker(&mutex);
    //1.获取当前线程ID
    //Qt::HANDLE 是 Qt 中定义的平台相关的句柄类型
    Qt::HANDLE threadId=QThread::currentThreadId();
    //2.判断容器中是否有属于该线程的连接
    //3.若有，并校验连接是否还有效，有效则返回该连接名对应的数据库连接对象
    if(threadCurrentConn.contains(threadId)){
        //获取连接名
        QString connName=threadCurrentConn[threadId];
        //验证连接是否有效
        if(isConnectionCalid(connName)){
            //返回该连接名对应的数据库连接对象
            return  QSqlDatabase::database(connName);
        }
        //无效
        else{
            qDebug()<<"链接无效"<<connName;
            //移除
            this->threadCurrentConn.remove(threadId);
        }
    }
    //4.若没有，则创建新的连接返回，并将该链接与当前线程号缓存到容器中
    //创建新链接
    QString connName=QUuid::createUuid().toString(); //获取唯一连接名
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL",connName);
    db.setHostName(this->hostName);
    db.setPort(this->port);
    db.setDatabaseName(this->databaseName);
    db.setUserName(this->userName);
    db.setPassword(this->password);
    if(db.open()){
        qDebug()<<"new db creat success"<<connName;
        this->threadCurrentConn[threadId]=connName;
        return db;
    }
    return QSqlDatabase();
}

//线程结束后，销毁链接
void DBConnectionPool::releaseConnection()
{
    QMutexLocker locker(&mutex);
    Qt::HANDLE tid=QThread::currentThreadId();
    if(!threadCurrentConn.contains(tid)){
        return;
    }
    QString connName=this->threadCurrentConn[tid];
    if(QSqlDatabase::contains(connName)){
        QSqlDatabase::removeDatabase(connName);
    }
    this->threadCurrentConn.remove(tid);
}
