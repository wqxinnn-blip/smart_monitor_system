#include "recordtask.h"
#include <QCoreApplication>
#include <QDir>

void RecordTask::setOnRecording(bool newOnRecording,int channel_id)
{
    onRecording[channel_id]= newOnRecording;
}

RecordTask::RecordTask(QObject *parent)
    : QObject{parent}
{
    for(int i=0;i<4;i++){
        m_snapFlag[i] = 0;
        m_fullVideoPath[i].clear();
        onRecording[i]=false;
        chIsDetectRec[i]=false;
        frameCount[i]=0;
    }
}

void RecordTask::saveVideo(Mat mat, int channel_id)
{
    //1.添加互斥锁
    QMutexLocker locker(&mutex);
    bool recEnable;
    recEnable = m_recEnable[channel_id];
    if(!recEnable) return;
    //2.根据channelId初始化数据videoWriter 利用数组同时写入数据
    VideoWriter& videoWriter=videoWriters[channel_id];
    int &curFrameCount=frameCount[channel_id];
    QString &curFileName=fileName[channel_id];
    QString &cur_timeFormat=createTime_format[channel_id];

    bool bDetectRec = this->onRecording[channel_id];
    //3.判断 videoWriter是否已打开
    if(!videoWriter.isOpened()){
        // 新增：视频创建瞬间，缓存当前类型
        chIsDetectRec[channel_id] = bDetectRec;
        //3.1.首帧,初始化保存视频的格式
        QString create_time=QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
        cur_timeFormat=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        QString direcPath;
        QString saveDir;
        //direcPath=QCoreApplication::applicationDirPath();//获取当前exe文件所在的绝对路径
        direcPath=SettingInfo::file_path;
        if(bDetectRec){
            curFileName="Record_"+create_time+"_"+QString::number(channel_id+1)+".mp4";
            // 拼接绝对路径保存到数据库            
            saveDir=direcPath+"/motionVideo";
        }
        else{
            curFileName="Record_"+create_time+"_"+QString::number(channel_id+1)+".mp4";
            // 拼接绝对路径保存到数据库
            saveDir=direcPath+"/records";
        }
        QDir dir;
        if(!dir.exists(saveDir)) dir.mkpath(saveDir);
        QString fullAbsPath=saveDir+"/"+curFileName;
        //保存绝对路径，用于结束时取用
        m_fullVideoPath[channel_id]=fullAbsPath;
        //qDebug()<<"recordTask创建文件"<<curFileName;
        qDebug()<<"recordTask文件的绝对路径"<<fullAbsPath;
        //视频格式
        int fourcc=VideoWriter::fourcc('H','2','6','4');
        Size frameSize(mat.cols,mat.rows);
        videoWriter=VideoWriter(fullAbsPath.toStdString(),fourcc,30,frameSize,true);
        if(!videoWriter.isOpened()){
            qDebug()<<"videoWriter创建失败";
            return;
        }
        //写入首帧后触发第一次截图
        if(bDetectRec){
            emit sigCaptureSnapShot(mat,channel_id+1,1,"M",curFileName,0);
            qDebug() <<"通道 "<<channel_id+1<<" 第一张截图";
            // 记录移动侦测信息
            m_snapFlag[channel_id]=1;  // 等待第二张
        }
    }
    //3.2写入数据
    videoWriter.write(mat);
    curFrameCount++;  //记录帧数量 用于根据配置时长保存视频
    if (bDetectRec) {
        //第二张截图
        if (m_snapFlag[channel_id]==1 && curFrameCount>=SNAP2_FRAMES) {
            int offsetMs = curFrameCount/30;
            emit sigCaptureSnapShot(mat,channel_id+1,1,"M",curFileName, offsetMs);
            qDebug() <<"通道 "<<channel_id+1<<" 第二张截图";
            m_snapFlag[channel_id]=2;
        }
        //第三张截图
        else if (m_snapFlag[channel_id]==2 && curFrameCount>=SNAP3_FRAMES) {
            int offsetMs = curFrameCount/ 30;
            emit sigCaptureSnapShot(mat, channel_id+1,1,"M",curFileName,offsetMs);
            qDebug() <<"通道 "<<channel_id+1<<" 第三张截图";
            m_snapFlag[channel_id]=3;
        }
    }
    //3.3尾帧 写入数据  释放资源 重置frameCount
    int maxFrames;
    if(onRecording[channel_id]){
        maxFrames=60*30;
    }
    else{
        maxFrames=SettingInfo::timeInterval*30;
    }
    if(curFrameCount>=maxFrames){
        videoWriter.release();
        qDebug()<<"文件"<<curFileName<<"写入结束";
        //重置视频帧
        curFrameCount=0;
        //将视频保存到数据库
        VideoModel vm;
        Video video;
        video.setVideoName(curFileName); //名称用相对路径
        video.setVideoPath(m_fullVideoPath[channel_id]);    //路径用绝对路径
        video.setChannelId(channel_id+1);
        video.setCreateTime(cur_timeFormat);
        if(bDetectRec){
            video.setVideo_type("M");
        }
        else{
            video.setVideo_type("N");
        }
        bool res=vm.insertVideo(video);
        if(res){
            qDebug()<<curFileName<<"视频插入成功";
        }
        //移动侦测的结束处理
        if(bDetectRec){
            qDebug()<<"执行onRecording";
            //错误问题：因为抢占锁导致切换时卡住 阻塞一通道的录制
            m_recEnable[channel_id]=false;  //重置允许写入标记位
            emit detectRecordFinish(channel_id); //发出结束信号
            this->onRecording[channel_id]=false; //重置移动侦测录制条件
            m_snapFlag[channel_id]=0; //恢复截图逻辑
            qDebug()<<"发送移动侦测录制结束信号";
        }
        m_fullVideoPath[channel_id].clear();
    }
}

//强制刷新
void RecordTask::flush()
{
    QMutexLocker locker(&mutex);
    for(int i=0;i<4;i++){
        if(onRecording[i]) return;
        VideoWriter &writer=videoWriters[i];
        if(writer.isOpened()){
            int frames=frameCount[i];
            qDebug()<<"通道"<<i<<"的帧数"<<frames;
            if(frames>0){
                writer.release();
                qDebug()<<"强制保存通道"<<i<<"的视频，帧数："<<frames;
                //保存到数据库
                VideoModel vm;
                Video v;
                //QString appPath = QCoreApplication::applicationDirPath();
                QString appPath=SettingInfo::file_path;
                QString fullPath;
                //使用首次进入时的状态信息
                bool bDetect = chIsDetectRec[i];
                if(bDetect){
                    fullPath = appPath + "/motionVideo/" + fileName[i];
                    v.setVideo_type("M");
                }
                else{
                    fullPath = appPath + "/records/" + fileName[i];
                    v.setVideo_type("N");
                }
                v.setVideoName(fileName[i]);
                v.setVideoPath(fullPath);
                v.setChannelId(i+1);
                v.setCreateTime(createTime_format[i]);
                vm.insertVideo(v);
                //清零、防止重复保存
                frameCount[i]=0;//清除帧计数
                fileName[i].clear();//清除文件名
                createTime_format[i].clear(); //清除创建时间
                chIsDetectRec[i] = false; //重置缓存标记位
            }
        }
    }
}

void RecordTask::setRecordEnable(int ch, bool enable)
{
    QMutexLocker locker(&mutex);
    if(ch>=0 && ch<4){
        m_recEnable[ch]=enable;
    }
}

void RecordTask::requestFlush()
{
    emit sigNeedFlush();
}
