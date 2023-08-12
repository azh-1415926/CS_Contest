#pragma once
#include "ui/ui_widgetOfStart.h"
#include <QWidget>
#include "excelReader.h"

class widgetOfStart : public QWidget
{
    Q_OBJECT

    public:
        explicit widgetOfStart(QWidget *parent = nullptr);
        ~widgetOfStart();

    public slots:
        void getPath();
        void saveSetting();
        void loadData();
        void selectQuestionType(int i);
        void switchPreQuestion();
        void switchNextQuestion();

    signals:
        void loadExcel(const QString& pathOfExcel);
        void ready();
        void updateTextOfOption(int i,const QString& text);

    private:
        Ui_widgetOfStart *ui;
        excelReader *reader;
        int currQuestionType;
        int currQuestionIndex;
        int currAnswer;
        QString pathOfExcel;
        QList<int> progress;
        QList<QPair<QString,QList<int>>> questionType;
        void initalStackWindow();
        void initalQuestionPage();
        void initalSelectPage();
        void switchQuestionByIndex(int i);
        void loadSetting();
};