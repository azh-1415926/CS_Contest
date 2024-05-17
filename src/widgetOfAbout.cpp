#include "widgetOfAbout.h"

#include <QHBoxLayout>

widgetOfAbout::widgetOfAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfAbout)
{
    ui->setupUi(this);
    /* 初始化窗口 */
    initalWindow();
}

widgetOfAbout::~widgetOfAbout()
{
    delete ui;
}

/* 初始化窗口 */
void widgetOfAbout::initalWindow()
{
    /* pixmap 为控件 info 中显示的图片，读取图片 */
    pixmap.load(":/image/info.png");
    ui->imageOfInfo->setPixmap(pixmap);
}