/* Copyright (C), 2014, Shane Betz */
#include "network_server.h"

network_server::network_server(QObject *parent) : QObject(parent){
    tx_msg_map = QMap<QString, json_t*>();
    client_map = QMap<QTcpSocket*, QString>();
    server = new QTcpServer();
    connect(server, SIGNAL(newConnection()), this, SLOT(slot_new_connection()));
    if(server->listen(QHostAddress::Any, 5555) == true){
        QTimer::singleShot(1, this, SLOT(slot_unload_msg_queue()));
    }

}

void network_server::slot_new_connection(){
    QTcpSocket * socket = server->nextPendingConnection();
    connect(socket, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_readyRead()));
    client_map.insert(socket, "");
}

void network_server::slot_readyRead(){
    QTcpSocket * socket = qobject_cast<QTcpSocket*>(sender());
    QByteArray client_request_bytearray = socket->readAll();
    char * client_request = client_request_bytearray.data();
    qDebug() << "CLIENT REQUEEST: " << client_request;
    json_error_t error;
    json_t * root_msg = json_loadb(client_request, strlen(client_request)+1, JSON_DISABLE_EOF_CHECK, &error);
    if(!root_msg){
        //failed to load
        qDebug() << "failed to load";
        qDebug() << error.text;
    }else{
        //extract request type
        char * request_type = (char*)json_string_value(json_object_get(root_msg, "request_type"));
        if(strcmp(request_type, "tx_msg") == 0){
            //extract recipient_identity
            char * recipient_identity = (char*)json_string_value(json_object_get(root_msg, "recipient_identity"));
            QString recipient_identity_string = QString::fromUtf8(recipient_identity, strlen(recipient_identity));
            //If a queue of messages already exist append to it, other wise create the queue and add the message
            if(tx_msg_map.contains(recipient_identity_string) == true){
                //extract json refernce and append to the object with this message packet
                json_t * multi_msg_root = tx_msg_map[recipient_identity];
                json_t * multi_msg_root_array = json_object_get(multi_msg_root, "msgs");

                //extract message (payload)
                char * message = (char*)json_string_value(json_object_get(root_msg, "msg"));

                /** Generate a storable JSON message to be stored inside the json array package, that will be later sent to the recipient/recieving identity **/
                json_t * recv_root = json_object();
                json_t * recv_sender_identity = json_string(client_map[socket].toUtf8().data());
                json_object_set(recv_root, "sender_identity", recv_sender_identity);

                json_t * recv_msg = json_string(message);
                json_object_set(recv_root, "msg", recv_msg);

                //merge the multi message array with the msg packet/payload
                json_array_append(multi_msg_root_array, recv_root);

                qDebug() << json_dumps(multi_msg_root, JSON_COMPACT);
            }else{
                qDebug() << "NEW MULTI MSG";
                //extract message (payload)
                char * message = (char*)json_string_value(json_object_get(root_msg, "msg"));

                //extract recipient_identity
                char * recipient_identity = (char*)json_string_value(json_object_get(root_msg, "recipient_identity"));

                //create a json package that will store potentially multiple messages
                json_t * multi_msg_root = json_object();
                json_t * multi_msg_root_array = json_array();
                json_object_set(multi_msg_root, "msgs", multi_msg_root_array);

                //create the actual msg packet to store inside the multi-msg json package
                json_t * recv_msg_root = json_object();
                json_t * recv_sender_identity = json_string(client_map[socket].toUtf8().data());
                json_object_set(recv_msg_root, "sender_identity", recv_sender_identity);

                json_t * recv_msg = json_string(message);
                json_object_set(recv_msg_root, "msg", recv_msg);

                //merge the multi msg root with the msg packet/payload
                json_array_append(multi_msg_root_array, recv_msg_root);

                QString recipient_identity_string = QString::fromUtf8(recipient_identity, strlen(recipient_identity));

                //store message to memory
                tx_msg_map.insert(recipient_identity_string, multi_msg_root);
            }
        }else if(strcmp(request_type, "greetings") == 0){
            //extract client identity
            char * client_identity = (char*)json_string_value(json_object_get(root_msg, "identity"));
            QString client_identity_string = QString::fromUtf8(client_identity, strlen(client_identity));

            //set socket identity information
            client_map[socket] = client_identity_string;
        }else{
            //no recognized request type
            qDebug() << "no recognized request type";
        }
    }
}

void network_server::slot_unload_msg_queue(){
    //scroll the connected clients and unload/push queued up messages per client
    QMap<QTcpSocket*, QString>::const_iterator iterator = client_map.constBegin();
    while(iterator != client_map.constEnd()){
        if(tx_msg_map.contains(iterator.value()) == true){
            /* client connected has queued messages to be pushed to client */
            //extract multi msg queue
            json_t * multi_msg_queue_root = tx_msg_map.value(iterator.value());

            //convert json_t to char string
            char * multi_msg_queue_char = json_dumps(multi_msg_queue_root, JSON_COMPACT);

            //push to client socket
            QTcpSocket * client_socket = client_map.key(iterator.value());
            client_socket->write(multi_msg_queue_char, (quint64)strlen(multi_msg_queue_char));

            //remove messages from queue
            qDebug() << "removed:" << tx_msg_map.remove(iterator.value());
        }
        ++iterator;
    }
    QTimer::singleShot(1, this, SLOT(slot_unload_msg_queue()));
}
