#include "logwidget.h"

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

LogWidget::LogWidget(QWidget *parent)
    : QWidget{parent}
{
    this->resize(800,600);
    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->setContentsMargins(40,40,40,40);
    main_layout->setSpacing(20);

    // 1. 初始化左按钮+右表格 水平布局
    initCenterLayout();
    main_layout->addLayout(center_layout);
    // 2. 初始化底部分页
    init_PageLayout();
    main_layout->addLayout(page_layout);
    init_qss();
    initConnect();
    refreshLogTable();
}

void LogWidget::initCenterLayout()
{
    center_layout = new QHBoxLayout();
    center_layout->setSpacing(30);
//左侧垂直按钮布局
    btn_layout = new QVBoxLayout();
    btn_layout->setSpacing(40);
    btn_layout->setAlignment(Qt::AlignVCenter);
    //两个按钮
    btn_OperLog=new QPushButton("操作日志");
    btn_OperLog->setFixedSize(100,60);
    btn_AlarmLog=new QPushButton("异常日志");
    btn_AlarmLog->setFixedSize(100,60);
    btn_layout->addWidget(btn_OperLog);
    btn_layout->addWidget(btn_AlarmLog);
    center_layout->addLayout(btn_layout);
//右侧表格垂直布局
    RightTable = new QVBoxLayout();
    tableLog = new QTableWidget();
    tableLog->setColumnCount(5);
    tableLog->verticalHeader()->hide();
    tableLog->setHorizontalHeaderLabels({"选择","日志ID", "日志操作", "操作员", "操作时间"});
    tableLog->setAlternatingRowColors(true);
    QHeaderView* header = tableLog->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->resizeSection(0, 40);
    // 其余4列均匀拉伸
    for (int i=1;i<5;i++) {
        header->setSectionResizeMode(i,QHeaderView::Stretch);
    }
    tableLog->verticalHeader()->setDefaultSectionSize(35);
    RightTable->addWidget(tableLog);
    center_layout->addLayout(RightTable);
}

void LogWidget::init_PageLayout()
{
    //设置布局
    page_layout = new QHBoxLayout();
    page_layout->setSpacing(6);
    page_layout->setAlignment(Qt::AlignCenter);
    //初始化按钮
    btnPrev=new QPushButton("<");
    page_layout->addWidget(btnPrev);
    for(int i=0;i<pagination;i++){
        btn_page[i]=new QPushButton(QString::number(i+1));
        page_layout->addWidget(btn_page[i]);
    }
    btnNext=new QPushButton(">");
    page_layout->addWidget(btnNext);
    btnDel=new QPushButton("删除");
    page_layout->addWidget(btnDel);
}

void LogWidget::init_qss()
{
    QFile file(":/qss/img.qss");
    if(file.open(QFile::ReadOnly)){
        this->setStyleSheet(file.readAll());
        file.close();
    }
}

void LogWidget::initConnect()
{
    connect(btnPrev,&QPushButton::clicked,this,[this](){
        if(this->curPage>1){
            if(this->curPage%pagination==1){
                m_startPage=this->curPage-pagination;
            }
            this->curPage--;
            this->isPage=true;
            refreshLogTable();
        }
    });
    connect(btnNext,&QPushButton::clicked,this,[this](){
        if(this->curPage<pageNum){
            if(this->curPage%pagination==0){
                m_startPage=this->curPage+1;
            }
            this->curPage++;
            this->isPage=true;
            refreshLogTable();
        }
    });
    connect(btn_OperLog,&QPushButton::clicked,this,[this](){
        typeFlag=0;
        refreshLogTable();
    });
    connect(btn_AlarmLog,&QPushButton::clicked,this,[this](){
        typeFlag=1;
        refreshLogTable();
    });
    connect(btnDel,&QPushButton::clicked,this,[this](){
        QList<int> rowNums;
        QList<int> delIds;
        for(int row=0;row<tableLog->rowCount();row++){
            QTableWidgetItem *checkItem=tableLog->item(row,0);
            if(checkItem->checkState()==Qt::Checked){
                int logId=tableLog->item(row,1)->text().toInt();
                delIds.append(logId);
                rowNums.append(row);
            }
        }
        if(rowNums.isEmpty()){
            QMessageBox::information(this,"提示","请勾选要删除的日志！");
            return;
        }
        int flag=QMessageBox::question(this,"确认删除",QString("确定删除选中的%1条日志？删除后不可恢复！").arg(rowNums.size()),
                 QMessageBox::Yes | QMessageBox::No);
        if(flag==QMessageBox::No){
            return;
        }
        bool delflag=OperLogService::getInstence()->deleteLog(delIds,typeFlag);
        if(!delflag){
            QMessageBox::critical(this,"失败","删除失败！");
            return;
        }
        //删除界面数据 倒序删除
        std::sort(rowNums.begin(),rowNums.end());
        for(int row : rowNums){
            tableLog->removeRow(row);
        }
        QMessageBox::information(this,"成功","选中日志已删除！");
        refreshLogTable();
    });
}

void LogWidget::refreshLogTable()
{
    if(!isPage){
        this->curPage=1;
        pageNum=1;
        totalCount=0;
        m_startPage=1;
    }
    isPage=false;
    //1.先清空当前表格
    tableLog->setRowCount(0);
    operLogList.clear();
    expLogList.clear();
    //2.数据库读数据
    if(typeFlag==0){
        tableLog->setHorizontalHeaderLabels({"选择","日志ID", "日志操作", "操作员", "操作时间"});
        operLogList=OperLogService::getInstence()->queryOperLog(curPage,pageSize);
        //3.填入数据
        for(int i=0;i<operLogList.size();i++){
            OperationInfo data=operLogList[i];
            int j=0;
            //插入第i行
            tableLog->insertRow(i);
            //填入数据
            QTableWidgetItem *checkItem=new QTableWidgetItem();
            checkItem->setCheckState(Qt::Unchecked);
            checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tableLog->setItem(i,j++,checkItem);
            tableLog->setItem(i,j++,new QTableWidgetItem(QString(QString::number(data.getAdmin_id()))));
            tableLog->setItem(i,j++,new QTableWidgetItem(data.getOper_func()));
            tableLog->setItem(i,j++,new QTableWidgetItem(data.getAdmin_name()));
            tableLog->setItem(i,j++,new QTableWidgetItem(data.getOper_time()));
        }
    }
    else if(typeFlag==1){
        tableLog->setHorizontalHeaderLabels({"选择","事件ID","通道号","触发时间","事件类型"});
        expLogList=OperLogService::getInstence()->queryExpLog(curPage,pageSize);
        //3.填入数据
        for(int i=0;i<expLogList.size();i++){
            ExceptionInfo data=expLogList[i];
            //插入第i行
            tableLog->insertRow(i);
            //填入数据
            int j=0;
            QTableWidgetItem *checkItem=new QTableWidgetItem();
            checkItem->setCheckState(Qt::Unchecked);
            checkItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable);
            tableLog->setItem(i,j++,checkItem);
            tableLog->setItem(i,j++,new QTableWidgetItem(QString(QString::number(data.getLog_id()))));
            tableLog->setItem(i,j++,new QTableWidgetItem(QString(QString::number(data.getChannel_id()))));
            tableLog->setItem(i,j++,new QTableWidgetItem(data.getEvent_time()));
            tableLog->setItem(i,j++,new QTableWidgetItem(data.getEvent_desc()));
        }
    }

    //翻页操作
    totalCount=OperLogService::getInstence()->getOperLogCount(typeFlag);
    qDebug()<<totalCount;
    pageNum=(totalCount+pageSize-1)/pageSize;
    if(pageNum==0) pageNum=1;
    for(int i=0;i<3;i++){
        int pageNumVal=m_startPage+i;
        if(pageNumVal<=pageNum){
            btn_page[i]->setText(QString::number(pageNumVal));
            if(pageNumVal==this->curPage){
                btn_page[i]->setStyleSheet(selectStyle);
            }
            else{
                btn_page[i]->setStyleSheet(normalStyle);
            }
            btn_page[i]->setVisible(true);
        }
        else{
            btn_page[i]->setVisible(false);
        }
    }
}


