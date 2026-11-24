#include "MotionReviewWidget.h"

//默认未选中
const QString normalStyle = R"(
QPushButton{
    padding:2px 5px;
    min-width:30px;
    margin:1px;
    border:1px solid #aaa;
}
)";
//选中高亮
const QString selectStyle = R"(
QPushButton{
    padding:2px 5px;
    min-width:30px;
    margin:1px;
    background:#2774d9;
    color:white;
    border:1px solid #2774d9;
}
)";

MotionReviewWidget::MotionReviewWidget(QWidget *parent)
    : QWidget{parent}
{
    //this->resize(1000,700); //测试用
    this->isPage=false;//翻页操作标记位
    //初始化分页变量
    curPage=1;
    pageSize=10;
    pageNum=1;
    Pagination=3;
    m_startShow_Page=1;
    //主布局
    main_layout=new QHBoxLayout;
    this->setLayout(main_layout);
    //初始化
    init_leftWidget();
    init_rightWidget();
    init_qss();
    start_play();
    init_connect();
    refreshRecordList();
}

void MotionReviewWidget::init_leftWidget()
{
    //整体采用垂直布局
    left_layout=new QVBoxLayout;
    main_layout->addLayout(left_layout,3);
    //1. 上方采用水平布局
    QHBoxLayout *select_layout=new QHBoxLayout;
    select_layout->setSpacing(10); //设置控件间的间距
    //1.1 添加日期选择和通道选择
    dateEdit=new QDateEdit;
    dateEdit->setCalendarPopup(true); //开启下拉日历
    //1.2 配置格式和默认当天
    dateEdit->setDisplayFormat("yyyy年MM月dd日");
    QDate custDate(2026,5,28);
    dateEdit->setDate(custDate);
    dateEdit->setMinimumDate(QDate(2026,1,1));
    dateEdit->setMaximumDate(QDate::currentDate());
    select_layout->addWidget(dateEdit);
    //1.3 通道选择的下拉框
    ch_box=new QComboBox;
    ch_box->setObjectName("ch_box");
    ch_box->addItem("所有通道",0);
    ch_box->addItem("通道1",1);
    ch_box->addItem("通道2",2);
    ch_box->addItem("通道3",3);
    ch_box->addItem("通道4",4);
    //1.4 默认显示第一个
    ch_box->setCurrentIndex(0);
    //1.5 加入布局
    select_layout->addWidget(ch_box);
    //2.添加QListWidget 显示视频路径
    listWidget=new QListWidget;
    //4. 添加翻页按钮进行交互
    QHBoxLayout *btn_layout=new QHBoxLayout;
    btn_layout->setAlignment(Qt::AlignCenter);
    btn_layout->setContentsMargins(0,0,0,0);
    btn_layout->setSpacing(5);
    //前一页
    btn_pre=new QPushButton();
    btn_pre->setIcon(QIcon(":/image/left-arrow.png"));
    btn_layout->addWidget(btn_pre);
    //中间分页
    for(int i=0;i<3;i++){
        btn_page[i]=new QPushButton;
        btn_page[i]->setText(QString::number(i+1));
        btn_page[i]->setProperty("page",i+1);
        btn_layout->addWidget(btn_page[i]);
    }
    //后一页
    btn_next=new QPushButton();
    btn_next->setIcon(QIcon(":/image/right-arrow.png"));
    btn_layout->addWidget(btn_next);
    btn_pre->setObjectName("btn_prev");
    btn_next->setObjectName("btn_next");
    //加入布局
    left_layout->addLayout(select_layout,1);
    left_layout->addWidget(listWidget,6);
    left_layout->addLayout(btn_layout,1);
}

void MotionReviewWidget::init_rightWidget()
{
    //主布局
    right_layout=new QVBoxLayout;
    main_layout->addLayout(right_layout,7);
    //1. 垂直布局
    QVBoxLayout *player_layout=new QVBoxLayout;
    right_layout->addLayout(player_layout,9);
    player_layout->setSpacing(10);
    player_layout->setContentsMargins(10,0,10,0);
    //1.1 视频播放窗口和进度条
    lab_frame=new QLabel;
    lab_frame->setObjectName("lab_video");
    lab_frame->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);//跟随布局
    lab_frame->setScaledContents(true);
    player_layout->addWidget(lab_frame,8);
    //1.2 进度条
    slider_play=new QSlider(Qt::Horizontal,this);
    slider_play->setMinimum(0);
    slider_play->setValue(0);
    slider_play->setEnabled(true);
    player_layout->addWidget(slider_play,1);
    //1.3 播放控制
    //水平布局 摆放
    QHBoxLayout *play_ctrl_layout=new QHBoxLayout;
    right_layout->addLayout(play_ctrl_layout,1);
    btn_pause=new QPushButton("暂停");
    box_speed=new QComboBox;
    box_speed->addItem("0.5x",0.5);
    box_speed->addItem("1x",1);
    box_speed->addItem("1.5x",1.5);
    box_speed->addItem("2x",2);
    box_speed->setCurrentIndex(1);
    btn_picshoot=new QPushButton("截图");
    btn_pause->setObjectName("btn_pause");
    btn_picshoot->setObjectName("btn_capture");
    //加入布局
    play_ctrl_layout->addStretch(1);
    play_ctrl_layout->addWidget(btn_pause);
    play_ctrl_layout->addWidget(box_speed);
    play_ctrl_layout->addStretch(1);
    play_ctrl_layout->addWidget(btn_picshoot);
    play_ctrl_layout->setAlignment(Qt::AlignCenter);
    play_ctrl_layout->setSpacing(20);
}

void MotionReviewWidget::init_qss()
{
    QFile file(":/qss/review.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MotionReviewWidget::init_connect()
{
    connect(btn_pre,&QPushButton::clicked,this,[this](){
        if(this->curPage>1){
            if(this->curPage%Pagination==1){
                m_startShow_Page=this->curPage-Pagination;
            }
            this->isPage=true;
            this->curPage--;
            refreshRecordList();
        }
    });
    connect(btn_next,&QPushButton::clicked,this,[this](){
        if(this->curPage<pageNum){
            if(this->curPage%Pagination==0){
                m_startShow_Page=1+this->curPage;
            }
            this->isPage=true;
            this->curPage++;
            refreshRecordList();
        }
    });
    connect(this->dateEdit,&QDateEdit::dateChanged,this,[this](){
        this->refreshRecordList();
    });
    connect(this->ch_box,&QComboBox::currentIndexChanged,this,[this](){
        this->refreshRecordList();
    });
    connect(listWidget,&QListWidget::itemDoubleClicked,this,[=](QListWidgetItem *item){
        //数据的自定义标识  用于给用户进行操作  定义自定义数据的起点 +1、+2..以此类推  取用时需进行对应
        absPath=item->data(Qt::UserRole).toString(); //获取自定义的数据
        qDebug()<<"get file absPath:"<<absPath;
        //只有初次双击时启动线程
        if(!m_playThread->isRunning()){
            m_playThread->start();
        }
        else{
            isSWitchVideo = true; //说明用户双击其他视频
            //暂停播放->会发送结束信号
            m_videotask->stopPlay();
        }
        //若没有运行，则直接启动新任务去播放视频
        emit sigStartPlay(absPath);
        //重置标记位
        btn_pause->setText("暂停");//重置暂停按钮
        isPause=false;
    });
    //进度条拖动
    connect(slider_play,&QSlider::sliderMoved,this,[=](int pos){
        m_videotask->setNewPos(pos);
    });
    //倍速播放
    connect(box_speed,&QComboBox::currentIndexChanged,this,[=](){
        double sp=box_speed->currentData().toDouble();
        m_videotask->setSpeed(sp);
    });
    //暂停/继续播放
    connect(btn_pause,&QPushButton::clicked,this,[=](){
        //此处的操作 有缺陷 采用硬编码判断文本
        //这里主要处理的是当视频播放结束 设置暂停按钮文本为重新播放；
        //无需用户重新双击视频
        QString btnText=btn_pause->text();
        if(btnText=="重新播放"){
            //qDebug()<<"重新播放了"<<absPath;
            emit sigStartPlay(absPath);
            btn_pause->setText("暂停");
            isPause=false;
            return;
        }
        //否则 就是正常的切换逻辑
        isPause=!isPause;
        m_videotask->pausePlay(isPause);
        if(isPause){
            btn_pause->setText("继续");
        }
        else{
            btn_pause->setText("暂停");
        }
    });
    //截图按钮
    connect(btn_picshoot,&QPushButton::clicked,this,[this](){
        m_videotask->getSnapFrame();
    });
}

void MotionReviewWidget::start_play()
{
    qDebug() << "MonitorReviewWidget->start_play called";  // 添加调试
    //创建视频播放线程
    m_videotask=new VideoTask;
    m_playThread=new QThread;
    m_videotask->moveToThread(m_playThread);
    //连接信号
    connect(this,&MotionReviewWidget::sigStartPlay,m_videotask,&VideoTask::startPlay); //初始化视频
    connect(m_videotask,&VideoTask::sigVideoFrame,this,&MotionReviewWidget::showFrame); //显示
    connect(m_videotask,&VideoTask::finishVideo,this,[=](){ //结束当前视频
        //1.切换其他视频 提前中断发出的信号
        if(isSWitchVideo){
            isSWitchVideo = false;
            return;
        }
        //2.重置 正常视频播放结束->执行
        slider_play->setValue(0);
        btn_pause->setText("重新播放");
        isPause=false;
    });
    connect(m_videotask,&VideoTask::sigTotalFrames,this,[=](int total){ //设置进度条的范围
        if(slider_play) {
            slider_play->setMaximum(total-1);
        }
    });
    //创建保存视频截图线程
    m_saveImageThread=new QThread;
    imgTask=new ImageTask;
    imgTask->moveToThread(m_saveImageThread);
    connect(m_videotask,&VideoTask::sigSendShotFrameAndTime,this,[=](Mat frame,int curMs){
        //获取通道id
        int channel_id=0;
        //获取类型
        QString ch;
        //获取视频名
        QString videoName;
        for(auto &video:videoList){
            if(video.getVideoPath()==absPath){
                channel_id=video.getChannelId();
                ch=video.getVideo_type();
                videoName=video.getVideoName();
                break;
            }
        }
        //qDebug()<<"发送截图到保存任务";
        Mat saveFrame=frame.clone();
        int offsetSec=qRound(curMs/1000.0);
        emit sigSaveImg(saveFrame,channel_id,0,ch,videoName,offsetSec);
    });
    connect(this,&MotionReviewWidget::sigSaveImg,imgTask,&ImageTask::saveImageToDb);
    m_saveImageThread->start();
}

//将视频帧转化成QImage返回
QImage MotionReviewWidget::videoToQImage(Mat &frame)
{
    if(frame.empty()) return QImage();
    //采用深拷贝
    Mat tmp=frame.clone();
    //判断帧的类型
    switch (frame.type()) {
    case CV_8UC1:
        return QImage(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cvtColor(frame,tmp,COLOR_BGR2RGB);
        return QImage(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}

void MotionReviewWidget::hideEvent(QHideEvent *event)
{
    //qDebug()<<"移动侦测回看窗口发生切换，结束播放！";
    if(m_videotask){
        m_videotask->stopPlay();
    }
    //调用基类
    QWidget::hideEvent(event);
}


void MotionReviewWidget::showFrame(Mat frame,int count)
{
    slider_play->setValue(count);
    if(frame.empty()) return;
    QImage img=videoToQImage(frame);
    if(!img.isNull()){
        QPixmap framePix=QPixmap::fromImage(img);
        //保持宽高比 支持平移变换
        lab_frame->setPixmap(framePix.scaled(lab_frame->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        lab_frame->setAlignment(Qt::AlignCenter);
    }
}

void MotionReviewWidget::refreshRecordList()
{
    if(!this->isPage){
        this->curPage=1;
        this->m_startShow_Page = 1;
    }
    this->isPage=false;

    VideoTask vtask; //因为这里需要创建视频人物 所以开启播放不放在任务的构造里  因为要传视频路径
    listWidget->clear();
    videoList.clear();
    QDate queryDate=this->dateEdit->date();
    qDebug()<<"select date:"<<queryDate;
    int channelId=this->ch_box->currentData().toInt();
    qDebug()<<"select channel:"<<channelId;
    //总记录数 只查询移动侦测视频
    total_page=vtask.videoPlayBack(videoList,channelId,queryDate,this->pageSize,(this->curPage-1)*this->pageSize,"M");
    //刷新到界面
    for(auto &video:videoList){
        QListWidgetItem *item=new QListWidgetItem(video.getVideoName()); //用于UI显示文件名称
        item->setData(Qt::UserRole,video.getVideoPath()); //后台保存程序路径 用户双击播放
        this->listWidget->addItem(item);
    }
    updatePage();
}

void MotionReviewWidget::updatePage()
{
    //计算总页数
    this->pageNum=(total_page+pageSize-1)/pageSize;
    if(this->pageNum==0) this->pageNum=1;
    //计算右边界
    int endShow_page=m_startShow_Page+2;
    //比较 当前显示的三个按钮 是否超出总页数 设置实际范围;
    int showPageCnt=3;
    if(endShow_page>pageNum){
        showPageCnt=3-(endShow_page-pageNum);
    }
    for(int i=0;i<3;i++){
        //渲染实际存在的按钮
        if(i<showPageCnt){
            if(curPage==m_startShow_Page+i){
                btn_page[i]->setStyleSheet(selectStyle);
            }
            else{
                btn_page[i]->setStyleSheet(normalStyle);
            }
            btn_page[i]->setText(QString::number(m_startShow_Page+i));
            btn_page[i]->setProperty("page",m_startShow_Page+i);
            btn_page[i]->setVisible(true);
        }
        else{
            btn_page[i]->setVisible(false);
        }
    }
}
