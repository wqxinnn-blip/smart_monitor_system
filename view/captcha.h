#ifndef CAPTCHA_H
#define CAPTCHA_H

#include <QLabel>
#include <QMouseEvent>
#include <random>

class Captcha : public QLabel
{
    Q_OBJECT
public:
    explicit Captcha(QWidget *parent = nullptr);

    QString getCaptcha() const;
    void refreshCaptcha();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString generateCaptcha();
    QString m_captcha;
    int randomInt();//生成随机数
};

#endif // CAPTCHA_H
