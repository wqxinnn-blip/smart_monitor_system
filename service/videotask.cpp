#include "videotask.h"

void VideoTask::setNewPos(int newNewPos)
{
    dragFlag=true;
    newPos = newNewPos;
}

QStringList VideoTask::searchDefaultVideo(QString dirPath,int num)
{
    QStringList list;
    QDir dir(dirPath);
    dir.setNameFilters(QStringList() <<"*.mp4");
    dir.setFilter(QDir::Files);

    QFileInfoList fileList=dir.entryInfoList();

    for(int i=0;i<num;i++){
        QFileInfo info=fileList.at(i);
        QString path=info.absoluteFilePath();
        list.append(path);
    }
    return list;
}

void VideoTask::getSnapFrame()
{
    if(curFrame.empty()){
        qDebug()<<"当前帧为空";
        return;
    }
    //新增逻辑
    int curMs=static_cast<int>(cap.get(CAP_PROP_POS_MSEC));
    Mat frameCopy = curFrame.clone();
    emit sigSendShotFrameAndTime(curFrame,curMs); //发送帧和毫秒数
    qDebug()<<"已发送要截图的帧";
}

VideoTask::VideoTask(QObject *parent)
    : QObject{parent}
{}

void VideoTask::pausePlay(bool pause)
{
    pauseFlag=pause;
}

void VideoTask::setSpeed(double newSpeed)
{
    speed = newSpeed;
}

void VideoTask::stopPlay()
{
    playFlag = false;
    pauseFlag = false;
    imgFrameCount = 0; //重置计数 兼容图片管理
}

void VideoTask::startPlay(QString absPath)
{
    cap.open(absPath.toStdString());
    if(!cap.isOpened()){
        qDebug()<<"video open fail:"<<absPath;
        emit finishVideo();
        return;
    }
    width=cap.get(CAP_PROP_FRAME_WIDTH);
    height=cap.get(CAP_PROP_FRAME_HEIGHT);
    fps=cap.get(CAP_PROP_FPS);
    total=cap.get(CAP_PROP_FRAME_COUNT); //总帧数
    //发送总帧数 设置滑动条的范围
    emit sigTotalFrames(total);
    playFlag=true; //正在读视频
    pauseFlag=false;//重置暂停标志
    qDebug() << "total frames:" << total;
    playFrame();
}

void VideoTask::playFrame()
{
    Mat frame;
    int count=imgFrameCount;
    qDebug()<<"读取视频帧";
    while(playFlag){
        while(pauseFlag){
            QThread::msleep(20);
        }
        //是否拖动进度条
        if(dragFlag){
            cap.set(CAP_PROP_POS_FRAMES,this->newPos);
            count=this->newPos;
            dragFlag=false;
        }
        cap>>frame;
        curFrame=frame.clone();
        if(frame.empty()) break;
        count++;
        qDebug()<<count;
        emit sigVideoFrame(frame,count);
        QThread::msleep(33/this->speed);
    }
    cap.release();
    qDebug()<<"视频播放完成！";
    emit finishVideo();
}

int VideoTask::videoPlayBack(QList<Video> &list, int channelId, QDate queryDate, int limit, int offset,QString type)
{
    bool flag=videomodel.queryVideoList(channelId,queryDate,limit,offset,list,type);
    if(flag){
        qDebug()<<"查询到"<<videomodel.getVideosTotal(channelId,queryDate,type)<<"条记录";
        return videomodel.getVideosTotal(channelId,queryDate,type);
    }
    return -1;
}

void VideoTask::seekToMsAndPlay(QString videoAbsPath,int ms)
{
    QMutexLocker locker(&mutex);
    if (cap.isOpened() && playFlag) {
        playFlag = false;
    }
    if (!cap.isOpened()) {
        cap.open(videoAbsPath.toStdString());
        if (!cap.isOpened()) {
            emit finishVideo();
            return;
        }
    }
    //定位
    cap.set(CAP_PROP_POS_MSEC, ms);
    int total=cap.get(CAP_PROP_FRAME_COUNT);
    imgFrameCount=(ms/1000)*30;
    emit sigTotalToImg(total,imgFrameCount);
    //启动新循环
    playFlag = true;
    pauseFlag = false;
    playFrame();
}



