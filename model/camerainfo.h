#ifndef CAMERAINFO_H
#define CAMERAINFO_H


#include <QString>

//摄像头通道类
class CameraInfo
{
private:
    int channelId;
    QString channelName; //通道名
    QString cameraName; //摄像头名
public:
    CameraInfo();


    int getChannelId() const;
    void setChannelId(int newChannelId);

    QString getChannelName() const;
    void setChannelName(const QString &newChannelName);

    QString getCameraName() const;
    void setCameraName(const QString &newCameraName);
};

#endif // CAMERAINFO_H
