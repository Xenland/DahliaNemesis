/* Copyright (C), 2014, Shane Betz */
#ifndef CRYPTO_MANAGER_H
#define CRYPTO_MANAGER_H

#include <QObject>
#include <QDebug>
#include <sodium.h>

class crypto_manager : public QObject
{
    Q_OBJECT
public:
    explicit crypto_manager(QObject *parent = 0);
    char * get_identity_encoded();
    QMap<QString, char*> encrypt_txt_msg(QString, QString);
private:
    unsigned char publickey[crypto_box_PUBLICKEYBYTES];
    unsigned char secretkey[crypto_box_SECRETKEYBYTES];

    unsigned char * get_secret_identity();

signals:

public slots:

};

#endif // CRYPTO_MANAGER_H
