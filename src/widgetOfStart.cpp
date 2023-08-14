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
    , currTypeOfQuestion(0)
    , currIndexOfQuestion(1)
    , currIndexOfCollection(0)
    , flagOfInital(0)
{
    ui->setupUi(this);
    initalStackWindow();
    initalQuestionPage();
    initalSelectionPage();
    initalCollectionPage();
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
    delete reader;
}

void widgetOfStart::resizeEvent(QResizeEvent *)
{
    if(flagOfInital==0)
        loadSetting();
}

void widgetOfStart::saveSetting()
{
    if(pathOfExcel.isEmpty())
        return;
    QJsonObject json;
    QJsonArray arrayOfQuestionProcess;
    QJsonArray arrayOfCollectProcess;
    QJsonDocument doc;
    QFile file("./settings.json");
    // insert settings to json
    json.insert("pathOfExcel",pathOfExcel);
    json.insert("currTypeOfQuestion",currTypeOfQuestion);
    json.insert("currIndexOfQuestion",currIndexOfQuestion);
    //save progress for question
    for(int i=0;i<progressOfQuestion.length();i++)
        arrayOfQuestionProcess.push_back(progressOfQuestion[i]);
    json.insert("progressOfQuestion",arrayOfQuestionProcess);
    // save progress for collect
    QJsonArray first;
    QJsonArray second;
    for(int i=0;i<progressOfCollection.length();i++){
        first.push_back(progressOfCollection[i].first);
        second.push_back(progressOfCollection[i].second);
    }
    arrayOfCollectProcess.push_back(first);
    arrayOfCollectProcess.push_back(second);
    json.insert("progressOfCollection",arrayOfCollectProcess);
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

void widgetOfStart::loadSetting()
{
    if(flagOfInital==1)
        return;
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
        currTypeOfQuestion=json.value("currTypeOfQuestion").toInt();
        currIndexOfQuestion=json.value("currIndexOfQuestion").toInt();
        QJsonArray arrayOfQuestionProcess=json.value("progressOfQuestion").toArray();
        for(int i=0;i<arrayOfQuestionProcess.count();i++)
            progressOfQuestion.push_back(arrayOfQuestionProcess[i].toInt());
        QJsonArray arrayOfCollectProcess=json.value("progressOfCollection").toArray();
        const QJsonArray& first=arrayOfCollectProcess[0].toArray();
        const QJsonArray& second=arrayOfCollectProcess[1].toArray();
        for(int i=0;i<first.count();i++){
            progressOfCollection.push_back(QPair<int,int>(first[i].toInt(),second[i].toInt()));
        }
        ui->sumOfCollection->setText(QString::number(progressOfCollection.length()));
        emit loadExcel(pathOfExcel);
    }
    QTimer* timer=new QTimer(this);
    connect(timer,QTimer::timeout,this,saveSetting);
    timer->start(1000);
    flagOfInital=1;
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
    if(reader->isReload()||progressOfQuestion.isEmpty()){
        progressOfQuestion.clear();
        if(reader->isReload())
            progressOfCollection.clear();
        for(int i=0;i<sumOfType;i++){
            progressOfQuestion.push_back(1);
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
    int currType=currTypeOfQuestion;
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
    currTypeOfQuestion=i;
    currIndexOfQuestion=progressOfQuestion[currTypeOfQuestion];
    ui->sumOfQuestions->setText(QString::number(questionType[currTypeOfQuestion].second[0]));
    switchQuestionByIndex(currIndexOfQuestion);
}

void widgetOfStart::switchPreQuestion()
{
    if(currIndexOfQuestion<=1)
        return;
    switchQuestionByIndex(--currIndexOfQuestion);
}

void widgetOfStart::switchNextQuestion()
{
    if(currTypeOfQuestion>=questionType.length()||currIndexOfQuestion>=questionType[currTypeOfQuestion].second[0])
        return;
    switchQuestionByIndex(++currIndexOfQuestion);
}

void widgetOfStart::switchPreCollection()
{
    if(currIndexOfCollection<=0)
        return;
    switchCollectionByIndex(currIndexOfCollection--);
}

void widgetOfStart::switchNextCollection()
{
    if(currIndexOfCollection>=progressOfCollection.length()-1)
        return;
    switchCollectionByIndex(currIndexOfCollection++);
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
        ui->pageOfSelection,
        ui->pageOfCollection
    };
    for(int i=0;i<sizeof(buttons)/sizeof(QPushButton*);i++){
        connect(buttons[i],QPushButton::clicked,this,[windows,buttons,pages,i](){
            windows->setCurrentIndex(windows->indexOf(pages[i]));
        });
    }
}

void widgetOfStart::initalQuestionPage()
{
    connect(ui->fowardQuestionButton,QPushButton::clicked,this,switchPreQuestion);
    connect(ui->nextQuestionButton,QPushButton::clicked,this,switchNextQuestion);
    connect(this,updateTextOfQuestion,ui->optionsOfQuestion,clickOptions::setTextOfOption);
    connect(ui->collectQuestionButton,collectButton::collected,this,[=](){
        int index=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
        if(index==-1){
            progressOfCollection.push_back(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
            ui->sumOfCollection->setText(QString::number(progressOfCollection.length()));
            if(progressOfCollection.length()==1)
                switchCollectionByIndex(currIndexOfCollection=0);
        }
    });
    connect(ui->collectQuestionButton,collectButton::uncollected,this,[=](){
        int index=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,currIndexOfQuestion));
        if(index!=-1){
            progressOfCollection.remove(index);
            if(currIndexOfCollection<progressOfCollection.length())
                switchCollectionByIndex(currIndexOfCollection);
            else
                switchCollectionByIndex(currIndexOfCollection=progressOfCollection.length()-1);
            ui->sumOfCollection->setText(QString::number(progressOfCollection.length()));
        }
    });
    ui->indexOfCurrentQuestion->setText("0");
    ui->sumOfQuestions->setText("0");
}

void widgetOfStart::initalSelectionPage()
{
    // alter the textOfPath
    connect(ui->inputButton,QRadioButton::clicked,this,getPath);
    // read excel
    connect(this,loadExcel,reader,excelReader::readExcel);
    // handle data
    connect(reader,excelReader::readed,this,loadData);
    // show question
    connect(this,ready,this,[=](){
        ui->questionType->setCurrentIndex(currTypeOfQuestion);
        if(currIndexOfCollection<progressOfCollection.length())
            switchCollectionByIndex(currIndexOfCollection);
    });
    // questionType changed
    connect(ui->questionType,QComboBox::currentIndexChanged,this,selectQuestionType);
}

void widgetOfStart::initalCollectionPage()
{
    connect(ui->fowardCollectionButton,QPushButton::clicked,this,switchPreCollection);
    connect(ui->nextCollectionButton,QPushButton::clicked,this,switchNextCollection);
    connect(this,updateTextOfCollection,ui->optionsOfCollection,clickOptions::setTextOfOption);
    connect(ui->cancelCollectionButton,collectButton::uncollected,this,[=](){
        if(progressOfCollection.length()<=1){
            switchCollectionByIndex(-1);
            return;
        }
        ui->indexOfCurrentCollection->setText("?");
    });
    connect(ui->cancelCollectionButton,collectButton::collected,this,[=](){
        ui->indexOfCurrentCollection->setText(QString::number(currIndexOfCollection));
    });
    switchCollectionByIndex(-1);
}

void widgetOfStart::switchQuestionByIndex(int i)
{
    if(!reader->isRead())
        return;
    const QVector<QVector<QString>>& data=reader->getData();
    int index=questionType[currTypeOfQuestion].second[i];
    progressOfQuestion[currTypeOfQuestion]=i;
    ui->indexOfCurrentQuestion->setText(QString::number(i));
    // set question
    ui->textOfQuestion->setText(QString::number(i)+". "+data[index][2]);
    // set options
    emit updateTextOfQuestion(0,data[index][3]);
    emit updateTextOfQuestion(1,data[index][4]);
    emit updateTextOfQuestion(2,data[index][5]);
    emit updateTextOfQuestion(3,data[index][6]);
    int status=progressOfCollection.indexOf(QPair<int,int>(currTypeOfQuestion,i));
    if(status!=-1)
        ui->collectQuestionButton->setText("⭐");
    else
        ui->collectQuestionButton->setText("☆");
    // currAnswerOfQuestion=data[index][7].toInt();
    // QMessageBox::about(nullptr,"answer",data[index][7]);
}

void widgetOfStart::switchCollectionByIndex(int i)
{
    if(progressOfCollection.isEmpty()||i==-1){
        if(i==-1&&progressOfCollection.length()==1){
            currIndexOfCollection=0;
            int row=progressOfCollection[0].first;
            int column=progressOfCollection[0].second;
            progressOfCollection.remove(0);
            if(currTypeOfQuestion==row&&currIndexOfQuestion==column)
                ui->collectQuestionButton->setText("☆");
        }
        ui->textOfCollection->setText("暂无收藏");
        emit updateTextOfCollection(0,"Option A");
        emit updateTextOfCollection(1,"Option B");
        emit updateTextOfCollection(2,"Option C");
        emit updateTextOfCollection(3,"Option D");
        ui->cancelCollectionButton->setText("⭐");
        ui->indexOfCurrentCollection->setText("0");
        ui->sumOfCollection->setText("0");
        return;
    }
    if(!reader->isRead())
        return;
    const QVector<QVector<QString>>& data=reader->getData();
    int row=progressOfCollection[currIndexOfCollection].first;
    int column=progressOfCollection[currIndexOfCollection].second;
    int index=questionType[row].second[column];
    if(ui->indexOfCurrentCollection->text()=="?"){
        row=progressOfCollection[i].first;
        column=progressOfCollection[i].second;
        progressOfCollection.remove(i);
        if(currTypeOfQuestion==row&&currIndexOfQuestion==column)
            ui->collectQuestionButton->setText("☆");
        if(i<currIndexOfCollection)
            currIndexOfCollection--;
        ui->sumOfCollection->setText(QString::number(progressOfCollection.length()));
    }
    const QString& number=QString::number(currIndexOfCollection+1);
    ui->indexOfCurrentCollection->setText(number);
    // set question
    ui->textOfCollection->setText(number+". "+data[index][2]);
    // set options
    emit updateTextOfCollection(0,data[index][3]);
    emit updateTextOfCollection(1,data[index][4]);
    emit updateTextOfCollection(2,data[index][5]);
    emit updateTextOfCollection(3,data[index][6]);
    // currAnswerOfQuestion=data[index][7].toInt();
    // QMessageBox::about(nullptr,"answer",data[index][7]);
    ui->cancelCollectionButton->setText("⭐");
}