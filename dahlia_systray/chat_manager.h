/* Copyright (C), 2014, Shane Betz */
#ifndef CHAT_MANAGER_H
#define CHAT_MANAGER_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include "chat_box.h"
#include "jansson.h"

class chat_manager : public QObject
{
    Q_OBJECT
public:
    explicit chat_manager(QObject *parent = 0);
    void show_chat(QString,QString);

private:
    QMap<QString, chat_box*> chatbox_list;
    QMap<QString, json_t*> chat_history;
signals:
    void net_tx_txt_msg(QString,QString);

public slots:
    void slot_incoming_msg(json_t*);

private slots:
    void slot_tx_txt_msg(QString,QString);

};

#endif // CHAT_MANAGER_H
