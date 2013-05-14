#include <QtGui/QApplication>
#include <iostream>
#include "qlms_public.h"
#include "mainwindow.h"

using namespace std;

QLMS_CLASS QLMS;

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QLMS.initialize_database();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}

void QLMS_CLASS::user_logout() {
    isLogin = 0;
    isAdmin = 0;
    stuid = "";
    name = "";
    num_borrowed = 0;
    num_limit = 0;
}

bool QLMS_CLASS::initialize_database() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    db.setDatabaseName("Driver={MySQL ODBC 5.2w Driver}; SERVER=10.202.68.49;DATABASE=dbms;UID=dbms;Pwd=renqing");

    if (!db.open()) {
        cerr << "Error: can't connect to database server!" << endl;
        cerr << "Reason: " << db.lastError().text().toStdString() << endl;
        is_connect_database = 0;

        return 0;
    } else {
        is_connect_database = 1;
    }

    return 1;
}

bool QLMS_CLASS::check_DatabaseConnect() {
    return is_connect_database;
}

bool QLMS_CLASS::check_isUserLogin() {
    return isLogin;
}

bool QLMS_CLASS::check_isAdmin() {
    return isAdmin;
}

bool QLMS_CLASS::modify_user_book(int num) {
    if (!isLogin) return 0;
    if (num_borrowed + num > num_limit) return 0;

    if (num > 0) {
        num_borrowed ++;
        QSqlQuery query("UPDATE qlms_user SET num_borrowed = num_borrowed + 1 WHERE stuid = '" + stuid +"';");
    } else {
        num_borrowed --;
        QSqlQuery query("UPDATE qlms_user SET num_borrowed = num_borrowed - 1 WHERE stuid = '" + stuid +"';");
    }

    return 1;
}

void QLMS_CLASS::set_number(int _num_borrowed, int _num_limit) {
    if (_num_borrowed >= 0) num_borrowed = _num_borrowed;
    if (_num_limit >= 0) num_limit = _num_limit;
}
