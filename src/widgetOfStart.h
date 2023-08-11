#pragma once
#include "ui/ui_widgetOfStart.h"
#include <QWidget>
#include <QEvent>
#include "excelReader.h"

class widgetOfStart : public QWidget
{
    Q_OBJECT

    public:
        explicit widgetOfStart(QWidget *parent = nullptr);
        ~widgetOfStart();

    protected:
        bool eventFilter(QObject *obj, QEvent *e) override;

    public slots:
        void getPath();
        void loadData();
        void switchPreQuestion();
        void switchNextQuestion();

    signals:
        void loadExcel(const QString& pathOfExcel);

    private:
        Ui_widgetOfStart *ui;
        QRect *border;
        excelReader *reader;
        QString pathOfExcecl;
        QList<QPair<QString,QList<int>>> questionType;
        int currQustionType;
        int currQuestionIndex;
        int currAnswer;
        void initalStackWindow();
        void initalQuestionPage();
        void initalSelectPage();
        void paintBorder(QWidget *widget);
        void clickRadioButton(clickLabel *label);
        void switchQuestionByIndex(int i);
};