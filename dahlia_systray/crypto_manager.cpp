/* Copyright (C), 2014, Shane Betz */
#include "crypto_manager.h"

crypto_manager::crypto_manager(QObject *parent) :
    QObject(parent)
{
    sodium_init();

    //generate an identity for this client
    crypto_box_keypair(publickey, secretkey);
}

void crypto_manager::get_identity_encoded(){
    //Convert identity(binary) to hex
    char hexoutput[crypto_box_PUBLICKEYBYTES * 2 + 1];
    sodium_bin2hex(hexoutput, (crypto_box_PUBLICKEYBYTES * 2 + 1), publickey, (size_t)crypto_box_PUBLICKEYBYTES);
    qDebug() << hexoutput;
}
