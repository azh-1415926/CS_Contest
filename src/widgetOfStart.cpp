#include "widgetOfStart.h"
#include <QRect>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
    , border(nullptr)
    , reader(new excelReader)
    ,currQustionType(0)
    ,currQuestionIndex(0)
{
    ui->setupUi(this);
    initalStackWindow();
    initalQuestionPage();
    initalSelectPage();
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
    delete border;
    delete reader;
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
        if(border!=nullptr){
            delete border;
            border=nullptr;
        }
        return true;
    }
    return QWidget::eventFilter(obj,e);
}

void widgetOfStart::loadData()
{
    char charOfType[]={
        /* 计算机应用基础 数据库结构 数据库原理 */
        'C','J','K',
        /* 网络 软件工程 操作系统 */
        'W','R','Z',
        /* 多媒体技术 硬件系统 移动互联网应用 */
        'D','Y','H',
        /* 数据表示和计算 离散数学 知识产权*/
        'S','L','Q',
        /* 1-6 C,C++,Java,JavaScript,C#,Python */
        '1','2','3','4','5','6'
    };
    QString stringOfType[]={
        "计算机应用基础","数据库结构","数据库原理","网络","软件工程","操作系统",
        "多媒体技术","硬件系统","移动互联网应用","数据表示和计算","离散数学","知识产权",
        "C","C++","Java","JavaScript","C#","Python"
    };
    int sumOfType=sizeof(charOfType)/sizeof(char);
    if(!questionType.isEmpty()){
        for(int i=0;i<sumOfType;i++){
            questionType[i].second.clear();
            questionType[i].second.push_back(0);
        }
    }else{
        for(int i=0;i<sumOfType;i++){
            QList<int> num;
            num.push_back(0);
            questionType.push_back(QPair<QString,QList<int>>(stringOfType[i],num));
        }
    }
    ui->tableOfQuestionType->setColumnCount(3);
    ui->tableOfQuestionType->setRowCount(sumOfType);
    for(int i=0;i<reader->getData().length();i++){
        for(int j=0;j<sumOfType;j++){
            if(reader->getData()[i][1]==charOfType[j]){
                questionType[j].second.push_back(i);
                break;
            }
        }
    }
    for(int i=0;i<sumOfType;i++){
        questionType[i].second[0]=questionType[i].second.length()-1;
        // string of type
        ui->tableOfQuestionType->setItem(i,0,new QTableWidgetItem(questionType[i].first));
        // char of type
        ui->tableOfQuestionType->setItem(i,1,new QTableWidgetItem(QString(charOfType[i])));
        // sum of type
        ui->tableOfQuestionType->setItem(i,2,new QTableWidgetItem(QString::number(questionType[i].second[0])));
    }
}

void widgetOfStart::switchPreQuestion()
{
    if(currQuestionIndex<=1)
        return;
    switchQuestionByIndex(currQuestionIndex--);
}

void widgetOfStart::switchNextQuestion()
{
    if(currQuestionIndex>=questionType[currQustionType].second.length())
        return;
    switchQuestionByIndex(currQuestionIndex++);
}

void widgetOfStart::getPath()
{
    const QString& filepath=QFileDialog::getOpenFileName(this, QStringLiteral("select excel file"), "",QStringLiteral("Exel file(*.xls *.xlsx)"));
    if(!filepath.isEmpty()){
        pathOfExcecl=filepath;
        ui->textOfPath->setText(pathOfExcecl);
        emit loadExcel(pathOfExcecl);
    }
}

void widgetOfStart::initalStackWindow()
{
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
}

void widgetOfStart::initalQuestionPage()
{
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
    connect(ui->fowardButton,QPushButton::clicked,this,switchPreQuestion);
    connect(ui->nextButton,QPushButton::clicked,this,switchNextQuestion);
}

void widgetOfStart::initalSelectPage()
{
    // alter the textOfPath
    connect(ui->inputButton,QRadioButton::clicked,this,getPath);
    connect(this,loadExcel,reader,excelReader::readExcel);
    connect(reader,excelReader::readed,this,loadData);
    if(!pathOfExcecl.isEmpty()){
        ui->textOfPath->setText(pathOfExcecl);
        emit loadExcel(pathOfExcecl);
    }
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

void widgetOfStart::switchQuestionByIndex(int i)
{
    if(!reader->isRead())
        return;
    const QVector<QVector<QString>>& data=reader->getData();
    int index=questionType[currQustionType].second[i];
    // set question
    ui->textOfQuestion->setText(data[index][2]);
    // set options
    ui->textOfA->setText(data[index][3]);
    ui->textOfB->setText(data[index][4]);
    ui->textOfC->setText(data[index][5]);
    ui->textOfD->setText(data[index][6]);
    currAnswer=data[index][7].toInt();
    QMessageBox::about(nullptr,"answer",data[index][7]);
}