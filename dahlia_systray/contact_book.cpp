/* Copyright (C), 2014, Shane Betz */
#include "contact_book.h"

contact_book::contact_book(QWidget *parent) : QWidget(parent){
    this->setWindowTitle(QString("Contacts Book"));

    //main layout
    contact_layout = new QVBoxLayout();
    this->setLayout(contact_layout);

    //buttons group layout
    contact_actions_group_widget = new QWidget();
    contact_actions_group = new QHBoxLayout();
    contact_layout->addWidget(contact_actions_group_widget);

    //add buttons to layout group for buttons
    contact_actions_group_widget->setLayout(contact_actions_group);
    contact_add_btn = new QPushButton(QString("Add"), this);
    connect(contact_add_btn, SIGNAL(clicked()), this, SLOT(add_contact()));
    contact_actions_group->addWidget(contact_add_btn);
    contact_remove_btn = new QPushButton(QString("Remove"), this);
    contact_actions_group->addWidget(contact_remove_btn);
    contact_chat_btn = new QPushButton(QString("Chat"), this);
    contact_actions_group->addWidget(contact_chat_btn);
    connect(contact_chat_btn, SIGNAL(clicked()), this, SLOT(show_chat_box()));

    //add table
    contact_table_list = new QTableView();
    contact_table_list_model = new QStandardItemModel(0,2,0);
    contact_table_list_model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
    contact_table_list_model->setHorizontalHeaderItem(1, new QStandardItem(QString("Identity")));
    contact_table_list->setModel(contact_table_list_model);
    contact_layout->addWidget(contact_table_list);

    //init pages
    add_contact_page = new QWidget();
    add_contact_layout = new QVBoxLayout();
    add_contact_page->setLayout(add_contact_layout);
    add_contact_page->setWindowTitle(QString("Add Contact"));
    add_contact_header = new QLabel(QString("<h3>Add a Contact</h3>"));
    add_contact_layout->addWidget(add_contact_header);
    add_contact_name = new QLineEdit(QString(""));
    add_contact_name->setPlaceholderText(QString("Contacts Name"));
    add_contact_layout->addWidget(add_contact_name);
    add_contact_identity = new QLineEdit(QString(""));
    add_contact_identity->setPlaceholderText(QString("Identity Information"));
    add_contact_layout->addWidget(add_contact_identity);
    add_contact_btn = new QPushButton(QString("Add to contacts book"));
    connect(add_contact_btn, SIGNAL(clicked()), this, SLOT(commit_add_contact()));
    add_contact_layout->addWidget(add_contact_btn);

    //init error message box
    selection_error = new QMessageBox();
}

void contact_book::closeEvent(QCloseEvent * event){
    event->ignore();
    this->hide();
}

void contact_book::add_contact(){
    //show window to add a contact
    add_contact_page->show();
}

void contact_book::commit_add_contact(){
    //commit
    int row_count = contact_table_list_model->rowCount();
    QStandardItem * name = new QStandardItem(add_contact_name->text());
    contact_table_list_model->setItem(row_count, 0, name);
    QStandardItem * identity = new QStandardItem(add_contact_identity->text());
    contact_table_list_model->setItem(row_count, 1, identity);

    //hide and clear add contact page
    add_contact_page->hide();
    add_contact_name->setText(QString(""));
    add_contact_identity->setText(QString(""));
}

void contact_book::show_chat_box(){
    //get selected contact to show chat box for
    QModelIndex selected_index = contact_table_list->selectionModel()->currentIndex();
    if(selected_index.isValid()){
        //extract selected row data
        QVariant selected_name = contact_table_list_model->index(selected_index.row(), 0).data();
        QVariant selected_identity = contact_table_list_model->index(selected_index.row(), 1).data();

        //emit information to chat manager to show the chat box for the selected contact
        emit show_chat(selected_name.toString(), selected_identity.toString());
    }else{
        selection_error->setWindowTitle(QString("Who are you trying to contact?"));
        selection_error->setText(QString("Please select an identity to chat with"));
        selection_error->show();
    }
}
