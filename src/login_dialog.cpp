#include <QMessageBox>
#include <QtSql>

#include "login_dialog.h"
#include "ui_login_dialog.h"

login_Dialog::login_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login_Dialog)
{
    ui->setupUi(this);
}

login_Dialog::~login_Dialog()
{
    delete ui;
}

void login_Dialog::on_login_action_button_clicked()
{
    QString stuid, password;
    stuid = ui->login_stuid_label->text();
    password = ui->login_password_label->text();

    if (stuid == "") {
        QMessageBox::warning(this, tr("登录失败"), tr("学号尚未填写"));
        return;
    }

    if (password == "") {
        QMessageBox::warning(this, tr("登录失败"), tr("密码尚未填写"));
        return;
    }

    QSqlQuery query("SELECT stuid, password, name, num_borrowed, num_limit, isadmin FROM qlms_user WHERE stuid = '" + stuid + "';");

    if (query.next()) {
        if (query.value(1) == password) {
            QLMS.isLogin = 1;
            QLMS.stuid = stuid;
            QLMS.name = query.value(2).toString();
            QLMS.isAdmin = query.value(5).toInt();
            QLMS.set_number(query.value(3).toInt(), query.value(4).toInt());

            QMessageBox::information(this, tr("登录成功"), tr("登录成功，欢迎%1登录QLMS系统~").arg(query.value(2).toString()));
            ui->login_stuid_label->setText("");
            ui->login_password_label->setText("");
            emit signal_change_login_status();
            this->close();
        } else {
            QMessageBox::information(this, tr("登录失败"), tr("您提供的密码错误，请检查后重新登录"));
            ui->login_password_label->setText("");
        }
    } else {
        QMessageBox::warning(this, tr("登录失败"), tr("学号不存在"));
        return;
    }

}

void login_Dialog::on_login_cancel_button_clicked()
{
    ui->login_stuid_label->setText("");
    ui->login_password_label->setText("");
    this->close();
}
