#include "clicklabel.h"

clickLabel::clickLabel(QWidget *parent)
    : QLabel(parent)
{
}

clickLabel::~clickLabel()
{
}

/* 重写鼠标点击事件，点击便会触发 clicked 信号，传递该对象的指针 */
void clickLabel::mousePressEvent(QMouseEvent *e)
{
    emit clicked(this);
}