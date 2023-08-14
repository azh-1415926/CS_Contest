#include "collectbutton.h"

collectButton::collectButton(QWidget *parent, const QString &collect, const QString &uncollect)
    : QPushButton(parent)
    , textOfCollect(collect)
    , textOfUncollect(uncollect)
{

}

collectButton::~collectButton()
{

}

bool collectButton::isCollect()
{
    return this->text()==textOfCollect;
}

void collectButton::mousePressEvent(QMouseEvent *e)
{
    if(this->text()==textOfCollect){
        this->setText(textOfUncollect);
        emit uncollected();
    }else{
        this->setText(textOfCollect);
        emit collected();
    }
}

void collectButton::setCollect()
{
    this->setText(textOfCollect);
    emit collected();
}

void collectButton::setUncollect()
{
    this->setText(textOfUncollect);
    emit uncollected();
}

void collectButton::setCollectNoSignal()
{
    this->setText(textOfCollect);
}

void collectButton::setUncollectNoSignal()
{
    this->setText(textOfUncollect);
}