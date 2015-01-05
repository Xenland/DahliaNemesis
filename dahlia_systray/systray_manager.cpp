/* Copyright (C), 2014, Shane Betz */
#include "systray_manager.h"

systray_manager::systray_manager(QWidget *parent) : QWidget(parent){
    connect(this, SIGNAL(execute_exit()), this, SLOT(close()));

    //init network
    network = new network_manager();

    //Init crypto
    crypto = new crypto_manager();

    //tell network our identity
    network->set_identity(crypto->get_identity_encoded());

    //Initalize the system tray operations
    system_tray = new QSystemTrayIcon();

    //init contact book
    contacts = new contact_book();
    connect(contacts, SIGNAL(show_chat(QString,QString)), this, SLOT(show_chatbox(QString,QString)));

    //init chats
    chats = new chat_manager();
    connect(chats, SIGNAL(net_tx_txt_msg(QString,QString)), network, SLOT(send_txt_message(QString,QString)));

    connect(network, SIGNAL(new_incoming_msg(json_t*)), chats, SLOT(slot_incoming_msg(json_t*)));

    //Initalize the menu for the system tray
    system_tray_menu = new QMenu();
    system_tray_menu->addAction(QString("Dahlia Menu"));
    system_tray_menu->addSeparator();

    system_tray_contacts_account = new QAction(QString("My Account"), system_tray);
    connect(system_tray_contacts_account, SIGNAL(triggered()), this, SLOT(account_triggered()));
    system_tray_menu->addAction(system_tray_contacts_account);

    system_tray_contacts_actions = new QAction(QString("Contact Book"), system_tray);
    connect(system_tray_contacts_actions, SIGNAL(triggered()), this, SLOT(contacts_triggered()));
    system_tray_menu->addAction(system_tray_contacts_actions);

    system_tray_menu->addSeparator();
    system_tray_quit_action = new QAction(QString("Close Dahlia"), system_tray);
    connect(system_tray_quit_action, SIGNAL(triggered()), this, SLOT(exit_app()));
    system_tray_menu->addAction(system_tray_quit_action);

    //Merge menu with system tray
    system_tray->setContextMenu(system_tray_menu);

    //Setup icon
    system_tray_icon = QIcon(":/icons/systray_icon.png");
    system_tray->setIcon(system_tray_icon);

    //Show tray
    system_tray->show();
}

systray_manager::~systray_manager(){

}

void systray_manager::contacts_triggered(){
    contacts->show();
}

void systray_manager::account_triggered(){
    crypto->get_identity_encoded();
}

void systray_manager::show_chatbox(QString name, QString identity){
    chats->show_chat(name, identity);
}

void systray_manager::exit_app(){
    emit execute_exit();
}
