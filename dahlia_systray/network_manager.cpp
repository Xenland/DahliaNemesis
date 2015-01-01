/* Copyright (C), 2014, Shane Betz */
#include "network_manager.h"

network_manager::network_manager(QObject *parent) : QObject(parent) {
    socket = new QTcpSocket();
    socket->connectToHost(QString("127.0.0.1"), 5555);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_message()));
}

void network_manager::send_txt_message(QString identity, QString message_to_send){
    /* Generate a JSON message packet to send */
    //create json root
    json_t * root_msg = json_object(); //root of json message packet
    //create and insert request type(tx_msg) into root
    json_t * request_type = json_string("tx_msg");
    json_object_set(root_msg, "request_type", request_type);
    //convert identity(qstring) to json_string and insert into root
    json_t * identity_info = json_string(identity.toUtf8().data());
    json_object_set(root_msg, "identity", identity_info);
    //convert message_to_send(qstring) to json_string and insert into root
    json_t * msg_to_send = json_string(message_to_send.toUtf8().data());
    json_object_set(root_msg, "msg", msg_to_send);

    //Convert JSON message to char
    char * client_char_data = json_dumps(root_msg, JSON_COMPACT);
    socket->write(client_char_data, (quint64)strlen(client_char_data));
}

void network_manager::read_message(){
    qDebug() << socket->readAll();
}
