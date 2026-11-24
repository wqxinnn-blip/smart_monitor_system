#include "imagetask.h"

ImageTask::ImageTask(QObject *parent)
    : QObject{parent}
{}

void ImageTask::saveImageToDb(Mat frame,int channel_id,int type,QString ch,QString videoName,int offsetSec)
{
    //调用生成文件名的工具类
    File file;
    QString imgName=file.getShotFileName(channel_id,type,ch,videoName);
    QString imgAbsPath=file.getFullPath()+imgName;
    QString createTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    qDebug() << "Image save path:" << imgAbsPath;
    //写入文件
    imwrite(imgAbsPath.toStdString(),frame);
    // 保存到特征图片类
    FeatureImage featureImage;
    featureImage.setImage_name(imgName);
    featureImage.setImage_path(imgAbsPath);
    featureImage.setChannelId(channel_id);
    featureImage.setOffset_second(offsetSec);
    featureImage.setCreateTime(createTime);
    //调用imgModel层的插入函数
    imageModel imgmodel;
    bool flag=imgmodel.insertImage(featureImage);
    if(flag){
        if(type==1){
            if(insertCount[channel_id-1]==0){
                OperLogService::getInstence()->insertExpLog(channel_id,createTime,"检测到人体出现",videoName);
            }
            insertCount[channel_id-1]++;
            if(insertCount[channel_id-1]==3){
                insertCount[channel_id-1]=0;
            }
        }
        qDebug()<<"图片插入成功！";
        return;
    }
    qDebug()<<"图片插入失败！";
}

int ImageTask::imageScan(QVector<FeatureImage> &imglist, int channel_id, int size, int last_id, QDate create_time,QMap<int,int> timeSlot)
{
    int total = imgmodel.getImagesTotal(channel_id,create_time,timeSlot);
    imgmodel.queryAllImage(imglist,channel_id,size,last_id,create_time,timeSlot);
    //qDebug() << "get"<<imglist.size()<<"records, total:"<< total;
    return total;
}

QSet<int> ImageTask::getHourSet(int channelId, QDate queryDate)
{
    QSet<int> set;
    set=imgmodel.getHourSet(channelId,queryDate);
    return set;
}




