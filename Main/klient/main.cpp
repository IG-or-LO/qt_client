#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include<QStyle>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;



    w.setGeometry(
           QStyle::alignedRect(
           Qt::LeftToRight,
           Qt::AlignCenter,
           w.size()/2,
           qApp->desktop()->availableGeometry()
       )
                );

    w.show();
    return a.exec();
}
