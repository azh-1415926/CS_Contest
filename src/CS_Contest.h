#pragma once
#include "ui_CS_Contest.h"
#include <QMainWindow>
#include <QMouseEvent>

#include "widgetOfStart.h"
#include "widgetOfSearch.h"
#include "widgetOfAbout.h"
#include "widgetOfMore.h"

class CS_Contest : public QMainWindow
{
    Q_OBJECT

    private:
        Ui_CS_Contest* ui;
        int flagOfClick;
        widgetOfStart* windowOfStart;
        widgetOfSearch* windowOfSearch;
        widgetOfAbout* windowOfAbout;
        widgetOfMore* windowOfMore;

    public:
        explicit CS_Contest(QWidget* parent = nullptr);
        ~CS_Contest();

    protected:
        void mousePressEvent(QMouseEvent* e) override;
        void closeEvent(QCloseEvent* e) override;

    public slots:
        void updateTime();
        void clickPoint(const QPoint& p);

    private:
        void initalTimer();
};