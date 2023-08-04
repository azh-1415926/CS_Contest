#include "CS_Contest.h"

#include <QTimer>
#include <QTime>
#include <QDateTime>
#include <QMessageBox>

CS_Contest::CS_Contest(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_CS_Contest)
    , flagOfClick(0)
{
    ui->setupUi(this);
    // set Windows's title
    this->setWindowTitle("CS_Contest");
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
}

void CS_Contest::clickPoint(const QPoint &p)
{
    int baseOfX,baseOfY,baseOfWidth,baseOfHeight;
    int x,y,width,height;
    QWidget* areas[]={
        ui->areaOfStart,
        ui->areaOfSearch,
        ui->areaOfAbout,
        ui->areaOfMore
    };
    QString names[]={
        "areaOfStart",
        "areaOfSearch",
        "areaOfAbout",
        "areaOfMore"
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
            names[i].append(",x:"+QString::number(x)+",y:"+QString::number(y)+",width:"+QString::number(width)+",height"+QString::number(height));
            QMessageBox::about(nullptr,"test",names[i]);
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