/* Copyright (C), 2014, Shane Betz */
#ifndef CHAT_BOX_H
#define CHAT_BOX_H

#include <QWidget>
#include <QCloseEvent>
#include <QDebug>
#include <QBoxLayout>
#include <QLabel>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QLineEdit>
#include <QPushButton>

class chat_box : public QWidget
{
    Q_OBJECT
public:
    explicit chat_box(QWidget *parent = 0);
    void setup_chat(QString, QString);

protected:
    void closeEvent(QCloseEvent *event);

private:
    QString recipient_name;
    QString recipient_identity;

    QVBoxLayout         * main_layout;
    QLabel              * header;
    QTableView          * chat_history;
    QStandardItemModel  * chat_history_table;
    QLineEdit           * chat_msg_input;
    QPushButton         * chat_submit_btn;

signals:
    void tx_txt_msg(QString, QString);

public slots:

private slots:
    void send_message();

};

#endif // CHAT_BOX_H
