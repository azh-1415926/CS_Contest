#include "widgetOfAbout.h"

#include <QHBoxLayout>
#include <QLabel>

widgetOfAbout::widgetOfAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfAbout)
{
    ui->setupUi(this);
    initalWindow();
}

widgetOfAbout::~widgetOfAbout()
{
    delete ui;
}

void widgetOfAbout::initalWindow()
{
    QHBoxLayout* layout=new QHBoxLayout(this);
    QLabel* label=new QLabel;
    label->setText("Picture");
    layout->addWidget(label);
}