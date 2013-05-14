#include "book_dialog.h"
#include "ui_book_dialog.h"
#include "qlms_public.h"

book_Dialog::book_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::book_Dialog)
{
    ui->setupUi(this);
    ui->book_group->hide();
}

book_Dialog::~book_Dialog()
{
    delete ui;
}

void book_Dialog::on_return_Button_clicked()
{
    ui->book_group->hide();
    this->close();
}

void book_Dialog::on_search_Button_clicked()
{
    QString keyword = ui->keyword_edit->text();
    keyword.replace("'","");

    QSqlQuery query("SELECT isbn, title, author, num_total, pub_press FROM qlms_book WHERE isbn LIKE '%" + keyword + "%' OR title LIKE '%" + keyword + "%' OR type LIKE '%" + keyword + "%' OR pub_press LIKE '%" + keyword + "%' OR author LIKE '%" + keyword + "%' ORDER BY isbn");

    QStandardItemModel* booklistModel=new QStandardItemModel(0,5,this);
    booklistModel->insertRow(0);
    booklistModel->setData(booklistModel->index(0,0), tr("图书ISBN"));
    booklistModel->setData(booklistModel->index(0,1), tr("图书名称"));
    booklistModel->setData(booklistModel->index(0,2), tr("作者"));
    booklistModel->setData(booklistModel->index(0,3), tr("馆藏数量"));
    booklistModel->setData(booklistModel->index(0,4), tr("出版社"));

    int i(1);

    while (query.next()) {
        booklistModel->insertRow(i);
        book_isbn[i] = query.value(0).toInt();
        booklistModel->setData(booklistModel->index(i,0), query.value(0).toString());
        booklistModel->setData(booklistModel->index(i,1), query.value(1).toString());
        booklistModel->setData(booklistModel->index(i,2), query.value(2).toString());
        booklistModel->setData(booklistModel->index(i,3), query.value(3).toString());
        booklistModel->setData(booklistModel->index(i,4), query.value(4).toString());

        i++;
    }

    ui->borrowedbookview->setModel(booklistModel);
    ui->book_group->show();
}

void book_Dialog::on_signal_init_book_dialog() {
    ui->keyword_edit->setText("");
    ui->book_group->hide();
}

void book_Dialog::on_borrowedbookview_clicked(const QModelIndex &index)
{
    if (index.row() == 0) return;

    emit signal_show_dialog(1);
    emit signal_load_bookItem(book_isbn[index.row()]);
}
