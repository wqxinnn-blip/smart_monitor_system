#include "featureimage.h"

QString FeatureImage::getImage_name() const
{
    return image_name;
}

void FeatureImage::setImage_name(const QString &newImage_name)
{
    image_name = newImage_name;
}

QString FeatureImage::getImage_path() const
{
    return image_path;
}

void FeatureImage::setImage_path(const QString &newImage_path)
{
    image_path = newImage_path;
}

int FeatureImage::getChannelId() const
{
    return channelId;
}

void FeatureImage::setChannelId(int newChannelId)
{
    channelId = newChannelId;
}

QString FeatureImage::getCreateTime() const
{
    return createTime;
}

void FeatureImage::setCreateTime(const QString &newCreateTime)
{
    createTime = newCreateTime;
}


int FeatureImage::getImgId() const
{
    return imgId;
}

void FeatureImage::setImgId(int newImgId)
{
    imgId = newImgId;
}

int FeatureImage::getOffset_second() const
{
    return offset_second;
}

void FeatureImage::setOffset_second(int newOffset_second)
{
    offset_second = newOffset_second;
}

FeatureImage::FeatureImage() {}
