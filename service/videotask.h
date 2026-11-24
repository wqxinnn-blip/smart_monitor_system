#ifndef VIDEOTASK_H
#define VIDEOTASK_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QString>
#include <QThread>
#include <QDir>
#include <QMutex>
#include <QMutexLocker>
#include "../model/videomodel.h"
using namespace cv;


class VideoTask : public QObject
{
    Q_OBJECT
private:
    VideoCapture cap;
    int width;
    int height;
    double fps;
    int total=0;
    int newPos;
    double speed=1.0;
    Mat curFrame; //保存当前读取到的帧  为截图做准备
    int imgFrameCount=0;

    bool playFlag=false;
    bool pauseFlag=false;
    bool dragFlag=false;
    QMutex mutex;
public:
    explicit VideoTask(QObject *parent = nullptr);
    int videoPlayBack(QList<Video> &list,int channelId,QDate queryDate,int limit,int offset,QString type);
    VideoModel videomodel;

    void playFrame();
    void startPlay(QString absPath);
    void pausePlay(bool pause);
    void setSpeed(double newSpeed);
    int getTotal() const;

    void stopPlay();

    void setNewPos(int newNewPos);
    QStringList searchDefaultVideo(QString paths,int num);

signals:
    void sigVideoFrame(Mat,int);
    void finishVideo();
    void sigTotalFrames(int);
    void sigTotalToImg(int,int);
    void sigSendShotFrame(Mat);
    void sigSendShotFrameAndTime(Mat,int);
public slots:
    void getSnapFrame();
    //++
    void seekToMsAndPlay(QString,int ms);
    //void seekToMs(int ms);
};

#endif // VIDEOTASK_H
