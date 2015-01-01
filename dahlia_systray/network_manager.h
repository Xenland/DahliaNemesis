/* Copyright (C), 2014, Shane Betz */
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QTcpSocket>
#include "jansson.h"
class network_manager : public QObject
{
    Q_OBJECT
public:
    explicit network_manager(QObject *parent = 0);

private:
    QTcpSocket * socket;

signals:

public slots:
    void send_txt_message(QString,QString);

private slots:

};

#endif // NETWORK_MANAGER_H
