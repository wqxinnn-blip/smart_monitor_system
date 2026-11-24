#include "monitortask.h"


void MonitorTask::setIsStop(bool newIsStop)
{
    isStop = newIsStop;
}

void MonitorTask::setIsDetect(bool newIsDetect)
{
    isDetect = newIsDetect;
}

MonitorTask::MonitorTask(int cameraDeviceId, int uiChannelIdx, QObject *parent)
    : QObject(parent)
{
    filePath="";
    this->channel_id=uiChannelIdx;
    isStop=false;

    cap.open(cameraDeviceId);
    if (!cap.isOpened())
    {
        qDebug() << "摄像头打开失败:" << cameraDeviceId;
    }
}


MonitorTask::MonitorTask(QString videoPath,int channel_id,QObject *parent)
    : QObject{parent}
{
    //1. 初始化视频路径、通道id和isStop标记位
    this->filePath=videoPath;
    this->channel_id=channel_id;
    this->isStop=false;
    //2. 判断视频路径是否为空
    if(!this->filePath.isEmpty()){
        //打开视频
        qDebug()<<"要打开的视频是"<<videoPath;
        cap.open(videoPath.toStdString());
    }
    else{
        cap.open(channel_id);
    }
    if(!cap.isOpened()){
        qDebug()<<"camera/video open fail";
        return;
    }
    //3. 获取帧信息
    width=cap.get(CAP_PROP_FRAME_WIDTH);
    height=cap.get(CAP_PROP_FRAME_HEIGHT);
    fps=cap.get(CAP_PROP_FPS);
    frameCount=cap.get(CAP_PROP_FRAME_COUNT);
}

void MonitorTask::playVideo()
{
    //1.获取视频帧
    Mat frame;
    while(!this->isStop){
        //读取视频帧
        bool flag=cap.read(frame);
        //判断
        if(flag){
            flip(frame,frame,1);
            resize(frame,frame,Size(640,480));
            //复制视频帧，发送信号
            Mat copyFrame=frame.clone();
            emit sendFrame(copyFrame,this->channel_id);
            if(isDetect){
                Mat grayFrame=frame.clone();
                //转换成灰度图检测
                cvtColor(grayFrame,grayFrame,COLOR_BGR2GRAY);
                emit sigDetectFrame(grayFrame,this->channel_id);
            }
        }
        else{
            //视频/摄像头读取完成
            //重置视频帧
            bool reset_ok=cap.set(CAP_PROP_POS_FRAMES,0); //可循环播放
            if(!reset_ok) break;
        }
        QThread::msleep(33);
    }
    qDebug()<<"视频"<<this->channel_id<<"播放结束，MonitorTask->释放资源";
    cap.release();
}


