#include "widgetOfAbout.h"

widgetOfAbout::widgetOfAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfAbout)
{
    ui->setupUi(this);
}

widgetOfAbout::~widgetOfAbout()
{
    delete ui;
}