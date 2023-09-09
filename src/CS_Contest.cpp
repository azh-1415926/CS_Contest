#include "CS_Contest.h"

#include <QTimer>
#include <QTime>
#include <QDateTime>

CS_Contest::CS_Contest(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_CS_Contest)
    , windowOfStart(new widgetOfStart)
    , windowOfSearch(new widgetOfSearch)
    , windowOfAbout(new widgetOfAbout)
    , windowOfMore(new widgetOfMore)
{
    ui->setupUi(this);
    /* 初始化窗口 */
    initalWindow();
    /* 初始化计时器 */
    initalTimer();
}

CS_Contest::~CS_Contest()
{
    delete ui;
    /* 释放各个窗口 */
    delete windowOfStart;
    delete windowOfSearch;
    delete windowOfAbout;
    delete windowOfMore;
}

/* 重写窗口关闭事件，关闭主窗口前，关闭所有子窗口 */
void CS_Contest::closeEvent(QCloseEvent *e)
{
    /* 关闭主窗口前，关闭所有子窗口 */
    windowOfStart->close();
    windowOfSearch->close();
    windowOfAbout->close();
    windowOfMore->close();
}

/* 更新主界面的时间显示 */
void CS_Contest::updateTime()
{
    /* 获取当前时间 */
    QDateTime time=QDateTime::currentDateTime();
    /* 转化为 "yyyy-MM-dd" 格式的字符串，效果为 2023-01-01 */
    QString timeOfYMD=time.toString("yyyy-MM-dd");
    /* 转化为 "hh:mm:ss" 格式的字符串，效果为 06:30:01 */
    QString timeOfHMS=time.toString("hh:mm:ss");
    /* 将时间展示到主界面 */
    ui->timeOfYMD->display(timeOfYMD);
    ui->timeOfHMS->display(timeOfHMS);
}

/* 初始化窗口 */
void CS_Contest::initalWindow()
{
    /* ui 文件里默认标题为 xxx，设置标题文本为 "知识竞赛答题" */
    ui->textOfTitle->setText("知识竞赛答题");
    /* 插入显示文本、对应窗口到 windows 中 */
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfStart,windowOfStart));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfSearch,windowOfSearch));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfAbout,windowOfAbout));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfMore,windowOfMore));
    for(int i=0;i<windows.length();i++)
    {
        connect(windows[i].first,&clickLabel::clicked,this,[=](const clickLabel* label)
        {
            /* 若该区域对应窗口隐藏，便将其显示 */
            if(windows[i].second->isHidden())
                windows[i].second->show();
            return;
        });
    }
}

/* 初始化计时器，更新主界面时间显示 */
void CS_Contest::initalTimer()
{
    /* 将 updateTime 绑定到 timer 上，接收到 timeout 信号立即更新时间，并重置鼠标点击的标志位 */
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
        updateTime();
    });
    /* 启动定时器 */
    timer->start(1000);
}