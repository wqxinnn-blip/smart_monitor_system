#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QFile>
#include <QDateEdit>
#include <QCheckBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include "../service/operlogservice.h"

class LogWidget : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* center_layout;
    // 左侧垂直布局（切换按钮）
    QVBoxLayout* btn_layout;
    QPushButton* btn_OperLog;  // 操作日志
    QPushButton* btn_AlarmLog; // 异常日志

    // 右侧垂直布局（表格区域）
    QVBoxLayout* RightTable;
    QTableWidget* tableLog;

    // ========== 底部分页布局 ==========
    QHBoxLayout* page_layout;
    QPushButton* btnPrev;
    QPushButton *btn_page[3];
    QPushButton* btnNext;
    QPushButton* btnDel;

    int curPage=1;
    const int pageSize=10;
    int pageNum=1;
    int totalCount=0;
    int m_startPage=1;
    int pagination=3;

    int typeFlag=0;//默认操作日志
    bool isPage=false;

    QList<OperationInfo> operLogList;
    QList<ExceptionInfo> expLogList;
    QCheckBox *box_choose;

public:
    explicit LogWidget(QWidget *parent = nullptr);
    void init_widget();
    void init_PageLayout();
    void init_qss();
    void initConnect();
    // 布局初始化函数
    void initCenterLayout();
    void initPageLayout();
    void connectSlots();

    void refreshLogTable();
    void deleteLog();

signals:
public slots:
};

#endif // LOGWIDGET_H
