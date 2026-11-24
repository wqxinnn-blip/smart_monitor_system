#include "exceptioninfo.h"

int ExceptionInfo::getChannel_id() const
{
    return channel_id;
}

void ExceptionInfo::setChannel_id(int newChannel_id)
{
    channel_id = newChannel_id;
}

QString ExceptionInfo::getEvent_time() const
{
    return event_time;
}

void ExceptionInfo::setEvent_time(const QString &newEvent_time)
{
    event_time = newEvent_time;
}

QString ExceptionInfo::getEvent_desc() const
{
    return event_desc;
}

void ExceptionInfo::setEvent_desc(const QString &newEvent_desc)
{
    event_desc = newEvent_desc;
}

QString ExceptionInfo::getPath() const
{
    return path;
}

void ExceptionInfo::setPath(const QString &newPath)
{
    path = newPath;
}

int ExceptionInfo::getLog_id() const
{
    return log_id;
}

void ExceptionInfo::setLog_id(int newLog_id)
{
    log_id = newLog_id;
}

ExceptionInfo::ExceptionInfo() {}
