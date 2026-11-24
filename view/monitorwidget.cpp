#include "monitorwidget.h"
#include "../util/winmanager.h"

MonitorWidget::MonitorWidget(QWidget *parent)
    : QWidget{parent},m_draw("data/model/yolov8n.onnx")
{
    //1.设置主布局 水平布局  分为左容器和右容器
    main_layout=new QHBoxLayout;
    this->setLayout(main_layout);
    //2.初始化左容器、右容器
    init_leftWidget();
    init_rightWidget();
    //3.加入左容器
    main_layout->addWidget(leftWidget,2);
    //3.1中间添加垂直线条
    QFrame  *vLine=new QFrame(this);
    vLine->setFrameShape(QFrame::VLine);
    vLine->setObjectName("line_V");
    vLine->setLineWidth(1);
    main_layout->addWidget(vLine);
    //3.2加入右容器
    main_layout->addWidget(rightWidget,8);
    //初始化
    this->init_qss();
    this->init_connect();
}

MonitorWidget::~MonitorWidget()
{
    qDebug()<<"调用析构";
    //释放四通道的任务和录制任务
    for(int i=0;i<4;++i)
    {
        if(mt1[i])
        {
            mt1[i]->setIsStop(true);
            mt1[i]->disconnect();
        }
    }
    if(rt)
    {
        for(int ch=0;ch<4;ch++){
            rt->setRecordEnable(ch,false);
        }
        rt->flush();
        rt->disconnect();
    }
    //停止线程等待退出
    //录制线程
    if(thread_r)
    {
        thread_r->quit();
        thread_r->wait(2000);
        delete rt;
        delete thread_r;
        rt = nullptr;
        thread_r = nullptr;
    }
    for(int i=0;i<4;++i)
    {
        if(thread_mul[i])
        {
            thread_mul[i]->quit();
            thread_mul[i]->wait(2000);
            delete mt1[i];
            delete thread_mul[i];
            mt1[i] = nullptr;
            thread_mul[i] = nullptr;
        }
    }
    //释放侦测线程
    for(int i=0;i<4;i++){
        if(dThread[i]){
            dThread[i]->quit();
            dThread[i]->wait(2000);
            delete dTask[i];
            delete dThread[i];
            dTask[i]=nullptr;
            dThread[i]=nullptr;
        }
    }
    //释放图片线程
    if(imgThread){
        imgThread->quit();
        imgThread->wait(2000);
        delete imgTask;
        delete imgThread;
        imgTask=nullptr;
        imgThread=nullptr;
    }
}

void MonitorWidget::init_leftWidget()
{
    leftWidget=new QWidget;
    //设置垂直布局
    QVBoxLayout *left_layout=new QVBoxLayout;
    leftWidget->setLayout(left_layout);
    //1.日期和通道选择
    lab_device=new QLabel("设备列表");
    left_layout->addWidget(lab_device);
    lab_device->setAlignment(Qt::AlignHCenter);
    //2.设置分割线
    QFrame *hLine=new QFrame;
    hLine->setObjectName("line_sep");
    hLine->setFixedHeight(2);
    left_layout->addWidget(hLine);
    //3.添加监控列表
    list_widget=new QListWidget;
    left_layout->addWidget(list_widget);
    //添加设备
    list_widget->clear();
}

void MonitorWidget::init_rightWidget()
{
    rightWidget=new QWidget;
    //设置主布局 垂直布局  上方添加单通道窗口和四通道窗口 下方添加两个按钮进行切换
    QVBoxLayout *right_layout=new QVBoxLayout;
    rightWidget->setLayout(right_layout);
    //1.创建堆叠布局存放单通道窗口和四通道窗口
    stack_layout=new QStackedLayout;
    right_layout->addLayout(stack_layout,8);
    //1.1 创建单通道窗口
    QWidget *single_widget=new QWidget;
    stack_layout->addWidget(single_widget);
    //1.2 创建多通道窗口
    QWidget *multi_widget=new QWidget;
    stack_layout->addWidget(multi_widget);
    //2.1 初始化单通道
    //设置单通道的布局
    QVBoxLayout *w1_layout=new QVBoxLayout;
    single_widget->setLayout(w1_layout);
    lab_w1=new QLabel;
    QPixmap pix(":/image/noVideo.png"); //默认展示
    lab_w1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored); //由布局处理
    lab_w1->setPixmap(pix.scaled(this->size()));
    lab_w1->setScaledContents(true); //开启自动缩放
    lab_w1->setAlignment(Qt::AlignCenter); //居中
    //加入单通道容器
    w1_layout->addWidget(lab_w1,0,Qt::AlignCenter); //不参与拉伸 保持居中
    //2.2 初始化四通道
    //设置多通道布局
    QGridLayout *w4_layout=new QGridLayout;
    multi_widget->setLayout(w4_layout);
    w4_layout->setSpacing(15);
    int x=0,y=0; //用于设置坐标
    for(int i=0;i<4;i++){
        QPixmap pix1(":/image/noVideo.png");
        lab_multi[i]=new QLabel;
        lab_multi[i]->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        lab_multi[i]->setPixmap(pix1.scaled(this->size()));
        lab_multi[i]->setScaledContents(true); //开启自动缩放
        lab_multi[i]->setAlignment(Qt::AlignCenter); //居中
        switch(i){
        case 0: x=0,y=0; break;
        case 1: x=0,y=1; break;
        case 2: x=1,y=0; break;
        case 3: x=1,y=1; break;
        }
        w4_layout->addWidget(lab_multi[i],x,y);
    }
    //3.默认显示第几个容器
    stack_layout->setCurrentIndex(0);
    num_channels=1;
    //4.下方采用水平布局 添加两个按钮
    QHBoxLayout *btn_layout=new QHBoxLayout;
    btn_layout->setSpacing(30);
    right_layout->addLayout(btn_layout,1);
    btn_w1=new QPushButton;
    btn_w1->setIcon(QIcon(":/image/c1_h.png"));
    btn_w4=new QPushButton;
    btn_w4->setIcon(QIcon(":/image/c4_h.png"));
    btn_w1->setIconSize(QSize(60,60));
    btn_w4->setIconSize(QSize(60,60));
    //新增移动检测
    check_detect=new QCheckBox("移动侦测");
    check_detect->setChecked(false);
    //4.1 添加到布局
    btn_layout->addWidget(btn_w1);
    btn_layout->addWidget(btn_w4);
    btn_layout->addStretch(1);
    btn_layout->addWidget(check_detect);
    btn_layout->addStretch(1);
}

void MonitorWidget::init_qss()
{
    QFile file(":/qss/left.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void MonitorWidget::init_connect()
{
    connect(btn_w1,&QPushButton::clicked,this,[=](){ //隐式捕获this
        //qDebug()<<"切换到单通道";
        num_channels=1;
        stack_layout->setCurrentIndex(0);
    });
    connect(btn_w4,&QPushButton::clicked,this,[=](){
        //qDebug()<<"切换到四通道";
        num_channels=4;
        stack_layout->setCurrentIndex(1);
    });
    connect(check_detect,&QCheckBox::toggled,this,&MonitorWidget::onMotionDetect);
    connect(list_widget,&QListWidget::itemDoubleClicked,this,[=](QListWidgetItem *item){
        qDebug()<<"切换通道画面";
        if(!item) return;
        int channel = item->data(Qt::UserRole).toUInt();
        //切换到单通道
        stack_layout->setCurrentIndex(0);
        num_channels=1;
        currentPreviewChannel=channel-1;
        if(!lab_multi[channel-1]->pixmap().isNull()){
            lab_w1->setPixmap(lab_multi[channel-1]->pixmap().scaled(lab_w1->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        }
        update();
        qDebug() << "双击切换，通道:" << channel;
    });
}

void MonitorWidget::onMotionDetect(bool checked)
{
    //1.开启/关闭检测帧的发送
    for(int i=0;i<4;i++){
        mt1[i]->setIsDetect(checked);
    }
    //如果是关闭侦测
    if(checked==false){
        update();
        this->stop_detectRecord();
        //qDebug()<<"停止移动侦测";
    }
    else{
        this->stop_record();
    }
}

void MonitorWidget::start_player()
{
    this->receivePaths();
    qDebug()<<"开启解析画面";
    //--------------------------------------------------------------------------------
    QVector<int> uiToRealCam;
    for(auto &cam : SettingInfo::cameras)
    {
        uiToRealCam.push_back(cam.getChannelId() - 1);
        //初始化设备列表
        QListWidgetItem *item=new QListWidgetItem(QIcon(":/image/camera.png"),cam.getChannelName());
        item->setData(Qt::UserRole,cam.getChannelId());
        list_widget->addItem(item);
    }
    //--------------------------------------------------------------------------------
    //1.1 创建保存移动帧的线程
    imgTask=new ImageTask;
    imgThread=new QThread;
    imgTask->moveToThread(imgThread);
    //1.2 创建四通道线程和任务
    for(int i=0;i<4;i++){
        thread_mul[i]=new QThread;
        int cId=uiToRealCam[i];
        if(cId==0){
            mt1[i]=new MonitorTask(0,i);
        }
        else{
            mt1[i]=new MonitorTask(paths[cId-1],i); //越界错误 初始化时没有视频路径
        }
        mt1[i]->moveToThread(thread_mul[i]);
        connect(thread_mul[i],&QThread::started,mt1[i],&MonitorTask::playVideo); //采集
        connect(mt1[i],&MonitorTask::sendFrame,this,&MonitorWidget::showFrame); //显示
        thread_mul[i]->start();
    }
    //2. 创建移动侦测线程和任务
    for(int i=0;i<4;i++){
        dTask[i]=new DetectTask;
        dThread[i]=new QThread;
        dTask[i]->moveToThread(dThread[i]);
        //通道0/其它通道的连接
        connect(mt1[i],&MonitorTask::sigDetectFrame,dTask[i],&DetectTask::beginDetect);
        //移动侦测 链接
        connect(dTask[i],&DetectTask::sendDetectResult,this,[this,i](std::vector<YOLO_OUT> yoloout,int channelId){
            //接收坐标值 相当于缓存坐标  用于判断后的绘制
            m_detectPos[channelId]=yoloout;
            //判断返回的数组是否为空
            bool flag=!yoloout.empty();
            //当detectFrameCount=5时开启移动侦测视频录制
            if(flag){
                detectFrameCount[i]++;
                //未符合结束录制的条件时  即emptyDetectCount=9 清零
                emptyDetectCount[i]=0;
                //qDebug()<<"detectFrameCount:"<<detectFrameCount[i];
                if(detectFrameCount[i]>=limitFrameCount){
                    detectFrameCount[i]=0;
                    //qDebug()<<"连续检测到五帧在移动，开始录制";
                    //开启移动侦测录制
                    rt->setOnRecording(true,i); //设置录制标记位  用于更改录制时长
                    rt->setRecordEnable(i,true);
                    isRecording[i]=true;
                }
            }
            else{
                //重置计数值
                detectFrameCount[i]=0;
                if(isRecording[i]){
                    emptyDetectCount[i]++;
                    if(emptyDetectCount[i]>=limitEmptyCount && isRecording[i]){
                        emptyDetectCount[i]=0;
                        qDebug() << "连续3s未检测到人体 停止录制";
                        rt->setOnRecording(false,i);
                        rt->setRecordEnable(i,false);
                        rt->requestFlush();
                        isRecording[i]=false;
                    }
                }
            }
        });
        dThread[i]->start();
    }
    imgThread->start();
}

void MonitorWidget::start_record()
{
    if(rt!=nullptr){
        for(int i=0;i<4;i++){
            rt->setRecordEnable(i,true);
        }
        return;
    }
    //qDebug()<<"开启四个通道的录制";
    //录制线程
    thread_r=new QThread;
    rt=new RecordTask;
    rt->moveToThread(thread_r);
    //建立连接
    connect(rt, &RecordTask::sigNeedFlush, rt, &RecordTask::flush, Qt::QueuedConnection);
    for(int i=0;i<4;i++){
        connect(mt1[i],&MonitorTask::sendFrame,rt,&RecordTask::saveVideo);
    }
    connect(rt,&RecordTask::detectRecordFinish,this,[this](int channelId){ //移动侦测录制结束
        isRecording[channelId]=false;
    });
    connect(rt,&RecordTask::sigCaptureSnapShot,imgTask,&ImageTask::saveImageToDb); //保存图片
    //默认全部开启录制
    for(int i=0;i<4;i++){
        rt->setRecordEnable(i,true);
    }
    thread_r->start();
}

//从普通到移动侦测
void MonitorWidget::stop_record()
{
    if(!rt) return;
    // 全部关闭写入
    for(int ch=0;ch<4;ch++)
        rt->setRecordEnable(ch,false);
    rt->flush();
}

void MonitorWidget::stop_detectRecord() //取消勾选
{
    //出现问题：切换回普通录制 会出现识别框的残留
    //把位置提前
    m_detectPos.clear();
    //录制任务内连接  执行强制刷新
    rt->requestFlush();
    //如果正在录制
    for(int i=0;i<4;i++){
        if(isRecording[i]){
            isRecording[i]=false;
            rt->setOnRecording(false,i); //改成正常的结束条件
        }
        detectFrameCount[i]=0;
        emptyDetectCount[i]=0;
    }
    //qDebug() << "手动取消/异常退出移动侦测，终止录制";
    if(WinManager::getInstence()->main_widget.getIsLogin()==true){
        //恢复四通道录制
        for(int i=0;i<4;i++){
            rt->setRecordEnable(i,true);
        }
    }
    update();
}

void MonitorWidget::receivePaths()
{
    paths.clear();
    VideoTask vt;
    paths= vt.searchDefaultVideo("./defaultVideo",3);
}

void MonitorWidget::cancelDetect()
{
    check_detect->setChecked(false);
}

QImage MonitorWidget::MatToQImage(Mat frame)
{
    if(frame.empty()) return QImage();
    switch(frame.type()){
    case CV_8UC1:
        return QImage(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cvtColor(frame,frame,COLOR_BGR2RGB);
        return QImage(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}

void MonitorWidget::showFrame(Mat frame,int channelId)
{
    if(frame.empty()) return;
    Mat drawMat=frame.clone();
    if(check_detect->isChecked()){
        auto it = m_detectPos.find(channelId);
        if (it != m_detectPos.end()) {
            m_draw.draw(drawMat, it.value());
        }
    }
    QImage img=MatToQImage(drawMat);
    if(!img.isNull()){
        lab_multi[channelId]->setPixmap(
            QPixmap::fromImage(img).scaled(
                lab_multi[channelId]->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
        // 如果是单通道模式且当前预览的通道就是这一帧，更新单通道显示
        if (stack_layout->currentIndex() == 0 && currentPreviewChannel == channelId) {
            lab_w1->setPixmap(QPixmap::fromImage(img).scaled(lab_w1->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }
    }
}

void MonitorWidget::paintEvent(QPaintEvent *event)
{
}

int MonitorWidget::getNum_channels() const
{
    return num_channels;
}

void MonitorWidget::detectDisabled(bool checked)
{
    this->check_detect->setEnabled(checked);
}

bool MonitorWidget::getCheckDetect()
{
    return check_detect->isChecked();
}

void MonitorWidget::switch_to_channel1()
{
    this->stack_layout->setCurrentIndex(0);
}