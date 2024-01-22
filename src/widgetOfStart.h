#pragma once

#include "ui_widgetOfStart.h"

#include <QWidget>

class excelReader;

class widgetOfStart : public QWidget
{
    Q_OBJECT

    private:
        Ui_widgetOfStart *ui;
        /* 读取题库文件模块，初始化比较慢，不要将初始化步骤写在构造函数内，会拖慢 ui 的显示 */
        excelReader *reader;
        /* 当前类型的题目(答题页) */
        int currTypeOfQuestion;
        /* 窗口初始化的标志 */
        int flagOfInital;
        /* 题库文件路径 */
        QString pathOfExcel;
        /* 答题(答题页)进度 */
        QList<int> progressOfQuestion;
        /* 答题(收藏页)进度 */
        QList<QPair<int,int>> progressOfCollection;
        /* 题目类型对应下标 */
        QList<QPair<QString,QList<int>>> questionType;

    public:
        explicit widgetOfStart(QWidget *parent = nullptr);
        ~widgetOfStart();

    protected:
        /* 窗口调整大小时调用，窗口第一次显示的时候也会调用 */
        void resizeEvent(QResizeEvent*) override;

    public slots:
        /* 获取题库文件路径 */
        void getPath();
        /* 导出、导入配置文件 */
        void exportSetting(const QString& fileName="settings.json");
        void importSetting(const QString& fileName="settings.json");
        /* 处理题库数据 */
        void handleData();
        /* 设置当前题型(答题页) */
        void setQuestionType(int i);
        /* 刷新题目，type 可为 Question、Collection */
        void showProblem(const QString& type,int i);

    signals:
        /* 读取 excel 文件 */
        void loadExcel(const QString& pathOfExcel);
        /* 读取 csv 文件 */
        void loadCSV(const QString& pathOfCSV);
        /* 题库读取完毕 */
        void ready();

    private:
        /* 初始化多级窗口 */
        void initalStackWindow();
        /* 初始化答题页窗口 */
        void initalQuestionPage();
        /* 初始化选择页窗口 */
        void initalSelectionPage();
        /* 初始化收藏页窗口 */
        void initalCollectionPage();
        /* 读取指定题库文件路径的配置文件，即去掉后缀名加上 .json */
        void loadSetting(const QString& path);
        /* 重置收藏页 */
        void resetCollection();
        /* 获取指定下标的题目文本、选项文本、答案选项 */
        QPair<QPair<QString,QStringList>,int> getInfoOfProblem(int index);
        /* 判断是否应该跳转下一题，i 为当前选择的下标，isHit 为是否命中正确答案，命中两次则返回 true */
        bool needToNext(const QString& id,int i,bool isHit);
};