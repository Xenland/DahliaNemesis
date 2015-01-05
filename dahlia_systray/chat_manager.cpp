/* Copyright (C), 2014, Shane Betz */
#include "chat_manager.h"

chat_manager::chat_manager(QObject *parent) :
    QObject(parent)
{
    //chats list
    chatbox_list = QMap<QString, chat_box*>();
    chat_history = QMap<QString, json_t*>();
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

void chat_manager::slot_incoming_msg(json_t * multi_msg_root){
    /* Extract msgs and place them into the appropriate chat history queue */
    json_t * array_of_message = json_object_get(multi_msg_root, "msgs");

    //incoming_msg
    size_t total_msgs = json_array_size(array_of_message);
    for(unsigned long a = 0; a < total_msgs; ++a){
        //get message from selected index
        json_t * message_obj = json_array_get(array_of_message, a);
        char * message_obj_char = json_dumps(message_obj, 0);
        qDebug() << "message_obj: " << message_obj_char;

        //extract sender identity
        json_t * sender_identity_jsont = json_object_get(message_obj, "sender_identity");
        //extract msg
        json_t * msg_jsont = json_object_get(message_obj, "msg");

        //send to chat box[temp, eventrually send to chat_history, then chat history sends to chat box]
        char * sender_identity_char = (char*)json_string_value(sender_identity_jsont);
        QString sender_identity_string = QString(sender_identity_char);
        chat_box * chat = chatbox_list.value(sender_identity_string);
        char * chat_msg = (char*)json_string_value(msg_jsont);
        QString message_string = QString(chat_msg);
        chat->incoming_msg(message_string);
    }
}

void chat_manager::slot_tx_txt_msg(QString identity, QString message){
    emit net_tx_txt_msg(identity, message);
}
