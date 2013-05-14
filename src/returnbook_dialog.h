#ifndef RETURNBOOK_DIALOG_H
#define RETURNBOOK_DIALOG_H

#include "qlms_public.h"
#include <QDialog>

namespace Ui {
class returnBook_Dialog;
}

class returnBook_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit returnBook_Dialog(QWidget *parent = 0);
    ~returnBook_Dialog();
    
private slots:
    void on_search_Button_clicked();

    void on_borrowedbookview_clicked(const QModelIndex &index);

    void on_return_Button_clicked();

private:
    Ui::returnBook_Dialog *ui;

    int book_id_list[1000];
    int last_stuid;

    void action_returnBook(int id);
    void load_book_list(int stuid);
};

#endif // RETURNBOOK_DIALOG_H
