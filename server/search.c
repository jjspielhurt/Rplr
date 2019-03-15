//searches the database for a book.returns to the client tbe results and updates the searches database.
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
#include "global.h"

sqlite3 *db;
char *zErrMsg = 0;
int rc;
const char* data = "Callback function called";


char *strlwr(char *str)
{
  unsigned char *p = (unsigned char *)str;

  while (*p) {
     *p = tolower((unsigned char)*p);
      p++;
  }

  return str;
}

int callback_update_searches(void *NotUsed, int argc, char **argv, char **azColName) 
{
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void update_searches(int user,int book)
{
 char sql[5000];
 char user_string[10],book_string[10];
 //convert from integer to string
  sprintf( user_string, "%d", user);
  sprintf( book_string, "%d",book );

 /* Open database */

 /* Create SQL statement */
 strcpy(sql,"INSERT INTO searches(user_id,searched_id) values(");
 strcat(sql,user_string);
 strcat(sql,",");
 strcat(sql,book_string);
 strcat(sql,")");
 printf("%s",sql);

 /* Execute SQL statement */
 rc = sqlite3_exec(db, sql, callback_update_searches, 0, &zErrMsg);
 
 if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
 } else {
    fprintf(stdout, "Records created successfully\n");
 }
 sqlite3_close(db);

}

int callback_books(void *data, int argc, char **argv, char **azColName)
{
  int i,id_book;
  fprintf(stderr, "%s: ", (const char*)data);
  char msgrasp[500];
  bzero(msgrasp,200);
  id_book=atoi(argv[0]);
  printf("user:%d book:%d\n",user_id,id_book );

  strcat(msgrasp,argv[0]);
  strcat(msgrasp,".");

  for(i = 1; i<argc; i++)
  {
  strcat(msgrasp,argv[i]);
  strcat(msgrasp,"|");
  }
      
      /* returnam mesajul clientului */
      if (write (client, msgrasp, 200) <= 0)
      {
        perror ("[server]Eroare la write() catre client.\n");
      }
      else
        printf ("[server]Mesajul a fost trasmis cu succes.\n");
      /* am terminat cu acest client, inchidem conexiunea */
   
  update_searches(user_id,id_book);
  return 0;
}
void search_books(char str[100])
{
 
  char sql[5000];
  int len;
  const char* data = "Callback function called";
  /*Opening the database*/
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }
      /* Create SQL statement */
  len=strlen(str);
  str[len]='%';
  str[len+1]='\"';
  str[len+2]='\0';
  strcpy(str,strlwr(str));
  strcpy(sql,"SELECT id_book,title,author,isbn,publish_year,isbn,average_rating from books where lower(title) like \"%");
  strcat(sql,str);
  strcat(sql,"or lower(genre_id) like \"%");
  strcat(sql,str);
  strcat(sql,"or lower(author) like \"%");
  strcat(sql,str);
  strcat(sql,"or lower(publish_year) like \"%");
  strcat(sql,str);
  strcat(sql,"or lower(isbn) like \"%");
  strcat(sql,str);
  strcat(sql,"or cast(average_rating as text) like \"%");
  strcat(sql,str);
  printf("Command:%s",sql);
  fflush(stdout);

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback_books, (void*)data, &zErrMsg);

  if( rc != SQLITE_OK ) {
  fprintf(stderr, "SQL error: %s\n", zErrMsg);
  sqlite3_free(zErrMsg);
  } else {
  fprintf(stdout, "Operation done successfully\n");
  }
  sqlite3_close(db);
  if (write (client, "#done_search", 100) <= 0)
  {
    perror ("[server]Eroare la write() flag catre client.\n");
  }
  else
    printf ("[server]Mesajul de flag a fost trasmis cu succes.\n");
}
