#pragma once

#include "ui_widgetOfAbout.h"

#include <QWidget>
#include <QLabel>

class widgetOfAbout : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfAbout* ui;
        QPixmap pixmap;

    public:
        explicit widgetOfAbout(QWidget* parent=nullptr);
        ~widgetOfAbout();

    private:
        void initalWindow();
};