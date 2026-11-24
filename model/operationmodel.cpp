#include "operationmodel.h"


OperationModel::OperationModel() {}

bool OperationModel::insertExp(ExceptionInfo &expLog)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QSqlQuery query(db);
    QString sql="insert into exception_log(channel_id, event_time, event_desc, related_video_path)"
                  "values(:id,:time,:desc,:path)";
    query.prepare(sql);
    query.bindValue(":id",expLog.getChannel_id());
    query.bindValue(":time",expLog.getEvent_time());
    query.bindValue(":desc",expLog.getEvent_desc());
    query.bindValue(":path",expLog.getPath());
    if(!query.exec()){
        return false;
    }
    return true;
}

bool OperationModel::insertOper(OperationInfo &operLog)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QSqlQuery query(db);
    QString sql="insert into operation_log(admin_id, admin_name, operation_func, operation_desc)"
                  "values(:id,:name,:func,:desc)";
    query.prepare(sql);
    query.bindValue(":id",operLog.getAdmin_id());
    query.bindValue(":name",operLog.getAdmin_name());
    query.bindValue(":func",operLog.getOper_func());
    QString desc=operLog.getOper_desc();
    if(desc.isEmpty()){
        query.bindValue(":desc",QVariant());
    }
    else{
        query.bindValue(":desc",operLog.getOper_desc());
    }
    if(!query.exec()){
        return false;
    }
    return true;
}

QList<OperationInfo> OperationModel::selectOperLog(int offset, int pageSize)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return {};
    }
    QList<OperationInfo> res;
    QSqlQuery query(db);
    QString sql="select id,admin_name,operation_func,operation_time from operation_log "
                  "order by operation_time desc limit :offset,:size";
    query.prepare(sql);
    query.bindValue(":offset",offset);
    query.bindValue(":size",pageSize);
    if(!query.exec()){
        return {};
    }
    while(query.next()){
        OperationInfo operInfo;
        operInfo.setAdmin_id(query.value("id").toInt());
        operInfo.setAdmin_name(query.value("admin_name").toString());
        operInfo.setOper_func(query.value("operation_func").toString());
        QDateTime dt = query.value("operation_time").toDateTime();
        operInfo.setOper_time(dt.toString("yyyy-MM-dd HH:mm:ss"));
        res.append(operInfo);
    }
    return res;
}

QList<ExceptionInfo> OperationModel::selectExpLog(int offset, int pageSize)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return {};
    }
    QList<ExceptionInfo> res;
    QSqlQuery query(db);
    QString sql="select id,channel_id,event_time,event_desc from exception_log "
                  "order by create_time desc limit :offset,:size";
    query.prepare(sql);
    query.bindValue(":offset",offset);
    query.bindValue(":size",pageSize);
    if(!query.exec()){
        return {};
    }
    while(query.next()){
        ExceptionInfo expInfo;
        expInfo.setLog_id(query.value("id").toInt());
        expInfo.setChannel_id(query.value("channel_id").toInt());
        QDateTime dt=query.value("event_time").toDateTime();
        expInfo.setEvent_time(dt.toString("yyyy-MM-dd HH:mm:ss"));
        expInfo.setEvent_desc(query.value("event_desc").toString());
        res.append(expInfo);
    }
    return res;
}

int OperationModel::getTotalCount(int type)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return -1;
    }
    QString tableName;
    if (type==0) {
        tableName = "operation_log";
    }
    else if (type==1) {
        tableName = "exception_log";
    }
    QSqlQuery query(db);
    QString sql=QString("select count(*) as count from %1").arg(tableName);
    if(query.exec(sql)){
        query.next();
        int count=query.value("count").toInt();
        return count;
    }
    return -1;
}

bool OperationModel::deleteLog(QList<int> delIds,int typeFlag)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QString tableName;
    if (typeFlag==0) {
        tableName = "operation_log";
    }
    else if (typeFlag==1) {
        tableName = "exception_log";
    }
    QSqlQuery query(db);
    QStringList idlist;
    for(int id : delIds){
        idlist.append(QString::number(id));
    }
    QString sql=QString("delete from %1 where id in (%2)").arg(tableName).arg(idlist.join(","));
    if(query.exec(sql)){
        return true;
    }
    return false;
}
