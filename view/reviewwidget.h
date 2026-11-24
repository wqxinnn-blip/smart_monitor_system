#ifndef REVIEWWIDGET_H
#define REVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QImage>
#include <QSlider>
#include <QList>
#include <QComboBox>
#include <QDateEdit>
#include <QThread>
#include "../service/videotask.h"
#include "../service/imagetask.h"
#include <opencv2/opencv.hpp>
using namespace cv;


class ReviewWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *main_layout;
//左侧布局变量
    QVBoxLayout *left_layout;
    QDateEdit *dateEdit;
    QComboBox *ch_box;
    QListWidget *listWidget;
    //分页
    QPushButton *btn_pre,*btn_next;
    QPushButton *btn_page[3];
    int Pagination; //每次限制显示三页
    int m_startShow_Page;
    //翻页变量
    bool isPage;
    int curPage;
    int total_page;//总记录数
    int pageNum; //总页数
    int pageSize; //页容量
    QList<Video> videoList;
//右侧布局变量
    QVBoxLayout *right_layout;
    QPushButton *btn_picshoot;
    QSlider *slider_play;
    QComboBox *box_speed;
    QPushButton *btn_pause;
    QLabel *lab_frame;
    //播放
    QString absPath=""; //正在播放视频的绝对路径，用于进行比对 找到对应信息发送给截图线程  拼接路径
    VideoTask *m_videotask=nullptr;
    QThread *m_playThread=nullptr;
    bool isPause=false;
    bool isSWitchVideo=false; //切换视频标记位
    //保存图片线程
    QThread *m_saveImageThread=nullptr;
    ImageTask *imgTask=nullptr;
public:
    explicit ReviewWidget(QWidget *parent = nullptr);
    //初始化操作
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
    void start_play();
    //转换成图片
    QImage videoToQImage(Mat &frame);
protected:
    void hideEvent(QHideEvent *event);
signals:
    void sigStartPlay(QString path); //新建一个发送播放路径的信号
    //保存帧--通道号--类型--标记--视频名--偏移秒数
    void sigSaveImg(Mat,int,int,QString,QString,int);
public slots:
    void showFrame(Mat frame,int count);
    void refreshRecordList();
    void updatePage();
};

#endif // REVIEWWIDGET_H
