/* Copyright (C), 2014, Shane Betz */
#include "network_manager.h"

network_manager::network_manager(QObject *parent) : QObject(parent) {
    //init
    hexencoded_identity = (char*) malloc(crypto_box_PUBLICKEYBYTES * 2 + 1);
    socket = new QTcpSocket();
    connect(socket, SIGNAL(connected()),this, SLOT(client_connected_to_server()));
    socket->connectToHost(QString("127.0.0.1"), 5555);
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_message()));
}

void network_manager::send_txt_message(QString identity, QString nonce, QString message_to_send){
    /* Generate a JSON message packet to send */
    //create json root
    json_t * root_msg = json_object(); //root of json message packet

    //create and insert request type(tx_msg) into root
    json_t * request_type = json_string("tx_msg");
    json_object_set(root_msg, "request_type", request_type);

    //convert recipient_identity(qstring) to json_string and insert into root
    json_t * recipient_identity_info = json_string(identity.toUtf8().data());
    json_object_set(root_msg, "recipient_identity", recipient_identity_info);

    //convert nonce(qstring) to json_string and insert into root
    json_t * nonce_info = json_string(nonce.toUtf8().data());
    json_object_set(root_msg, "nonce", nonce_info);

    //convert message_to_send(qstring) to json_string and insert into root
    json_t * msg_to_send = json_string(message_to_send.toUtf8().data());
    json_object_set(root_msg, "msg", msg_to_send);

    //Convert JSON message to char
    char * client_char_data = json_dumps(root_msg, JSON_COMPACT);
    socket->write(client_char_data, (quint64)strlen(client_char_data));
}

void network_manager::set_identity(char * new_hexencoded_identity){
    hexencoded_identity = new_hexencoded_identity;
}

char * network_manager::get_identity(){
    return hexencoded_identity;
}

void network_manager::client_connected_to_server(){
    //notify server our identity so we don't get disconnected
    /* Generate a JSON message packet to send */
    //create json root
    json_t * root_msg = json_object(); //root of json message packet

    //create and insert request type(tx_msg) into root
    json_t * request_type = json_string("greetings");
    json_object_set(root_msg, "request_type", request_type);

    //convert client identity(char) to json_string and insert into root
    json_t * client_identity_info = json_string(this->get_identity());
    json_object_set(root_msg, "identity", client_identity_info);

    //Convert JSON message to char
    char * greetings_char_data = json_dumps(root_msg, JSON_COMPACT);
    socket->write(greetings_char_data, (quint64)strlen(greetings_char_data));
}


void network_manager::read_message(){
    //convert char to json_t
    char * incoming_msg_char = socket->readAll().data();
    json_error_t incoming_msg_error;
    json_t * incoming_msg = json_loadb(incoming_msg_char, strlen(incoming_msg_char)+1, JSON_DISABLE_EOF_CHECK, &incoming_msg_error);

    //push to chat manager
    emit new_incoming_msg(incoming_msg);
}

