#pragma once

#include "ui_CS_Contest.h"

#include <QMainWindow>

class widgetOfStart;
class widgetOfSearch;
class widgetOfAbout;
class widgetOfMore;

class CS_Contest : public QMainWindow
{
    Q_OBJECT

    private:
        /* ui 控件 */
        Ui_CS_Contest* ui;
        /* 四个子窗口 */
        widgetOfStart* windowOfStart;
        widgetOfSearch* windowOfSearch;
        widgetOfAbout* windowOfAbout;
        widgetOfMore* windowOfMore;
        /* clickLabel 和子窗口一一对应的列表 */
        QList<QPair<clickLabel*,QWidget*>> windows;

    public:
        explicit CS_Contest(QWidget* parent = nullptr);
        ~CS_Contest();

    protected:
        /* 事件过滤器 */
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        /* 刷新屏幕右上角显示的时间 */
        void updateTime();

    private:
        /* 初始化窗口，连接各个控件的信号和槽 */
        void initalWindow();
        /* 初始化计时器，用于定时刷新时间 */
        void initalTimer();
        /* 显示指定的 clickLabel 对应的窗口 */
        void showWindow(const clickLabel* label);
};