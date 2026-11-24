#include "mainwidget.h"
#include "../util/winmanager.h"

mainWidget::mainWidget(QWidget *parent) : QWidget(parent)
{
    //设置窗口标题
    this->setWindowTitle("监控系统");
    this->resize(1200,700);
    //设置主布局 垂直布局  加入窗口
    main_layout=new QVBoxLayout;
    this->setLayout(main_layout);
    //顶层采用水平布局
    topH_layout=new QHBoxLayout;
    topH_layout->setSpacing(30);
    //下方窗口采用堆叠布局
    buttom_layout=new QStackedLayout;
    buttom_layout->setStackingMode(QStackedLayout::StackOne);//单一页面同时可见
    //监控窗口添加
    m_widget=new MonitorWidget;
    m_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //回放窗口添加
    review_widget=new ReviewWidget;
    review_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //图片管理窗口的添加
    image_widget=new ImageWidget;
    image_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //移动侦测窗口
    motionReview_widget=new MotionReviewWidget;
    motionReview_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //日志查看窗口
    log_widget=new LogWidget;
    log_widget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //添加到底部布局
    buttom_layout->addWidget(m_widget); //0
    buttom_layout->addWidget(review_widget); //1
    buttom_layout->addWidget(image_widget); //2
    buttom_layout->addWidget(motionReview_widget); //3
    buttom_layout->addWidget(log_widget);//4
    //将两个布局加入主布局
    buttom_layout->setCurrentIndex(0); //默认显示监控窗口
    main_layout->addLayout(topH_layout,1);
    main_layout->addLayout(buttom_layout,8);
    //初始化
    this->init_top_widget();
    this->init_qss();
    this->init_connect();
    this->btn_Off();//默认禁用交互
}

void mainWidget::init_top_widget()
{
    //创建子横向布局
    top_layout=new QHBoxLayout;
    //将顶部布局放入容器 进行统一管理  用于处理交互
    btnContainer=new QWidget;
    btnContainer->setLayout(top_layout);
    //初始化平台名称
    lab_title=new QLabel("安防视频监控管理平台");
    lab_title->setObjectName("Title");
    top_layout->addWidget(lab_title);
    //初始化五个按钮 监控、监控记录、视频回看、回放、设置
    //1.初始化监控按钮
    btn_monitor=new QPushButton("监控");
    btn_monitor->setIcon(QIcon(":/image/jiankong.png"));
    top_layout->addWidget(btn_monitor);
    //2.初始化监控记录按钮
    btn_motion_detect=new QPushButton("移动侦测回看");
    btn_motion_detect->setIcon(QIcon(":/image/monitor_record.png"));
    top_layout->addWidget(btn_motion_detect);
    //3.初始化视频回看按钮
    btn_video_replay=new QPushButton("普通视频回看");
    btn_video_replay->setIcon(QIcon(":/image/video_review.png"));
    top_layout->addWidget(btn_video_replay);
    //4.初始化回放按钮
    btn_img=new QPushButton("图片管理");
    btn_img->setIcon(QIcon(":/image/review.png"));
    top_layout->addWidget(btn_img);
    //日志查看
    btn_log=new QPushButton("日志查看");
    btn_log->setIcon(QIcon(":/image/log.png"));
    top_layout->addWidget(btn_log);
    //5.初始化设置按钮
    btn_setting=new QPushButton("设置");
    btn_setting->setIcon(QIcon(":/image/setting.png"));
    top_layout->addWidget(btn_setting);
    //6.添加退出按钮
    btn_exit=new QPushButton("退出");
    top_layout->addWidget(btn_exit);
    // -----------------------------------------------------------------------
    //7.初始化登录
    //7.1 图标在上、文字在下，采用垂直布局 嵌套在按钮里 布局里添加一个QLbale和图标
    btn_login=new QPushButton;
    btn_login->setObjectName("btnLogin");
    QVBoxLayout *login_layout=new QVBoxLayout(btn_login);
    login_layout->setAlignment(Qt::AlignCenter); //让布局中的控件居中排列
    btn_login->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed); //固定上下左右的伸缩
    login_layout->setSpacing(2); //设置每个控件间的边距
    login_layout->setContentsMargins(0,0,0,0); //内边距为0
    //7.2 添加图标
    QLabel *lab_icon=new QLabel;
    lab_icon->setPixmap(QPixmap(":/image/user.png").scaled(60,60));
    lab_icon->setObjectName("iconLogin");
    lab_text=new QLabel("未登录");
    lab_text->setAlignment(Qt::AlignCenter);
    lab_text->setStyleSheet("background-color:DodgerBlue;color:white;font-size:20px;");
    //7.3 加入布局
    login_layout->addWidget(lab_icon);
    login_layout->addWidget(lab_text);
    //8. 加入总布局
    topH_layout->addWidget(btnContainer);
    topH_layout->addWidget(btn_login);
}

void mainWidget::init_qss()
{
    QFile file(":/qss/demo.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void mainWidget::init_connect()
{
    connect(btn_monitor,&QPushButton::clicked,this,[this](){
        this->buttom_layout->setCurrentIndex(0);
    });
    connect(btn_video_replay,&QPushButton::clicked,this,[this](){
        this->buttom_layout->setCurrentIndex(1);
        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"查看回放");
    });
    connect(btn_img,&QPushButton::clicked,this,[this](){
        this->buttom_layout->setCurrentIndex(2);
        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"查看图片管理");
    });
    connect(btn_motion_detect,&QPushButton::clicked,this,[this](){
        this->buttom_layout->setCurrentIndex(3);
        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"查看移动侦测回放");
    });
    connect(btn_setting,&QPushButton::clicked,this,[](){
        WinManager::getInstence()->setting_widget.show();
        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"修改设置");
    });
    connect(btn_exit,&QPushButton::clicked,this,[this](){
        //日志操作
        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"退出登录");
        OperLogService::getInstence()->resetInfo();
        GlobalUser::curAdmin_id=0;
        GlobalUser::curAdmin_name="";
        //重置登录信息和登录状态
        this->lab_text->setText("未登录");
        this->isLogin=false;
        //释放解码和录制线程
        if(m_widget->getCheckDetect()){
            m_widget->stop_detectRecord(); //移动侦测下异常退出
        }
        else{
            m_widget->stop_record(); //普通状态下退出
        }
        //取消移动侦测
        m_widget->cancelDetect(); //停止检测
        qDebug()<<"用户已退出";
        this->buttom_layout->setCurrentIndex(0); //切回监控界面
        this->btn_Off();
    });
    connect(btn_log,&QPushButton::clicked,this,[this](){
        this->buttom_layout->setCurrentIndex(4);
    });
    connect(btn_login,&QPushButton::clicked,this,[this](){
        this->hide();
        WinManager::getInstence()->login_widget.show();
    });
}

void mainWidget::showUserName(QString username)
{
    this->show();
    lab_text->setText(username);
    this->m_widget->start_record(); //开启录制
    this->setIsLogin(true); //设置登录状态
    this->btn_On(); //开启按钮交互
}

void mainWidget::showCamera()
{
    this->m_widget->start_player();
}

bool mainWidget::getIsLogin() const
{
    return isLogin;
}

void mainWidget::setIsLogin(bool newIsLogin)
{
    isLogin = newIsLogin;
}



void mainWidget::btn_Off()
{
    this->btnContainer->setEnabled(false);
    this->setIsLogin(false);
    m_widget->detectDisabled(false);//禁用移动侦测按钮
}

void mainWidget::btn_On()
{
    this->btnContainer->setEnabled(true);
    m_widget->detectDisabled(true);
}

void mainWidget::closeEvent(QCloseEvent *event)
{
    qDebug() << "mainWidget closeEvent 开始";
    if (m_widget) {
        m_widget->stop_record();
        m_widget->deleteLater();
        m_widget = nullptr;
    }
    event->accept();
    qDebug() << "mainWidget closeEvent 完成";
}



