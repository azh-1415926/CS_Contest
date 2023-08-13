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

    protected:
        void resizeEvent(QResizeEvent*) override;

    public slots:
        void getPath();
        void saveSetting();
        void loadSetting();
        void loadData();
        void selectQuestionType(int i);
        void switchPreQuestion();
        void switchNextQuestion();
        void switchPreCollection();
        void switchNextCollection();

    signals:
        void loadExcel(const QString& pathOfExcel);
        void ready();
        void updateTextOfQuestion(int i,const QString& text);
        void updateTextOfCollection(int i,const QString& text);
        void collectQuestion();
        void uncollectQuestion();
        void cancelCollection();
        void uncancelCollection();

    private:
        Ui_widgetOfStart *ui;
        excelReader *reader;
        int currTypeOfQuestion;
        int currIndexOfQuestion;
        int currIndexOfCollection;
        int flagOfInital;
        QString pathOfExcel;
        QList<int> progressOfQuestion;
        QList<QPair<int,int>> progressOfCollection;
        QList<QPair<QString,QList<int>>> questionType;
        void initalStackWindow();
        void initalQuestionPage();
        void initalSelectionPage();
        void initalCollectionPage();
        void switchQuestionByIndex(int i);
        void switchCollectionByIndex(int i);
};