#include "bookitem_dialog.h"
#include "ui_bookitem_dialog.h"

bookitem_Dialog::bookitem_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookitem_Dialog)
{
    ui->setupUi(this);
}

bookitem_Dialog::~bookitem_Dialog()
{
    delete ui;
}

void bookitem_Dialog::on_signal_load_bookItem(int isbn) {
    global_isbn = isbn;
    QSqlQuery query(tr("SELECT title, isbn, type, pub_press, pub_year, author, price, num_total FROM qlms_book WHERE isbn = %1").arg(isbn));

    if (!query.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("您要查询的图书不存在哦，可能程序出现了问题"));
        this->close();
        return;
    }

    QString html;
    html = tr("<table width=\"560\" border=\"0\">");
    html = html + tr("<tr><td colspan=\"2\">图书名称：%1</td></tr>").arg(query.value(0).toString());
    html = html + tr("<tr><td>图书类型：%1</td>\n<td>ISBN：%2</td></tr>").arg(query.value(2).toString()).arg(query.value(1).toString());
    html = html + tr("<tr colspan=\"2\"><td>出版社：%1</td></tr>").arg(query.value(3).toString());
    html = html + tr("<tr colspan=\"2\"><td>作者：%1</td></tr>").arg(query.value(5).toString());
    html = html + tr("<tr><td>出版年份：%1</td>\n").arg(query.value(4).toString());
    html = html + tr("<tr><td>单册价格：%1</td>\n<td>馆藏数量：%2册</td></tr>").arg(query.value(6).toString()).arg(query.value(7).toString());
    html = html + tr("</table");

    ui->bookItem_label->setText(html);

    QSqlQuery query_list(tr("SELECT qlms_book_item.id, qlms_book_item.location, qlms_book_item.status, qlms_user.name FROM qlms_book_item LEFT JOIN qlms_record ON (qlms_record.id = qlms_book_item.id AND qlms_record.status = 0) LEFT JOIN qlms_user ON (qlms_user.stuid = qlms_record.stuid) WHERE isbn = %1 ORDER BY status, id;").arg(isbn));

    QStandardItemModel* booklistModel=new QStandardItemModel(0,4,this);
    booklistModel->insertRow(0);
    booklistModel->setData(booklistModel->index(0,0), tr("图书编号"));
    booklistModel->setData(booklistModel->index(0,1), tr("图书位置"));
    booklistModel->setData(booklistModel->index(0,2), tr("单册状态"));
    booklistModel->setData(booklistModel->index(0,3), tr("借书者"));

    int i(1);

    while (query_list.next()) {
        booklistModel->insertRow(i);
        query_list_book[i] = query_list.value(0).toInt();
        booklistModel->setData(booklistModel->index(i,0), query_list.value(0).toString());
        booklistModel->setData(booklistModel->index(i,1), query_list.value(1).toString());
        booklistModel->setData(booklistModel->index(i,2), query_list.value(2).toInt() == 1 ? tr("在馆") : tr("已借出"));
        if (query_list.value(2).toInt() == 1) {
            booklistModel->setData(booklistModel->index(i,3), tr("图书馆在馆"));
        } else {
            booklistModel->setData(booklistModel->index(i,3), query_list.value(3).toString());
        }

        i++;
    }

    ui->bookview->setModel(booklistModel);
}

void bookitem_Dialog::on_bookview_clicked(const QModelIndex &index)
{
    if (index.row() == 0) return;

    QSqlQuery query(tr("SELECT id, status FROM qlms_book_item WHERE id = %1").arg(query_list_book[index.row()]));

    if (!QLMS.check_isUserLogin()) {
        QMessageBox::warning(this, tr("出错啦"), tr("亲，您还没有登录图书管理系统，不能进行借阅哦"));
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("貌似您要查看的图书被猫吃了？还是数据库出问题了-_-#"));
        return;
    }

    if (query.value(1).toInt() == 0) {
        QMessageBox::warning(this, tr("出错啦"), tr("这本图书已经被借出咯，无法进行借阅操作"));
        return;
    } else {
        int msg_ret = QMessageBox::information(this, tr("询问"), tr("您是否要借阅该册图书？"), QMessageBox::Yes | QMessageBox::No);
        if (msg_ret == QMessageBox::No) return;

        if (QLMS.modify_user_book(1)) {
            QSqlQuery(tr("UPDATE qlms_book_item SET status = 0 WHERE id = %1").arg(query_list_book[index.row()]));
            QSqlQuery(tr("INSERT INTO qlms_record (id, stuid, status, time_borrow, time_return) VALUES(%1, %2, 0, NOW(), NULL)").arg(query_list_book[index.row()]).arg(QLMS.stuid));
            QMessageBox::information(this, tr("借阅成功"), tr("恭喜您，单册读书已经借阅完毕"));
            bookitem_Dialog::on_signal_load_bookItem(global_isbn);
        } else {
            QMessageBox::warning(this, tr("超出借阅上限"), tr("对不起，您已经超出图书借阅上限了T^T"));
            return;
        }
    }
}
