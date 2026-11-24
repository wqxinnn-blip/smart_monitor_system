#ifndef VIDEO_H
#define VIDEO_H

#include <QStringList>
#include <QString>

class Video
{
private:
    int videoId;
    QString videoName;
    QString videoPath;
    int channelId;
    QString video_type;
    QString createTime;
public:
    Video();

    int getVideoId() const;
    void setVideoId(int newVideoId);

    QString getVideoName() const;
    void setVideoName(const QString &newVideoName);

    QString getVideoPath() const;
    void setVideoPath(const QString &newVideoPath);

    int getChannelId() const;
    void setChannelId(int newChannelId);

    QString getCreateTime() const;
    void setCreateTime(const QString &newCreateTime);
    QString getVideo_type() const;
    void setVideo_type(const QString &newVideo_type);
};

#endif // VIDEO_H
