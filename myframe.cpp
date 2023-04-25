#include "myframe.h"

#include <QDebug>
MyFrame::MyFrame(QWidget *parent)
{
_scrollAreaBg.load(":/new/img/fon_information.jpg");
}
void MyFrame::paintEvent(QPaintEvent *e) {
    QWidget::paintEvent(e);

    QPainter p(this);

    p.drawPixmap(0,0,
                 this->width(),this->height(),_scrollAreaBg);
}
