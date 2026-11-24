#ifndef THUMBLABEL_H
#define THUMBLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class ThumbLabel : public QLabel
{
    Q_OBJECT
private:
    //拖动参数
    QString imgPath; //图片路径
    QPointF m_pressPos; //记录点击时的坐标
    bool isMoving=false; //是否在移动  保证是鼠标左键触发的
    //缩放参数 原始QPixmap 缩放比例  最大最小比例
    QPixmap orignalPix;
    double scaleFactor=1.0;
    const double minScale=0.3;
    const double maxScale=3;
    //原图的尺寸
    int orignalW;
    int orignalH;
    double scale; //原图和label的相差倍数
    //两个偏移量
    double offset_x=0.0;
    double offset_y=0.0;
    //拖动限制
    double drag_offset_x=0.0;
    double drag_offset_y=0.0;
public:
    ThumbLabel();
    //获取图片 进行修改
    void setOriginPix(const QPixmap &pix);
    //初始化符合label大小的图片
    bool accptDrag();
protected:
    void wheelEvent(QWheelEvent *event);//滚轮监听
    void paintEvent(QPaintEvent *event);
    void updateScaled();
    //拖动
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
signals:
    void clickLeft();
};

#endif // THUMBLABEL_H
