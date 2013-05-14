#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    if (!QLMS.check_DatabaseConnect()) {
        QMessageBox::warning(this, tr("通信故障"), tr("连接数据库服务器出现问题，请检查网络"));
        this->close();
    }

    inst_login_Dialog = new login_Dialog;
    inst_user_Dialog = new user_Dialog;
    inst_book_Dialog = new book_Dialog;
    inst_bookItem_Dialog = new bookitem_Dialog;
    inst_returnBook_Dialog = new returnBook_Dialog;
    inst_userManagement_Dialog = new userManagement_Dialog;
    inst_manageBook_Dialog = new manageBook_Dialog;
    connect(inst_login_Dialog, SIGNAL(signal_change_login_status()), this, SLOT(on_signal_change_login_status()));
    connect(this, SIGNAL(signal_load_user_dialog()), inst_user_Dialog, SLOT(on_signal_load_user_dialog()));
    connect(this,SIGNAL(signal_init_book_dialog()), inst_book_Dialog, SLOT(on_signal_init_book_dialog()));
    connect(inst_book_Dialog, SIGNAL(signal_load_bookItem(int)), inst_bookItem_Dialog, SLOT(on_signal_load_bookItem(int)));
    connect(inst_book_Dialog, SIGNAL(signal_show_dialog(int)), this, SLOT(on_signal_show_dialog(int)));
    connect(inst_manageBook_Dialog, SIGNAL(signal_load_bookItem(int)), inst_bookItem_Dialog, SLOT(on_signal_load_bookItem(int)));
    connect(inst_manageBook_Dialog, SIGNAL(signal_show_dialog(int)), this, SLOT(on_signal_show_dialog(int)));

    MainWindow::on_signal_change_login_status();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_main_log_Button_clicked()
{
    if (!QLMS.check_isUserLogin()) {
        inst_login_Dialog->show();
    } else {
        QLMS.user_logout();
        MainWindow::on_signal_change_login_status();
    }
}

void MainWindow::on_signal_change_login_status(){
    if (QLMS.check_isUserLogin()) {
        ui->main_log_Button->setText(tr("注销"));
        ui->main_user_status_label->setText(tr("当前使用者：%1 [学号:%2]").arg(QLMS.name).arg(QLMS.stuid));
        ui->main_userinfo_Button->setEnabled(true);
        if (QLMS.isAdmin) {
            ui->main_returnBook_Button->setEnabled(true);
            ui->main_manageBook_Button->setEnabled(true);
            ui->main_userManagement_Button->setEnabled(true);
        }
    } else {
        ui->main_log_Button->setText(tr("登录"));
        ui->main_user_status_label->setText("");
        ui->main_userinfo_Button->setEnabled(false);
        ui->main_returnBook_Button->setEnabled(false);
        ui->main_manageBook_Button->setEnabled(false);
        ui->main_userManagement_Button->setEnabled(false);
    }
}

void MainWindow::on_main_userinfo_Button_clicked()
{
    if (!QLMS.check_isUserLogin()) {
        QMessageBox::warning(this, tr("尚未登录"), tr("亲，您尚未登录求是图书管理系统，请您登录后再查看您的信息"));
        inst_login_Dialog->show();
        return;
    }

    inst_user_Dialog->show();
    emit signal_load_user_dialog();
}

void MainWindow::on_main_bookSearch_Button_clicked()
{
    inst_book_Dialog->show();
    emit signal_init_book_dialog();
}

void MainWindow::on_signal_show_dialog(int dialog_id) {
    switch (dialog_id) {
        case 1: inst_bookItem_Dialog->show();
    }
}

void MainWindow::on_main_returnBook_Button_clicked()
{
    if (!QLMS.check_isUserLogin() || !QLMS.isAdmin) {
        QMessageBox::warning(this, tr("尚未登录"), tr("亲，您尚未登录求是图书管理系统或者没有管理权限哟"));
        return;
    }

    inst_returnBook_Dialog->show();
}

void MainWindow::on_main_userManagement_Button_clicked()
{
    if (!QLMS.check_isUserLogin() || !QLMS.isAdmin) {
        QMessageBox::warning(this, tr("尚未登录"), tr("亲，您尚未登录求是图书管理系统或者没有管理权限哟"));
        return;
    }

    inst_userManagement_Dialog->show();
}

void MainWindow::on_main_manageBook_Button_clicked()
{
    if (!QLMS.check_isUserLogin() || !QLMS.isAdmin) {
        QMessageBox::warning(this, tr("尚未登录"), tr("亲，您尚未登录求是图书管理系统或者没有管理权限哟"));
        return;
    }

    inst_manageBook_Dialog->show();
}
