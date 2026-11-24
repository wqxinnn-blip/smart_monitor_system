#include "detecttask.h"

DetectTask::DetectTask(QObject *parent)
    : QObject{parent},detect("data/model/yolov8n.onnx")
{
    //1. 加载模型文件
    detect.loadModel();
    //2. 获取模型尺寸
    detect.model_input_size = cv::Size(320, 320);
}

void DetectTask::beginDetect(cv::Mat grayframe,int channelId)
{
    //模型推理 获取检测到的目标数据（主要是坐标和尺寸）
    if(this->frameCount%15==0){
        //获取原始图片大小
        detect.org_Size=grayframe.size();
        //伪三通道
        std::vector<cv::Mat> vec{grayframe,grayframe,grayframe};
        cv::Mat gray3;
        cv::merge(vec,gray3);
        cv::Mat out_frame=detect.letterbox(
            gray3,
            detect.model_input_size.width,
            detect.model_input_size.height,
            cv::Scalar(114, 114, 114));
        yoloOut.clear();
        detect.detect(out_frame, yoloOut); //耗时操作
        emit sendDetectResult(yoloOut,channelId);
        //qDebug()<<"检测返回";
    }
    this->frameCount++;
}
