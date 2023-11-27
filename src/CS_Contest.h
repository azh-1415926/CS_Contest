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
        Ui_CS_Contest* ui;
        widgetOfStart* windowOfStart;
        widgetOfSearch* windowOfSearch;
        widgetOfAbout* windowOfAbout;
        widgetOfMore* windowOfMore;
        QList<QPair<clickLabel*,QWidget*>> windows;

    public:
        explicit CS_Contest(QWidget* parent = nullptr);
        ~CS_Contest();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    public slots:
        void updateTime();

    private:
        void initalWindow();
        void initalTimer();
        void showWindow(const clickLabel* label);
};