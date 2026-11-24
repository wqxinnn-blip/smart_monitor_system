#ifndef DBCONNECT_H
#define DBCONNECT_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDateTime>
#include <QCryptographicHash>
#include <QString>
#include <QMutex>
#include <QMutexLocker>

class DbConnect
{
public:
    static DbConnect *getInstence();
    static void releaseInstence();
    DbConnect(const DbConnect&)=delete ;
    DbConnect &operator=(const DbConnect&)=delete;
    QSqlDatabase getDb() const;
    QString getMD5(const QString &str);
    static void setHost(const QString &newHost);

    static void setPort(const int &newPort);

    static void setDbname(const QString &newDbname);

    static void setUser(const QString &newUser);

    static void setPwd(const QString &newPwd);

    static QString getHost();

    static int getPort();

    static QString getDbname();

    static QString getUser();

    static QString getPwd();

private:
    DbConnect();
    ~DbConnect();
    static DbConnect *s_instence;
    QSqlDatabase db;
    static QMutex mutex;

    //基础信息
    static QString host;
    static int port;
    static QString dbname;
    static QString user;
    static QString pwd;
};

#endif // DBCONNECT_H
