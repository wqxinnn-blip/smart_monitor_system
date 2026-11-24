#include "file.h"
int File::num[4]={1,1,1,1};

File::File() {}

QString File::getShotFileName(int chId, int type,QString ch,QString videoName)
{
    QFileInfo info(videoName);
    QString baseName=info.completeBaseName(); //获取不带后缀的文件名;
    //根据下划线进行分割
    QStringList parts=baseName.split("_");

    //时间要和原视频路径一致
    QString date=parts[1];
    QString time=parts[2];
    QString ms = QString::number(QDateTime::currentDateTime().time().msec());
    if(type==0){
        //回放截图
        return QString("screenshot_ch%1_%2_%3_%4_%5.jpg").arg(chId).arg(date).arg(time).arg(ch).arg(ms);
    }
    else if(type==1){
        if(num[chId-1]>3){
            num[chId-1]=1;
        }
        //实时侦测截图
        //qDebug()<<"通道 "<<chId<<" 的第"<< num[chId-1]<<"次截图";
        return QString("exception_ch%1_%2_%3_%4_%5.jpg").arg(chId).arg(date).arg(time).arg(num[chId-1]++).arg(ms);
    }
    else{
        qDebug()<<"无法生成路径";
        return "";
    }
}

QString File::getFullPath()
{
    const QString root=SettingInfo::file_path;
    QString dir=root+"/feature_image/";
    //没有路径创建
    QDir().mkpath(dir);
    return dir;
}

QString File::getFrameType(QString imgName)
{
    //根据下划线进行分割
    QStringList parts=imgName.split("_");
    //先判断是否为空 再取最后一段内容 判断类型
    if(parts.isEmpty()){
        return "";
    }
    if(parts[4].trimmed()=="M" || parts[4].trimmed()=="N"){
        return parts[4].trimmed();
    }
    else{
        qDebug()<<"自动截取的异常图片";
        return "";
    }
}

QString File::getVideoPathByFrameType(QString imgPath)
{
    QFileInfo info(imgPath);
    QString tmp_dir=info.absolutePath(); //获取目录路径
    QString dirpath = QFileInfo(tmp_dir).absolutePath();//获取该文件的上上级目录
    QString name=info.baseName(); //获取图片的文件名(不带后缀）
    QStringList parts=name.split("_");
    //取出关键数据
    int channelId=parts[1].mid(2).toInt();
    QString timeFirst=parts[2];
    QString timeSecond=parts[3];

    QString fullPath="Record_"+timeFirst+"_"+timeSecond+"_"+QString::number(channelId)+".mp4";
    //判断
    QString ch=getFrameType(name);
    if(ch=="" || ch=="M"){
        return dirpath+"/motionVideo/"+fullPath;
    }
    else{
        return dirpath+"/records/"+fullPath;
    }
}


