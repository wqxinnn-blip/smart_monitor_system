#ifndef RECORDTASK_H
#define RECORDTASK_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDateTime>
#include "../model/videomodel.h"
#include "../model/settinginfo.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class RecordTask : public QObject
{
    Q_OBJECT
private:
    QMutex mutex;
    //文件相关成员
    VideoWriter videoWriters[4];
    int frameCount[4];
    QString fileName[4];
    QString createTime_format[4];

    //标记位
    bool m_recEnable[4];//是否能写入
    bool onRecording[4]={false}; //是否正在移动录制
    bool chIsDetectRec[4] = {false}; //存放首次录制时的标记位 用于退出时进行释放处理

    //记录视频路径和起始毫秒
    QString m_fullVideoPath[4]; //保存每个通道的文件路径  绝对路径

    //标记
    int m_snapFlag[4];  // 0-未截图 1-已截取第一张 2-已截取第二张 3-已截取第三张
    int m_TriggerFrameCount=0;// 触发后对帧计数  用于触发第二三张图片
    //每隔一秒抓拍一次
    int SNAP2_FRAMES=30; //一秒抓拍一次
    int SNAP3_FRAMES=60;
public:
    explicit RecordTask(QObject *parent = nullptr);

    void setRecordEnable(int ch, bool enable);
    void setOnRecording(bool newOnRecording,int channel_id);
    bool isOnRecording() const; // 读取当前状态
    //触发强制刷新
    void requestFlush();
    void flush();
signals:
    void detectRecordFinish(int channelId);
    void sigNeedFlush();
    void sigCaptureSnapShot(Mat frame,int channel_id,int type,QString ch,QString videoName,int offsetSec);
public slots:
    void saveVideo(Mat mat,int channel_id);
};

#endif // RECORDTASK_H
