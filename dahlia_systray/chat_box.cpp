/* Copyright (C), 2014, Shane Betz */
#include "chat_box.h"

chat_box::chat_box(QWidget *parent) :
    QWidget(parent)
{
    recipient_name = QString("");
    recipient_identity = QString("");

    main_layout = new QVBoxLayout();
    this->setLayout(main_layout);

    //header (label)
    header = new QLabel("");
    main_layout->addWidget(header);

    //history (tableview)
    chat_history = new QTableView();
    chat_history_table = new QStandardItemModel(0, 1);
    chat_history->setModel(chat_history_table);
    main_layout->addWidget(chat_history);

    //message input (lineedit)
    chat_msg_input = new QLineEdit();
    chat_msg_input->setPlaceholderText(QString("Type your message here"));
    main_layout->addWidget(chat_msg_input);

    //submit message (button)
    chat_submit_btn = new QPushButton(QString("Send Message"));
    connect(chat_submit_btn, SIGNAL(clicked()), this, SLOT(send_message()));
    main_layout->addWidget(chat_submit_btn);
}

void chat_box::closeEvent(QCloseEvent * event){
    event->ignore();
    this->hide();
}

void chat_box::setup_chat(QString name, QString identity){
    recipient_name = QString(name);
    recipient_identity = QString(identity);
    this->setWindowTitle(name);
    header->setText(QString("<h3>%1</h3>").arg(name));
}

void chat_box::incoming_msg(QString msg){
    int row_count = chat_history_table->rowCount();
    QStandardItem * msg_stditem = new QStandardItem(msg);
    chat_history_table->setItem(row_count, 0, msg_stditem);
}


void chat_box::send_message(){
    //get the inputted message
    QString recip_ident = recipient_identity;
    QString msg_to_send = chat_msg_input->text();
    emit tx_txt_msg(recip_ident, msg_to_send);
}
