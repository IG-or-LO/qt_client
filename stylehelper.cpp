#include "stylehelper.h"



QString stylehelper::getTextBrowserStyle()
{
    return "QFrame{"
            "background:none;"
            "background-color: qlineargradient(spread:reflect, x1:1, y1:0.494409, x2:1, y2:0.983, stop:0 rgba(180, 41, 170, 255), stop:0.995 rgba(0, 0, 63, 255));"
            "background-attachment:scroll;"
//            "color:white;"
//            "font-size:20px;"
//            "font-family: 'Roboto-Medium';"
//            "padding:10px 10px 10px 10px;"
//            "}"
//            "QFrame::item{"
//            "border: 5px solid black"
//           "}"
//            "#outtext{"
//           "float:right;"
//           "color:gold;"
            "}"
            ;
}

QString stylehelper::getListWidgetStyle()
{
    return "QListWidget{"
            "border:none;"
           "}";
}

QString stylehelper::getTopFrameStyle()
{
    return "QFrame{"
           "border: none;"
           "background-color: transparent;"
           "}";
}

QString stylehelper::getButtonsStyle()
{
    return "QPushButton{"
           "border:none;"
            "}"
            "QPushButton::hover{"
            "background:#961f99;"
            "}";
}

QString stylehelper::getTabsStyle()
{
    return "QWidget#enter {"
           " border-image: url(:/new/img/registration) 0 0 0 0 stretch stretch;"
           "} "
            "QWidget#reg {"
           " border-image: url(:/new/img/registration) 0 0 0 0 stretch stretch;"
           "} "
            "QWidget#perinfo {"
                       " border-image: url(:/new/img/fon_information.jpg) 0 0 0 0 stretch stretch;"
                       "} "
            "QWidget#info {"
                       " border-image: url(:/new/img/fon_information.jpg) 0 0 0 0 stretch stretch;"
                       "} "
             "QWidget#about {"
                              " border-image: url(:/new/img/fon_information.jpg) 0 0 0 0 stretch stretch;"
                              "} ";
}




QString stylehelper::getLabelsStyle()
{
    return "QLabel{"
            "color:#fff;"
            "background: none;"
            "border: none;"
            "border-radius:2px;"
            "font-family: 'Roboto-Medium';"
            "font-size:20px;"
           "}";
}

QString stylehelper::getButtonsRegAndEnterStyle()
{
    return
            "QPushButton{"
      "color: #fff;"
      "border-radius: 5px;"
      "padding: 10px 25px;"
      "font-family: 'Roboto-Medium';"
      "font-weight: 500;"
      "background: transparent;"
      "position: relative;"
      "outline: none;"
    "}"

     "QPushButton{"
      "background: #b621fe;"
      "border: none;"
    "}"


    "QPushButton::hover {"
      "background:qlineargradient(spread:pad, x1:0.99, y1:0, x2:0.035, y2:1, stop:0 rgba(240, 0, 172, 255), stop:1 rgba(26, 126, 164, 255));"
      "color: #fff;"
    "}";
}
QString stylehelper::getTabWidgetStyle()
{
    return "QTabWidget{"
           "border: none;"
            "background: none;"//test
            "}"
            "QTabBar::tab {"
            "border:none;"
            "background:none;"//test
            "}";
}
