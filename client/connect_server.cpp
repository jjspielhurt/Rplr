#define _BSD_SOURCE
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include "connect_server.h"

/* codul de eroare returnat de anumite apeluri */
extern int errno;
/* portul de conectare la server*/
int port;
int sd;     // descriptorul de socket
struct sockaddr_in server;  // structura folosita pentru conectare 

void client_quit()
{
  if (write (sd,"quit", 100) <= 0)
  {
    perror ("[client][quit]Eroare la write() spre server.\n");
  }
  /* inchidem conexiunea, am terminat */
  close (sd);

}
//I/O;username as char/1 if logged 0 else
int client_login(char username[100])
{
  char msg[100];

  /* citirea mesajului */
  bzero (msg, 100);
  /* trimiterea mesajului la server */
  if (write (sd, username, 100) <= 0)
  {
    perror ("[client][login]Eroare la write() spre server.\n");
    //return errno;
  }
  if (read (sd, msg, 100) < 0)
  {
    perror ("[client][login]Eroare la read() de la server.\n");
    //return errno;
  }
if(strstr(msg,"User logat cu succes")==NULL) return 0;
else return 1;
}
void client_download(char book_id[100])
{
  int b_to_read,block_size=1;
  char carte[320001],b_to_read_char[100],filename[100];
  FILE * fout;
  FILE * fclear;
  //create file
  strcpy(filename,book_id);
  strcat(filename,".txt");
  fclear=fopen(filename,"w");
  fclose(fclear);

  fout=fopen(filename,"a");

  if (write (sd,"download", 100) <= 0)
  {
    perror ("[client][download]Eroare la write() spre server.\n");
    //return errno;
  }
  if (write (sd,book_id, 100) <= 0)
  {
    perror ("[client][download]Eroare la write() spre server.\n");
    //return errno;
  }
  if (read (sd,b_to_read_char, 100) < 0)
  {
    perror ("[client][download]Eroare la read() de la server.\n");
    //return errno;
  }
  printf("[client][download]Primit de citit %s bytes\n",b_to_read_char );
  b_to_read=atoi(b_to_read_char);

  while(b_to_read>0&&block_size>0)
  { bzero(carte,320000);

    if ((block_size=read (sd, carte, 320000)) < 0)
    {
      perror ("[client][download]Eroare la read() de la server.\n");
      //return errno;
    }
    printf("[client][download] Primit block de %d bytes\n",block_size);
    b_to_read=b_to_read-block_size;
    fprintf(fout,"%s",carte);
    //fflush(fout);

  }
}
void client_rate(char book_id[10],char rating_value[2])
{
  char flag[100];
  if (write (sd,"rate", 100) <= 0)
  {
    perror ("[client][rate]Eroare la write() spre server.\n");
  }
  if (write (sd,book_id, 10) <= 0)
  {
    perror ("[client][rate]Eroare la write() spre server.\n");
  }
  if (write (sd,rating_value, 2) <= 0)
  {
    perror ("[client][rate]Eroare la write() spre server.\n");
  }
 
  if (read (sd, flag, 100) < 0)
  {
    perror ("[client][rate]Eroare la read() de la server.\n");
  }
  else printf ("[client][rate]Mesajul primit este: %s\n", flag);
  
}
//I/O:book_name as char
void client_search(char book_name[100])
{
    int all_flag=0;
    char num_results_char[100],result[200];
    FILE * fout;
    FILE * fclear;
    char filename[50],id[50];
    sprintf(id,"%d",getpid());
    strcpy(filename,"search_");
    strcat(filename,id);
    strcat(filename,".txt");
    fclear=fopen(filename,"w");
    fclose(fclear);
    fout=fopen(filename,"a");

    //search intent
    if (write (sd,"search", 100) <= 0)
    {
    perror ("[client][search]Eroare la write() spre server.\n");
    //return errno;
    }
    if (write (sd,book_name, 100) <= 0)
    {
    perror ("[client][search]Eroare la write() spre server.\n");
    //return errno;
    }
    while(all_flag==0)
    {
    if (read (sd, result, 200) < 0)
    {
      perror ("[client][search]Eroare la read() de la server.\n");
      //return errno;
    }
    if(strcmp(result,"#done_search")==0) all_flag=1;
    else //send_result to interface
    {

        fprintf(fout,"%s\n",result);
        fflush(fout);
    }
    }
    fclose(fout);
}
void client_get_recommendations()
{
  int num_recomm;
  char num_recomm_char[10],info[100];
  FILE * fout;
  FILE * fclear;
  char filename[50],id[50];
  sprintf(id,"%d",getpid());
  strcpy(filename,"recom_");
  strcat(filename,id);
  strcat(filename,".txt");
  fclear=fopen(filename,"w");
  fclose(fclear);
  fout=fopen(filename,"a");

  if (write (sd,"recomm", 100) <= 0)
  {
    perror ("[client][recommendations]Eroare la write() spre server.\n");
    //return errno;
  }
  if (read (sd,num_recomm_char, 10) < 0)
  {
    perror ("[client][recommendations]Eroare la read() de la server.\n");
    //return errno;
  }
  num_recomm=atoi(num_recomm_char);
  //printf("num recomm%d\n",num_recomm);
  while(num_recomm!=0)
  {
    if (read (sd, info, 100) < 0)
    {
      perror ("[client][recommendations]Eroare la read() de la server.\n");
      exit(-1);
    }
    fprintf(fout,"%s\n",info);
    fflush(fout);
    num_recomm--;
  }
fclose(fout);
}
void client_connect(char adresa_server_char[100],char port_char[50])
{

  char msg[100];
  /* stabilim portul */
  port = atoi (port_char);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
     exit(-1);
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(adresa_server_char);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      exit(-1);
    }
}
