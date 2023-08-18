#include "clickoptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>

clickOptions::clickOptions(QWidget *parent)
    : QGroupBox(parent)
    , numOfOptions(4)
    , answerOfOptions(-1)
    , hoverOption(-1)
    , checkedOption(-1)
    , hoverBox(nullptr)
    , correctBox(nullptr)
    , incorrectBox(nullptr)
{
    initalOptions();
    initalEvent();
}

clickOptions::~clickOptions()
{
    for(int i=0;i<numOfOptions;i++){
        delete buttons[i];
        delete labels[i];
    }
    freeBoxes();
}

/* 返回答案选项的下标 */
int clickOptions::getAnswer()
{
    return this->answerOfOptions;
}

/* 重写事件过滤器 */
bool clickOptions::eventFilter(QObject *obj, QEvent *e)
{
    /* 先将捕获到的对象在所有按钮里查找，若找不到 index=-1，则在 所有选项的 label 里查找 */
    int index=buttons.indexOf(obj);
    if(index==-1)
        index=labels.indexOf(obj);
    /* 查找到适合更新悬浮选框更新的组件（在按钮或者选项之中），便更新悬浮选框，并在下一次绘制的时候应用 */
    if(index!=-1&&(e->type()==QEvent::HoverEnter||e->type()==QEvent::HoverLeave)){
        hoverBox=setOptionOfBox(index,hoverBox);
        hoverOption=index;
        this->update();
        return true;
    }
    /* 若为当前对象的绘制事件，则一并绘制悬浮、正确、错误选框 */
    if(obj==this&&e->type()==QEvent::Paint){
        paintBox(this,hoverBox);
        paintBox(this,correctBox);
        paintBox(this,incorrectBox);
        return true;
    /* 若为当前对象窗口大小调整事件，则清空所有选框，并更新所有选框，若没有被选中的选项，那么只更新悬浮选框 */
    }else if(obj==this&&e->type()==QEvent::Resize){
        freeBoxes();
        hoverBox=setOptionOfBox(hoverOption,hoverBox);
        if(checkedOption!=-1)
            displayAnswer(true);
        else
            this->update();
        return true;
    }
    return QWidget::eventFilter(obj,e);
}

/* 设置答案选项的下标 */
void clickOptions::setAnswer(int i)
{
    this->answerOfOptions=i;
}

/* 设置是否展示出答案与错误选项 */
void clickOptions::displayAnswer(bool state)
{
    /* 显示正确、错误选项 */
    if(state){
        /* 若当前选项不为正确选项，则设置错误选框的坐标 */
        if(checkedOption!=answerOfOptions)
            incorrectBox=setOptionOfBox(checkedOption,incorrectBox);
        /* 若当前选项为正确选项，且错误选框存在，则删除错误选框（实现两次执行此函数便可以做到更新操作） */
        else if(incorrectBox!=nullptr){
            delete incorrectBox;
            incorrectBox=nullptr;
        }
        /* 无论如何，始终设置正确选框的坐标 */
        correctBox=setOptionOfBox(answerOfOptions,correctBox);
    /* 隐藏正确、错误选项 */
    }else{
        /* 若正确、错误选框存在，便删除 */
        if(correctBox!=nullptr){
            delete correctBox;
            correctBox=nullptr;
        }
        if(incorrectBox!=nullptr){
            delete incorrectBox;
            incorrectBox=nullptr;
        }
    }
    this->update();
}

/* 重置当前选中的选项、答案选项，并取消答案的显示 */
void clickOptions::resetOption()
{
    displayAnswer(false);
    if(this->checkedOption!=-1){
        buttons[this->checkedOption]->setCheckable(false);
        buttons[this->checkedOption]->setCheckable(true);
        checkedOption=-1;
    }
    answerOfOptions=-1;
}

/* 设置指定选项的文本内容 */
void clickOptions::setTextOfOption(int i,const QString& text)
{
    if(i>=0&&i<numOfOptions)
        labels[i]->setText(text);
}

/* 初始化选项 */
void clickOptions::initalOptions()
{
    /* 主体采用垂直布局 */
    QVBoxLayout* options=new QVBoxLayout(this);
    /* 创建 numOfOptions 个按钮和选项标签 */
    for(int i=0;i<numOfOptions;i++){
        buttons.push_back(new QRadioButton);
        labels.push_back(new clickLabel);
        /* 设置按钮和选项标签初始文本，分别为 "A"、"Option A"，选项从 A 开始 */
        char ch='A'+i;
        QString str(ch);
        buttons[i]->setFont(QFont("Microsoft YaHei UI",10));
        buttons[i]->setText(str);
        labels[i]->setFont(QFont("Microsoft YaHei UI",10));
        labels[i]->setText("option "+str);
        labels[i]->setMargin(14);
        /* 设置选项标签为左对齐、垂直居中对齐，文本自动缩放 */
        labels[i]->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        labels[i]->setWordWrap(true);
        /* 按钮被点击，则更新被选中的选项，且发送 selectOption 信号，传递当前被选中的选项下标 */
        connect(buttons[i],QRadioButton::clicked,this,[=](){
            this->checkedOption=i;
            emit selectOption(i);
        });
        /* 标签被点击，则选中对应的按钮、更新被选中的选项，且发送 selectOption 信号 */
        connect(labels[i],clickLabel::clicked,this,[=](){
            buttons[i]->setChecked(true);
            this->checkedOption=i;
            emit selectOption(i);
        });
        /* 将按钮和选项标签水平布局，并添加到主体中 */
        QHBoxLayout* layout=new QHBoxLayout(this);
        layout->addWidget(buttons[i],0);
        layout->addWidget(labels[i],5);
        options->addLayout(layout,1);
    }
}

/* 初始化对应的事件（安装事件过滤器） */
void clickOptions::initalEvent()
{
    for(int i=0;i<numOfOptions;i++){
        buttons[i]->setAttribute(Qt::WA_Hover,true);
        labels[i]->setAttribute(Qt::WA_Hover,true);
        buttons[i]->installEventFilter(this);
        labels[i]->installEventFilter(this);
    }
    this->installEventFilter(this);
}

/* 在指定窗口绘制指定选框 */
void clickOptions::paintBox(QWidget *widget, QRect *box)
{
    if(box!=nullptr){
        QPainter painter(widget);
        QPen pen(Qt::black,5);
        /* 对应选框的笔刷颜色 */
        if(box==hoverBox){
            pen.setBrush(QBrush(qRgb(30,144,255)));
        }else if(box==correctBox){
            pen.setBrush(QBrush(qRgb(144,238,144)));
        }else if(box==incorrectBox){
            pen.setBrush(QBrush(qRgb(255,0,0)));
        }
        painter.setPen(pen);
        if(box==hoverBox){
            painter.drawRoundedRect(QRect(box->topLeft(),box->bottomRight()),20,20);
        }else{
            /* 正确、错误选框往里缩 5 格像素，方便悬浮选框展示 */
            QPainterPath path;
            path.addRoundedRect(
                QRect(
                    box->x()+5,
                    box->y()+5,
                    box->width()-10,
                    box->height()-10
                ),
                15,15
            );
            /* 填充选框 */
            painter.fillPath(path,pen.brush());
            pen.setColor(Qt::black);
            pen.setWidth(5);
            painter.setPen(pen);
            /* 绘制黑色边框 */
            painter.drawPath(path);
        }
    }
}

/* 释放所有选框 */
void clickOptions::freeBoxes()
{
    if(hoverBox!=nullptr){
        delete hoverBox;
        hoverBox=nullptr;
    }
    if(correctBox!=nullptr){
        delete correctBox;
        correctBox=nullptr;
    }
    if(incorrectBox!=nullptr){
        delete incorrectBox;
        incorrectBox=nullptr;
    }
}

/* (返回/修改)指定选项的选框，设置合适的坐标 */
QRect *clickOptions::setOptionOfBox(int i,QRect* rect)
{
    /* 若没有改选项，直接返回 */
    if(i<0||i>=numOfOptions)
        return nullptr;
    if(rect==nullptr)
        rect=new QRect;
    /*
        设置矩形的坐标，要覆盖到按钮和选项标签。
        考虑到选项的布局，于是取
            按钮的 x 坐标，
            选项标签的 y 坐标，
            按钮和选项标签的 width 之和，
            选项标签的 height
    */
    rect->setRect(
        buttons[i]->geometry().x(),
        labels[i]->geometry().y(),
        buttons[i]->geometry().width()+labels[i]->geometry().width(),
        labels[i]->geometry().height()
    );
    return rect;
}