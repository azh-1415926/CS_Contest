#pragma once
#include "ui_widgetOfMore.h"
#include <QWidget>

class widgetOfMore : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfMore* ui;

    public:
        explicit widgetOfMore(QWidget* parent=nullptr);
        ~widgetOfMore();

    private:
        ;
};