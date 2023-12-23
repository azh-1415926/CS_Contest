#pragma once

#include "ui_widgetOfStart.h"

#include <QWidget>

class excelReader;

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
        void exportSetting(const QString& fileName="settings.json");
        void importSetting(const QString& fileName="settings.json");
        void handleData();
        void setQuestionType(int i);
        void showProblem(const QString& type,int i);

    signals:
        void loadExcel(const QString& pathOfExcel);
        void loadCSV(const QString& pathOfCSV);
        void ready();
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
        void loadSetting(const QString& path);
        void resetCollection();
        QPair<QPair<QString,QStringList>,int> getInfoOfProblem(int index);
        bool needToNext(const QString& id,int i,bool isHit);
};