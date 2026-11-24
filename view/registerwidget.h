#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QFont>
#include <QPixmap>
#include <QString>
#include <QPalette>
#include <QMessageBox>
#include <QFile>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include "../util/dbconnect.h"
#include "../service/userservice.h"

class RegisterWidget : public QWidget
{
    Q_OBJECT
private:
    QLineEdit *edit_username,*edit_pwd,*edit_rePwd;
    QPushButton *btn_enter,*btn_cancel;
public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    void init_qss();
    void init_connect();
    void reg();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
};

#endif // REGISTERWIDGET_H
