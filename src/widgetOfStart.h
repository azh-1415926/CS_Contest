#pragma once
#include "ui/ui_widgetOfStart.h"
#include <QWidget>

class widgetOfStart : public QWidget {
    Q_OBJECT

    public:
        explicit widgetOfStart(QWidget* parent=nullptr);
        ~widgetOfStart();

    private:
        Ui_widgetOfStart* ui;
};