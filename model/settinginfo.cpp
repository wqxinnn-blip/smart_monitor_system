#include "settinginfo.h"
int SettingInfo::timeInterval=0;
QString SettingInfo::file_path=QString();
QList<CameraInfo> SettingInfo::cameras=QList<CameraInfo>();

SettingInfo::SettingInfo() {}

//从文件中读取配置信息
void SettingInfo::loadFromFile()
{
    //1.打开配置文件
    QString configPath="./config.json";
    QFile file(configPath);
    //2.判断是否开启成功 以只读方式/文本（读文件自动换行）
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        //如果配置文件不存在，先初始化摄像头信息
        SettingInfo::initCamera();
        DbConnect::setHost("localhost");
        DbConnect::setPort(3306);
        DbConnect::setDbname("monitorsys");
        DbConnect::setUser("root");
        DbConnect::setPwd("123456");
        return;
    }
    //3.读取文档所有内容并解析为JSON文档
    QByteArray jsonData=file.readAll();
    QJsonParseError jsonError;
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError || !doc.isObject()){
        file.close();
        return;
    }
    //4.解析根对象
    QJsonObject rootObj=doc.object();
    //读取基本配置信息（保存路径、时间间隔、摄像信息） 保存到静态成员变量
    SettingInfo::file_path=rootObj["file_path"].toString();
    SettingInfo::timeInterval=rootObj["timeInterval"].toInt();
    //读取数据库相关配置信息
    DbConnect::setHost(rootObj["host"].toString());
    DbConnect::setPort(rootObj["port"].toInt());
    DbConnect::setDbname(rootObj["dbname"].toString());
    DbConnect::setUser(rootObj["user"].toString());
    DbConnect::setPwd(rootObj["pwd"].toString());
    //现清空摄像头列表
    SettingInfo::cameras.clear();
    QJsonArray camArray=rootObj["cameras"].toArray(); //将键对应的值转为数组
    //遍历数组逐个赋值
    for(int i=0;i<camArray.size();i++){
        QJsonObject camObj=camArray[i].toObject(); //数组中每个元素也是一个对象
        CameraInfo cam;
        cam.setChannelId(camObj["ChannelId"].toInt());
        cam.setCameraName(camObj["cameraName"].toString());
        cam.setChannelName(camObj["channelName"].toString());
        //添加静态列表
        SettingInfo::cameras.append(cam);
    }
    //关闭文件
    file.close();
}

void SettingInfo::initCamera()
{
    //首先要在工程的配置文件中，加上multimedia
    //1.获取本机可用的摄像头设备列表（返回的QList<QCameraDevice>)
    QList<QCameraDevice> cameraList=QMediaDevices::videoInputs();
    //2.判断是否有可用的摄像头 打印日志
    if(cameraList.isEmpty()){
        qDebug()<<"本机未检测到任何摄像头设备";
        return;
    }
    //3.遍历所有摄像头，获取信息
    qDebug()<<"本机检测到"<<cameraList.size()<<"个摄像头";
    int i=0;
    for(i=0;i<cameraList.size();i++){
        QCameraDevice camera=cameraList.at(i);
        QString cameraName=camera.description(); //摄像头名称
        QString cameraId=camera.id(); //摄像头唯一标识符
        //bool isDefault=camera.isDefault(); //是否是默认摄像头
        if(cameraName.contains("IR")) continue; //过滤红外

        CameraInfo camInfo;
        camInfo.setChannelId(i+1);
        camInfo.setCameraName(cameraName);
        camInfo.setChannelName(cameraName);
        SettingInfo::cameras.append(camInfo);
    }
    // 填充剩余通道
    for(i--;i<4;i++){
        CameraInfo camInfo;
        camInfo.setChannelId(i+1);
        camInfo.setCameraName("virtual camera");
        camInfo.setChannelName("virtual camera");
        SettingInfo::cameras.append(camInfo);
    }
}

void SettingInfo::detectCamera(){
    QList<QCameraDevice> cameraList=QMediaDevices::videoInputs();
    qDebug()<<"本机检测到"<<cameraList.size()<<"个摄像头";
    //创建一个新的列表 存放新检测到的摄像头列表
    QList<CameraInfo> newCameras;
    int realCount = 0;
    for(int i=0;i<cameraList.size();i++){
        QCameraDevice camera=cameraList.at(i);
        QString cameraName=camera.description(); //摄像头名称
        if(cameraName.contains("IR")) continue; //过滤红外
        CameraInfo camInfo;
        camInfo.setChannelId(realCount+1);
        camInfo.setCameraName(cameraName);
        camInfo.setChannelName(cameraName);
        newCameras.append(camInfo);
        realCount++;
        if(realCount>=4) break; //只需要四路显示
    }
    for(int i=realCount;i<4;i++){
        CameraInfo camInfo;
        camInfo.setChannelId(i+1);
        camInfo.setCameraName("virtual camera");
        camInfo.setChannelName("virtual camera");
        newCameras.append(camInfo);
    }

    bool isChanged = (newCameras.size() != SettingInfo::cameras.size());
    if (!isChanged) { // 长度相等的情况下，再逐个比对名字
        for (int i = 0; i < SettingInfo::cameras.size(); ++i) {
            if (SettingInfo::cameras[i].getCameraName() != newCameras[i].getCameraName()) {
                isChanged = true;
                break;
            }
        }
    }
    if (isChanged) {
        qDebug() << "摄像头发生变更，更新配置信息";
        SettingInfo::cameras = newCameras;
    } else {
        qDebug() << "摄像头未发生变更，无需更新";
    }
}

//保存配置信息到文件
void SettingInfo::saveToFile()
{
    //1.打开配置文件
    QString configPath="./config.json";
    QFile file(configPath);
    //2.判断是否开启成功
    if(!file.open(QFile::WriteOnly | QIODevice::Text)){
        qDebug()<<"file.open(QFile::WriteOnly | QIODevice::Text) error";
        return;
    }
    //3.写入基础配置 定义一个QJsonObject 把文件路径和时间间隔保存进去
    QJsonObject rootObj;
    rootObj["file_path"]=SettingInfo::file_path;
    rootObj["timeInterval"]=SettingInfo::timeInterval;
    //4.写入摄像头列表
    //思路：定义一个Qt提供的QjsonArray，遍历cameras（QList）一个个赋值
    QJsonArray camArray; //创建空的json数组 [ {...}, {...}, {...} ]
    for(CameraInfo &cam : SettingInfo::cameras){
        QJsonObject camObj; //为每个摄像头创建 JSON 对象
        //添加键值对
        camObj["cameraName"]=cam.getCameraName();
        camObj["channelName"]=cam.getChannelName();
        camObj["ChannelId"]=cam.getChannelId();
        camArray.append(camObj);
    }
    //5.写入文件、格式化写入
    //写入数据库配置信息
    rootObj["cameras"]=camArray;
    rootObj["dbname"]=DbConnect::getDbname();
    rootObj["host"]=DbConnect::getHost();
    rootObj["port"]=DbConnect::getPort();
    rootObj["pwd"]=DbConnect::getPwd();
    rootObj["user"]=DbConnect::getUser();
    //写入文件、格式化写入
    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented)); //格式化输出，可读性强
    file.close();
}

