/* Copyright (C), 2014, Shane Betz */
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTcpSocket>
#include <sodium.h>
#include "jansson.h"
#include "crypto_manager.h"

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
    void new_incoming_msg(json_t*);

public slots:
    void send_txt_message(QString,QString,QString);

private slots:
    void client_connected_to_server();
    void read_message();
};

#endif // NETWORK_MANAGER_H
