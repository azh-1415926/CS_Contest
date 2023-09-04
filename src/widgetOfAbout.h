#pragma once
#include "ui_widgetOfAbout.h"
#include <QWidget>
#include <QLabel>

class widgetOfAbout : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfAbout* ui;
        QLabel* info;
        QPixmap pixmap;

    public:
        explicit widgetOfAbout(QWidget* parent=nullptr);
        ~widgetOfAbout();

    protected:
        void resizeEvent(QResizeEvent* e) override;

    private:
        void initalWindow();
};