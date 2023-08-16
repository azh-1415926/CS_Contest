#include "switchquestion.h"
#include <QGridLayout>

switchQuestion::switchQuestion(QWidget *parent)
    : QWidget(parent)
    , indexOfQuestion(0)
    , sumOfQuestion(0)
    , forwardBtn(new QPushButton)
    , nextBtn(new QPushButton)
    , collectBtn(new collectButton)
    , textOfTag(new QLabel)
    , textOfIndex(new QLabel)
    , textOfSum(new QLabel)
{
    inital();
}

switchQuestion::~switchQuestion()
{
    delete forwardBtn;
    delete nextBtn;
    delete collectBtn;
    delete textOfTag;
    delete textOfIndex;
    delete textOfSum;
}

const QString &switchQuestion::title()
{
    const QString& str=this->textOfTag->text();
    return str;
}

int switchQuestion::index()
{
    return this->indexOfQuestion;
}

const QString &switchQuestion::stringOfIndex()
{
    const QString& str=this->textOfIndex->text();
    return str;
}

int switchQuestion::count()
{
    return this->sumOfQuestion;
}

void switchQuestion::setTitle(const QString& title)
{
    this->textOfTag->setText(title);
}

void switchQuestion::setIndex(int i)
{
    this->indexOfQuestion=i;
    if(this->indexOfQuestion<this->sumOfQuestion)
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
    else
        this->textOfIndex->setText(QString::number(this->indexOfQuestion));
}

void switchQuestion::setSum(int n)
{
    this->sumOfQuestion=n;
    this->textOfSum->setText(QString::number(this->sumOfQuestion));
}

void switchQuestion::setTextOfIndex(const QString &str)
{
    this->textOfIndex->setText(str);
}

void switchQuestion::setTextOfSum(const QString &str)
{
    this->textOfSum->setText(str);
}

void switchQuestion::setCollect(bool status)
{
    if(status)
        this->collectBtn->setCollectNoSignal();
    else
        this->collectBtn->setUncollectNoSignal();
}


void switchQuestion::inital()
{
    QGridLayout* layout=new QGridLayout(this);
    forwardBtn->setText("←");
    nextBtn->setText("→");
    collectBtn;
    layout->addWidget(forwardBtn,0,0);
    layout->addWidget(collectBtn,0,1);
    layout->addWidget(nextBtn,0,2);
    layout->addWidget(textOfTag,1,0);
    layout->addWidget(textOfIndex,1,1);
    layout->addWidget(textOfSum,1,2);
    connect(forwardBtn,QPushButton::clicked,this,[=](){
        if(this->indexOfQuestion<=0)
            return;
        emit lastIndex(this->indexOfQuestion--);
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    connect(nextBtn,QPushButton::clicked,this,[=](){
        if(this->indexOfQuestion>=this->sumOfQuestion-1)
            return;
        emit lastIndex(this->indexOfQuestion++);
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    connect(collectBtn,collectButton::collected,this,[=](){
        emit collectQuestion();
    });
    connect(collectBtn,collectButton::uncollected,this,[=](){
        emit uncollectQuestion();
    });
}