#include "Captcha.h"
#include <QPainter>
#include <QTime> // Qt4 用这个生成随机数

Captcha::Captcha(QWidget *parent)
    : QLabel(parent)
{
    // Qt4 必须初始化随机数种子
    //qsrand(QTime::currentTime().msec());

    setFixedSize(100, 40);
    setCursor(Qt::PointingHandCursor);
    refreshCaptcha();
}

// Qt4 版随机验证码生成
QString Captcha::generateCaptcha()
{
    const QString chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    QString captcha;
    for (int i = 0; i < 4; ++i) {
        int index = randomInt() % chars.size();
        captcha.append(chars.at(index));
    }
    return captcha;
}

int Captcha::randomInt()
{
    // 静态随机数引擎（仅初始化一次，避免重复种子）
    static std::mt19937 engine(std::random_device{}());
    // 0-32768之间
    std::uniform_int_distribution<int> dist(0,RAND_MAX);
    return dist(engine);
}



void Captcha::refreshCaptcha()
{
    m_captcha = generateCaptcha();
    update();
}

QString Captcha::getCaptcha() const
{
    return m_captcha;
}

void Captcha::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        refreshCaptcha();
    }
    QLabel::mousePressEvent(event);
}

void Captcha::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 背景
    painter.fillRect(rect(), QColor(240, 240, 240));

    // 干扰线
    for (int i = 0; i < 3; ++i) {
        painter.setPen(QColor(randomInt()%256, randomInt()%256, randomInt()%256));
        painter.drawLine(QPoint(randomInt()%width(), randomInt()%height()),
                         QPoint(randomInt()%width(), randomInt()%height()));
    }

    // 噪点
    for (int i = 0; i < 30; ++i) {
        painter.setPen(QColor(randomInt()%256, randomInt()%256, randomInt()%256));
        painter.drawPoint(randomInt()%width(), randomInt()%height());
    }

    // 文字
    QFont font;
    font.setPointSize(18);
    font.setBold(true);
    painter.setFont(font);

    for (int i = 0; i < m_captcha.size(); ++i) {
        painter.setPen(QColor(randomInt()%100+80, randomInt()%100+80, randomInt()%100+80));
        painter.drawText(10 + i * 22, 28, QString(m_captcha.at(i)));
    }
}
