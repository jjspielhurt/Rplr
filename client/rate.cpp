#include "rate.h"
#include "ui_rate.h"
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include "connect_server.h"
#include <string.h>
rate::rate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::rate)
{
    ui->setupUi(this);
}

rate::~rate()
{
    delete ui;
}


void rate::on_pushButton_5_clicked()
{
    char filename[50],id[50],book_id[10];

    sprintf(id,"%d",getpid());
    strcpy(filename,"rate_id");
    strcat(filename,id);
    strcat(filename,".txt");

    std::ifstream file(filename);
    std::string line;
    std::getline(file,line);
    strcpy(book_id,line.c_str());
    client_rate(book_id,"5");
    close();
    remove(filename);

}
void rate::on_pushButton_4_clicked()
{
    char filename[50],id[50],book_id[10];

    sprintf(id,"%d",getpid());
    strcpy(filename,"rate_id");
    strcat(filename,id);
    strcat(filename,".txt");

    std::ifstream file(filename);
    std::string line;
    std::getline(file,line);
    strcpy(book_id,line.c_str());
    client_rate(book_id,"4");
    close();
    remove(filename);
}
void rate::on_pushButton_3_clicked()
{
    char filename[50],id[50],book_id[10];

    sprintf(id,"%d",getpid());
    strcpy(filename,"rate_id");
    strcat(filename,id);
    strcat(filename,".txt");

    std::ifstream file(filename);
    std::string line;
    std::getline(file,line);
    strcpy(book_id,line.c_str());
    client_rate(book_id,"3");
    close();
    remove(filename);
}
void rate::on_pushButton_2_clicked()
{
    char filename[50],id[50],book_id[10];

    sprintf(id,"%d",getpid());
    strcpy(filename,"rate_id");
    strcat(filename,id);
    strcat(filename,".txt");

    std::ifstream file(filename);
    std::string line;
    std::getline(file,line);
    strcpy(book_id,line.c_str());
    client_rate(book_id,"2");
    close();
    remove(filename);
}
void rate::on_pushButton_1_clicked()
{
    char filename[50],id[50],book_id[10];

    sprintf(id,"%d",getpid());
    strcpy(filename,"rate_id");
    strcat(filename,id);
    strcat(filename,".txt");

    std::ifstream file(filename);
    std::string line;
    std::getline(file,line);
    strcpy(book_id,line.c_str());
    client_rate(book_id,"1");
    close();
    remove(filename);
}

