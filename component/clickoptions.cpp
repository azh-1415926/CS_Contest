#include "clickoptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMessageBox>

clickOptions::clickOptions(QWidget *parent)
    : QGroupBox(parent)
    , numOfOptions(4)
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

bool clickOptions::eventFilter(QObject *obj, QEvent *e)
{
    // find item in objs,and watch hover event
    int index=buttons.indexOf(obj);
    if(index==-1)
        index=labels.indexOf(obj);
    if(index!=-1&&(e->type()==QEvent::HoverEnter||e->type()==QEvent::HoverLeave)){
        // QMessageBox::warning(nullptr,"warnning",QString::number(index));
        int x=buttons[index]->geometry().x();
        int y=labels[index]->geometry().y();
        int width=buttons[index]->geometry().width()+labels[index]->geometry().width();
        int height=labels[index]->geometry().height();
        if(hoverBox==nullptr){
            hoverBox=new QRect(x,y,width,height);
        }else{
            hoverBox->setRect(x,y,width,height);
        }
        this->update();
        return true;
    }
    if(obj==this&&e->type()==QEvent::Paint){
        paintBox(this,hoverBox);
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
        QPen pen;
        pen.setBrush(QBrush(qRgb(30,144,255)));
        painter.setPen(pen);
        painter.drawRect(QRect(box->topLeft(),box->bottomRight()));
    }
}