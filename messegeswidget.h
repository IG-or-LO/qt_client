#ifndef MESSEGESWIDGET_H
#define MESSEGESWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QLabel>
#include <QLayout>
#include <QDateTime>



class MessegesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MessegesWidget(QWidget *parent = nullptr);

    ~MessegesWidget();
    static int WidgetID;
    int getID();
    void setWidgetDisign(bool inORoutMessage);
    void addWidgetTitle(QString title);
    void setWidgetStatus(bool ReadOrNot);
    void addWidgetMessIconData(QString mess,QPixmap pixMess,QDateTime dateTIme );

private:
    QBoxLayout *horizLayoutMain;
    QHBoxLayout *horizLayoutMessage;
    QBoxLayout *verticalLayoutMessage;



    QFrame *frameMess;
    QLabel *labelText;
    QLabel *labelIcon;
    QLabel *labelData;
    QLabel *labelTitle;
    QLabel *labelStatus;
    QPixmap _pixMess;
    QString _message;
    int messWidgetID=0;



protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void slotCopyText();
    void slotDeleteMess();
    void slotCustomMenuRequested(QPoint pos);



};

#endif // MESSEGESWIDGET_H
