#include "registerwidget.h"
#include "../util/winmanager.h"

RegisterWidget::RegisterWidget(QWidget *parent)
    : QWidget{parent}
{
    this->setWindowTitle("注册界面");
    this->resize(800,600);
    this->setAutoFillBackground(true);  //填满背景
    this->setFont(QFont("宋体",16,QFont::Bold));
    //1. 主布局 垂直布局
    QVBoxLayout *main_layout=new QVBoxLayout;
    this->setLayout(main_layout);
    main_layout->setAlignment(Qt::AlignCenter);
    //2. 注册界面
    QLabel *title=new QLabel("注册界面");
    title->setStyleSheet("font-size:40px");
    title->setAlignment(Qt::AlignCenter);
    //3. 表单布局  填写用户名、密码、确认密码
    QHBoxLayout *h_layout=new QHBoxLayout;
    QFormLayout *form_layout=new QFormLayout;
    form_layout->setSpacing(20);
    //3.1 用户名
    QLabel *lab_username=new QLabel("用户名：");
    edit_username=new QLineEdit;
    form_layout->addRow(lab_username,edit_username);
    //3.2 密码
    QLabel *lab_password=new QLabel("密码：");
    edit_pwd=new QLineEdit;
    form_layout->addRow(lab_password,edit_pwd);
    //3.3 确认密码
    QLabel *lab_repwd=new QLabel("确认密码：");
    edit_rePwd=new QLineEdit;
    form_layout->addRow(lab_repwd,edit_rePwd);
    //4. 确认/取消按钮
    QHBoxLayout *btn_layout=new QHBoxLayout;
    btn_enter=new QPushButton("确认");
    btn_cancel=new QPushButton("取消");
    btn_layout->addWidget(btn_enter);
    btn_layout->addWidget(btn_cancel);
    form_layout->addRow(btn_layout);

    //5. 表单布局加入水平布局  用于拉伸左右两侧空白空间
    h_layout->addStretch();
    h_layout->addLayout(form_layout);
    h_layout->addStretch();

    //6. 添加控件到主布局
    main_layout->addStretch(2);
    main_layout->addWidget(title);
    main_layout->addStretch(1);
    main_layout->addLayout(h_layout);
    main_layout->addStretch(2);

    //7.初始化
    init_qss();
    init_connect();
}

void RegisterWidget::init_qss()
{
    QFile file(":/qss/login_reg.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void RegisterWidget::init_connect()
{
    connect(btn_enter,&QPushButton::clicked,this,&RegisterWidget::reg);
    connect(btn_cancel,&QPushButton::clicked,this,[this](){
        this->hide();
        WinManager::getInstence()->login_widget.show();
    });
}

void RegisterWidget::reg()
{
    //1.获取用户名、密码和验证码
    QString username=edit_username->text().trimmed();
    QString password=edit_pwd->text().trimmed();
    QString rePassword=edit_rePwd->text().trimmed();

    if(!(password==rePassword)){
        QMessageBox::critical(this,"错误","两次密码输入不一致，请重新输入！");
        this->edit_rePwd->setText("");
        return;
    }
    //验证用户名和密码
    UserService userService;
    bool flag=userService.regsiter(username,password);
    if(flag){
        this->hide();
        WinManager::getInstence()->login_widget.show();
    }
    else{
        QMessageBox::critical(this,"错误","用户名，请重新输入！");
        //清空所有输入
        this->edit_username->setText("");
        this->edit_pwd->setText("");
        this->edit_rePwd->setText("");
    }
}

void RegisterWidget::resizeEvent(QResizeEvent *event)
{
    QPixmap pix(":/image/bg.jpg");
    QPalette pal = palette();
    pal.setBrush(QPalette::Window, QBrush(pix.scaled(size())));
    setPalette(pal);
    QWidget::resizeEvent(event);
}
