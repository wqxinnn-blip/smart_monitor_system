#include "imagemodel.h"

imageModel::imageModel() {}

bool imageModel::insertImage(FeatureImage &fatImage)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QString sql="insert into feature_image(image_name,image_path,channel_id,offset_second,create_time)"
                  "values(:name,:path,:channel_id,:offset_second,:time)";
    QSqlQuery query(db);
    query.prepare(sql);
    query.bindValue(":name",fatImage.getImage_name());
    query.bindValue(":path",fatImage.getImage_path());
    query.bindValue(":channel_id",fatImage.getChannelId());
    query.bindValue(":offset_second",fatImage.getOffset_second());
    query.bindValue(":time",fatImage.getCreateTime());
    if(!query.exec()){
        qDebug()<<"数据插入失败！";
        return false;
    }
    qDebug()<<"affect rows:"<<query.numRowsAffected();
    return true;
}

bool imageModel::queryAllImage(QVector<FeatureImage> &imglist, int channel_id, int size, int last_id, QDate queryDate,QMap<int,int> timeSlot)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return false;
    }
    QString start_time=queryDate.toString("yyyy-MM-dd")+" 00:00:00";
    QString end_time=queryDate.toString("yyyy-MM-dd")+" 23:59:59";
    QString sql="select * from feature_image where create_time >= :start_time AND create_time <= :end_time";
    //遍历QMap里的值
    //若为空跳过
    QStringList list;
    if(!timeSlot.empty()){
        for(int num : timeSlot.keys()){
            //生成占位符
            QString oneRange = QString("(create_time >= :slotF_%1 AND create_time <= :slotE_%1)").arg(num);
            list.append(oneRange);
        }
        sql+=QString(" and (%1)").arg(list.join(" or "));
    }
    if(channel_id>0){
        sql+=" and channel_id=:channel_id";
    }
    if(last_id>0){
        sql+=" and id<:last_id";
    }
    sql += " order by id desc limit :size";
    QSqlQuery query(db);
    query.prepare(sql);
    //对占位符进行填写数据
    if(!timeSlot.empty()){
        for(int num : timeSlot.keys()){
            QString start=queryDate.toString("yyyy-MM-dd")+QString(" %1:00:00").arg(num,2,10,QChar('0'));
            QString end=queryDate.toString("yyyy-MM-dd")+QString(" %1:59:59").arg(num,2,10,QChar('0'));
            query.bindValue(QString(":slotF_%1").arg(num),start);
            query.bindValue(QString(":slotE_%1").arg(num),end);
        }
    }
    query.bindValue(":start_time",start_time);
    query.bindValue(":end_time",end_time);
    if(channel_id>0) query.bindValue(":channel_id",channel_id);
    query.bindValue(":size",size);
    if(last_id > 0) query.bindValue(":last_id", last_id);
    qDebug()<<sql;
    if(query.exec()){
        while(query.next()){
            FeatureImage image;
            image.setImgId(query.value("id").toInt());
            image.setImage_name(query.value("image_name").toString());
            image.setImage_path(query.value("image_path").toString());
            image.setChannelId(query.value("channel_id").toInt());
            image.setOffset_second(query.value("offset_second").toInt());
            image.setCreateTime(query.value("create_time").toString());
            imglist.append(image);
        }
        return true;
    }
    qDebug()<<query.lastError().text();
    return false;
}



int imageModel::getImagesTotal(int channelId, QDate queryDate,QMap<int,int> timeSlot)
{
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return -1;
    }
    QString start_time=queryDate.toString("yyyy-MM-dd")+" 00:00:00";
    QString end_time=queryDate.toString("yyyy-MM-dd")+" 23:59:59";
    QString sql="select count(*) as count from feature_image where create_time >= :start_time AND create_time <= :end_time";
    QStringList list;
    if(!timeSlot.empty()){
        for(int num : timeSlot.keys()){
            //生成占位符
            QString oneRange = QString("(create_time >= :slotF_%1 AND create_time <= :slotE_%1)").arg(num);
            list.append(oneRange);
        }
        sql+=QString(" and (%1)").arg(list.join(" or "));
    }
    if(channelId>0){
        sql+=" and channel_id=:channel_id";
    }
    QSqlQuery query(db);
    query.prepare(sql);
    //对占位符进行填写数据
    if(!timeSlot.empty()){
        for(int num : timeSlot.keys()){
            QString start=queryDate.toString("yyyy-MM-dd")+QString(" %1:00:00").arg(num,2,10,QChar('0'));
            QString end=queryDate.toString("yyyy-MM-dd")+QString(" %1:59:59").arg(num,2,10,QChar('0'));
            query.bindValue(QString(":slotF_%1").arg(num),start);
            query.bindValue(QString(":slotE_%1").arg(num),end);
        }
    }
    query.bindValue(":start_time",start_time);
    query.bindValue(":end_time",end_time);
    if(channelId>0) query.bindValue(":channel_id",channelId);
    int total=0;
    if(query.exec()){
        if(query.next()){
            total=query.value("count").toInt();
        }
    }
    else{
        qDebug()<<"Count error:"<<query.lastError().text();
    }
    return total;
}

QSet<int> imageModel::getHourSet(int channelId, QDate queryDate)
{
    QSet<int> hourSet;
    QSqlDatabase db=DbConnect::getInstence()->getDb();
    if(!db.isOpen()){
        qDebug()<<"datebase open fail";
        return hourSet;
    }
    QString start_time=queryDate.toString("yyyy-MM-dd")+" 00:00:00";
    QString end_time=queryDate.toString("yyyy-MM-dd")+" 23:59:59";
    QString sql = "SELECT HOUR(create_time) FROM feature_image where create_time >= :start_time AND create_time <= :end_time";
    if(channelId>0){
        sql+=" and channel_id=:channelId";
    }
    QSqlQuery query(db);
    query.prepare(sql);
    if(channelId>0) query.bindValue(":channelId", channelId);
    query.bindValue(":start_time",start_time);
    query.bindValue(":end_time",end_time);
    //执行
    if (!query.exec()) {
        //qDebug() << "getHourSet query failed:" << query.lastError().text();
        return hourSet;
    }
    while (query.next()) {
        int hour=query.value(0).toInt();
        hourSet.insert(hour);
    }
    return hourSet;
}
