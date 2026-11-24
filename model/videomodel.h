#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include "video.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDate>
#include "../util/dbconnect.h"

class VideoModel
{
public:
    VideoModel();
    bool selectVideoPath(QStringList &paths,int num_channels);

    //查询单页数据
    bool queryVideoList(int channelId,QDate queryDate,int limit,int offset,QList<Video> &videoList,QString type="");
    //查询符合条件的记录数
    int getVideosTotal(int channelId,QDate queryDate,QString type);
    //插入视频到数据库
    bool insertVideo(Video &video);
};

#endif // VIDEOMODEL_H
