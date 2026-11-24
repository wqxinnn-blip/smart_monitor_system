#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollArea>
#include <QFile>
#include <QList>
#include <QVector>
#include <QComboBox>
#include <QMap>
#include <QCheckBox>
#include <QDialog>
#include <QDateEdit>
#include <QThread>
#include <algorithm>
#include "../service/imagetask.h"
#include "../service/videotask.h"
#include "../util/thumblabel.h"
#include <opencv2/opencv.hpp>
#include "../util/file.h"
using namespace cv;

class ImageWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *main_layout;
//1. 左侧布局成员
    QWidget *left_widget;
    QVBoxLayout *left_layout;
    QDateEdit *dateEdit; //日期选择
    QComboBox *ch_box; //通道下拉框
    //设置时间段
    QScrollArea *m_scrollArea;//提供滚动视图的控件，当内容区域超过可视区域时，自动显示滚动条
    QVBoxLayout *time_layout;
    QWidget *m_timeWidget;
    QList<QCheckBox *> timeCheckList;
    QMap<int,int> timeSlot;
//2. 右侧布局变量
    QWidget *right_widget;
    QVBoxLayout *right_layout;
    QGridLayout *grid_layout;
    //新建的QLabel类型
    ThumbLabel *lab_img[3][3];
    ThumbLabel *lab_path[3][3];
    //新建一个遮盖布局  用于承载放大的图片 以及一些交互按键
    QWidget *m_maskWidget;
    ThumbLabel *m_magnify_lab; //存放放大后的图片
    ThumbLabel *lab; //获取当前点击的Label控件
    QPushButton *btn_preImg,*btn_nextImg; //上一张 / 下一张图片
    QPushButton *btn_scanVideo,*btn_back; //查看视频 / 返回
    //分页按钮
    QPushButton *btn_page[3];
    QPushButton *btn_pre,*btn_next;
    int Pagination; //每次显示三个页
    int m_startShow_Page; //当前界面的起始页
    QVector<FeatureImage> featureImg; //存储本页的所有特征图片信息
    QVector<int> record_prePage;//记录前一页的last_id
    int last_id=0;
    //放大图片后的相关成员变量
    QVector<FeatureImage> m_copyImage;
    QVector<int> record_prePage1;//记录前一页的last_id
    QPushButton *btn_pause,*btn_return;
    QSlider *slider;
    int m_scanIndex=0;
    int m_nowPage=0;
    int last_id1; 
    //翻页变量
    bool isPage; //是否正在翻页
    int curPage; //当前页
    int total_page; //总记录数
    int pageNum; //总页数
    int pageSize; //每页容量
    //查看视频的弹窗
    static QDialog *g_videoDialog;
    static QLabel *g_frameLabel;
    bool isPause=false;
    //线程 、任务 、标记位
    QThread *videoThread=nullptr;
    VideoTask *videoTask=nullptr;
    bool isPlaying=false;
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
    void updatePage();
    //初始化时间段
    void init_timeSelect();
    void init_maskWidget();
    void init_videoWidget();
    //当返回时，重置放大预览的资源
    void releaseSoruce();
    QImage MatToImage(Mat frame);
signals:
    void sigSeekAndPlay(QString,int ms);
    void sigStopPlay();
public slots:
    void refreshImg();
    void onCheckTime(bool); //存储勾选的时间段范围
    void labOperation(); //点击图片时触发
    void updateFrame(Mat,int);
};

#endif // IMAGEWIDGET_H
