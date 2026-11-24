#ifndef MOTIONREVIEWWIDGET_H
#define MOTIONREVIEWWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QFile>
#include <QImage>
#include <QPaintEvent>
#include <QFrame>
#include <QEvent>
#include <QSlider>
#include <QList>
#include <QComboBox>
#include <QDateEdit>
#include <QThread>
#include "../service/videotask.h"
#include "../service/imagetask.h"
#include <opencv2/opencv.hpp>
using namespace cv;


class MotionReviewWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout *main_layout;
//左侧布局
    QVBoxLayout *left_layout;
    QDateEdit *dateEdit;
    QComboBox *ch_box;
    QListWidget *listWidget;
    QPushButton *btn_pre,*btn_next;
    //分页按钮
    QPushButton *btn_page[3];
    int Pagination;
    int m_startShow_Page;
    //翻页变量
    bool isPage;
    int curPage;
    int total_page;
    int pageNum; //总页数
    int pageSize; //每页容量
//右侧布局
    QVBoxLayout *right_layout;
    QPushButton *btn_picshoot;
    QList<Video> videoList;
    //播放
    QString absPath=""; //正在播放视频的绝对路径
    VideoTask *m_videotask=nullptr;
    QThread *m_playThread=nullptr;
    bool isPause=false;
    bool isSWitchVideo=false;
    QSlider *slider_play;
    QComboBox *box_speed;
    QPushButton *btn_pause;
    QLabel *lab_frame;
    //保存图片线程
    QThread *m_saveImageThread=nullptr;
    ImageTask *imgTask=nullptr;
public:
    explicit MotionReviewWidget(QWidget *parent = nullptr);
//初始化操作
    void init_leftWidget();
    void init_rightWidget();
    void init_qss();
    void init_connect();
    void start_play();
    QImage videoToQImage(Mat &frame);
protected:
    void hideEvent(QHideEvent *event);
signals:
    void sigStartPlay(QString path); //新建一个发送播放路径的信号
    void sigSaveImg(Mat,int,int,QString,QString,int);
public slots:
    void showFrame(Mat frame,int count);
    void refreshRecordList();
    void updatePage();
};

#endif // MOTIONREVIEWWIDGET_H
