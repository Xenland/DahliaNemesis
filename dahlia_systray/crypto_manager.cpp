/* Copyright (C), 2014, Shane Betz */
#include "crypto_manager.h"

crypto_manager::crypto_manager(QObject *parent) :
    QObject(parent)
{
    sodium_init();

    //generate an identity for this client
    crypto_box_keypair(publickey, secretkey);
}

char * crypto_manager::get_identity_encoded(){
    //Convert identity(binary) to hex
    char * hexoutput = (char*)malloc(crypto_box_PUBLICKEYBYTES * 2 + 1);
    sodium_bin2hex(hexoutput, (crypto_box_PUBLICKEYBYTES * 2 + 1), publickey, (size_t)crypto_box_PUBLICKEYBYTES);
    qDebug() << hexoutput;
    return hexoutput;
}

unsigned char * crypto_manager::get_secret_identity(){
    return secretkey;
}

QMap<QString, char*> crypto_manager::encrypt_txt_msg(QString identity, QString message_to_send){
    QMap<QString, char*>output = QMap<QString, char*>();

    //convert recipients identity(qstring) to char
    char * identity_hexencoded_char = identity.toUtf8().data();
    size_t identity_hexencoded_char_size = strlen(identity_hexencoded_char);

    //convert char (hex encoded) to unsigned char (binary) identity
    unsigned char identity_uchar[identity_hexencoded_char_size];
    size_t bin_len;
    const char * identity_hex_end;
    sodium_hex2bin(identity_uchar, identity_hexencoded_char_size, identity_hexencoded_char, identity_hexencoded_char_size, NULL, &bin_len, &identity_hex_end);

    //convert message to send to char
    char * message_to_send_char = message_to_send.toUtf8().data();
    size_t message_to_send_char_size = crypto_box_MACBYTES + strlen(message_to_send_char);

    //encrypt message
    unsigned char nonce[crypto_box_NONCEBYTES]; //init memory for nonce
    unsigned char ciphertext[message_to_send_char_size]; //init memory for output/ciphertext
    randombytes_buf(nonce, sizeof(nonce)); //define a randomized nonce
    crypto_box_easy(ciphertext, (unsigned char*)message_to_send_char, message_to_send_char_size, nonce, identity_uchar, this->get_secret_identity());

    //convert nonce(binary) to hex
    size_t nonce_hex_size = crypto_box_NONCEBYTES * 2 + 1;
    char * nonce_hex = (char*)malloc(nonce_hex_size);
    sodium_bin2hex(nonce_hex, nonce_hex_size, nonce, crypto_box_NONCEBYTES);
    output.insert(QString("nonce"), nonce_hex);

    //convert ciphertext(binary) to hex
    size_t ciphertext_hex_size = message_to_send_char_size * 2 + 1;
    char * ciphertext_hex = (char*)malloc(ciphertext_hex_size);
    sodium_bin2hex(ciphertext_hex, ciphertext_hex_size, ciphertext, message_to_send_char_size);
    output.insert(QString("ciphertext"), ciphertext_hex);

    return output;
}
