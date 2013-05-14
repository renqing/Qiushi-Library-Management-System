#ifndef BOOKITEM_DIALOG_H
#define BOOKITEM_DIALOG_H

#include "qlms_public.h"
#include <QDialog>

namespace Ui {
class bookitem_Dialog;
}

class bookitem_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit bookitem_Dialog(QWidget *parent = 0);
    ~bookitem_Dialog();
    
private:
    Ui::bookitem_Dialog *ui;
    int query_list_book[MXN];
    int global_isbn;

private slots:
    void on_signal_load_bookItem(int isbn);

    void on_bookview_clicked(const QModelIndex &index);
};

#endif // BOOKITEM_DIALOG_H
