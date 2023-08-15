#include "switchquestion.h"
#include <QGridLayout>

switchQuestion::switchQuestion(QWidget *parent)
    : QWidget(parent)
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

void switchQuestion::setTitle(const QString& title)
{
    this->textOfTag->setText(title);
}

void switchQuestion::setIndex(int i)
{
    this->index=i;
    this->textOfIndex->setText(QString::number(this->index+1));
}

void switchQuestion::setSum(int n)
{
    this->sum=n;
    this->textOfSum->setText(QString::number(this->sum));
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
        if(this->index<=0)
            return;
        --this->index;
        this->textOfIndex->setText(QString::number(this->index+1));
        emit changeQuestion(this->index);
    });
    connect(nextBtn,QPushButton::clicked,this,[=](){
        if(this->index>=this->sum-1)
            return;
        ++this->index;
        this->textOfIndex->setText(QString::number(this->index+1));
        emit changeQuestion(this->index);
    });
    connect(collectBtn,collectButton::collected,this,[=](){
        emit collectQuestion();
    });
    connect(collectBtn,collectButton::uncollected,this,[=](){
        emit uncollectQuestion();
    });
}