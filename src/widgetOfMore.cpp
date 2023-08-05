#include "widgetOfMore.h"

widgetOfMore::widgetOfMore(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfMore)
{
    ui->setupUi(this);
}

widgetOfMore::~widgetOfMore()
{
    delete ui;
}