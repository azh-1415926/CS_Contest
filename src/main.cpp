#include "CS_Contest.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CS_Contest w;
    w.show();
    return a.exec();
}