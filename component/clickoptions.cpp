#include "clickoptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>
#include <QMessageBox>

clickOptions::clickOptions(QWidget *parent)
    : QGroupBox(parent)
    , numOfOptions(4)
    , answerOfOptions(0)
    , checkedOption(0)
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

int clickOptions::getAnswer()
{
    return this->answerOfOptions;
}

bool clickOptions::eventFilter(QObject *obj, QEvent *e)
{
    // find item in objs,and watch hover event
    int index=buttons.indexOf(obj);
    if(index==-1)
        index=labels.indexOf(obj);
    if(index!=-1&&(e->type()==QEvent::HoverEnter||e->type()==QEvent::HoverLeave)){
        hoverBox=setOptionOfBox(index,hoverBox);
        this->update();
        return true;
    }
    if(obj==this&&e->type()==QEvent::Paint){
        paintBox(this,hoverBox);
        paintBox(this,correctBox);
        paintBox(this,incorrectBox);
        return true;
    }else if(obj==this&&e->type()==QEvent::Resize){
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
        return true;
    }
    return QWidget::eventFilter(obj,e);
}

void clickOptions::setAnswer(int i)
{
    this->answerOfOptions=i;
}

void clickOptions::displayAnswer(bool state)
{
    if(state){
        if(checkedOption!=answerOfOptions)
            incorrectBox=setOptionOfBox(checkedOption,incorrectBox);
        else if(incorrectBox!=nullptr){
            delete incorrectBox;
            incorrectBox=nullptr;
        }
        correctBox=setOptionOfBox(answerOfOptions,correctBox);
    }else{
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

void clickOptions::resetOption()
{
    displayAnswer(false);
    buttons[this->checkedOption]->setCheckable(false);
    buttons[this->checkedOption]->setCheckable(true);
}

void clickOptions::setTextOfOption(int i,const QString& text)
{
    if(i>=0&&i<numOfOptions)
        labels[i]->setText(text);
}

void clickOptions::initalOptions()
{
    QVBoxLayout* options=new QVBoxLayout(this);
    for(int i=0;i<numOfOptions;i++){
        buttons.push_back(new QRadioButton);
        labels.push_back(new clickLabel);
        char ch='A'+i;
        QString str(ch);
        buttons[i]->setText(str);
        labels[i]->setText("option "+str);
        labels[i]->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
        labels[i]->setWordWrap(true);
        connect(buttons[i],QRadioButton::clicked,this,[=](){
            this->checkedOption=i;
            emit selectOption(i);
        });
        connect(labels[i],clickLabel::clicked,this,[=](){
            buttons[i]->setChecked(true);
            this->checkedOption=i;
            emit selectOption(i);
        });
        QHBoxLayout* layout=new QHBoxLayout(this);
        layout->addWidget(buttons[i],1);
        layout->addWidget(labels[i],5);
        options->addLayout(layout,1);
    }
}

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

void clickOptions::paintBox(QWidget *widget, QRect *box)
{
    if(box!=nullptr){
        QPainter painter(widget);
        QPen pen(Qt::black,5);
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
            painter.fillPath(path,pen.brush());
            pen.setColor(Qt::black);
            pen.setWidth(5);
            painter.setPen(pen);
            painter.drawPath(path);
        }
    }
}

QRect *clickOptions::setOptionOfBox(int i,QRect* rect)
{
    if(i>=numOfOptions)
        return nullptr;
    if(rect==nullptr)
        rect=new QRect;
    rect->setRect(
        buttons[i]->geometry().x(),
        labels[i]->geometry().y(),
        buttons[i]->geometry().width()+labels[i]->geometry().width(),
        labels[i]->geometry().height()
    );
    return rect;
}
