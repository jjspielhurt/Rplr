#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//headerul de internet
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/wait.h>

#include <sqlite3.h> 
#include "global.h"

int log=0,call_log=0;

int callback_login(void *data, int argc, char **argv, char **azColName)
{
  int i;
  call_log=1;
  fprintf(stderr, "%s: ", (const char*)data);
  log=1; 
  user_id=atoi(argv[0]);  
  /* returnam mesajul clientului */
  if (write (client, "User logat cu succes", 100) <= 0)
  {
    perror ("[server]Eroare la write() catre client.\n");
  }
  else
    printf ("[server]Mesajul a fost trasmis cu succes.\n");   
  return 0;
}

int login(char id[100])
{
  sqlite3 *db;
  char *zErrMsg = 0;
  char sql[5000];
  int rc,len;
  const char* data = "Callback function called";
  /*Opening the database*/
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

  /*Create SQL statement*/
  len=strlen(id);
  id[len]='\"';
  id[len+1]='\0';
  strcpy(sql,"SELECT * from users where username like \"");
  strcat(sql,id);
  printf("Command:%s",sql);
  fflush(stdout);
  //execute sql command
  call_log=0;
  rc = sqlite3_exec(db, sql, callback_login, (void*)data, &zErrMsg);
 
 if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
 } else {
    fprintf(stdout, "Operation done successfully\n");
    if(call_log==0)
    {
        if (write (client, "User invalid", 100) <= 0)
      {
        perror ("[server]Eroare la write() catre client.\n");
      }
      else
        printf ("[server]Mesajul a fost trasmis cu succes.\n"); 
    }
 }
  sqlite3_close(db);
 return log;

}