#include "usermanagement_dialog.h"
#include "ui_usermanagement_dialog.h"

userManagement_Dialog::userManagement_Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userManagement_Dialog)
{
    ui->setupUi(this);

    userManagement_Dialog::on_guide_user_Button_clicked();
}

userManagement_Dialog::~userManagement_Dialog()
{
    delete ui;
}

void userManagement_Dialog::on_newuser_add_Button_clicked()
{
    if (ui->newuser_department->text() == "" || ui->newuser_num_limit->text() == ""|| ui->newuser_password->text() == "" || ui->newuser_stuid->text() == "" || ui->newuser_name->text() == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("新用户的具体信息没有填写完整"));
        return;
    }

    QString stuid = ui->newuser_stuid->text();
    QString password = ui->newuser_password->text();
    QString department = ui->newuser_department->text();
    QString name = ui->newuser_name->text();
    int num_limit = ui->newuser_num_limit->text().toInt();
    int isAdmin =  ui->newuser_isadmin->currentText() == tr("管理员");

    stuid.replace("'","");
    password.replace("'","");
    department.replace("'","");
    name.replace("'","");

    if (num_limit < 0) {
        QMessageBox::warning(this, tr("出错啦"), tr("用户借阅图书上限不能小于0哦"));
        return;
    }

    QSqlQuery query_user(tr("SELECT * FROM qlms_user WHERE stuid = %1").arg(stuid));

    if (query_user.next()) {
        QMessageBox::warning(this, tr("出错啦"), tr("您要添加的学号已经被使用过咯"));
        return;
    }

    QSqlQuery(tr("INSERT INTO qlms_user (stuid,name,password,department,num_limit,num_borrowed,isAdmin) VALUES(%1,'%2','%3','%4',%5,0,%6)").arg(stuid).arg(name).arg(password).arg(department).arg(num_limit).arg(isAdmin));

    ui->newuser_department->setText("");
    ui->newuser_name->setText("");
    ui->newuser_num_limit->setText(tr("10"));
    ui->newuser_password->setText("");
    ui->newuser_stuid->setText("");

    QMessageBox::information(this, tr("操作成功"), tr("恭喜您，新用户已经成功创建完毕"));
}

void userManagement_Dialog::on_guide_user_Button_clicked()
{
    QSqlQuery query("SELECT stuid, name, department, num_borrowed, isadmin FROM qlms_user ORDER BY stuid;");


    QStandardItemModel* listModel=new QStandardItemModel(0,5,this);
    listModel->insertRow(0);
    listModel->setData(listModel->index(0,0), tr("学号"));
    listModel->setData(listModel->index(0,1), tr("姓名"));
    listModel->setData(listModel->index(0,2), tr("单位"));
    listModel->setData(listModel->index(0,3), tr("借书量"));
    listModel->setData(listModel->index(0,4), tr("身份"));

    int i(1);

    while (query.next()) {
        listModel->insertRow(i);
        user_list_stuid[i] = query.value(0).toInt();
        listModel->setData(listModel->index(i,0), query.value(0).toString());
        listModel->setData(listModel->index(i,1), query.value(1).toString());
        listModel->setData(listModel->index(i,2), query.value(2).toString());
        listModel->setData(listModel->index(i,3), query.value(3).toString());
        listModel->setData(listModel->index(i,4), query.value(4).toInt() == 1 ? tr("管理员") : tr("读者"));

        i++;
    }

    ui->userview->setModel(listModel);
    ui->group_user->show();
    ui->group_newuser->hide();
    ui->group_manageuser->hide();
}

void userManagement_Dialog::on_guide_newuser_Button_clicked()
{
    ui->group_newuser->show();
    ui->group_user->hide();
    ui->group_manageuser->hide();
}

void userManagement_Dialog::on_manageuser_modify_Button_clicked()
{
    int stuid = ui->manage_stuid->text().toInt();
    int num_limit = ui->manage_num_limit->text().toInt();
    int isAdmin = ui->manage_isadmin->currentText() == tr("管理员") ? 1 : 0;
    QString name = ui->manage_name->text();
    QString department = ui->manage_department->text();
    QString password = ui->manage_password->text();

    name.replace("'","");
    department.replace("'","");
    password.replace("'","");

    if (ui->manage_stuid->text() == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("窗口非法调用"));
        return;
    }

    if (num_limit < 0) {
        QMessageBox::warning(this, tr("出错啦"), tr("用户可借阅图书的上限不能为负数"));
        return;
    }

    if (name == "" || department == "" || password == "") {
        QMessageBox::warning(this, tr("出错啦"), tr("用户信息没有填写完整，请检查后再点击修改按钮"));
        return;
    }

    QSqlQuery (tr("UPDATE qlms_user SET name = '%1', department = '%2', password = '%3', num_limit = %4, isadmin = %5 WHERE stuid = %6").arg(name).arg(department).arg(password).arg(num_limit).arg(isAdmin).arg(stuid));

    QMessageBox::information(this, tr("修改成功"), tr("恭喜您，已经成功修改完毕"));
    userManagement_Dialog::on_guide_user_Button_clicked();
}

void userManagement_Dialog::on_userview_clicked(const QModelIndex &index)
{
    if (index.row() < 1) return;

    QSqlQuery query(tr("SELECT name, password, department, num_limit FROM qlms_user WHERE stuid = %1").arg(user_list_stuid[index.row()]));

    if (!query.next()) {
        QMessageBox::information(this, tr("出错啦"), tr("您要操作的用户不存在哦"));
        return;
    }

    ui->manage_name->setText(query.value(0).toString());
    ui->manage_password->setText(query.value(1).toString());
    ui->manage_department->setText(query.value(2).toString());
    ui->manage_num_limit->setText(query.value(3).toString());
    ui->manage_stuid->setText(tr("%1").arg(user_list_stuid[index.row()]));

    ui->group_manageuser->show();
    ui->group_newuser->hide();
    ui->group_user->hide();
}

void userManagement_Dialog::on_manageuser_delete_Button_clicked()
{
    int stuid = ui->manage_stuid->text().toInt();

    QSqlQuery(tr("DELETE FROM qlms_user WHERE stuid = %1").arg(stuid));

    QMessageBox::information(this, tr("删除成功"), tr("恭喜您，已经成功删除该用户"));
    userManagement_Dialog::on_guide_user_Button_clicked();
}
