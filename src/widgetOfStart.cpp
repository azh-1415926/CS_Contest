#include "widgetOfStart.h"
#include <QRect>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
    , reader(new excelReader)
{
    ui->setupUi(this);
    // add action for buttons
    QStackedWidget* windows=this->ui->stackOfWindows;
    QPushButton* buttons[]={
        ui->buttonOfQuestion,
        ui->buttonOfSelect,
        ui->buttonOfCollect
    };
    QWidget* pages[]={
        ui->pageOfQuestion,
        ui->pageOfSelect,
        ui->pageOfCollect
    };
    for(int i=0;i<sizeof(buttons)/sizeof(QPushButton*);i++){
        connect(buttons[i],QPushButton::clicked,this,[windows,buttons,pages,i](){
            windows->setCurrentIndex(windows->indexOf(pages[i]));
        });
    }
    // install filter for options
    QObject* options[]={
        ui->optionOfA,
        ui->textOfA,
        ui->optionOfB,
        ui->textOfB,
        ui->optionOfC,
        ui->textOfC,
        ui->optionOfD,
        ui->textOfD
    };
    ui->optionsOfQuestion->installEventFilter(this);
    for(int i=0;i<sizeof(options)/sizeof(QObject*);i++){
        dynamic_cast<QWidget*>(options[i])->setAttribute(Qt::WA_Hover,true);
        options[i]->installEventFilter(this);
    }
    this->installEventFilter(this);
    // click these labels can click buttons
    for(int i=1;i<sizeof(options)/sizeof(QObject*);i+=2)
        connect((clickLabel*)options[i],clickLabel::clicked,this,clickRadioButton);
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
    delete border;
}

bool widgetOfStart::eventFilter(QObject *obj, QEvent *e)
{
    // tracing those objs,draw border for one for objs
    QObject* objs[]={
        ui->optionOfA,
        ui->textOfA,
        ui->optionOfB,
        ui->textOfB,
        ui->optionOfC,
        ui->textOfC,
        ui->optionOfD,
        ui->textOfD
    };
    // find item in objs,and watch hover event
    for(int i=0;i<sizeof(objs)/sizeof(QObject*);i++){
        if(e->type()!=QEvent::HoverEnter&&e->type()!=QEvent::HoverLeave)
            break;
        if(obj==objs[i]){
            QRect rect1=dynamic_cast<QRadioButton*>(objs[(i>>1)<<1])->geometry();
            QRect rect2=dynamic_cast<QLabel*>(objs[(i>>1)<<1|1])->geometry();
            if(border==nullptr){
                border=new QRect(
                    rect1.x(),
                    rect2.y(),
                    rect1.width()+rect2.width(),
                    rect2.height()
                );
            }else{
                border->setX(rect1.x());
                border->setY(rect2.y());
                border->setWidth(rect1.width()+rect2.width());
                border->setHeight(rect2.height());
            }
            ui->optionsOfQuestion->update();
            return true;
        }
    }
    if(obj==ui->optionsOfQuestion&&e->type()==QEvent::Paint){
        paintBorder(dynamic_cast<QWidget*>(obj));
        return true;
    }else if(obj==this&&e->type()==QEvent::Resize){
        if(border!=nullptr)
            delete border;
        border=nullptr;
        return true;
    }
    return QWidget::eventFilter(obj,e);
}

void widgetOfStart::paintBorder(QWidget *widget)
{
    if(border!=nullptr){
        QPainter painter(widget);
        QPen pen;
        pen.setBrush(QBrush(qRgb(30,144,255)));
        painter.setPen(pen);
        painter.drawRect(QRect(border->topLeft(),border->bottomRight()));
    }
}

void widgetOfStart::clickRadioButton(clickLabel* label)
{
    QRadioButton* buttons[]={
        ui->optionOfA,
        ui->optionOfB,
        ui->optionOfC,
        ui->optionOfD
    };
    QLabel* labels[]={
        ui->textOfA,
        ui->textOfB,
        ui->textOfC,
        ui->textOfD
    };
    int i=0;
    while(i<sizeof(buttons)/sizeof(QRadioButton*)){
        if(labels[i]==label){
            buttons[i]->setChecked(true);
            break;
        }
        i++;
    }
}

const QString &widgetOfStart::getPath()
{
    const QString& filepath=QFileDialog::getOpenFileName(this, QStringLiteral("select excel file"), "",QStringLiteral("Exel file(*.xls *.xlsx)"));
    return filepath;
}
