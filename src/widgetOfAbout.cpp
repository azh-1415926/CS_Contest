#include "widgetOfAbout.h"

#include <QHBoxLayout>
#include <QResizeEvent>

widgetOfAbout::widgetOfAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfAbout)
    , info(new QLabel)
{
    ui->setupUi(this);
    initalWindow();
}

widgetOfAbout::~widgetOfAbout()
{
    delete ui;
    delete info;
}

void widgetOfAbout::resizeEvent(QResizeEvent *e)
{
    if(!pixmap.isNull())
        info->setPixmap(pixmap.scaled(info->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void widgetOfAbout::initalWindow()
{
    QHBoxLayout* layout=new QHBoxLayout(this);
    info->setAlignment(Qt::AlignCenter);
    pixmap.load(":/image/info.png");
    /*
        填充 info
        label->setScaledContents(true);
    */
    layout->addWidget(info);
}