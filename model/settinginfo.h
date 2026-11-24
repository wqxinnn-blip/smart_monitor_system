#ifndef SETTINGINFO_H
#define SETTINGINFO_H

#include <QList>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QFile>
#include "camerainfo.h"
#include "../util/dbconnect.h"

//配置信息类
class SettingInfo
{

public:
    SettingInfo();
    static int timeInterval; //录制时间间隔
    static QString file_path; //保存文件路径
    static QList<CameraInfo> cameras; //摄像头信息

    static void saveToFile();
    static void loadFromFile();
    static void initCamera();
    static void detectCamera();
};

#endif // SETTINGINFO_H
