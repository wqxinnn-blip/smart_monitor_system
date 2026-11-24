#ifndef DBCONNECTIONPOOL_H
#define DBCONNECTIONPOOL_H

#include <QString>
#include <QMutex>
#include <QHash>
#include <QThread>
#include <QUuid>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSettings> //用于读取配置文件

class DBConnectionPool
{
private:
    DBConnectionPool();
    ~DBConnectionPool();
    static DBConnectionPool* instence;
    //禁止拷贝构造和赋值操作
    DBConnectionPool(const DBConnectionPool&) = delete;
    DBConnectionPool& operator=(const DBConnectionPool&) =delete;

    bool isConnectionCalid(const QString& connName);

    QMutex mutex; //互斥锁，保护连接队列
    QHash<Qt::HANDLE,QString> threadCurrentConn; //保护连接的容器（线程ID---数据库连接名）
    QString hostName;
    QString databaseName;
    QString userName;
    QString password;
    int port;
public:
    static DBConnectionPool *getInstance(); //单例获取接口
    QSqlDatabase getConnection(); //获取当前线程可用连接
    void releaseConnection(); //归还当前线程链接
};

#endif // DBCONNECTIONPOOL_H
