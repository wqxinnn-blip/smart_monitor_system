#include "imagewidget.h"
QDialog *ImageWidget::g_videoDialog=nullptr;
QLabel *ImageWidget::g_frameLabel=nullptr;

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setFixedSize(1000,600);
    //初始化变量
    curPage=1;
    pageNum=1;
    pageSize=9;
    Pagination=3;
    m_startShow_Page=1;
    this->isPage=false;//翻页操作标记位
    //整体布局 
    main_layout=new QHBoxLayout;
    this->setLayout(main_layout);
    //初始化
    init_qss();
    init_leftWidget();
    init_rightWidget();
    init_maskWidget();
    init_connect();
    init_timeSelect();
    refreshImg();
}

void ImageWidget::init_leftWidget()
{
    left_widget=new QWidget;
    //整体采用垂直布局
    left_layout=new QVBoxLayout(left_widget);
    main_layout->addWidget(left_widget,2);
    //垂直布局
    QVBoxLayout *select_layout=new QVBoxLayout;
    select_layout->setSpacing(10);
    //1.添加日期选择和通道选择
    dateEdit=new QDateEdit;
    dateEdit->setCalendarPopup(true); //开启下拉日历
    //1.1 配置格式和默认当天
    dateEdit->setDisplayFormat("yyyy年MM月dd日");
    QDate custDate(2026,6,14);
    dateEdit->setDate(custDate);
    dateEdit->setMinimumDate(QDate(2026,1,1));
    dateEdit->setMaximumDate(QDate::currentDate());
    select_layout->addWidget(dateEdit);
    //2.通道选择的下拉框
    ch_box=new QComboBox;
    ch_box->setObjectName("ch_box");
    ch_box->addItem("所有通道",0);
    ch_box->addItem("通道1",1);
    ch_box->addItem("通道2",2);
    ch_box->addItem("通道3",3);
    ch_box->addItem("通道4",4);
    //2.3 默认显示第一个
    ch_box->setCurrentIndex(0);
    //2.4 加入布局
    select_layout->addWidget(ch_box);
    left_layout->addLayout(select_layout);
    //-------------------------------------------------------------------------
    //设置滚动容器（主）
    //滚动容器里套一个容器，里面采用垂直布局，填写时间段按钮
    m_scrollArea=new QScrollArea;
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); //始终关闭
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded); //需要时显示
    m_scrollArea->setWidgetResizable(true);
    //设置时间容器（副）
    m_timeWidget=new QWidget;
    m_timeWidget->setObjectName("timeWidget");
    time_layout=new QVBoxLayout(m_timeWidget);
    time_layout->setContentsMargins(5,5,5,5);
    time_layout->setSpacing(10);
    time_layout->setAlignment(Qt::AlignTop);
    //垂直布局
    m_scrollArea->setWidget(m_timeWidget);
    left_layout->addWidget(m_scrollArea);
}

void ImageWidget::init_rightWidget()
{
    right_widget=new QWidget;
    //1. 右侧主布局
    right_layout=new QVBoxLayout(right_widget);
    right_layout->setContentsMargins(0,0,0,0);
    main_layout->addWidget(right_widget,8);
    //2. 网格布局  填入3*3的图片
    grid_layout=new QGridLayout;
    grid_layout->setSpacing(5);
    grid_layout->setContentsMargins(0,0,0,0);
    right_layout->addLayout(grid_layout,8);
    //3. 插入图片数据
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            //套一个容器 上面是图片 下面是路径
            QWidget *widget=new QWidget;
            QVBoxLayout *img_layout=new QVBoxLayout(widget);
            // 上面图片
            lab_img[i][j] = new ThumbLabel;
            lab_img[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            lab_img[i][j]->setScaledContents(true);
            // 加载原图并缩放
            QPixmap original(":/image/noImg.png");
            QPixmap scaled = original.scaled(200, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            lab_img[i][j]->setPixmap(scaled);
            // 设置最小大小
            lab_img[i][j]->setMinimumSize(150,100);
            //下面路径
            lab_path[i][j]=new ThumbLabel;
            lab_path[i][j]->setText("none");
            //设置连接
            connect(lab_img[i][j],&ThumbLabel::clickLeft,this,&ImageWidget::labOperation);
            //加入img_layout
            img_layout->addWidget(lab_img[i][j]);
            img_layout->addWidget(lab_path[i][j]);
            grid_layout->addWidget(widget,i,j);
        }
    }
    //3.分页操作的水平布局
    QHBoxLayout *btn_layout=new QHBoxLayout;
    right_layout->addLayout(btn_layout,1);
    //前一页
    btn_pre=new QPushButton("<");
    btn_layout->addWidget(btn_pre);
    //中间分页
    for(int i=0;i<3;i++){
        btn_page[i]=new QPushButton;
        btn_page[i]->setText(QString::number(i+1));
        btn_page[i]->setProperty("page",i+1);
        btn_layout->addWidget(btn_page[i]);
    }
    //后一页
    btn_next=new QPushButton(">");
    btn_layout->addWidget(btn_next);
    btn_pre->setObjectName("btn_prev");
    btn_next->setObjectName("btn_next");
}

void ImageWidget::init_qss()
{
    QFile file(":/qss/img.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void ImageWidget::init_timeSelect()
{
    //1. 清空 把存放时间段复选框的列表清空
    qDeleteAll(timeCheckList);
    timeCheckList.clear();
    //2. 查阅符合当前条件的所有数据 显示包含数据的时间段
    ImageTask imgtask;
    //2.1 获取日期选择和通道的内容
    QDate queryDate=this->dateEdit->date();
    int channelId=this->ch_box->currentData().toInt();
    //2.2 去重 QSet支持数据去重
    QSet<int> hourSet=imgtask.getHourSet(channelId,queryDate);
    //2.3 转换成列表排序
    QList<int> hours = hourSet.values();
    std::sort(hours.begin(), hours.end());
    //3. 添加时间段
    //3.1 给复选框控件添加文本
    for(auto x:hours){
        QCheckBox *box=new QCheckBox;
        QString time=QString("%1:00~%2:00").arg(x,2,10,QChar('0')).arg(x+1,2,10,QChar('0'));
        box->setText(time);
        box->setProperty("time",x);
        connect(box,&QCheckBox::toggled,this,&ImageWidget::onCheckTime);
        time_layout->addWidget(box);
        timeCheckList.append(box);
    }
}

void ImageWidget::init_maskWidget()
{
    //1.父容器为右侧容器，放大后铺满
    m_maskWidget=new QWidget(right_widget);
    //2.设置半透明背景
    m_maskWidget->setStyleSheet("background-color: rgba(0,0,0,180);");
    //3.默认先隐藏
    m_maskWidget->hide();
    //4.设置里面的布局 垂直布局
    QVBoxLayout *big_layout=new QVBoxLayout(m_maskWidget);
    big_layout->setContentsMargins(0,0,0,0);
    big_layout->setSpacing(0);
    //网格布局 用于叠加控件
    QGridLayout *imgGrid_layout=new QGridLayout;
    imgGrid_layout->setContentsMargins(0,0,0,0);
    imgGrid_layout->setSpacing(0);
    //4.1 存放放大图片的label
    m_magnify_lab=new ThumbLabel;
    m_magnify_lab->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    imgGrid_layout->addWidget(m_magnify_lab,0,0);
    //4.2 两个交互按钮 上一张/下一张
    btn_preImg=new QPushButton("<");
    btn_preImg->setFixedSize(40,80);
    imgGrid_layout->addWidget(btn_preImg,0,0,Qt::AlignLeft | Qt::AlignVCenter);
    btn_nextImg=new QPushButton(">");
    btn_nextImg->setFixedSize(40,80);
    imgGrid_layout->addWidget(btn_nextImg,0,0,Qt::AlignRight | Qt::AlignVCenter);
    //4.3 加入垂直布局
    big_layout->addLayout(imgGrid_layout,8);
    //5. 下方布局 水平布局  查看视频-返回
    QHBoxLayout *h_layout=new QHBoxLayout;
    h_layout->setSpacing(20);//设置间距
    big_layout->addLayout(h_layout,1);
    btn_scanVideo=new QPushButton("查看视频");
    btn_scanVideo->setObjectName("btn_scanVideo");
    btn_back=new QPushButton("返回");
    btn_back->setObjectName("btn_back");
    h_layout->addStretch();
    h_layout->addWidget(btn_scanVideo);
    h_layout->addWidget(btn_back);
    h_layout->addStretch();
}

void ImageWidget::init_connect()
{
    connect(this->dateEdit,&QDateEdit::dateChanged,this,[this](){
        this->init_timeSelect();
        this->refreshImg();
    });
    connect(this->ch_box,&QComboBox::currentIndexChanged,this,[this](){
        this->init_timeSelect();
        this->refreshImg();
    });
    connect(btn_pre,&QPushButton::clicked,this,[this](){
        if(this->curPage>1){
            //找到上一页的最大id，同时删除上一页的最小id
            last_id=record_prePage[this->curPage-2];
            record_prePage.pop_back();
            if(this->curPage%Pagination==1){
                m_startShow_Page=this->curPage-Pagination;
            }
            this->isPage=true;
            this->curPage--;
            refreshImg();
        }
    });
    connect(btn_next,&QPushButton::clicked,this,[this](){
        if(this->curPage<pageNum){
            if(featureImg.isEmpty()) return;
            //记录本页的最小id（即下一页查询时的起始id）利用栈思想
            record_prePage.append(featureImg.last().getImgId());
            last_id=featureImg.last().getImgId();
            if(this->curPage%Pagination==0){
                m_startShow_Page=1+this->curPage;
            }
            this->isPage=true;
            this->curPage++;
            refreshImg();
        }
    });
    connect(btn_back,&QPushButton::clicked,this,[this](){
        this->m_maskWidget->hide();
        //调用
        releaseSoruce();
    });
    connect(btn_preImg,&QPushButton::clicked,this,[this](){
        if(m_scanIndex>0){
            m_scanIndex--;
            QPixmap pix(m_copyImage[m_scanIndex].getImage_path());
            //更改 去thumbLabel里重设
            m_magnify_lab->setOriginPix(pix);
        }
        else{
            //判断当前页是否为第一页
            if(m_nowPage>1){
                last_id1=record_prePage1[m_nowPage-2];
                //先进行清空  只保存本页的数据
                m_copyImage.clear();
                //初始化重新加载数据到 m_copyImage
                ImageTask iTask;
                //获取日期选择和通道的内容
                QDate queryDate=this->dateEdit->date();
                int channelId=this->ch_box->currentData().toInt();
                iTask.imageScan(m_copyImage,channelId,pageSize,last_id1,queryDate,timeSlot);
                //修改必要数据
                last_id1=m_copyImage.last().getImgId();
                m_nowPage--;
                m_scanIndex=m_nowPage*pageSize-1;
                QPixmap pix(m_copyImage[m_scanIndex].getImage_path());
                m_magnify_lab->setOriginPix(pix);
            }
            else{
                qDebug() << "已是第一页，没有更多图片了！";
                return;
            }
        }
    });
    connect(btn_nextImg,&QPushButton::clicked,this,[this](){
        //计算当前获取的总记录的最后一个数据的下标
        int lastIdx=m_copyImage.size()-1;
        if(m_scanIndex<lastIdx){
            m_scanIndex++;
            QPixmap pix(m_copyImage[m_scanIndex].getImage_path());
            m_magnify_lab->setOriginPix(pix);
        }
        else{
            if(m_copyImage.size()+(m_nowPage-1)*pageSize<total_page){
                //先进行清空  只保存本页的数据
                record_prePage1.append(last_id1);
                m_copyImage.clear();
                //初始化重新加载数据到 m_copyImage
                ImageTask iTask;
                //获取日期选择和通道的内容
                QDate queryDate=this->dateEdit->date();
                int channelId=this->ch_box->currentData().toInt();

                iTask.imageScan(m_copyImage,channelId,pageSize,last_id1,queryDate,timeSlot);
                if(m_copyImage.isEmpty()) {
                    qDebug() << "该时间段无图片数据";
                    return;
                }
                last_id1=m_copyImage.last().getImgId();
                m_nowPage++; //当前页+1
                //更新索引重绘
                m_scanIndex=0;
                QPixmap pix(m_copyImage[m_scanIndex].getImage_path());
                m_magnify_lab->setOriginPix(pix);
            }
            else{
                qDebug()<<"已是最后一张图片！";
            }
        }
    });
    connect(btn_scanVideo,&QPushButton::clicked,this,[this](){
        //1.取出当前选中的图片完整路径
        File file;
        if(m_copyImage.isEmpty() || m_scanIndex < 0 || m_scanIndex >= m_copyImage.size()) {
            return;
        }
        QString videoPath=m_copyImage[m_scanIndex].getImage_path();
        qDebug()<<"videoPath"<<videoPath;
        QString videoAbsPath=file.getVideoPathByFrameType(videoPath);
        int offsetSec=m_copyImage[m_scanIndex].getOffset_second();
        //初始化线程
        if(!videoThread){
            videoThread=new QThread;
            videoTask=new VideoTask;
            videoTask->moveToThread(videoThread);
            connect(videoTask,&VideoTask::sigVideoFrame,this,&ImageWidget::updateFrame); //显示
            connect(videoTask,&VideoTask::finishVideo,this,[](){ //结束信号
                qDebug()<<"视频播放结束";
            });
            connect(this,&ImageWidget::sigSeekAndPlay,videoTask,&VideoTask::seekToMsAndPlay); //初始化 启动
            connect(videoTask,&VideoTask::sigTotalToImg,this,[this](int total,int nowPos){
                slider->setMaximum(total);
                slider->setValue(nowPos);//直接从当前图片的时间点进行播放 定位
            });
            videoThread->start();
        }
        //打开弹窗
        init_videoWidget();
        emit sigSeekAndPlay(videoAbsPath ,offsetSec*1000);
    });

}

void ImageWidget::updateFrame(Mat frame,int count)
{
    if(frame.empty()) return;
    slider->setValue(count);
    QImage img=MatToImage(frame);
    if(!img.isNull()){
        g_frameLabel->setPixmap(QPixmap::fromImage(img).scaled(g_frameLabel->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation));
    }
}

QImage ImageWidget::MatToImage(Mat frame)
{
    if(frame.empty()) return QImage();
    Mat tmp=frame.clone();
    switch(tmp.type()){
    case CV_8UC1:
        return QImage(frame.data,frame.cols,frame.rows,frame.step,QImage::Format_Grayscale8).copy();
    case CV_8UC3:
        cvtColor(frame,tmp,COLOR_BGR2RGB);
        return QImage(tmp.data,tmp.cols,tmp.rows,tmp.step,QImage::Format_RGB888).copy();
    default:
        return QImage();
    }
}

//绘制弹窗
void ImageWidget::init_videoWidget()
{
    if(g_videoDialog!=nullptr){
        g_videoDialog->show();
        g_videoDialog->raise();
        return;
    }
    //弹窗创建
    g_videoDialog=new QDialog;
    g_videoDialog->setWindowTitle("视频播放");
    g_videoDialog->resize(800,600);
    g_videoDialog->setAttribute(Qt::WA_DeleteOnClose); //关闭窗口时自动delete窗口对象
    //布局
    QVBoxLayout* layout=new QVBoxLayout(g_videoDialog);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(0);
    //画面
    g_frameLabel=new QLabel();
    g_frameLabel->setStyleSheet("background:#000000;"); //纯黑
    g_frameLabel->setAlignment(Qt::AlignCenter);
    //拖动条
    slider=new QSlider(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setValue(0);
    slider->setEnabled(true);
    //暂停 返回按钮
    QHBoxLayout *btn_layout=new QHBoxLayout;
    btn_layout->setSpacing(15);
    btn_layout->setAlignment(Qt::AlignCenter);
    btn_pause=new QPushButton("暂停");
    btn_return=new QPushButton("返回");
    btn_layout->addWidget(btn_pause);
    btn_layout->addWidget(btn_return);
    layout->addWidget(g_frameLabel,1);
    layout->addWidget(slider,0);
    layout->addLayout(btn_layout,0);
    //连接
    connect(g_videoDialog,&QDialog::destroyed,[](){
        g_videoDialog=nullptr;
        g_frameLabel=nullptr;
    });
    connect(btn_return,&QPushButton::clicked,this,[this](){
        qDebug()<<"执行返回动作";
        //结束播放任务
        if(videoTask){
            videoTask->stopPlay();
            videoTask->disconnect();
        }
        if(videoThread){
            videoThread->quit();
            videoThread->wait(2000);
        }
        if(videoTask){
            delete videoTask;
            videoTask=nullptr;
        }
        if(videoThread){
            delete videoThread;
            videoThread=nullptr;
        }
        if(g_videoDialog){
            g_videoDialog->close();
        }
    });
    connect(btn_pause,&QPushButton::clicked,this,[this](){
        isPause=!isPause;
        videoTask->pausePlay(isPause);
        if(isPause){
            btn_pause->setText("继续");
        }
        else{
            btn_pause->setText("暂停");
        }
    });
    connect(slider,&QSlider::sliderMoved,this,[=](int pos){
        videoTask->setNewPos(pos);
    });
    g_videoDialog->show();
}


void ImageWidget::releaseSoruce()
{
    //清空
    m_magnify_lab->setPixmap(QPixmap());
    m_magnify_lab->setProperty("path",QVariant());
    m_copyImage.clear();
    record_prePage1.clear();
    m_scanIndex=0;
    m_nowPage=0;
    last_id1=0;
}


void ImageWidget::labOperation()
{
    if(featureImg.isEmpty()) return;
    //获取当前正在处理的信号的控件指针
    lab=qobject_cast<ThumbLabel*>(sender());
    if(!lab) return;
    //获取该Label的图片路径 并初始化相关数据
    QString imgPath=lab->property("path").toString();
    int now_posI=lab->property("i").toInt();
    int now_posJ=lab->property("j").toInt();
    //-------------------------------------------------------------------------------------------//
    m_scanIndex=now_posI*3+now_posJ;
    m_copyImage=featureImg; //将获取到的当页图片拷贝一份 用于放大后的翻页功能
    m_nowPage=this->curPage;
    //添加前n页数据
    record_prePage1.append(record_prePage.begin(),record_prePage.begin()+this->curPage);
    last_id1=featureImg.last().getImgId();
    //------------------------------------------------------------------------------------------//
    //绘图 存入数据
    QPixmap pix(imgPath);
    m_magnify_lab->setProperty("path",imgPath); 
    //绘制大小,并显示
    m_maskWidget->setGeometry(right_widget->rect());
    m_maskWidget->show();
    //出错位置： 第一次启用时 先绘制图片 后show窗口 导致第一次没应用上窗口的尺寸
    m_magnify_lab->setOriginPix(pix);
}


void ImageWidget::onCheckTime(bool)
{
    for(auto box : timeCheckList){
        int num=box->property("time").toInt();
        if(box->isChecked()==true){
            timeSlot[num]=num;
        }
        else{
            timeSlot.remove(num);
        }
    }
    refreshImg();
}

void ImageWidget::refreshImg()
{
    //1.前置检查
    if(!this->isPage){
        this->curPage=1;
        this->m_startShow_Page=1;
        this->last_id=0;
        this->featureImg.clear();
        this->record_prePage.clear();
        this->record_prePage.append(0);
    }
    //1.1重置翻页标记位
    this->isPage=false;
    //新建任务去数据库查询数据
    ImageTask imgtask;
    //获取日期选择和通道的内容
    QDate queryDate=this->dateEdit->date();
    int channelId=this->ch_box->currentData().toInt();
    //总记录数
    featureImg.clear();
    total_page=imgtask.imageScan(featureImg,channelId,pageSize,last_id,queryDate,timeSlot);
    // qDebug()<<"total_page:"<<total_page;
    // qDebug()<<"Size:"<<featureImg.size();
    int index=0;
    for(int i=0;i<3;i++) {
        for(int j=0;j<3;j++) {
            if(index<featureImg.size() && !featureImg[index].getImage_path().isEmpty()) {
                QPixmap pix(featureImg[index].getImage_path());
                if(!pix.isNull())
                {
                    lab_img[i][j]->setPixmap(pix.scaled(200,150,Qt::KeepAspectRatio,Qt::SmoothTransformation));
                    lab_img[i][j]->setProperty("path",featureImg[index].getImage_path());
                    lab_img[i][j]->setProperty("i",i);
                    lab_img[i][j]->setProperty("j",j);
                    //启用
                    lab_img[i][j]->setEnabled(true);
                }
                lab_path[i][j]->setText(featureImg[index].getImage_name());
            }
            else
            {
                lab_img[i][j]->setPixmap(QPixmap());
                lab_path[i][j]->setText("");
                lab_img[i][j]->setEnabled(false);
            }
            index++;
        }
    }
    updatePage();
}


void ImageWidget::updatePage()
{
    pageNum=(total_page+pageSize-1)/pageSize;
    if(pageNum==0) pageNum=1;
    //更新页码
    for(int i=0;i<3;i++) {
        int pageNumVal=m_startShow_Page+i;
        if(pageNumVal<=pageNum) {
            btn_page[i]->setText(QString::number(pageNumVal));
            btn_page[i]->setProperty("page",pageNumVal);
            btn_page[i]->setVisible(true);
            if(pageNumVal==curPage) {
                btn_page[i]->setStyleSheet("background:#409EFF;color:white;");
            }
            else
            {
                btn_page[i]->setStyleSheet("");
            }
        }
        else
        {
            btn_page[i]->setVisible(false);
        }
    }
}






