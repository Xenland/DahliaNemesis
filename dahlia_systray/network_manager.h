/* Copyright (C), 2014, Shane Betz */
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTcpSocket>
#include <sodium.h>
#include "jansson.h"
class network_manager : public QObject
{
    Q_OBJECT
public:
    explicit network_manager(QObject *parent = 0);
    void set_identity(char * new_hexencoded_identity);
    char * get_identity();

private:
    QTcpSocket * socket;
    char * hexencoded_identity;

signals:

public slots:
    void send_txt_message(QString,QString);

private slots:
    void client_connected_to_server();
};

#endif // NETWORK_MANAGER_H
