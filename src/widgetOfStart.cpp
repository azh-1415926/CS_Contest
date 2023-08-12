#include "widgetOfStart.h"
#include <QRect>
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
    , reader(new excelReader)
    ,currQuestionType(0)
    ,currQuestionIndex(1)
{
    ui->setupUi(this);
    initalStackWindow();
    initalQuestionPage();
    initalSelectPage();
    loadSetting();
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
    delete reader;
}

void widgetOfStart::saveSetting()
{
    if(pathOfExcel.isEmpty())
        return;
    QJsonObject json;
    QJsonArray arrayOfProcess;
    QJsonDocument doc;
    QFile file("./settings.json");
    // insert settings to json
    json.insert("pathOfExcel",pathOfExcel);
    json.insert("currQuestionType",currQuestionType);
    json.insert("currQuestionIndex",currQuestionIndex);
    for(int i=0;i<progress.length();i++)
        arrayOfProcess.push_back(progress[i]);
    json.insert("process",arrayOfProcess);
    // save settings
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);
        doc.setObject(json);
        stream<<doc.toJson();
        file.close();
    }else{
        QMessageBox::warning(nullptr,"error","save \"settings.json\" is failed!");
    }
}

void widgetOfStart::loadData()
{
    char charOfType[]={
        /* 计算机应用基础 数据结构 数据库原理 */
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
        "计算机应用基础","数据结构","数据库原理","网络","软件工程","操作系统",
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
    // set process 0
    if(reader->isReload()||progress.isEmpty()){
        progress.clear();
        for(int i=0;i<sumOfType;i++){
            progress.push_back(1);
        }
    }
    // show all types
    for(int i=0;i<sumOfType;i++){
        questionType[i].second[0]=questionType[i].second.length()-1;
        // string of type
        ui->tableOfQuestionType->setItem(i,0,new QTableWidgetItem(questionType[i].first));
        // char of type
        ui->tableOfQuestionType->setItem(i,1,new QTableWidgetItem(QString(charOfType[i])));
        // sum of type
        ui->tableOfQuestionType->setItem(i,2,new QTableWidgetItem(QString::number(questionType[i].second[0])));
    }
    // clear the combobox
    int currType=currQuestionType;
    if(ui->questionType->count()!=0)
        ui->questionType->clear();
    // add question types
    for(int i=0;i<sumOfType;i++){
        ui->questionType->addItem(stringOfType[i]);
    }
    ui->questionType->setCurrentIndex(currType);
    emit ready();
}

void widgetOfStart::selectQuestionType(int i)
{
    currQuestionType=i;
    currQuestionIndex=progress[currQuestionType];
    ui->sumOfQuestions->setText(QString::number(questionType[currQuestionType].second[0]));
    switchQuestionByIndex(currQuestionIndex);
}

void widgetOfStart::switchPreQuestion()
{
    if(currQuestionIndex<=1)
        return;
    switchQuestionByIndex(--currQuestionIndex);
}

void widgetOfStart::switchNextQuestion()
{
    if(currQuestionIndex>=questionType[currQuestionType].second[0])
        return;
    switchQuestionByIndex(++currQuestionIndex);
}

void widgetOfStart::getPath()
{
    const QString& filepath=QFileDialog::getOpenFileName(this, QStringLiteral("select excel file"), "",QStringLiteral("Exel file(*.xls *.xlsx)"));
    if(!filepath.isEmpty()){
        pathOfExcel=filepath;
        ui->textOfPath->setText(pathOfExcel);
        emit loadExcel(pathOfExcel);
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
    connect(ui->fowardButton,QPushButton::clicked,this,switchPreQuestion);
    connect(ui->nextButton,QPushButton::clicked,this,switchNextQuestion);
    connect(this,updateTextOfOption,ui->optionsOfQuestion,clickOptions::setTextOfOption);
}

void widgetOfStart::initalSelectPage()
{
    // alter the textOfPath
    connect(ui->inputButton,QRadioButton::clicked,this,getPath);
    // read excel
    connect(this,loadExcel,reader,excelReader::readExcel);
    // handle data
    connect(reader,excelReader::readed,this,loadData);
    // show question
    connect(this,ready,this,[=](){
        ui->questionType->setCurrentIndex(currQuestionType);
    });
    // questionType changed
    connect(ui->questionType,QComboBox::currentIndexChanged,this,selectQuestionType);
}

void widgetOfStart::switchQuestionByIndex(int i)
{
    if(!reader->isRead())
        return;
    const QVector<QVector<QString>>& data=reader->getData();
    int index=questionType[currQuestionType].second[i];
    progress[currQuestionType]=i;
    ui->indexOfCurrentQuestion->setText(QString::number(i));
    // set question
    ui->textOfQuestion->setText(QString::number(i)+". "+data[index][2]);
    // set options
    emit updateTextOfOption(0,data[index][3]);
    emit updateTextOfOption(1,data[index][4]);
    emit updateTextOfOption(2,data[index][5]);
    emit updateTextOfOption(3,data[index][6]);
    currAnswer=data[index][7].toInt();
    // QMessageBox::about(nullptr,"answer",data[index][7]);
}

void widgetOfStart::loadSetting()
{
    QJsonObject json;
    QJsonDocument doc;
    QFile file("./settings.json");
    if(file.open(QIODevice::ReadOnly|QFile::Text)){
        QJsonParseError error;
        QTextStream stream(&file);
        QString str=stream.readAll();
        file.close();
        doc=QJsonDocument::fromJson(str.toUtf8(),&error);
        if(error.error!=QJsonParseError::NoError&&!doc.isNull())
            QMessageBox::warning(nullptr,"json parse error","json 格式错误!");
        json=doc.object();
        pathOfExcel=json.value("pathOfExcel").toString();
        currQuestionType=json.value("currQuestionType").toInt();
        currQuestionIndex=json.value("currQuestionIndex").toInt();
        QJsonArray arrayOfProcess=json.value("process").toArray();
        for(int i=0;i<arrayOfProcess.count();i++)
            progress.push_back(arrayOfProcess[i].toInt());
        emit loadExcel(pathOfExcel);
        QTimer* timer=new QTimer(this);
        connect(timer,QTimer::timeout,this,saveSetting);
        timer->start(1000);
    }
}