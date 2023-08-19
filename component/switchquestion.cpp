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

/* 返回当前组件的标题 */
const QString &switchQuestion::title() const
{
    const QString& str=this->textOfTag->text();
    return str;
}

/* 返回当前问题的下标 */
int switchQuestion::index() const
{
    return this->indexOfQuestion;
}

/* 返回当前问题下标对应标签上的字符串文本 */
const QString &switchQuestion::stringOfIndex() const
{
    const QString& str=this->textOfIndex->text();
    return str;
}

/* 返回当前问题的总数 */
int switchQuestion::count() const
{
    return this->sumOfQuestion;
}

/* 设置当前组件的标题 */
void switchQuestion::setTitle(const QString& title)
{
    this->textOfTag->setText(title);
}

/* 设置当前问题的下标 */
void switchQuestion::setIndex(int i)
{
    this->indexOfQuestion=i;
    if(this->indexOfQuestion<this->sumOfQuestion)
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
    else
        this->textOfIndex->setText(QString::number(this->indexOfQuestion));
}

/* 设置当前问题的总数 */
void switchQuestion::setSum(int n)
{
    this->sumOfQuestion=n;
    this->textOfSum->setText(QString::number(this->sumOfQuestion));
}

/* 设置当前问题下标对应标签的文本 */
void switchQuestion::setTextOfIndex(const QString &str)
{
    this->textOfIndex->setText(str);
}

/* 设置当前问题总数对应标签的文本 */
void switchQuestion::setTextOfSum(const QString &str)
{
    this->textOfSum->setText(str);
}

/* 设置当前问题收藏的状态，true 为设置为被收藏 */
void switchQuestion::setCollect(bool status)
{
    if(status)
        this->collectBtn->setCollectNoSignal();
    else
        this->collectBtn->setUncollectNoSignal();
}

/* 初始化布局、按钮、文本 */
void switchQuestion::inital()
{
    /* 主体采用网格布局 */
    QGridLayout* layout=new QGridLayout(this);
    /* 切换问题的左右按钮的文本分别设置为 "←"、"→"，设置收藏按钮为取消收藏状态 */
    forwardBtn->setText("←");
    nextBtn->setText("→");
    collectBtn->setUncollectNoSignal();
    /* 将所有组件添加到布局中 */
    layout->addWidget(forwardBtn,0,0);
    layout->addWidget(collectBtn,0,1);
    layout->addWidget(nextBtn,0,2);
    layout->addWidget(textOfTag,1,0);
    layout->addWidget(textOfIndex,1,1);
    layout->addWidget(textOfSum,1,2);
    /* 向前按钮点击事件 */
    connect(forwardBtn,QPushButton::clicked,this,[=](){
        /* 若下标小于等于 0，即左边已经没有问题了，直接返回 */
        if(this->indexOfQuestion<=0)
            return;
        /* 若下标合法，发送 lastIndex 信号，传入未切换前的下标 */
        emit lastIndex(this->indexOfQuestion--);
        /* 默认设置下标文本为当前下标+1，发送 changeQuestion 信号，传入切换后的下标 */
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    /* 向后按钮点击事件 */
    connect(nextBtn,QPushButton::clicked,this,[=](){
        /* 若下标大于等于问题的总数，即右边已经没有问题了，直接返回 */
        if(this->indexOfQuestion>=this->sumOfQuestion-1)
            return;
        /* 若下标合法，发送 lastIndex 信号，传入未切换前的下标 */
        emit lastIndex(this->indexOfQuestion++);
        /* 默认设置下标文本为当前下标+1，发送 changeQuestion 信号，传入切换后的下标 */
        this->textOfIndex->setText(QString::number(this->indexOfQuestion+1));
        emit changeQuestion(this->indexOfQuestion);
    });
    /* 当收藏按钮发送 collected、uncollected 信号时，发送 collectQuestion、uncollectQuestion 信号，当做转发 */
    connect(collectBtn,collectButton::collected,this,[=](){
        emit collectQuestion();
    });
    connect(collectBtn,collectButton::uncollected,this,[=](){
        emit uncollectQuestion();
    });
}