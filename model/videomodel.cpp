#include "videomodel.h"

VideoModel::VideoModel() {

}

bool VideoModel::selectVideoPath(QStringList &paths, int num_channels)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    if(num_channels==0) return false;
    QSqlQuery query(db);
    QString sql="select video_path from video order by id desc limit :num";
    query.prepare(sql);
    query.bindValue(":num",num_channels);
    if(!query.exec()){
        qDebug() << "查询失败：" << query.lastError().text();
        return false;
    }
    while(query.next()){
        QString path=query.value("video_path").toString();
        paths.append(path);
    }
    qDebug() << "查询到" << paths.size() << "条视频路径";
    return true;
}

bool VideoModel::queryVideoList(int channelId, QDate queryDate, int limit, int offset, QList<Video> &videoList,QString type)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QString beginTime=queryDate.toString("yyyy-MM-dd")+" 00:00:00";
    QString endTime=queryDate.toString("yyyy-MM-dd")+" 23:59:59";
    //构建sql
    QString sql="select * from video where create_time>=:beginTime and create_time<=:endTime";
    if(channelId > 0){
        sql+=" and channel_id=:channelId";
    }
    if(type!=""){
        sql+=" and video_type=:type";
    }
    sql+=" order by create_time desc limit :limit offset :offset";
    //绑定数据
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":beginTime",beginTime);
    query.bindValue(":endTime",endTime);
    if(channelId>0) query.bindValue(":channelId",channelId);
    if(type!="") query.bindValue(":type",type);
    query.bindValue(":limit",limit);
    query.bindValue(":offset",offset);
    //执行
    if(query.exec()){
        while(query.next()){
            Video video;
            video.setVideoId(query.value("id").toInt());
            video.setVideoName(query.value("video_name").toString());
            video.setVideoPath(query.value("video_path").toString());
            video.setChannelId(query.value("channel_id").toInt());
            video.setVideo_type(query.value("video_type").toString());
            videoList.append(video);
        }
        return true;
    }
    qDebug()<<query.lastError().text();
    return false;
}

int VideoModel::getVideosTotal(int channelId, QDate queryDate,QString type)
{
    int total=0;
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return -1;
    }
    QString beginTime=queryDate.toString("yyyy-MM-dd")+" 00:00:00";
    QString endTime=queryDate.toString("yyyy-MM-dd")+" 23:59:59";
    //构建sql
    QString sql="select count(*) as count from video where create_time>=:beginTime and create_time<=:endTime";
    if(channelId > 0){
        sql+=" and channel_id=:channelId";
    }
    if(type!=""){
        sql+=" and video_type=:type";
    }
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":beginTime",beginTime);
    query.bindValue(":endTime",endTime);
    if(channelId>0) query.bindValue(":channelId",channelId);
    if(type!="") query.bindValue(":type",type);
    if(query.exec()){
        if(query.next()){
            total=query.value("count").toInt();
        }
    }
    else{
        qDebug()<<query.lastError().text();
    }
    return total;
}

bool VideoModel::insertVideo(Video &video)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QString sql="insert into video(video_name,video_path,channel_id,video_type,create_time) "
                  "values(:name,:path,:id,:type,:create_time)";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":name",video.getVideoName());
    query.bindValue(":path",video.getVideoPath());
    query.bindValue(":id",video.getChannelId());
    query.bindValue(":type",video.getVideo_type());
    query.bindValue(":create_time",video.getCreateTime());
    if(query.exec()){
        qDebug()<<"视频添加成功！";
        return query.numRowsAffected();
    }
    return false;
}


