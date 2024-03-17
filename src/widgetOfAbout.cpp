#include "widgetOfAbout.h"

#include <QHBoxLayout>
#include <QResizeEvent>

widgetOfAbout::widgetOfAbout(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui_widgetOfAbout)
    , info(new QLabel)
{
    ui->setupUi(this);
    /* 初始化窗口 */
    initalWindow();
}

widgetOfAbout::~widgetOfAbout()
{
    delete ui;
    delete info;
}

/* 重写窗口大小调整事件，窗口变化便重新调整说明图片的大小 */
void widgetOfAbout::resizeEvent(QResizeEvent *e)
{
    /* 当 pixmap 不为空时调整大小并重设图片，缺点是只能变大不能变小，之后再考虑修复 */
    if(!pixmap.isNull())
        // info->setPixmap(pixmap.scaled(info->size(),Qt::KeepAspectRatio, Qt::SmoothTransformation));
        info->setPixmap(pixmap);
}

/* 初始化窗口 */
void widgetOfAbout::initalWindow()
{
    /* 以垂直布局为当前窗口的布局 */
    QHBoxLayout* layout=new QHBoxLayout(this);
    /* 显示控件为 info，其显示内容设为居中 */
    info->setAlignment(Qt::AlignCenter);
    /* pixmap 为控件 info 中显示的图片，读取图片 */
    pixmap.load(":/image/info.png");
    /*
        填充 info，图片会变形，故不启用
        label->setScaledContents(true);
    */
    layout->addWidget(info);
}