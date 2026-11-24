#include "thumblabel.h"
#include "qevent.h"

ThumbLabel::ThumbLabel() {}

void ThumbLabel::wheelEvent(QWheelEvent *event)
{
    //1. 获取当前鼠标的位置
    QPointF nowPos=event->position();
    //2.换算出原图坐标--锁定鼠标的位置
    QPointF imgPoint((nowPos.x()-offset_x)/scaleFactor,(nowPos.y()-offset_y)/scaleFactor);
    //3.更新缩放比例
    //2.获取滚轮操作
    int delta=event->angleDelta().y();
    //2.1根据返回的int型 变量 判断
    //放大
    if(delta>0){
        scaleFactor *=1.1;
        //qDebug()<<"滚轮向上";
    }
    //2.2 缩小
    else if(delta<0){
        scaleFactor /=1.1;
        //qDebug()<<"滚轮向下";
    }
    scaleFactor=qBound(minScale,scaleFactor,maxScale);
    //4.计算画布的偏移量
    if (scaleFactor<=scale) {
        offset_x=(width()-orignalW*scaleFactor)/2.0;
        offset_y=(height()-orignalH*scaleFactor)/2.0;
    } else {
        // 按鼠标位置缩放
        offset_x=nowPos.x()-imgPoint.x()*scaleFactor;
        offset_y=nowPos.y()- imgPoint.y()*scaleFactor;
    }
    //5.更新，显示
    update();
    event->accept();
}

void ThumbLabel::paintEvent(QPaintEvent *event)
{
    //错误处理：在进入图片管理界面时 显示的图片此时不需要进行交互
    //这时 orignalPix为空 尺寸为0,也无法进行点击触发操作 没有交给基类处理
    //解决：在图片绘制前先进行判断  为空 直接调用基类 继续下发
    if (orignalPix.isNull()) {
        QLabel::paintEvent(event);
        return;
    }
    //------------------------------------
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    //计算缩放后的尺寸
    QSize scaledSize=orignalPix.size()*scaleFactor;
    //使用偏移量
    int x=offset_x;
    int y=offset_y;
    //qDebug()<<scaledSize.width()<<scaledSize.height();
    painter.drawPixmap(QRect(x, y, scaledSize.width(), scaledSize.height()), orignalPix);
    event->accept();

}

void ThumbLabel::mousePressEvent(QMouseEvent *event)
{
    //1.判断是否是鼠标左键触发
    if(event->button()==Qt::LeftButton){
        //qDebug()<<"现在的缩放倍数是:"<<scaleFactor;
        //修改标记位
        isMoving=true;
        m_pressPos=event->position();
        //qDebug()<<"鼠标点击成功";
        emit clickLeft();
    }
    QLabel::mousePressEvent(event);
}

void ThumbLabel::mouseMoveEvent(QMouseEvent *event)
{
    if(!isMoving){
        QLabel::mouseMoveEvent(event);
        return;
    }
    if(!accptDrag()){
        QLabel::mouseMoveEvent(event);
        return;
    }
    QPointF newPos=event->position();
    int x=newPos.x()-m_pressPos.x();
    int y=newPos.y()-m_pressPos.y();
    //计算放大后的尺寸
    double imgW=orignalW*scaleFactor;
    double imgH=orignalH*scaleFactor;
    //判断是否溢出
    bool w=imgW > this->width();
    bool h=imgH > this->height();
    //计算出限位的边界
    double minX=this->width()-imgW;
    double maxX=0.0;
    double minY=this->height()-imgH;
    double maxY=0.0;
    //计算新的偏移量
    double newX=offset_x+x;
    double newY=offset_y+y;
    if(w){
        newX=qMax(minX,qMin(newX,maxX));
    }
    else{
        newX=offset_x;
    }
    if(h){
        newY=qMax(minY,qMin(newY,maxY));
    }
    else{
        newY=offset_y;
    }
    offset_x=newX;
    offset_y=newY;
    m_pressPos = newPos;
    update();
    QLabel::mouseMoveEvent(event);
}

void ThumbLabel::mouseReleaseEvent(QMouseEvent *event)
{
    //重置移动标记位
    if(event->button()==Qt::LeftButton){
        isMoving=false;
    }
    QLabel::mouseReleaseEvent(event);
}

void ThumbLabel::setOriginPix(const QPixmap &pix)
{
    orignalPix=pix;
    scaleFactor = 1.0;//重置
    //记录初始图片的宽高 和倍数
    orignalH=pix.height();
    orignalW=pix.width();
    double tmp_w = (double)width()/orignalW;
    double tmp_h = (double)height()/orignalH;
    scale=qMin(tmp_w,tmp_h);  // 取最小值，图片完全可见
    //qDebug()<<"first scale:"<<scaleFactor;
    // 居中显示
    offset_x=(this->width()-orignalPix.width()*scaleFactor)/2.0;
    offset_y=(this->height()-orignalPix.height()*scaleFactor)/2.0;
    update();
}

bool ThumbLabel::accptDrag()
{
    //计算出图片当前的宽高
    double imgW=orignalW*scaleFactor;
    double imgH=orignalH*scaleFactor;
    //有一个超出边界 就运行拖拽
    return imgW>this->width() || imgH>this->height();
}
