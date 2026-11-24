#ifndef FEATUREIMAGE_H
#define FEATUREIMAGE_H

#include <QString>
#include <QDateTime>


class FeatureImage
{
private:
    int imgId;
    QString image_name;
    QString image_path;
    int channelId;
    QString createTime;
    int offset_second;
public:
    FeatureImage();

    QString getImage_name() const;
    void setImage_name(const QString &newImage_name);

    QString getImage_path() const;
    void setImage_path(const QString &newImage_path);

    int getChannelId() const;
    void setChannelId(int newChannelId);

    QString getCreateTime() const;
    void setCreateTime(const QString &newCreateTime);
    int getImgId() const;
    void setImgId(int newImgId);

    int getOffset_second() const;
    void setOffset_second(int newOffset_second);
};

#endif // FEATUREIMAGE_H
