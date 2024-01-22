#include "CS_Contest.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CS_Contest w;
    w.show();
    return a.exec();
}