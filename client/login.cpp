#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "connect_server.h"
#include <QMessageBox>
#include <QCloseEvent>

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}
void login::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question( this,"ReadsProfiler",
                                                                tr("Are you sure?\n"),
                                                                QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                                                QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        client_quit();
        event->accept();
    }
}

void login::on_loginButton_clicked()
{
    //if button is clicked send username to client
    QString username=ui->usernameEdit->text();

    if(client_login(username.toLatin1().data())==1)
    {   hide();
        mainWindow mainWind;
        mainWind.setModal(true);
        mainWind.exec();
    }
    else
    {
        QMessageBox::information(this,"Cannot log in","Username is invalid");
    }
}
