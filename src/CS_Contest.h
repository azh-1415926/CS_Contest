#pragma once
#include "ui_CS_Contest.h"
#include <QMainWindow>
#include <QMouseEvent>

class CS_Contest : public QMainWindow {
    Q_OBJECT
    
public:
    CS_Contest(QWidget* parent = nullptr);
    ~CS_Contest();

public slots:
    void updateTime();
    void clickPoint(const QPoint& p);

protected:
    void mousePressEvent(QMouseEvent* e) override;

private:
    Ui_CS_Contest* ui;
    int flagOfClick;
};