#include "stylehelper.h"



QString stylehelper::getTextBrowserStyle()
{
    return "QFrame{"
            "background:none;"
            "background-color: qlineargradient(spread:reflect, x1:1, y1:0.494409, x2:1, y2:0.983, stop:0 rgba(180, 41, 170, 255), stop:0.995 rgba(0, 0, 63, 255));"
            "background-attachment:scroll;"
            "}"
            ;
}

QString stylehelper::getListWidgetStyle()
{
    return "QListWidget{"
            "border:none;"
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

QString stylehelper::getButtonMenuStyle()
{
    return "QToolButton{"
           "border:none;"
           "background:transparent;"
            "margin:5px;"
            "}"
            "QToolButton::pressed{"
            "  border-top: 2px solid blue;"
            "border-bottom: 2px solid blue;"
            "}"
            "QToolButton::menu-indicator{"
           " width: 0px; "
           "}";
}

QString stylehelper::getTabsStyle()
{
    return "QWidget#auth {"
           " background: #eee9fd;"
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
            "color:black;"
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

QString stylehelper::getFrameNoBorderStyle()
{
    return "QFrame{"
           "border:none;"
           "}";
}

QString stylehelper::getLineEditSearchStyle()
{
    return "QLineEdit{"
           "border:none;"
           "border-radius:5px;"
           "background:grey;"
            "margin:5px;"
            "padding:3px;"
            "color:black;"
            "}"
            "QLineEdit::focus{"
            "border:3px solid blue;"
            "background:white;"
           "}";

}

QString stylehelper::getLabelLogoStyle()
{
    return "QLabel{"
            "border-image:url(:/new/img/auth_logo.png) 0 0 0 0 stretch stretch;"
           "}";
}

QString stylehelper::getButtonConnectionStyle()
{
    return "QPushButton{"
      "border-radius: 5px;"
      "padding: 10px 25px;"
      "background: transparent;"
      "position: relative;"
      "outline: none;"
      "border-image:url(:/new/img/icon-settings(70-70).png) 0 0 0 0 stretch stretch;"
      "border: none;"
    "}"


            "QPushButton::hover {"
      "background:qlineargradient(spread:pad, x1:0.99, y1:0, x2:0.035, y2:1, stop:0 rgba(240, 0, 172, 255), stop:1 rgba(26, 126, 164, 255));"
           "}";
}

QString stylehelper::getScrollAreaStyle()
{
    return
                                  "QScrollBar:vertical{"
                                  "border:none;"
                                  "background-color:grey;"
                                  "width:7px;"
                                  "margin:5px,0,5px,0;"
                                  "border-radius:5px;"
                                  "}"
                                  //handle bar vertical
                                  "QScrollBar::handle:vertical{"
                                  "background-color: red"
                                  ";"
                                  "min-height:15px;"
                                  "border-radius:3px;"
                                  "}"
                                  "QScrollBar::handle:vertical:hover{"
                                  "background-color:blue;"
                                  "}"
                                  "QScrollBar::handle:vertical:pressed{"
                                  "background-color:maroon;"
                                  "}"
                                  //btn-top vertical
                                  "QScrollBar::sub-line:vertical{"
                                  "border:none;"
                                  "height:0px;"
                                  "}"
                                  "QScrollBar::add-line:vertical{"
                                  "border:none;"
                                  "}"
                                  "QScrollBar::up-arrow::vertical, QScrollBar::down-arrow:vertical{"
                                  "background:none;"
                                  "}"
                                  "QScrollBar::add-page::vertical, QScrollBar::sub-page:vertical{"
                                  "background:none;"
                                  "}" ;
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
