/* Copyright (C), 2014, Shane Betz */
#include "network_server.h"

network_server::network_server(QObject *parent) :
    QObject(parent)
{
    client_list = QList<QTcpSocket*>();
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(slot_new_connection()));
    qDebug() << server->listen(QHostAddress::Any, 5555);
}

void network_server::slot_new_connection(){
    QTcpSocket * socket = server->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    client_list.append(socket);
}

void network_server::slot_readyRead(){
    QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
    qDebug() << socket->readAll();
}
