#ifndef OPERLOGSERVICE_H
#define OPERLOGSERVICE_H

#include <QObject>
#include <QMutex>
#include <QMutexLocker>
#include "../model/operationmodel.h"


class OperLogService : public QObject
{
    Q_OBJECT
private:
    explicit OperLogService(QObject *parent = nullptr);
    ~OperLogService();

    OperLogService(const OperLogService&)=delete;
    OperLogService& operator=(const OperLogService&)=delete;

    static QMutex mutex;
    static OperLogService *oper_instence;
    OperationModel operModel;

    //记录信息
    QMap<QString,QDateTime> operTimeMap;
    QDateTime lastOperTime=QDateTime(); //做后一次操作的时间
public:
    static OperLogService *getInstence();
    static void releaseInstence();
    void resetInfo();

    void insertOperLog(int admin_id,QString admin_name,QString func,QString desc="");
    void insertExpLog(int channel_id,QString event_time,QString desc,QString video_path);

    QList<OperationInfo> queryOperLog(int curPage,int pageSize);
    QList<ExceptionInfo> queryExpLog(int curPage,int pageSize);

    int getOperLogCount(int type);
    bool deleteLog(QList<int> delIds,int typeFlag);


signals:
};

#endif // OPERLOGSERVICE_H
