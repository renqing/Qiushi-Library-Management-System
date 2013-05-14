#ifndef LOGIN_DIALOG_H
#define LOGIN_DIALOG_H

#include "qlms_public.h"

#include <QDialog>

namespace Ui {
class login_Dialog;
}

class login_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit login_Dialog(QWidget *parent = 0);
    ~login_Dialog();
    
private slots:
    void on_login_action_button_clicked();

    void on_login_cancel_button_clicked();

private:
    Ui::login_Dialog *ui;

signals:
    void signal_change_login_status();
};

extern QLMS_CLASS QLMS;
extern bool isLogin;

#endif // LOGIN_DIALOG_H
