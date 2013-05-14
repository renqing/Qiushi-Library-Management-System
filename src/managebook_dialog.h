#ifndef MANAGEBOOK_DIALOG_H
#define MANAGEBOOK_DIALOG_H

#include "qlms_public.h"
#include <QDialog>

namespace Ui {
class manageBook_Dialog;
}

class manageBook_Dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit manageBook_Dialog(QWidget *parent = 0);
    ~manageBook_Dialog();
    
private slots:
    void on_single_clear_clicked();

    void on_single_add_clicked();

    void on_guide_booklist_clicked();

    void on_userview_clicked(const QModelIndex &index);

    void on_guide_add_single_clicked();

    void on_guide_add_batch_clicked();

signals:
    void signal_load_bookItem(int isbn);
    void signal_show_dialog(int dialog_id);

private:
    Ui::manageBook_Dialog *ui;

    int book_isbn[MXN];
    void add_new_book(QString pub_press, QString title, QString type, QString price, QString author, int pub_year, int num_total, QString _isbn);
};

#endif // MANAGEBOOK_DIALOG_H
