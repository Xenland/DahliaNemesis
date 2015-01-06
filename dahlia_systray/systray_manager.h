/* Copyright (C), 2014, Shane Betz */
#ifndef SYSTRAY_MANAGER_H
#define SYSTRAY_MANAGER_H

#include <QWidget>
#include <QMenu>
#include <QIcon>
#include <QAction>
#include <QFile>
#include <QDebug>
#include <QPixmap>
#include <QSystemTrayIcon>
#include "crypto_manager.h"
#include "contact_book.h"
#include "chat_manager.h"
#include "network_manager.h"

class systray_manager : public QWidget{
    Q_OBJECT

public:
    systray_manager(QWidget *parent = 0);
    ~systray_manager();

signals:
    void request_to_exit();
    void execute_exit();

private:
    crypto_manager  * crypto;
    network_manager * network;
    QSystemTrayIcon * system_tray;
    QMenu           * system_tray_menu;
    QIcon             system_tray_icon;
    QAction         * system_tray_contacts_actions;
    QAction         * system_tray_contacts_account;
    QAction         * system_tray_quit_action;

    contact_book * contacts;
    chat_manager * chats;

private slots:
    void account_triggered();
    void contacts_triggered();
    void show_chatbox(QString,QString);
    void slot_send_txt_message(QString,QString);
    void exit_app();
};

#endif // SYSTRAY_MANAGER_H
