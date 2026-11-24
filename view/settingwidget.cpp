#include "settingwidget.h"
#include "../util/winmanager.h"

SettingWidget::SettingWidget(QWidget *parent)
    : QWidget{parent}
{
    this->init_widget();
    this->init_connect();
    this->init_qss();
}

void SettingWidget::init_widget()
{
    //1.前置准备
    setFixedSize(800,600);
    this->setFont(QFont("宋体",16,400));
    //2.移除系统默认的标题栏和边框、将窗口变成无边框样式
    this->setWindowFlag(Qt::FramelessWindowHint);
    //3.设置主布局及格式
    QVBoxLayout *main_layout=new QVBoxLayout;
    this->setLayout(main_layout);
    main_layout->setContentsMargins(0,0,0,0);
    main_layout->setSpacing(0);
    // -------------------界面交互设置--------------------------------
    //1.设置三个按钮在右上角
    QWidget *titleBar = new QWidget(this);
    titleBar->setFixedHeight(40); //固定高度
    //水平布局及内边距
    QHBoxLayout *right_layout = new QHBoxLayout(titleBar);
    right_layout->setContentsMargins(0,5,10,0);
    right_layout->addStretch(); //压缩到右侧
    //三个按钮 最小化 放大 关闭
    btn_min=new QPushButton("-");
    btn_max=new QPushButton;
    btn_max->setIcon(QIcon(":/image/max.png"));
    btn_close=new QPushButton("x");
    //1.2设置别名
    btn_min->setObjectName("btn_min");
    btn_max->setObjectName("btn_max");
    btn_close->setObjectName("btn_close");
    //1.3加入布局
    right_layout->addWidget(btn_min);
    right_layout->addWidget(btn_max);
    right_layout->addWidget(btn_close);
    main_layout->addWidget(titleBar);
    // --------------------主设置--------------------------------
    //1. 设置下布局 垂直布局
    QVBoxLayout *maindown_layout=new QVBoxLayout;
    maindown_layout->setSpacing(20);
    maindown_layout->setContentsMargins(100,10,100,10);
    //2. 主标题
    QHBoxLayout *title_layout=new QHBoxLayout;
    QLabel *title=new QLabel("智能监控系统设置页");
    title->setObjectName("lab_title");
    title->setAlignment(Qt::AlignCenter);
    title_layout->addWidget(title);
    maindown_layout->addLayout(title_layout);
    //3. 保存路径、输入框和浏览按钮
    QHBoxLayout *path_layout=new QHBoxLayout;
    QLabel *lab_path=new QLabel("保存路径：");
    lab_path->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    //文件路径需验证 原因：当收到外部因素导致路径更改 与实际ui显示的错误，此时保存会出现异常！
    edit_path=new QLineEdit(SettingInfo::file_path);
    edit_path->setEnabled(false);
    //直接检查路径是否存在
    if(!QFile::exists(SettingInfo::file_path)){
        edit_path->setStyleSheet("color:red;"); //标红进行注释
    }
    btn_browse=new QPushButton("浏览");
    //3.1加入布局
    path_layout->addWidget(lab_path,2);
    path_layout->addWidget(edit_path,8);
    path_layout->addWidget(btn_browse,1);
    maindown_layout->addLayout(path_layout);
    edit_path->setText(SettingInfo::file_path);
    //4.间隔时间
    QHBoxLayout *interval_layout=new QHBoxLayout;
    QLabel *lab_interval=new QLabel("间隔时间：");
    lab_interval->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    com_box=new QComboBox();
    com_box->addItem("30秒",30);
    com_box->addItem("60秒",60);
    com_box->addItem("120秒",120);
    interval_layout->addWidget(lab_interval,2);
    interval_layout->addWidget(com_box,2);
    interval_layout->addStretch(8);
    maindown_layout->addLayout(interval_layout);
    com_box->setCurrentText(QString::number(SettingInfo::timeInterval)+"秒");
    //5. 摄像头配置
    for(int i=0;i<4;i++){
        QHBoxLayout *h_layout=new QHBoxLayout;
        QLabel *lab_camera=new QLabel(SettingInfo::cameras[i].getCameraName());
        lab_camera->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        edit_channelNames[i]=new QLineEdit(SettingInfo::cameras[i].getChannelName());
        channelId_box[i]=new QComboBox();
        channelId_box[i]->addItem("通道1",1);
        channelId_box[i]->addItem("通道2",2);
        channelId_box[i]->addItem("通道3",3);
        channelId_box[i]->addItem("通道4",4);
        channelId_box[i]->setCurrentIndex(SettingInfo::cameras[i].getChannelId()-1);
        h_layout->addWidget(lab_camera,3);
        h_layout->addWidget(edit_channelNames[i],5);
        h_layout->addWidget(channelId_box[i],2);
        maindown_layout->addLayout(h_layout);
    }
    //6. 保存和取消按钮
    QHBoxLayout *h_layout=new QHBoxLayout;
    btn_save=new QPushButton("保存");
    btn_cancel=new QPushButton("取消");
    h_layout->addStretch(4);
    h_layout->addWidget(btn_save);
    h_layout->addStretch(2);
    h_layout->addWidget(btn_cancel);
    h_layout->addStretch(4);
    maindown_layout->addLayout(h_layout);
    maindown_layout->addStretch(1);
    main_layout->addLayout(maindown_layout);
}

void SettingWidget::init_qss()
{
    QFile file(":/qss/setting.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void SettingWidget::init_connect()
{
    //链接路径选择按钮 将选择的路径呈现在路径输入框中
    //1. 使用文件选择器
    connect(btn_browse,&QPushButton::clicked,this,[this](){
        // tr()支持多语言翻译 ./默认打开目录  当前目录
        QString selectPath=QFileDialog::getExistingDirectory(this,tr("请选择目标目录"),"./");
        if(selectPath.isEmpty()){
            qDebug()<<"用户未选择目录";
            return; //不执行后续操作
        }
        //打印选中的目录
        qDebug()<<"选中的目录是:"<<selectPath;
        //把选择的路径加入到输入框 显示
        this->edit_path->setText(selectPath);
        this->edit_path->setStyleSheet("color:white;");
        //解决超长尾部显示-> 强制显示开头内容  因为光标默认显示在末尾
        this->edit_path->setCursorPosition(0);
        //鼠标悬停显示所有目录
        this->edit_path->setToolTip(selectPath);
    });
    connect(btn_min,&QPushButton::clicked,this,[this](){
        this->window()->showMinimized();
    });
    connect(btn_max,&QPushButton::clicked,this,[this](){
        if(this->window()->isMaximized()) this->window()->showNormal();
        else this->window()->showMaximized();
    });
    connect(btn_close,&QPushButton::clicked,this,[=](){
        auto flag=QMessageBox::warning(this,"提示","是否退出?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if(flag==QMessageBox::Yes){
            qDebug() << "用户确认退出";
            this->close();
        }
    });
    connect(btn_save,&QPushButton::clicked,this,[this](){
        SettingInfo::file_path=edit_path->text();
        SettingInfo::timeInterval=com_box->currentData().toInt(); //拿复选框后面的整数数据
        //定义一个QSet容器，使用contains函数判断 true代表有重复
        QSet<int> chael_ids;
        for(int i=0;i<4;i++){
            SettingInfo::cameras[i].setChannelName(edit_channelNames[i]->text().trimmed());
            int chel_id=channelId_box[i]->currentData().toInt();
            if(chael_ids.contains(chel_id)){
                QMessageBox::warning(this,"提示","通道重复!");
                return;
            }
            chael_ids.insert(chel_id);
            SettingInfo::cameras[i].setChannelId(chel_id);
        }
        if(!QFile::exists(SettingInfo::file_path)){
            QMessageBox::warning(this,"路径无效","保存失败，请选择正确的路径！");
            return;
        }
        auto flag=QMessageBox::question(this,"配置确认","是否确认保存?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No);
        if(flag==QMessageBox::Yes){
            SettingInfo::saveToFile();
            if(!isSave){
                isSave=true;
                WinManager::getInstence()->main_widget.show();
                WinManager::getInstence()->main_widget.showCamera();
            }
            qDebug()<<"保存成功";
            this->hide();
        }
    });
    connect(btn_cancel,&QPushButton::clicked,this,[this](){
        if(!isSave){
            this->close();
        }
        else{
            this->hide();
        }
    });
}

void SettingWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){ //鼠标左键是否按下
        //1. 修改标记位
        isMoving=true;
        //2. 记录当前鼠标位置相对于左上角的偏移量
        m_dragPos=event->globalPosition().toPoint()-frameGeometry().topLeft();
    }
    //调用父类事件
    QWidget::mousePressEvent(event);
}

void SettingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(isMoving && (event->buttons() & Qt::LeftButton)){
        //1.计算出两次鼠标的偏移量相减后就是窗口的实际偏移
        this->move(event->globalPosition().toPoint()-m_dragPos);
    }
    QWidget::mouseMoveEvent(event);
}

void SettingWidget::mouseReleaseEvent(QMouseEvent *event)
{
    this->isMoving=false;
    QWidget::mouseReleaseEvent(event);
}