#include "user_dialog.h"
#include "ui_user_dialog.h"
#include <iostream>

using namespace std;

user_Dialog::user_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::user_Dialog)
{
    ui->setupUi(this);
}

user_Dialog::~user_Dialog()
{
    delete ui;
}


void user_Dialog::on_return_Button_clicked()
{
    this->close();
}

void user_Dialog::on_signal_load_user_dialog() {
    QSqlQuery query_user("SELECT stuid, name, department, num_borrowed, num_limit FROM qlms_user WHERE stuid = '" + QLMS.stuid + "';");

    if (!query_user.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("貌似出现问题咯，您当前所登录的用户在数据库貌似丢失了，请重新登录"));
        this->close();
        return;
    }

    QString label_html;
    label_html = tr("<p>姓名：%1</p><p>学号：%2</p><p>所属学院：%3</p><p>当前借书数量：%4</p><p>最大借书量：%5</p>").arg(query_user.value(1).toString()).arg(query_user.value(0).toString()).arg(query_user.value(2).toString()).arg(query_user.value(3).toString()).arg(query_user.value(4).toString());

    ui->user_info_label->setText(label_html);

    QSqlQuery query_record("SELECT qlms_record.id, qlms_book.isbn, qlms_book.title, qlms_record.status, qlms_record.time_borrow, qlms_record.time_return FROM qlms_record LEFT JOIN qlms_book_item ON qlms_book_item.id = qlms_record.id LEFT JOIN qlms_book ON qlms_book.isbn = qlms_book_item.isbn WHERE qlms_record.stuid = '" + QLMS.stuid + "' ORDER BY qlms_record.id DESC");


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
