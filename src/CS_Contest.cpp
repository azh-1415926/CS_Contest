#include "CS_Contest.h"

#include <QTimer>
#include <QTime>
#include <QDateTime>

CS_Contest::CS_Contest(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_CS_Contest)
    , flagOfClick(0)
{
    ui->setupUi(this);
    // set Windows's title
    this->setWindowTitle("CS_Contest");
    // set title
    ui->textOfTitle->setText("阳光养猪场");
    // show Time
    QTimer* timer=new QTimer(this);
    connect(timer,QTimer::timeout,this,updateTime);
    // reset flagOfClick
    connect(timer,QTimer::timeout,this,[&](){
        this->flagOfClick=0;
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

}

void CS_Contest::mousePressEvent(QMouseEvent *e)
{
    // handle by function: clickPoint()
    clickPoint(QPoint(e->position().x(),e->position().y()));
}