#pragma once
#include "ui_widgetOfStart.h"

#include <QWidget>

#include "excelReader.h"

class widgetOfStart : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfStart *ui;
        excelReader *reader;
        int currTypeOfQuestion;
        int flagOfInital;
        QString pathOfExcel;
        QList<int> progressOfQuestion;
        QList<QPair<int,int>> progressOfCollection;
        QList<QPair<QString,QList<int>>> questionType;

    public:
        explicit widgetOfStart(QWidget *parent = nullptr);
        ~widgetOfStart();

    protected:
        void resizeEvent(QResizeEvent*) override;

    public slots:
        void getPath();
        void exportSetting();
        void importSetting();
        void handleData();
        void setQuestionType(int i);
        void showQuestionByIndex(int i);
        void showCollectionByIndex(int i);

    signals:
        void loadExcel(const QString& pathOfExcel);
        void loadCSV(const QString& pathOfCSV);
        void ready();
        void updateTextOfQuestion(int i,const QString& text);
        void updateTextOfCollection(int i,const QString& text);
        void updateIndexOfQuestion(int i);
        void updateIndexOfCollection(int i);
        void updateIndexOfCollection(const QString& text);
        void updateSumOfQuestion(int n);
        void updateSumOfCollection(int n);
        void updateAnswerOfQuestion(int i);
        void updateAnswerOfCollection(int i);
        void isCollect(bool iscollected);

    private:
        void initalStackWindow();
        void initalQuestionPage();
        void initalSelectionPage();
        void initalCollectionPage();
        void resetCollection();
};