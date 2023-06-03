#ifndef MYLISTWIDGETITEM_H
#define MYLISTWIDGETITEM_H

#include <QLayout>
#include <QLabel>
#include <QDateTime>
#include <QPixmap>
#include <QMouseEvent>

#include <QObject>

class MyListWidgetItem : public QWidget
{
    Q_OBJECT
public:

    MyListWidgetItem( QPixmap pixIcon, const QString &title, QWidget *parent = nullptr);
    ~MyListWidgetItem();
    void setMessShort(const QString &mess, QDateTime date);
    void setMessCountUnread(int count);
    QString getUserName();
    QPixmap getUserPixmap();

signals:
    void onWidgetClicked(QString user_name,QPixmap pictureOfUser);

private:
    void setInterfaceStyle();

    QHBoxLayout *mainHorizLayout;
    QVBoxLayout *titleMessLayout;
    QVBoxLayout *dataCountMessLayout;

    QPixmap picture;
    QLabel *labelIcon;
    QLabel *labelTitle;
    QLabel *labelMess;
    QLabel *labelData;
    QLabel *labelCountMess;

    QString user_name;
  //  bool already_clicked;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);


    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);


};

#endif // MYLISTWIDGETITEM_H
