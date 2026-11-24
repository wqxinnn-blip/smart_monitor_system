#ifndef IMAGEMODEL_H
#define IMAGEMODEL_H

#include "featureimage.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include <QFileInfo>
#include <QList>
#include "../util/dbconnect.h"
#include "featureimage.h"

class imageModel
{
public:
    imageModel();
    bool insertImage(FeatureImage &fatImage);
    bool queryAllImage(QVector<FeatureImage> &imglist,int channel_id,int size,int last_id,QDate queryDate,QMap<int,int> timeSlot);
    int getImagesTotal(int channelId,QDate queryDate,QMap<int,int> timeSlot);
    QSet<int> getHourSet(int channelId, QDate queryDate);
};

#endif // IMAGEMODEL_H
