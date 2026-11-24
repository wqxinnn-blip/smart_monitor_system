#ifndef OPERATIONMODEL_H
#define OPERATIONMODEL_H

#include <QString>
#include <QStringList>
#include "operationinfo.h"
#include "exceptioninfo.h"
#include "../util/dbconnect.h"

class OperationModel
{
public:
    OperationModel();
    //插入一条日志
    bool insertOper(OperationInfo &operLog);
    bool insertExp(ExceptionInfo &expLog);
    //读日志数据
    QList<OperationInfo> selectOperLog(int offset,int pageSize);
    QList<ExceptionInfo> selectExpLog(int offset,int pageSize);
    //总记录数
    int getTotalCount(int type);

    bool deleteLog(QList<int> delIds,int typeFlag);
};

#endif // OPERATIONMODEL_H
