#pragma once
#include "ui_widgetOfAbout.h"
#include <QWidget>

class widgetOfAbout : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfAbout* ui;

    public:
        explicit widgetOfAbout(QWidget* parent=nullptr);
        ~widgetOfAbout();

    private:
        void initalWindow();
};