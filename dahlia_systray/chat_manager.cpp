/* Copyright (C), 2014, Shane Betz */
#include "chat_manager.h"

chat_manager::chat_manager(QObject *parent) :
    QObject(parent)
{
    //chats list
    chatbox_list = QMap<QString, chat_box*>();
}

void chat_manager::show_chat(QString name, QString identity){
    if(chatbox_list.contains(identity)){
        chat_box * chat = chatbox_list.value(identity);
        chat->show();
    }else{
        //create chat box to communicate with this user
        chat_box * chat = new chat_box();
        connect(chat, SIGNAL(tx_txt_msg(QString, QString)), this, SLOT(slot_tx_txt_msg(QString, QString)));
        chat->setup_chat(name, identity);
        chatbox_list.insert(identity, chat);
        chat->show();
    }
}

void chat_manager::slot_tx_txt_msg(QString identity, QString message){
    emit net_tx_txt_msg(identity, message);
}
