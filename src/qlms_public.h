#ifndef QLMS_PUBLIC_H
#define QLMS_PUBLIC_H

#include <QMainWindow>
#include <QMessageBox>
#include <QStandardItem>
#include <QtSql>
#include <QSqlError>
#include <QTextCodec>

#define MXN 1000

class QLMS_CLASS {

private:
    bool is_connect_database;
    int num_borrowed;
    int num_limit;

public:
    bool isLogin;
    bool isAdmin;
    QString stuid;
    QString name;

    void user_logout();
    bool modify_user_book(int num);
    void set_number(int _num_borrowed, int _num_limit);
    bool initialize_database();

    bool check_DatabaseConnect();
    bool check_isUserLogin();
    bool check_isAdmin();
};

#include "login_dialog.h"
#include "user_dialog.h"
#include "book_dialog.h"
#include "bookitem_dialog.h"
#include "returnbook_dialog.h"
#include "usermanagement_dialog.h"
#include "managebook_dialog.h"

#endif // QLMS_PUBLIC_H
