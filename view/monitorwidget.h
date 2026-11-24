#ifndef MONITORWIDGET_H
#define MONITORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QImage>
#include <QCheckBox>
#include <QPaintEvent>
#include <QFrame>
#include <QVector>
#include <QEvent>
#include <QMutex>
#include <QMutexLocker>
#include <QStringList>
#include <QDateEdit>
#include <QThread>
#include "../service/monitortask.h"
#include "../service/recordtask.h"
#include "../service/detecttask.h"
#include "../service/imagetask.h"
#include "../yolo/Detect.h"

class MonitorWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *main_layout;
    QWidget *leftWidget,*rightWidget;
    QLabel *lab_device;
    QListWidget *list_widget;
    QLabel *lab_w1,*lab_multi[4],*lab_replay;
    QPushButton *btn_w1,*btn_w4;
    QStackedLayout *stack_layout;
    QCheckBox *check_detect;

    //线程和任务
    //摄像
    QThread *thread_m=nullptr,*thread_mul[4]={nullptr};
    MonitorTask *mt,*mt1[4];
    //录制
    QThread *thread_r=nullptr;
    RecordTask *rt=nullptr;
    //监测
    DetectTask *dTask[4]={nullptr};
    QThread *dThread[4]={nullptr};
    //截图
    ImageTask *imgTask;
    QThread *imgThread;

    QStringList paths; //接收要查询的视频路径
    int num_channels; //记录要开启几通道
    bool isCreateMul4=false;
    QImage img;

    //标记位
    int currentPreviewChannel=0;
    QVector<int> UiIdx;
    int limitFrameCount=5;//连续出现五次移动物体 触发录制（并截取首张图片）
    int detectFrameCount[4]={0};//连续移动帧次数
    int limitEmptyCount=9;
    int emptyDetectCount[4]={0};
    bool isRecording[4]={false}; //是否正在录制
    //自动检测保存图片
    int autoDetect=1;
    //定义一个QMap
    QMap<int,std::vector<YOLO_OUT>> m_detectPos;
    Detect m_draw;
public:
    explicit MonitorWidget(QWidget *parent = nullptr);
    ~MonitorWidget();
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
    QImage MatToQImage(Mat frame);
    void showFrame(Mat frame,int channelId);
    void paintEvent(QPaintEvent *event);
    void switch_to_channel1();

    void start_player();
    void start_record();
    void stop_record();
    void stop_detectRecord();
    void receivePaths();
    void cancelDetect();

    int getNum_channels() const;
    void detectDisabled(bool checked);
    bool getCheckDetect();
signals:
    void saveSnapshot(Mat frame,int channel_id,int type,QString ch,QString,int);
public slots:
    void onMotionDetect(bool checked);

};

#endif // MONITORWIDGET_H
