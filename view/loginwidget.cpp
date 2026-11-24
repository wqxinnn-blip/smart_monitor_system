#include "loginwidget.h"
#include "../util/winmanager.h"

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("登录窗口");
    this->resize(800,600);
    this->setFont(QFont("宋体",16,QFont::Bold));
    this->setAutoFillBackground(true);  //填满背景
    //1. 设置背景图片
    // QPixmap pix(":/image/bg.jpg");
    // QPalette pal=this->palette();
    // pal.setBrush(QPalette::Window,QBrush(pix.scaled(this->size())));
    //this->setPalette(pal);
    //2.主布局 垂直布局
    QVBoxLayout *main_layout=new QVBoxLayout(this);
    main_layout->setContentsMargins(80,40,80,40);
    main_layout->setSpacing(15);
    this->setLayout(main_layout);
    //3.设置主标题
    QLabel *title=new QLabel("XX监控登录系统");
    title->setObjectName("Title");
    title->setStyleSheet("font-size:36px");
    title->setAlignment(Qt::AlignCenter);
    //4.表单布局  完成用户名、密码、验证码的创建
    //水平布局里嵌套表单 做横向添加
    QHBoxLayout *h_layout=new QHBoxLayout;
    QFormLayout *form_layout=new QFormLayout;
    form_layout->setLabelAlignment(Qt::AlignRight);
    form_layout->setFormAlignment(Qt::AlignHCenter);
    form_layout->setSpacing(18);
    form_layout->setContentsMargins(0,0,0,0);
    //4.1 用户名
    QHBoxLayout *name_layout=new QHBoxLayout;
    QLabel *lab_username=new QLabel("用户名：");
    edit_username=new QLineEdit;
    lab_nameTip=new QLabel;
    //设置样式
    lab_nameTip->setFixedWidth(180);
    lab_nameTip->setStyleSheet("font-size:14px;color:#888888;");
    lab_nameTip->setText("");
    name_layout->addWidget(edit_username);
    name_layout->addWidget(lab_nameTip);
    form_layout->addRow(lab_username,name_layout);
    //4.2 密码
    QHBoxLayout *pwd_layout=new QHBoxLayout;
    QLabel *lab_password=new QLabel("密  码：");
    edit_password=new QLineEdit;
    edit_password->setMaxLength(8);
    edit_password->setEchoMode(QLineEdit::Password);
    //设置切换明/密文按钮
    btn_show=new QPushButton;
    btn_show->setMaximumWidth(20);
    btn_show->setObjectName("btn_show");
    btn_show->setIcon(QIcon(":/image/dark.png"));
    // //////////////////////////
    lab_pwdTip=new QLabel;
    lab_pwdTip->setFixedWidth(180);
    lab_pwdTip->setStyleSheet("font-size:14px;color:#888888;");
    lab_pwdTip->setText("");
    pwd_layout->addWidget(edit_password);
    pwd_layout->addWidget(btn_show);
    pwd_layout->addWidget(lab_pwdTip);
    form_layout->addRow(lab_password,pwd_layout);
    //4.3 验证码
    //4.3.1创建水平布局
    QHBoxLayout *vcode_layout=new QHBoxLayout;
    vcode_layout->setSpacing(10);
    QLabel *lab_verCode=new QLabel("验证码：");
    edit_verCode=new QLineEdit;
    edit_verCode->setMaxLength(4);
    edit_verCode->setFixedWidth(80);
    //创建验证码对象
    captcha=new Captcha;
    //4.3.2 加入布局
    vcode_layout->addWidget(edit_verCode);
    vcode_layout->addWidget(captcha);
    vcode_layout->addStretch();
    //4.3.3 加入表单布局
    form_layout->addRow(lab_verCode,vcode_layout);
    //5. 创建三个按钮
    //5.1 水平布局
    QHBoxLayout *btn_layout=new QHBoxLayout;
    btn_layout->setSpacing(30);
    btn_login=new QPushButton("登录");
    btn_reg=new QPushButton("注册");
    btn_cancel=new QPushButton("取消");
    //5.2 加入布局
    btn_layout->addStretch(1);
    btn_layout->addWidget(btn_login);
    btn_layout->addWidget(btn_reg);
    btn_layout->addWidget(btn_cancel);
    btn_layout->addStretch(1);
    //6. h_layout左右两侧加入弹簧
    h_layout->addStretch(3);
    h_layout->addLayout(form_layout);
    h_layout->addStretch(1);
    //7. 给主布局加入弹簧
    main_layout->addStretch(3);
    main_layout->addWidget(title);
    main_layout->addStretch(1);
    main_layout->addLayout(h_layout);
    main_layout->addLayout(btn_layout);
    main_layout->addStretch(3);
    //初始化
    this->init_qss();
    this->init_connect();
}

void LoginWidget::init_qss()
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void LoginWidget::login()
{
    //1.获取用户名、密码和验证码
    QString username=edit_username->text().trimmed();
    QString password=edit_password->text().trimmed();
    QString vcode=edit_verCode->text().trimmed();
    //1.1 首先校验用户名或密码是否为空
    if(username.isEmpty() || password.isEmpty()){
        QMessageBox::warning(this,"提示","用户名或密码不能为空");
        return;
    }
    //2.比较验证码
    QString verifyCode=captcha->getCaptcha();
    //qDebug()<<"验证码是"<<verifyCode;
    if(!(verifyCode==vcode)){
        QMessageBox::critical(this,"错误","验证码不匹配，请重新输入！");
        this->edit_verCode->setText("");
        captcha->refreshCaptcha(); //刷新验证码
        return;
    }
    //验证用户名和密码
    UserService userService;
    int userId;
    bool flag=userService.login(username,password,userId);
    if(flag){
        //登录成功
        this->hide();
        WinManager::getInstence()->main_widget.showUserName(username);
        //记录日志
        GlobalUser::curAdmin_id=userId;
        GlobalUser::curAdmin_name=username;

        OperLogService::getInstence()->insertOperLog(GlobalUser::curAdmin_id,GlobalUser::curAdmin_name,"用户登录");
    }
    else{
        QMessageBox::critical(this,"错误","用户名或密码错误，请重新输入！");
    }
    //清空所有输入
    this->edit_username->setText("");
    this->edit_password->setText("");
    this->edit_verCode->setText("");
    //刷新验证码
    captcha->refreshCaptcha();
}

void LoginWidget::init_connect()
{
    connect(btn_login,&QPushButton::clicked,this,&LoginWidget::login);
    connect(btn_reg,&QPushButton::clicked,this,[this](){
        this->hide();
        WinManager::getInstence()->register_widget.show();
    });
    connect(btn_cancel,&QPushButton::clicked,this,[this](){
        this->hide();
        WinManager::getInstence()->main_widget.show();
    });
    connect(edit_username,&QLineEdit::textChanged,this,[=](const QString &text){
        //1.定义一个字符串接收输入框的内容
        QString str=text.trimmed();
        //2.判断
        //2.1 先判断是否为数字/字母
        if(str.isEmpty()){
            this->lab_nameTip->setText("");
        }
        else{
            QChar lastChar = text.back();
            if(!(lastChar.isDigit() || lastChar.isLetter())){
                this->lab_nameTip->setText("请输入数字或字母");
            }
        }
    });
    connect(edit_username,&QLineEdit::editingFinished,this,[=](){
        if(this->edit_username->text().size()<6 || this->edit_username->text().size()>10){
            lab_nameTip->setText("请输入6-10位的数字或字母");
        }
        else{
            lab_nameTip->setText("");
        }
    });
    connect(edit_password,&QLineEdit::textChanged,this,[=](const QString &text){
        //1.定义一个字符串接收输入框的内容
        QString str=text.trimmed();
        //2.判断
        //2.1 先判断是否为数字/字母
        if(str.isEmpty()){
            this->lab_pwdTip->setText("");
        }
        else{
            QChar lastChar = text.back();
            if(!(lastChar.isDigit() || lastChar.isLetter())){
                this->lab_pwdTip->setText("请输入数字或字母");
            }
        }
    });
    connect(edit_password,&QLineEdit::editingFinished,this,[=](){
        if(this->edit_password->text().size()<6 || this->edit_password->text().size()>8){
            lab_pwdTip->setText("请输入6-8位的数字或字母");
        }
        else{
            lab_pwdTip->setText("");
        }
    });
    connect(btn_show,&QPushButton::clicked,this,[this](){
        isShow=!isShow;
        if(this->edit_password->echoMode() == QLineEdit::Password){
            this->btn_show->setIcon(QIcon(":/image/light.png"));
            this->edit_password->setEchoMode(QLineEdit::Normal);
        }
        else{
            this->btn_show->setIcon(QIcon(":/image/dark.png"));
            this->edit_password->setEchoMode(QLineEdit::Password);
        }
    });
}

//背景随窗口大小缩放
void LoginWidget::resizeEvent(QResizeEvent *event){
    QPixmap pix(":/image/bg.jpg");
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(pix.scaled(size())));
    setPalette(pal);
    QWidget::resizeEvent(event);
}