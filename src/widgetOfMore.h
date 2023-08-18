#pragma once
#include "ui_widgetOfMore.h"
#include <QWidget>

class widgetOfMore : public QWidget {
    Q_OBJECT

    public:
        explicit widgetOfMore(QWidget* parent=nullptr);
        ~widgetOfMore();

    private:
        Ui_widgetOfMore* ui;
};