#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QTimer>
#include <QThread>
#include "monitorwidget.h"
#include "reviewwidget.h"
#include "imagewidget.h"
#include "logwidget.h"
#include "motionreviewwidget.h"
#include "../service/monitortask.h"


class mainWidget : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *main_layout;   //主布局
    QHBoxLayout *topH_layout;    //顶层布局
    QHBoxLayout *top_layout;  //顶层子布局
    QStackedLayout *buttom_layout;
    QWidget *btnContainer;
    MonitorWidget *m_widget; //监控窗口对象
    ReviewWidget *review_widget;
    ImageWidget *image_widget;
    MotionReviewWidget *motionReview_widget;
    LogWidget *log_widget;

    QLabel *lab_title;          //平台名称
    //初始化五个按钮 监控、监控记录、视频回看、回放、设置
    QPushButton *btn_monitor,*btn_motion_detect,*btn_video_replay,*btn_img,*btn_setting,*btn_login,*btn_exit,*btn_log;
    QLabel *cur_time; //当前时间
    QLabel *lab_text; //登录人昵称

    bool isLogin=false;

public:
    explicit mainWidget(QWidget *parent = nullptr);
    void init_top_widget();
    void init_qss();
    void init_connect();
    void showUserName(QString username);

    void showCamera();
    //按钮禁用和开启
    void btn_Off();
    void btn_On();


    bool getIsLogin() const;
    void setIsLogin(bool newIsLogin);

    void closeMonitor();

protected:
    void closeEvent(QCloseEvent *event);

signals:

public slots:

};

#endif // MAINWIDGET_H
