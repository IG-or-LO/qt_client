#include "messegeswidget.h"
#include <QLabel>
#include <QLayout>
#include <QStyleOption>
#include <QPainter>

#include <QDebug>


#include <QMenu>
#include <QClipboard>
#include <QApplication>

MessegesWidget::MessegesWidget(QWidget *parent)
{
    WidgetID++;
    messWidgetID=WidgetID;

    frameMess= new QFrame(this);

    labelIcon=new QLabel;
    labelText=new QLabel;
    labelData=new QLabel;
    labelTitle=new QLabel;
    labelStatus=new QLabel;

    horizLayoutMessage=new QHBoxLayout;
    verticalLayoutMessage=new QBoxLayout(QBoxLayout::BottomToTop);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    //conncetcions
    connect(this,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(slotCustomMenuRequested(QPoint)));
    //
}

MessegesWidget::~MessegesWidget()
{

}

int MessegesWidget::getID()
{
    return messWidgetID;
}

void MessegesWidget::setWidgetDisign(bool inORoutMessage)
{
    if(inORoutMessage)
    {
         horizLayoutMain=new QBoxLayout(QBoxLayout::LeftToRight);
         frameMess->setStyleSheet("border:none; border-radius:10px;  "
                                  "background-color: #effdde");
        horizLayoutMain->addWidget(frameMess);
        horizLayoutMain->addWidget(labelIcon);
    }

    else
    {
         horizLayoutMain=new QBoxLayout(QBoxLayout::RightToLeft);
         frameMess->setStyleSheet("border:none; border-radius:10px;  "
                                  "background-color: #ffffff");

        horizLayoutMain->addWidget(labelIcon);
        horizLayoutMain->addWidget(frameMess);
    }

    horizLayoutMain->addStretch(-1);

    horizLayoutMain->setSpacing(0);
    horizLayoutMain->setContentsMargins(0,0,0,0);

    horizLayoutMessage->setSpacing(0);
    horizLayoutMessage->setContentsMargins(0,0,0,0);

    verticalLayoutMessage->setSpacing(0);
    verticalLayoutMessage->setContentsMargins(0,0,0,0);

    labelIcon->setFixedSize(40,40);
    labelIcon->setStyleSheet( "border-radius:10px;" );
    labelIcon->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    labelText->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
    labelText->setStyleSheet(" color:black; background:none; font-size:15px;"
                                "border:none; font-family: 'Roboto-Medium';");
    labelText->setWordWrap(true);

    labelData->setFixedSize(60,30);
    labelData->setStyleSheet("color:blue; font-size:10px; font-family: 'Roboto-Medium'; "
                             "background:none; border:none; font-weight:bold;");
    labelData->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);


    labelTitle->setBackgroundRole(QPalette::WindowText);
    labelTitle->setStyleSheet("color:blue; font-size:15px; font-family: 'Roboto-Medium';"
                              "border:5px solid black; font-weight:bold;background:none; ");
    labelTitle->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    labelTitle->setWordWrap(true);

    labelStatus->setFixedSize(50,30);
   // labelStatus->setStyleSheet("border:none; background:none;");

    //установка layoutов и виджетов в них
    frameMess->setLayout(verticalLayoutMessage);
    verticalLayoutMessage->addLayout(horizLayoutMessage);
    horizLayoutMessage->addWidget(labelText);
    horizLayoutMessage->addWidget(labelData);


    this->setLayout(horizLayoutMain);
}

void MessegesWidget::addWidgetTitle(QString title)
{
    verticalLayoutMessage->addWidget(labelTitle);
    labelTitle->setText(title);
}

void MessegesWidget::setWidgetStatus(bool ReadOrNot)
{
    QPixmap read;
    if(ReadOrNot)
        read = QPixmap(":/res/mess_read.png").scaled(30,30);
    else
      //  read = QPixmap(":/res/mess_notRead.png").scaled(30,30);
        read = QPixmap(":/new/img/smile.svg").scaled(30,30);

    labelStatus->setPixmap(read);
    horizLayoutMessage->addWidget(labelStatus);
}

void MessegesWidget::addWidgetMessIconData(QString mess, QPixmap pixMess, QDateTime dateTIme)
{
    _message=mess;
    labelText->setText(_message);
   // pixMess.scaled(40,40);
    labelIcon->setPixmap(pixMess);
    labelData->setText(dateTIme.time().toString(Qt::SystemLocaleShortDate));
    labelData->setToolTip(dateTIme.toString());

}

void MessegesWidget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MessegesWidget::slotCopyText()
{
    QClipboard *_clip=QApplication::clipboard()  ;
    _clip->setText(_message,QClipboard::Clipboard);
}

void MessegesWidget::slotDeleteMess()
{
    this->deleteLater();
}

void MessegesWidget::slotCustomMenuRequested(QPoint pos)
{
  QMenu *menu=new QMenu(this);
  menu->setStyleSheet("QMenu{background-color:white; border-radius:5px;}"
                      "QMenu::item:selected{background-color:grey;}");

  QAction *actCopy=new QAction("Копировать текст",this);
  QAction *actDelete=new QAction("Удалить",this);


  actCopy->setIcon(QIcon(":/res/copy_icon.png"));
  actDelete->setIcon(QIcon(":/res/delete_icon.png"));

    connect(actCopy,SIGNAL(triggered()),this,SLOT(slotCopyText()));
    connect(actDelete,SIGNAL(triggered()),this,SLOT(slotDeleteMess()));

    menu->addAction(actCopy);
    menu->addAction(actDelete);

    menu->popup(this->mapToGlobal(pos));
}

int MessegesWidget::WidgetID=0;
