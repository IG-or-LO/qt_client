#ifndef MYFRAME_H
#define MYFRAME_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
class MyFrame : public QWidget
{
public:
    MyFrame(QWidget *parent = nullptr);
private:
 QPixmap   _scrollAreaBg;

protected:
  virtual void paintEvent(QPaintEvent* e);
};




#endif // MYFRAME_H
