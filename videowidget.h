#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H
#include<QVideoWidget>

class VideoWidget:public QVideoWidget
{
public:
    VideoWidget(QWidget *parent = 0);
protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
};

#endif // VIDEOWIDGET_H
