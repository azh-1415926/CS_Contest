#include "clicklabel.h"

clickLabel::clickLabel(QWidget *parent)
    : QLabel(parent)
{

}

clickLabel::~clickLabel()
{

}

void clickLabel::mousePressEvent(QMouseEvent *e)
{
    emit clicked(this);
}