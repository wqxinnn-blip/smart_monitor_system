#include "operlogservice.h"
OperLogService *OperLogService::oper_instence=nullptr;
QMutex OperLogService::mutex;

OperLogService::OperLogService(QObject *parent)
    : QObject{parent}
{}

OperLogService *OperLogService::getInstence()
{
    if(OperLogService::oper_instence==nullptr){
        QMutexLocker locker(&mutex);
        if(OperLogService::oper_instence==nullptr){
            OperLogService::oper_instence=new OperLogService;
        }
    }
    return OperLogService::oper_instence;
}

void OperLogService::releaseInstence()
{
    QMutexLocker locker(&mutex);
    if(OperLogService::oper_instence != nullptr)
    {
        delete OperLogService::oper_instence;
        OperLogService::oper_instence = nullptr;
    }
}

void OperLogService::resetInfo()
{
    operTimeMap.clear();
}

OperLogService::~OperLogService()
{
    qDebug()<<"执行析构";
}

void OperLogService::insertOperLog(int admin_id, QString admin_name, QString func, QString desc)
{
    //安全性判断
    if(admin_id<=0 || admin_name.isEmpty()){
        qDebug()<<"写入操作日志失败！";
        return;
    }
    //重复日志判断
    QDateTime dt=QDateTime::currentDateTime();
    //初次写入
    if(operTimeMap.contains(func)){
        QDateTime lastVisit=operTimeMap[func];
        if(lastVisit.secsTo(dt) < 10){
            return;
        }
    }
    //封装数据
    OperationInfo operInfo;
    operInfo.setAdmin_id(admin_id);
    operInfo.setAdmin_name(admin_name);
    operInfo.setOper_func(func);
    operInfo.setOper_desc(desc);
    //写入数据库
    bool flag=operModel.insertOper(operInfo);
    // 更新当前页面的最后访问时间
    operTimeMap[func]=dt;
    if(!flag){
        qDebug()<<"日志写入失败！";
        return;
    }
    qDebug()<<"日志写入成功！";
}

void OperLogService::insertExpLog(int channel_id, QString event_time, QString desc, QString video_path)
{
    ExceptionInfo expInfo;
    expInfo.setChannel_id(channel_id);
    expInfo.setEvent_time(event_time);
    expInfo.setEvent_desc(desc);
    expInfo.setPath(video_path);
    bool flag=operModel.insertExp(expInfo);
    if(!flag){
        qDebug()<<"异常日志写入失败！";
        return;
    }
    qDebug()<<"异常日志写入成功！";
}

QList<OperationInfo> OperLogService::queryOperLog(int curPage, int pageSize)
{
    int offset=(curPage-1)*pageSize;
    QList<OperationInfo> operList=operModel.selectOperLog(offset,pageSize);
    return operList;
}

QList<ExceptionInfo> OperLogService::queryExpLog(int curPage, int pageSize)
{
    int offset=(curPage-1)*pageSize;
    QList<ExceptionInfo> expList=operModel.selectExpLog(offset,pageSize);
    return expList;
}

int OperLogService::getOperLogCount(int type)
{
    int count=operModel.getTotalCount(type);
    return count;
}

bool OperLogService::deleteLog(QList<int> delIds,int typeFlag)
{
    bool flag=operModel.deleteLog(delIds,typeFlag);
    return flag;
}


