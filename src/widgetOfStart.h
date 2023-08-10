#pragma once
#include "ui/ui_widgetOfStart.h"
#include <QWidget>
#include <QEvent>
#include "excelReader.h"

class widgetOfStart : public QWidget {
    Q_OBJECT

    public:
        explicit widgetOfStart(QWidget* parent=nullptr);
        ~widgetOfStart();

    protected:
        bool eventFilter(QObject* obj,QEvent* e) override;

    private:
        Ui_widgetOfStart* ui;
        QRect* border;
        excelReader* reader;
        void paintBorder(QWidget* widget);
        void clickRadioButton(clickLabel* label);
        const QString& getPath();
};