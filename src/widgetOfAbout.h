#pragma once
#include "ui/ui_widgetOfAbout.h"
#include <QWidget>

class widgetOfAbout : public QWidget {
    Q_OBJECT

    public:
        explicit widgetOfAbout(QWidget* parent=nullptr);
        ~widgetOfAbout();

    private:
        Ui_widgetOfAbout* ui;
};