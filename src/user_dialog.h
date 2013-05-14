#ifndef USER_DIALOG_H
#define USER_DIALOG_H

#include <QDialog>
#include "qlms_public.h"

namespace Ui {
class user_Dialog;
}

class user_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit user_Dialog(QWidget *parent = 0);
    ~user_Dialog();
    
private slots:
    void on_return_Button_clicked();
    void on_signal_load_user_dialog();

private:
    Ui::user_Dialog *ui;
};

extern QLMS_CLASS QLMS;

#endif // USER_DIALOG_H
