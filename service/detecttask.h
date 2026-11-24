#ifndef DETECTTASK_H
#define DETECTTASK_H

#include <QObject>
#include <QDebug>
#include "../yolo/Detect.h"

class DetectTask : public QObject
{
    Q_OBJECT
private:
    Detect detect;
    std::vector<YOLO_OUT> yoloOut;
    int frameCount; //帧计数器 用于抽帧
public:
    explicit DetectTask(QObject *parent = nullptr);

signals:
    void sendDetectResult(std::vector<YOLO_OUT>,int channelId);
public slots:
    void beginDetect(cv::Mat grayframe,int channelId);
};

#endif // DETECTTASK_H
