#include "BackuperUI.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BackuperUI w;
    w.show();
    return a.exec();
}
