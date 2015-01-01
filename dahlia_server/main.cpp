/* Copyright (C), 2014, Shane Betz */
#include <QCoreApplication>
#include "network_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    network_server * network = new network_server();

    return a.exec();
}
