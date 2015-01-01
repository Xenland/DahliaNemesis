/* Copyright (C), 2014, Shane Betz */
#include "systray_manager.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    systray_manager systray;

    return a.exec();
}
