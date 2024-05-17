#include "CS_Contest.h"

#include "widgetOfStart.h"
#include "widgetOfSearch.h"
#include "widgetOfAbout.h"
#include "widgetOfMore.h"
#include "constants.h"

#include <QTimer>
#include <QTime>
#include <QDateTime>

#ifdef __ANDROID__
#include <QObject>
#include <QKeyEvent>
#endif

CS_Contest::CS_Contest(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_CS_Contest)
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
}

bool CS_Contest::eventFilter(QObject *obj, QEvent *e)
{
    if(e->type()==QEvent::Close)
    {
        if(ui->allWidgets->currentIndex()!=0)
        {
            e->ignore();
            this->resetAllWidgets();
            return true;
        }
            
    }
    return false;
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
    /* 为此窗口安装事件过滤器 */
    installEventFilter(this);
    /* ui 文件里默认标题为 xxx，设置标题文本为 constants.h 里面定义的 titleOfProgram */
    ui->textOfTitle->setText(titleOfProgram);

    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfStart,ui->pageOfStart));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfSearch,ui->pageOfSearch));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfAbout,ui->pageOfAbout));
    windows.push_back(QPair<clickLabel*,QWidget*>(ui->textOfMore,ui->pageOfMore));
    for(int i=0;i<windows.length();i++)
    {
        /* 连接 clickLabel 的点击信号，触发子窗口的显示，及为子窗口安装事件过滤器 */
        connect(windows[i].first,&clickLabel::clicked,this,[=]
        {
            ui->allWidgets->setCurrentIndex(i+1);
        });
        
        windows[i].second->installEventFilter(this);
    }
}

/* 初始化计时器，更新主界面时间显示 */
void CS_Contest::initalTimer()
{
    /* 将 updateTime 绑定到 timer 上，接收到 timeout 信号立即更新时间 */
    QTimer* timer=new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=]()
    {
        updateTime();
    });
    /* 启动定时器 */
    timer->start(1000);
}

void CS_Contest::resetAllWidgets()
{
    ui->allWidgets->setCurrentIndex(0);

    #ifndef __ANDROID__
        this->resize(800,600);
    #endif
}
