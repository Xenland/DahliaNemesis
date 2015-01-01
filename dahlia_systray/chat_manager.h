/* Copyright (C), 2014, Shane Betz */
#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include "chat_box.h"

class chat_manager : public QObject
{
    Q_OBJECT
public:
    explicit chat_manager(QObject *parent = 0);
    void show_chat(QString,QString);

private:
    QMap<QString, chat_box*> chatbox_list;

signals:
    void net_tx_txt_msg(QString,QString);

public slots:

private slots:
    void slot_tx_txt_msg(QString,QString);

};

#endif // CHAT_MANAGER_H
