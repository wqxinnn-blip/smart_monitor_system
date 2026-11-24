#ifndef MONITORTASK_H
#define MONITORTASK_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <vector>
#include <QThread>
#include <QStringList>
#include "../yolo/Detect.h"
#include "../model/videomodel.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class MonitorTask : public QObject
{
    Q_OBJECT
private:
    int width;
    int height;
    int fps;
    int frameCount;

    VideoCapture cap;
    QString filePath;
    int channel_id;
    bool isStop; //是否发送视频帧
    bool isDetect=false; 
public:
    explicit MonitorTask(QString videoPath=nullptr,int channel_id=0,QObject *parent = nullptr);
    MonitorTask(int cameraDeviceId, int uiChannelIdx, QObject *parent = nullptr);
    void setIsStop(bool newIsStop);
    void setIsDetect(bool newIsDetect);
signals:
    void sendFrame(Mat frame,int channel_id);
    void sigDetectFrame(Mat grayFrame,int channel_id);
public slots:
    void playVideo();
};

#endif // MONITORTASK_H
