#ifndef PAGETABWIDGET_H
#define PAGETABWIDGET_H

#include <QWidget>
#include <QScrollArea>

class PageTabWidget : public QScrollArea
{
public:
    PageTabWidget(QWidget *parent = nullptr);
private:
    QPixmap  _scrollAreaBg;
protected:
    void paintEvent (QPaintEvent *);
};

#endif // PAGETABWIDGET_H
