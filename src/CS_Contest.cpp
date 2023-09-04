#include "CS_Contest.h"

#include <QTimer>
#include <QTime>
#include <QDateTime>

CS_Contest::CS_Contest(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_CS_Contest)
    , flagOfClick(1)
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

/* 重写鼠标点击事件，点击便调用 clickPoint 函数（1 秒只能调用一次），传入当前鼠标点击坐标 */
void CS_Contest::mousePressEvent(QMouseEvent *e)
{
    /* flagOfClick 用于标记当前点击是否合法，若它为 1，才调用 clickPoint，否则无操作 */
    if(flagOfClick)
    {
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            clickPoint(QPoint(e->pos().x(),e->pos().y()));
        #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
            clickPoint(QPoint(e->position().x(),e->position().y()));
        #endif
        /* 发送完当前鼠标点击的位置后，将其置为 0，直到定时器每隔 1 秒重置该标志 */
        flagOfClick=0;
    }
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

/* 处理所点击坐标的特殊事件 */
void CS_Contest::clickPoint(const QPoint &p)
{
    /* baseOf前缀的变量用于记录父窗口（这4个区域有一个父窗口）的绝对坐标 */
    int baseOfX,baseOfY,baseOfWidth,baseOfHeight;
    int x,y,width,height;
    /* 获取父窗口（areaOfBottom）相对于主界面的坐标 */
    ui->areaOfBottom->geometry().getRect(&baseOfX,&baseOfY,&baseOfWidth,&baseOfHeight);
    /* 获取对应区域相对于父窗口的坐标 */
    for(int i=0;i<windows.length();i++)
    {
        windows[i].first->geometry().getRect(&x,&y,&width,&height);
        /* 相加获得绝对坐标 */
        x+=baseOfX;
        y+=baseOfY;
        /* 判断鼠标点击坐标是否在对应区域的范围内 */
        if(p.x()>=x&&p.x()<=x+width&&p.y()>=y&&p.y()<=y+height)
        {
            /* 若该区域对应窗口隐藏，便将其显示 */
            if(windows[i].second->isHidden())
                windows[i].second->show();
            return;
        }
    }
}

/* 初始化窗口 */
void CS_Contest::initalWindow()
{
    /* ui 文件里默认标题为 xxx，设置标题文本为 "知识竞赛答题" */
    ui->textOfTitle->setText("知识竞赛答题");
    /* 插入显示的文本区域、对应窗口到 windows 中 */
    windows.push_back(QPair<QWidget*,QWidget*>(ui->areaOfStart,windowOfStart));
    windows.push_back(QPair<QWidget*,QWidget*>(ui->areaOfSearch,windowOfSearch));
    windows.push_back(QPair<QWidget*,QWidget*>(ui->areaOfAbout,windowOfAbout));
    windows.push_back(QPair<QWidget*,QWidget*>(ui->areaOfMore,windowOfMore));
}

/* 初始化计时器，更新主界面时间显示 */
void CS_Contest::initalTimer()
{
    /* 将 updateTime 绑定到 timer 上，接收到 timeout 信号立即更新时间，并重置鼠标点击的标志位 */
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
        updateTime();
        this->flagOfClick=1;
    });
    /* 启动定时器 */
    timer->start(1000);
}