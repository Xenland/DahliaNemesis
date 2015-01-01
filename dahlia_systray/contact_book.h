/* Copyright (C), 2014, Shane Betz */
#ifndef CONTACT_BOOK_H
#define CONTACT_BOOK_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QTableView>
#include <QLabel>
#include <QDebug>
#include <QCloseEvent>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QMessageBox>

class contact_book : public QWidget
{
    Q_OBJECT
public:
    explicit contact_book(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event);

private:
    QVBoxLayout         * contact_layout;
    QWidget             * contact_actions_group_widget;
    QHBoxLayout         * contact_actions_group;
    QPushButton         * contact_add_btn;
    QPushButton         * contact_remove_btn;
    QPushButton         * contact_chat_btn;
    QTableView          * contact_table_list;
    QStandardItemModel  * contact_table_list_model;

    //page (add contacts page)
    QWidget     * add_contact_page;
    QVBoxLayout * add_contact_layout;
    QLabel      * add_contact_header;
    QLineEdit   * add_contact_name;
    QLineEdit   * add_contact_identity;
    QPushButton * add_contact_btn;

    //error message box
    QMessageBox * selection_error;

signals:
    void show_chat(QString, QString);

public slots:

private slots:
    void add_contact();
    void commit_add_contact();
    void show_chat_box();

};

#endif // CONTACT_BOOK_H
