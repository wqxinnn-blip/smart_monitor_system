#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QEvent>
#include <QFont>
#include <QSet>
#include "../model/settinginfo.h"

class SettingWidget : public QWidget
{
    Q_OBJECT
private:
    //主要控件
    QLineEdit *edit_path,*edit_channelNames[4];
    QPushButton *btn_min,*btn_max,*btn_close;
    QComboBox *com_box,*channelId_box[4];
    //右上角交互
    QPushButton *btn_browse,*btn_save,*btn_cancel;
    //窗口移动
    bool isMoving=false; //配合鼠标移动事件，如果未添加标记位 会导致任意按键移动窗口即移动
    QPoint m_dragPos; //记录平面上一个点的坐标
    //区别主界面保存和直接保存的标记位
    bool isSave=false;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
public:
    explicit SettingWidget(QWidget *parent = nullptr);
    void init_widget();
    void init_qss();
    void init_connect();
signals:
};

#endif // SETTINGWIDGET_H
