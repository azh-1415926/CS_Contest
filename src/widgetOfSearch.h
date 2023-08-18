#pragma once
#include "ui_widgetOfSearch.h"
#include <QWidget>

class widgetOfSearch : public QWidget {
    Q_OBJECT

    public:
        explicit widgetOfSearch(QWidget* parent=nullptr);
        ~widgetOfSearch();

    private:
        Ui_widgetOfSearch* ui;
};