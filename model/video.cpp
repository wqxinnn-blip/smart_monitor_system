#include "video.h"

int Video::getVideoId() const
{
    return videoId;
}

void Video::setVideoId(int newVideoId)
{
    videoId = newVideoId;
}

QString Video::getVideoName() const
{
    return videoName;
}

void Video::setVideoName(const QString &newVideoName)
{
    videoName = newVideoName;
}

QString Video::getVideoPath() const
{
    return videoPath;
}

void Video::setVideoPath(const QString &newVideoPath)
{
    videoPath = newVideoPath;
}

int Video::getChannelId() const
{
    return channelId;
}

void Video::setChannelId(int newChannelId)
{
    channelId = newChannelId;
}

QString Video::getCreateTime() const
{
    return createTime;
}

void Video::setCreateTime(const QString &newCreateTime)
{
    createTime = newCreateTime;
}

QString Video::getVideo_type() const
{
    return video_type;
}

void Video::setVideo_type(const QString &newVideo_type)
{
    video_type = newVideo_type;
}

Video::Video() {}


