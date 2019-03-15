#include <string.h>
#include "login.h"
#include "connect_server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    char ad[100],port[50];
    /* exista toate argumentele in linia de comanda? */
    if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

    strcpy(ad,argv[1]);
    strcpy(port,argv[2]);
    client_connect(ad,port);

    QApplication a(argc, argv);

    login l;
    l.show();
    return a.exec();
}
