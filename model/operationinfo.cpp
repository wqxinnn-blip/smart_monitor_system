#include "operationinfo.h"

QString OperationInfo::getOper_time() const
{
    return oper_time;
}

void OperationInfo::setOper_time(const QString &newOper_time)
{
    oper_time = newOper_time;
}

OperationInfo::OperationInfo() {}

QString OperationInfo::getOper_desc() const
{
    return oper_desc;
}

void OperationInfo::setOper_desc(const QString &newOper_desc)
{
    oper_desc = newOper_desc;
}

QString OperationInfo::getOper_func() const
{
    return oper_func;
}

void OperationInfo::setOper_func(const QString &newOper_func)
{
    oper_func = newOper_func;
}

QString OperationInfo::getAdmin_name() const
{
    return admin_name;
}

void OperationInfo::setAdmin_name(const QString &newAdmin_name)
{
    admin_name = newAdmin_name;
}

int OperationInfo::getAdmin_id() const
{
    return admin_id;
}

void OperationInfo::setAdmin_id(int newAdmin_id)
{
    admin_id = newAdmin_id;
}
