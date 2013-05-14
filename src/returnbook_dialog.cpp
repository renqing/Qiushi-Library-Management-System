#include "returnbook_dialog.h"
#include "ui_returnbook_dialog.h"

returnBook_Dialog::returnBook_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::returnBook_Dialog)
{
    ui->setupUi(this);
}

returnBook_Dialog::~returnBook_Dialog()
{
    delete ui;
}

void returnBook_Dialog::on_search_Button_clicked()
{
    if (ui->stuid_label->text() == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("借书者学号尚未输入，不能进行单用户操作哦"));
        return;
    }

    QString stuid = ui->stuid_label->text();
    last_stuid = stuid.toInt();

    returnBook_Dialog::load_book_list(last_stuid);
}

void returnBook_Dialog::action_returnBook(int id) {
    QSqlQuery query_bookItem(tr("SELECT qlms_record.uuid, qlms_book_item.status, qlms_record.stuid FROM qlms_book_item LEFT JOIN qlms_record ON (qlms_record.status = 0 AND qlms_record.id = qlms_book_item.id) WHERE qlms_book_item.id = %1 ").arg(id));

    if (!query_bookItem.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("您要操作的单册图书不存在哦"));
        return;
    }

    if (query_bookItem.value(1).toInt() == 1) {
        QMessageBox::warning(this, tr("出错啦"), tr("您要操作的单册图书已经在馆"));
        return;
    }

    int msg_ret = QMessageBox::information(this, tr("询问"), tr("请确认图书已经归还完毕"), QMessageBox::Yes | QMessageBox::No);
    if (msg_ret == QMessageBox::No) return;

    QSqlQuery(tr("UPDATE qlms_record SET status = 1, time_return = NOW() WHERE uuid = %1").arg(query_bookItem.value(0).toInt()));
    QSqlQuery(tr("UPDATE qlms_user SET num_borrowed = num_borrowed - 1 WHERE stuid = %1").arg(query_bookItem.value(2).toInt()));
    QSqlQuery(tr("UPDATE qlms_book_item SET status = 1 WHERE id = %1").arg(id));

    QMessageBox::information(this, tr("操作成功"), tr("操作成功，图书已经完成单册归还"));
    returnBook_Dialog::load_book_list(last_stuid);
}

void returnBook_Dialog::on_borrowedbookview_clicked(const QModelIndex &index)
{
    if (index.row() <=0 ) return;

    returnBook_Dialog::action_returnBook(book_id_list[index.row()]);
}

void returnBook_Dialog::on_return_Button_clicked()
{
    if (ui->bookId_label->text() == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("您还没有填写要归还的图书编号"));
        return;
    }

    returnBook_Dialog::action_returnBook(ui->bookId_label->text().toInt());
}

void returnBook_Dialog::load_book_list(int stuid) {
    QSqlQuery query_user(tr("SELECT stuid, name, department, num_borrowed, num_limit FROM qlms_user WHERE stuid = %1;").arg(stuid));


    if (!query_user.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("这个借书证[学号]不存在哟，请重新输入"));
        return;
    }

    QSqlQuery query_record(tr("SELECT qlms_record.id, qlms_book.isbn, qlms_book.title, qlms_record.status, qlms_record.time_borrow, qlms_record.time_return FROM qlms_record LEFT JOIN qlms_book_item ON qlms_book_item.id = qlms_record.id LEFT JOIN qlms_book ON qlms_book.isbn = qlms_book_item.isbn WHERE qlms_record.stuid = %1 ORDER BY qlms_record.status, qlms_record.uuid DESC").arg(stuid));

    QStandardItemModel* booklistModel=new QStandardItemModel(0,6,this);
    booklistModel->insertRow(0);
    booklistModel->setData(booklistModel->index(0,0), tr("图书编号"));
    booklistModel->setData(booklistModel->index(0,1), tr("图书ISBN"));
    booklistModel->setData(booklistModel->index(0,2), tr("图书名称"));
    booklistModel->setData(booklistModel->index(0,3), tr("图书状态"));
    booklistModel->setData(booklistModel->index(0,4), tr("借阅时间"));
    booklistModel->setData(booklistModel->index(0,5), tr("归还时间"));

    int i(1);

    while (query_record.next()) {
        booklistModel->insertRow(i);
        book_id_list[i] = query_record.value(0).toInt();
        booklistModel->setData(booklistModel->index(i,0), query_record.value(0).toString());
        booklistModel->setData(booklistModel->index(i,1), query_record.value(1).toString());
        booklistModel->setData(booklistModel->index(i,2), query_record.value(2).toString());
        booklistModel->setData(booklistModel->index(i,3), query_record.value(3).toInt() == 0 ? tr("未归还") : tr("已归还"));
        booklistModel->setData(booklistModel->index(i,4), query_record.value(4).toString());
        booklistModel->setData(booklistModel->index(i,5), query_record.value(5).toString());

        i++;
    }

    ui->borrowedbookview->setModel(booklistModel);
}
