/* Copyright (C), 2014, Shane Betz */
#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QTimer>
#include <QMap>
#include <QList>
#include "jansson.h"
class network_server : public QObject
{
    Q_OBJECT
public:
    explicit network_server(QObject *parent = 0);

private:
    QTcpServer * server;
    QMap<QTcpSocket*, QString> client_map;
    QMap<QString, json_t*> tx_msg_map;
signals:

public slots:

private slots:
    void slot_new_connection();
    void slot_readyRead();

};

#endif // NETWORK_SERVER_H
