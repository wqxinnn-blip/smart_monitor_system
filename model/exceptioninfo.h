#ifndef EXCEPTIONINFO_H
#define EXCEPTIONINFO_H

#include <QString>

class ExceptionInfo
{
private:
    int log_id;
    int channel_id;
    QString event_time;
    QString event_desc;
    QString path;
public:
    ExceptionInfo();
    int getChannel_id() const;
    void setChannel_id(int newChannel_id);

    QString getEvent_time() const;
    void setEvent_time(const QString &newEvent_time);

    QString getEvent_desc() const;
    void setEvent_desc(const QString &newEvent_desc);

    QString getPath() const;
    void setPath(const QString &newPath);
    int getLog_id() const;
    void setLog_id(int newLog_id);
};

#endif // EXCEPTIONINFO_H
