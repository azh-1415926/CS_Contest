#include "CS_Contest.h"

#include <QApplication>

#ifndef __ANDROID__
#pragma comment(lib, "user32.lib")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CS_Contest w;
    w.show();
    return a.exec();
}