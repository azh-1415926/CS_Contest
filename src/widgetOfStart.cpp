#include "widgetOfStart.h"

widgetOfStart::widgetOfStart(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfStart)
{
    ui->setupUi(this);
}

widgetOfStart::~widgetOfStart()
{
    delete ui;
}