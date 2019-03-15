#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//headerul de internet
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h> //for tolower

#include <sys/wait.h>

#include <sqlite3.h> 

#include "login.h"
#include "search.h"
#include "download.h"
#include "submit_rating.h"
#include "recommend.h"
#include "global.h"

/* codul de eroare returnat de anumite apeluri */
extern int errno;


int main (int argc,char *argv[])
{
  struct sockaddr_in server;  // structura folosita de server
  struct sockaddr_in from;  
  char msg[100];    //mesajul primit de la client 
  char msgrasp[100]=" ";        //mesaj de raspuns pentru client
  int sd,logged_flag=0,quit_flag=0;     //descriptorul de socket 
  char command[100];
  char rating_value[2],book_id[10];
  int port;
  
  /* exista toate argumentele in linia de comanda? */
  if(argc!=2)
  {
    printf ("Sintaxa: %s <port>\n", argv[0]);
    return -1;
  }
  /* stabilim portul */
  port = atoi (argv[1]);

  /* crearea unui socket */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
  {
    perror ("[server]Eroare la socket().\n");
    return errno;
  }

  /* pregatirea structurilor de date */
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;  
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);//ipv4
  /* utilizam un port utilizator */
    server.sin_port = htons (port);//maxim 2 octeti 
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
  {
    perror ("[server]Eroare la bind().\n");
    return errno;
  }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
  if (listen (sd, 5) == -1)//face o coada de asteptare
  {
    perror ("[server]Eroare la listen().\n");
    return errno;
  }

  /* servim in mod concurent clientii... */
  while (1)
  {
    int length = sizeof (from);
    printf ("[%d][server]Asteptam la portul %d...\n",getpid(),port);
    fflush (stdout);
    int pid_fiu;
    client = accept (sd, (struct sockaddr *) &from, &length);
    if (client < 0)
    {
      perror ("[server]Eroare la accept().\n");
      continue;
    }
    bzero(msg, 100);
    printf ("[%d][server]Asteptam mesajul...\n",getpid());
    fflush (stdout);
    if(-1 == (pid_fiu=fork()) )
    {
    perror("Eroare la fork");
    exit(1);
    }
    /* ramificarea executiei */
    /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
    // printf("pid_fiu:%d\n",pid_fiu);
    
    //printf("fiu %d :::%d\n",pid_fiu,getpid());

    /* eroare la acceptarea conexiunii de la un client */

    /* s-a realizat conexiunea, se astepta mesajul */

    if(pid_fiu == 0)
    { 
      /* zona de cod executata doar de catre fiu */
      printf("pid=%d\n",getpid());
      fflush(stdout);
      //logare
      do
      {
        bzero(msg,100);
        if (read (client, msg, 100) <= 0)
        {
          perror ("[server]Eroare la read() user de la client.\n");
          close (client); /* inchidem conexiunea cu clientul */
          continue;   /* continuam sa ascultam */
        }
      
        printf ("[server]Userul %s incearca sa se conecteze...verificam in baza de date\n", msg);
        logged_flag=login(msg);//se face logarea
      }
      while(logged_flag==0);

      printf("[server] Logare completa\n");
      quit_flag=0;
      do
      {
        //citire comanda
        bzero(command,100);
        if (read (client, command, 100) <= 0)
        {
         perror ("[server]Eroare la read() comanda de la client.\n");
          close (client); /* inchidem conexiunea cu clientul */
          continue;   /* continuam sa ascultam */
        }
        printf("[server]Comanda primita:%s\n",command);
        fflush(stdout);

        if(strcmp(command,"quit")==0)
          quit_flag=1;
        else
        {
          if(strcmp(command,"search")==0)
          {
            bzero(msg,100);
            printf("[server] Cautare carte %s in database.\n",msg);
            if (read (client, msg, 100) <= 0)
            {
              perror ("[server]Eroare la read() de la client.\n");
              close (client); /* inchidem conexiunea cu clientul */
              continue;   /* continuam sa ascultam */
            }
            search_books(msg);
          }

          else if(strcmp(command,"recomm")==0)
          { 
           send_recommendations(user_id);
          }
          else if(strcmp(command,"download")==0)
          { 
            bzero(msg,100);
            if (read (client, msg, 100) <= 0)
            {
              perror ("[server]Eroare la read() de la client.\n");
              close (client); /* inchidem conexiunea cu clientul */
              continue;   /* continuam sa ascultam */
            }
            
            download_book(msg);
          }
          else if(strcmp(command,"rate")==0)
          { 
            bzero(book_id,10);
            bzero(rating_value,2);
            
            if (read (client, book_id, 10) <= 0)
            {
              perror ("[server]Eroare la read() de la client.\n");
              close (client); /* inchidem conexiunea cu clientul */
              continue;   /* continuam sa ascultam */
            }
            if (read (client, rating_value, 2) <= 0)
            {
              perror ("[server]Eroare la read() de la client.\n");
              close (client); /* inchidem conexiunea cu clientul */
              continue;   /* continuam sa ascultam */
            }

            submit_rating(atoi(book_id),atoi(rating_value));
            if (write (client,"#done_rate",100) <= 0)
            {
            perror ("[server]Eroare la write() catre client.\n");
            } 
          }

        }

      }while(quit_flag==0);

      close (client);
    }
    else
    {
      close(client);
      while(waitpid(-1,NULL,WNOHANG)>0); 
    }
  }       
}