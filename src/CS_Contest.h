#pragma once
#include "ui/ui_CS_Contest.h"
#include <QMainWindow>
#include <QMouseEvent>

#include "widgetOfStart.h"
#include "widgetOfSearch.h"
#include "widgetOfAbout.h"
#include "widgetOfMore.h"

class CS_Contest : public QMainWindow {
    Q_OBJECT
    
public:
    explicit CS_Contest(QWidget* parent = nullptr);
    ~CS_Contest();

protected:
    void mousePressEvent(QMouseEvent* e) override;

public slots:
    void updateTime();
    void clickPoint(const QPoint& p);

private:
    Ui_CS_Contest* ui;
    int flagOfClick;
    widgetOfStart* windowOfStart;
    widgetOfSearch* windowOfSearch;
    widgetOfAbout* windowOfAbout;
    widgetOfMore* windowOfMore;
    void initalTimer();
};