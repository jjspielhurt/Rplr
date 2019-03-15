//#define _GNU_SOURCE
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string.h>
#include <QMessageBox>
#include "connect_server.h"
#include "rate.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>

#include <sys/stat.h>
#include <fcntl.h>


mainWindow::mainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::mainWindow)
{
    ui->setupUi(this);

}

mainWindow::~mainWindow()
{

    delete ui;
}
void mainWindow::reject()
{
    QMessageBox::StandardButton resBtn = QMessageBox::Yes;

        resBtn = QMessageBox::question( this, "ReadsProfiler",
                                        tr("Are you sure?\n"),
                                        QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes,
                                        QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        client_quit();
        QDialog::reject();
    }
}


void mainWindow::on_searchButton_clicked()
{
    QString search=ui->lineEdit->text();
    client_search(search.toLatin1().data());

    char filename[50],id[50];

    sprintf(id,"%d",getpid());
    strcpy(filename,"search_");
    strcat(filename,id);
    strcat(filename,".txt");
    ui->listWidget->clear();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file,line))
    {
        ui->listWidget->addItem(line.c_str());
    }
    remove(filename);
}

void mainWindow::on_rateButton_clicked()
{
    if (ui->listWidget->count() == 0)
    {
         QMessageBox::information(this,"Cannot Rate","Search for a book you would wish to rate.");

    }
    else if(ui->listWidget->currentItem()==NULL)
    {
        QMessageBox::information(this,"No book selected","Please select a book from the list.");
    }
    else
    {
       QString book_info=ui->listWidget->currentItem()->text();
       char* book_info_char=book_info.toLatin1().data();
       char book_id[10];
       int i=0;
       while(book_info_char[i]!='.'&&i<strlen(book_info_char))
       {   book_id[i]=book_info_char[i];
           i++;
       }
       book_id[i]='\0';
       char filename[50],id[50];

       sprintf(id,"%d",getpid());
       strcpy(filename,"rate_id");
       strcat(filename,id);
       strcat(filename,".txt");
       //write the name of the book we want to read
       std::ofstream file(filename);
       file<< book_id;
       file.close();
       rate rateWind;
       rateWind.setModal(true);
       rateWind.exec();

    }


}

void mainWindow::on_downloadButton_clicked()
{
    if (ui->listWidget->count() == 0)
    {
         QMessageBox::information(this,"Cannot Download","Search for a book you would wish to download");

    }
    else if(ui->listWidget->currentItem()==NULL)
    {
        QMessageBox::information(this,"No book selected","Please select a book from the list");
    }
    else
    {
        QMessageBox::StandardButton reply =QMessageBox::question(this,"Download","Are you sure you want to download",
                                                                   QMessageBox::Yes|QMessageBox::No);
           if(reply==QMessageBox::Yes)
           {
               //download
                QString book_info=ui->listWidget->currentItem()->text();
                char* book_info_char=book_info.toLatin1().data();
                char book_id[10];
                int i=0;
                while(book_info_char[i]!='.')
                {   book_id[i]=book_info_char[i];
                    i++;
                }
                book_id[i]='\0';

               client_download(book_id);
           }
    }

}

void mainWindow::on_pushButton_recom_clicked()
{
    if (ui->listWidget_recom->count() == 0)
    {
         QMessageBox::information(this,"Cannot Download","Search for a book you would wish to download");

    }
    else if(ui->listWidget_recom->currentItem()==NULL)
    {
        QMessageBox::information(this,"No book selected","Please select a book from the list");
    }
    else
    {
        QMessageBox::StandardButton reply =QMessageBox::question(this,"Download","Are you sure you want to download",
                                                                   QMessageBox::Yes|QMessageBox::No);
           if(reply==QMessageBox::Yes)
           {
               //download
                QString book_info=ui->listWidget_recom->currentItem()->text();
                char* book_info_char=book_info.toLatin1().data();
                char book_id[10];
                int i=0;
                while(book_info_char[i]!='.')
                {   book_id[i]=book_info_char[i];
                    i++;
                }
                book_id[i]='\0';

               client_download(book_id);
           }
    }

}

void mainWindow::on_pushButton_refresh_clicked()
{

    char filename[50],id[50];
    ui->listWidget_recom->clear();
    sprintf(id,"%d",getpid());
    strcpy(filename,"recom_");
    strcat(filename,id);
    strcat(filename,".txt");
    client_get_recommendations();
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file,line))
    {
        ui->listWidget_recom->addItem(line.c_str());
    }
    remove(filename);
}

void mainWindow::on_quitButton_clicked()
{
    QMessageBox::StandardButton reply =QMessageBox::question(this,"Quit","Are you sure?",
                                                            QMessageBox::Yes|QMessageBox::No);
    if(reply==QMessageBox::Yes)
    {
        client_quit();
        QApplication::quit();
    }

}
