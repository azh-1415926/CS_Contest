#pragma once
#include "ui_CS_Contest.h"
#include <QMainWindow>

#include "widgetOfStart.h"
#include "widgetOfSearch.h"
#include "widgetOfAbout.h"
#include "widgetOfMore.h"

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
        void closeEvent(QCloseEvent* e) override;

    public slots:
        void updateTime();

    private:
        void initalWindow();
        void initalTimer();
};