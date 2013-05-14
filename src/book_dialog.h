#ifndef BOOK_DIALOG_H
#define BOOK_DIALOG_H

#include "qlms_public.h"
#include <QDialog>

namespace Ui {
class book_Dialog;
}

class book_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit book_Dialog(QWidget *parent = 0);
    ~book_Dialog();
    
private slots:
    void on_return_Button_clicked();
    void on_search_Button_clicked();

    void on_signal_init_book_dialog();

    void on_borrowedbookview_clicked(const QModelIndex &index);

signals:
    void signal_load_bookItem(int isbn);
    void signal_show_dialog(int dialog_id);

private:
    Ui::book_Dialog *ui;

    int book_isbn[MXN];
};

#endif // BOOK_DIALOG_H
