#ifndef OPERATIONINFO_H
#define OPERATIONINFO_H

#include <QString>

class OperationInfo
{
private:
    //操作日志
    int admin_id;
    QString admin_name;
    QString oper_time;
    QString oper_func;
    QString oper_desc;

public:
    OperationInfo();
    int getAdmin_id() const;
    void setAdmin_id(int newAdmin_id);

    QString getAdmin_name() const;
    void setAdmin_name(const QString &newAdmin_name);

    QString getOper_func() const;
    void setOper_func(const QString &newOper_func);


    QString getOper_desc() const;
    void setOper_desc(const QString &newOper_desc);

    QString getOper_time() const;
    void setOper_time(const QString &newOper_time);

signals:

};

#endif // OPERATIONINFO_H
