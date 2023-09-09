#include "collectbutton.h"

collectButton::collectButton(QWidget *parent, const QString &collect, const QString &uncollect)
    : QPushButton(parent)
    , textOfCollect(collect), textOfUncollect(uncollect)
{
}

collectButton::~collectButton()
{
}

#ifndef __ANDROID__

/* 重写鼠标点击事件，每点击一次，都会触发收藏或取消收藏，并发送 collected、uncollected 信号 */
void collectButton::mousePressEvent(QMouseEvent *e)
{
    if(this->text()==textOfCollect)
    {
        this->setText(textOfUncollect);
        emit uncollected();
    }
    else
    {
        this->setText(textOfCollect);
        emit collected();
    }
}

#endif

/* 设置按钮状态为被收藏，发送 collected 信号 */
void collectButton::setCollect()
{
    this->setText(textOfCollect);
    emit collected();
}

/* 设置按钮状态为取消收藏，发送 uncollected 信号 */
void collectButton::setUncollect()
{
    this->setText(textOfUncollect);
    emit uncollected();
}

/* 设置按钮状态为被收藏，不发送信号 */
void collectButton::setCollectNoSignal()
{
    this->setText(textOfCollect);
}

/* 设置按钮状态为取消收藏，不发送信号 */
void collectButton::setUncollectNoSignal()
{
    this->setText(textOfUncollect);
}