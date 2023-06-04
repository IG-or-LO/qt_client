#include "mylistwidgetitem.h"

#include <QPainter>

#include <QDebug>

MyListWidgetItem::MyListWidgetItem(QPixmap pixIcon, const QString &text,QWidget *parent)
{
    labelTitle=new QLabel;
    labelMess=new QLabel;
    labelData=new QLabel;
    labelCountMess=new QLabel;
    labelIcon=new QLabel;

    picture=pixIcon.scaled(50,50,Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    user_name=text;


    setInterfaceStyle();


}

MyListWidgetItem::~MyListWidgetItem()
{

}

void MyListWidgetItem::setInterfaceStyle()
{
    mainHorizLayout=new QHBoxLayout;
    titleMessLayout=new QVBoxLayout;
    dataCountMessLayout=new QVBoxLayout;

    mainHorizLayout->setSpacing(0);
    titleMessLayout->setSpacing(0);
    dataCountMessLayout->setSpacing(0);

    mainHorizLayout->setContentsMargins(0,0,0,5);
    titleMessLayout->setContentsMargins(5,0,0,0);
    dataCountMessLayout->setContentsMargins(0,0,0,0);

    labelTitle->setText(user_name);
    labelTitle->setStyleSheet("font-size:15px; color:black; font-weight:bold;");
    labelTitle->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    labelMess->setStyleSheet("font-size:13px; color:grey; ");

    labelData->setStyleSheet("font-size:13px; color:grey;");
    labelData->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    labelCountMess->setStyleSheet("font-size:13px; color:white; border-radius:3px; background-color:grey;");
    labelCountMess->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelCountMess->setLayoutDirection(Qt::RightToLeft);

    labelIcon->setStyleSheet("border-radius:25px; "
                             "border:none;"
                           //  "border-style: transparent;"
                            " max-width:50px;"
                           "  max-height:50px;"
                            " min-width:50px;"
                            " min-height:50px;");
   // labelIcon->setFixedSize(70,70);

    titleMessLayout->addWidget(this->labelTitle);
    titleMessLayout->addWidget(this->labelMess);

    dataCountMessLayout->addWidget(this->labelData);
    dataCountMessLayout->addWidget(this->labelCountMess);

    mainHorizLayout->addWidget(labelIcon);

    mainHorizLayout->addLayout(titleMessLayout);
    mainHorizLayout->addLayout(dataCountMessLayout);

    //чтобы не двигался тайтл с текстом
    mainHorizLayout->insertStretch(2,0);

    this->labelMess->setMaximumWidth(200);
    //

    this->setObjectName("main");
    this->setStyleSheet("QWidget#main::hover{background-color: rgba(193, 196, 187,1); border:none;}");

    this->setLayout(mainHorizLayout);

}

void MyListWidgetItem::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setBrush(picture);
    p.drawEllipse(this->labelIcon->rect());
}

void MyListWidgetItem::mousePressEvent(QMouseEvent *event)
{

    if ( event->button() == Qt::LeftButton) //&& !already_clicked)
    {
        //already_clicked=true;
        //this->setStyleSheet("QWidget#main{background-color: rgba(193, 196, 187,1);");
        emit onWidgetClicked();
    }
}


void MyListWidgetItem::setMessShort(const QString &mess,  QDateTime date)
{

    labelMess->setText(mess);
    labelData->setText(date.toString("H:mm"));
}

void MyListWidgetItem::setMessCountUnread(int count)
{
    if(count==0)
    {
      labelCountMess->setStyleSheet("background-color:transparent;");
      labelCountMess->setText("");
    }
    else
    {
        labelCountMess->setStyleSheet("background-color:grey; color:white;border-radius:3px;");
        labelCountMess->setText(QString::number(count));
    }
}


QString MyListWidgetItem::getUserName()
{
    return user_name;
}

QPixmap MyListWidgetItem::getUserPixmap()
{
    return picture;
}

