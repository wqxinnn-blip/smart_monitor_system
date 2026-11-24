#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
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
#include "captcha.h"
#include "../util/globaluser.h"
#include "../service/operlogservice.h"

class LoginWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton *btn_login,*btn_reg,*btn_cancel,*btn_show;
    QLineEdit *edit_username,*edit_password,*edit_verCode;
    QLabel *lab_nameTip,*lab_pwdTip;
    Captcha *captcha;
    bool isShow=false; // 明/密文切换
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    void init_qss();
    void login();
    void init_connect();
protected:
    void resizeEvent(QResizeEvent *event);
signals:
};

#endif // LOGINWIDGET_H
