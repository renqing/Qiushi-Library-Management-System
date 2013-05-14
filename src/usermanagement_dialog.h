#ifndef USERMANAGEMENT_DIALOG_H
#define USERMANAGEMENT_DIALOG_H

#include "qlms_public.h"
#include <QDialog>

namespace Ui {
class userManagement_Dialog;
}

class userManagement_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit userManagement_Dialog(QWidget *parent = 0);
    ~userManagement_Dialog();
    
private slots:
    void on_newuser_add_Button_clicked();

    void on_guide_user_Button_clicked();

    void on_guide_newuser_Button_clicked();

    void on_manageuser_modify_Button_clicked();

    void on_userview_clicked(const QModelIndex &index);

    void on_manageuser_delete_Button_clicked();

private:
    Ui::userManagement_Dialog *ui;
    int user_list_stuid[1000];

};

#endif // USERMANAGEMENT_DIALOG_H

