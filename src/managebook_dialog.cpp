#include "managebook_dialog.h"
#include "ui_managebook_dialog.h"
#include <QFileDialog>
#include <QFile>
#include <iostream>

using namespace std;

manageBook_Dialog::manageBook_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::manageBook_Dialog)
{
    ui->setupUi(this);
    manageBook_Dialog::on_guide_booklist_clicked();
}

manageBook_Dialog::~manageBook_Dialog()
{
    delete ui;
}

void manageBook_Dialog::on_single_clear_clicked()
{
    ui->single_location->setText("");
    ui->single_num_total->setText("1");
    ui->single_price->setText("");
    ui->single_pub_press->setText("");
    ui->single_pub_year->setText("");
    ui->single_title->setText("");
    ui->single_type->setText("");
    ui->single_author->setText("");
    ui->single_isbn->setText("");
}

void manageBook_Dialog::on_single_add_clicked()
{
    QString location = ui->single_location->text();
    QString pub_press = ui->single_location->text();
    QString title = ui->single_title->text();
    QString type = ui->single_type->text();
    QString price = ui->single_price->text();
    QString author = ui->single_author->text();
    QString _isbn = ui->single_isbn->text();
    _isbn.replace("-","");
    _isbn.replace(" ","");
    int pub_year = ui->single_pub_year->text().toInt();
    int num_total = ui->single_num_total->text().toInt();
    int isbn = _isbn.toInt();

    if (ui->single_isbn->text() == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("ISBN编号没有填写哦"));
        return;
    }

    if (location == "" || pub_press == "" || title == "" || type == "" || price == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("在添加图书的过程中，表单没有填写完整，请检查"));
        return;
    }

    if (pub_year < 1000 || pub_year > 3000) {
        QMessageBox::warning(this, tr("貌似出错了?"), tr("您填写的图书的出版日期过于神奇，请检查"));
        return;
    }

    if (num_total < 1) {
        QMessageBox::warning(this, tr("出错啦"), tr("图书数量必须大于0哦"));
        return;
    }

    QSqlQuery query(tr("SELECT * FROM qlms_book WHERE isbn = %1").arg(isbn));
    if (query.next()) {
        //图书已经存在
        int msg_ret = QMessageBox::information(this, tr("询问"), tr("您要添加的图书ISBN已经存在，您是否要进行单册图书入库操作？"), QMessageBox::Yes | QMessageBox::No);
        if (msg_ret == QMessageBox::No) return;
        for (int i=0;i<num_total;i++) {
            QSqlQuery(tr("INSERT INTO qlms_book_item (isbn,status,location) VALUES(%1,1,'%2')").arg(isbn).arg(location));
            QSqlQuery(tr("UPDATE qlms_book SET num_total = num_total + 1 WHERE isbn = %1").arg(isbn));
        }
        QMessageBox::information(this, tr("操作成功"), tr("图书已经完成入库操作"));

    } else {
        //新图书哦
        QSqlQuery(tr("INSERT INTO qlms_book (isbn,title,type,pub_press,pub_year,author,price,num_total) VALUES(%1,'%2','%3','%4','%5','%6',%7,%8)").arg(isbn).arg(title).arg(type).arg(pub_press).arg(pub_year).arg(author).arg(price).arg(num_total));
        for (int i=0;i<num_total;i++) {
            QSqlQuery(tr("INSERT INTO qlms_book_item (isbn,status,location) VALUES(%1,1,'%2')").arg(isbn).arg(location));
        }
        QMessageBox::information(this, tr("操作成功"), tr("图书已经完成录入操作"));
    }

}

void manageBook_Dialog::on_guide_booklist_clicked()
{
    QSqlQuery query("SELECT isbn,title,type,pub_press,pub_year,author,price,num_total FROM qlms_book ORDER BY isbn");

    QStandardItemModel* listModel=new QStandardItemModel(0,5,this);
    listModel->insertRow(0);
    listModel->setData(listModel->index(0,0), tr("ISBN"));
    listModel->setData(listModel->index(0,1), tr("图书名称"));
    listModel->setData(listModel->index(0,2), tr("出版社"));
    listModel->setData(listModel->index(0,3), tr("作者"));
    listModel->setData(listModel->index(0,4), tr("数量"));

    int i(1);

    while (query.next()) {
        listModel->insertRow(i);
        book_isbn[i] = query.value(0).toInt();
        listModel->setData(listModel->index(i,0), query.value(0).toString());
        listModel->setData(listModel->index(i,1), query.value(1).toString());
        listModel->setData(listModel->index(i,2), query.value(3).toString());
        listModel->setData(listModel->index(i,3), query.value(5).toString());
        listModel->setData(listModel->index(i,4), query.value(7).toString());

        i++;
    }

    ui->userview->setModel(listModel);

    ui->group_booklist->show();
    ui->group_add_single->hide();
}

void manageBook_Dialog::on_userview_clicked(const QModelIndex &index)
{
    if (index.row() < 1) return;

    emit signal_load_bookItem(book_isbn[index.row()]);
    emit signal_show_dialog(1);
}

void manageBook_Dialog::on_guide_add_single_clicked()
{
    ui->group_add_single->show();
    ui->group_booklist->hide();
}

void manageBook_Dialog::on_guide_add_batch_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择要批量导入的文件"));

    QFile file(fileName);
    if (file.open( QFile::ReadOnly )) {
        QTextStream stream(&file);
        QString line;
        while (!stream.atEnd()) {
            line = stream.readLine();
            QString str = "";
            QString str_array[10];
            int j(0);
            for (int i=0;i<line.size();i++) {
                if (line[i] == '(') continue;
                if (line[i] == ',' || line[i] == ')') {
                    str_array[++j] = str;
                    str = "";
                } else {
                    if (str != "" or line[i] != ' ') str = str + tr("%1").arg(line[i]);
                }
            }
            add_new_book(str_array[4], str_array[3], str_array[2], str_array[7], str_array[6], str_array[5].toInt(), str_array[8].toInt(), str_array[1]);
        }

        file.close();
    } else {
        QMessageBox::information(this, tr("出错啦"), tr("您选择的文件不可读取"));
        return;
    }

}

void manageBook_Dialog::add_new_book(QString pub_press, QString title, QString type, QString price, QString author, int pub_year, int num_total, QString _isbn) {

    _isbn.replace("-","");
    _isbn.replace(" ","");
    int isbn = _isbn.toInt();
    QString location = tr("默认位置");

    if (location == "" || pub_press == "" || title == "" || type == "" || price == "") {
        return;
    }

    if (pub_year < 1000 || pub_year > 3000) {
        return;
    }

    if (num_total < 1) {
        return;
    }

    QSqlQuery query(tr("SELECT * FROM qlms_book WHERE isbn = %1").arg(isbn));
    if (query.next()) {
        //图书已经存在
        for (int i=0;i<num_total;i++) {
            QSqlQuery(tr("INSERT INTO qlms_book_item (isbn,status,location) VALUES(%1,1,'%2')").arg(isbn).arg(location));
            QSqlQuery(tr("UPDATE qlms_book SET num_total = num_total + 1 WHERE isbn = %1").arg(isbn));
        }

    } else {
        //新图书哦
        QSqlQuery(tr("INSERT INTO qlms_book (isbn,title,type,pub_press,pub_year,author,price,num_total) VALUES(%1,'%2','%3','%4','%5','%6','%7',%8)").arg(isbn).arg(title).arg(type).arg(pub_press).arg(pub_year).arg(author).arg(price).arg(num_total));
        for (int i=0;i<num_total;i++) {
            QSqlQuery(tr("INSERT INTO qlms_book_item (isbn,status,location) VALUES(%1,1,'%2')").arg(isbn).arg(location));
        }
    }

    manageBook_Dialog::on_guide_booklist_clicked();
    QMessageBox::information(this, tr("操作成功"), tr("恭喜您，已经完成图书批量录入工作"));
}
