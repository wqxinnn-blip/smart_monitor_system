#ifndef IMAGETASK_H
#define IMAGETASK_H

#include <QObject>
#include <QString>
#include "../model/featureimage.h"
#include "../util/file.h"
#include "../model/imagemodel.h"
#include "operlogservice.h"
#include <opencv2/opencv.hpp>

using namespace cv;

class ImageTask : public QObject
{
    Q_OBJECT
private:
    int insertCount[4]={0};
public:
    explicit ImageTask(QObject *parent = nullptr);
    void saveImageToDb(Mat frame,int channel_id,int type,QString ch,QString videoName,int offsetSec);
    int imageScan(QVector<FeatureImage> &imglist,int channel_id,int size,int last_id,QDate create_time,QMap<int,int> timeSlot);
    QSet<int> getHourSet(int channelId, QDate queryDate);
    imageModel imgmodel;
signals:
};

#endif // IMAGETASK_H
