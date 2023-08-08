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
    // set title
    ui->textOfTitle->setText("知识竞赛答题");
    // show Time
    QTimer* timer=new QTimer(this);
    connect(timer,QTimer::timeout,this,updateTime);
    // reset flagOfClick
    connect(timer,QTimer::timeout,this,[&](){
        this->flagOfClick=1;
    });
    // start timer
    timer->start(1000);
}

void CS_Contest::updateTime(){
    // get time
    QDateTime time=QDateTime::currentDateTime();
    // format time to year-month-day,example: 2023-01-01
    QString timeOfYMD=time.toString("yyyy-MM-dd");
    // format time to hour:minute:second,example: 06:30:01
    QString timeOfHMS=time.toString("hh:mm:ss");
    // show the time
    ui->timeOfYMD->display(timeOfYMD);
    ui->timeOfHMS->display(timeOfHMS);
}

CS_Contest::~CS_Contest()
{
    delete ui;
    delete windowOfStart;
    delete windowOfSearch;
    delete windowOfAbout;
    delete windowOfMore;
}

void CS_Contest::clickPoint(const QPoint &p)
{
    int baseOfX,baseOfY,baseOfWidth,baseOfHeight;
    int x,y,width,height;
    // main window show areas
    QWidget* areas[]={
        ui->areaOfStart,
        ui->areaOfSearch,
        ui->areaOfAbout,
        ui->areaOfMore
    };
    // other windows
    QWidget* windows[]={
        windowOfStart,
        windowOfSearch,
        windowOfAbout,
        windowOfMore
    };
    ui->areaOfBottom->geometry().getRect(&baseOfX,&baseOfY,&baseOfWidth,&baseOfHeight);
    // get position of area,and compare it with p
    for(int i=0;i<sizeof(areas)/sizeof(QWidget*);i++){
        areas[i]->geometry().getRect(&x,&y,&width,&height);
        x+=baseOfX;
        y+=baseOfY;
        // if(p.x()<=baseOfX||p.y()<=baseOfY||p.x()>=baseOfX+baseOfWidth||p.y()>=baseOfHeight)
        //     return;
        if(p.x()>=x&&p.x()<=x+width&&p.y()>=y&&p.y()<=y+height){
            // if not show,show it
            if(windows[i]->isHidden())
                windows[i]->show();
            return;
        }
    }
}

void CS_Contest::mousePressEvent(QMouseEvent *e)
{
    // handle by function: clickPoint()
    if(flagOfClick){
        clickPoint(QPoint(e->position().x(),e->position().y()));
        flagOfClick=0;
    }
}