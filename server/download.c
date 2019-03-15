#include "global.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//headerul de internet
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include<ctype.h> //for tolower
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>


int callback_update_downloads(void *NotUsed, int argc, char **argv, char **azColName) 
{
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void update_downloads(int user,int book)
{
 char sql[5000];
 char user_string[10],book_string[10];
 sqlite3 *db;
char *zErrMsg = 0;
int rc;
 //convert from integer to string
  sprintf( user_string, "%d", user);
  sprintf( book_string, "%d",book );

 /* Open database */
    const char* data = "Callback function called";
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

 /* Create SQL statement */
 strcpy(sql,"INSERT INTO downloads(user_id,book_id) values(");
 strcat(sql,user_string);
 strcat(sql,",");
 strcat(sql,book_string);
 strcat(sql,")");
 printf("%s",sql);

 /* Execute SQL statement */
 rc = sqlite3_exec(db, sql, callback_update_downloads, 0, &zErrMsg);
 
 if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
 } else {
    fprintf(stdout, "[downloads]Records created successfully\n");
 }
 sqlite3_close(db);

}


void download_book(char book_id[100])
{
	//get path to pdf
	char path[50],buff[320001],length[100];
	int book_file,size;
	strcpy(path,"./books/");
	strcat(path,book_id);
	strcat(path,".txt");
	
	book_file=open(path,O_RDONLY);

	bzero(buff,320000);

	struct stat st;
	fstat(book_file, &st);
	size = st.st_size;
	sprintf(length,"%d",size);

	if (write (client, length, strlen(length)) <= 0)
	{
	perror ("[server]Eroare la write() catre client.\n");
	}
	bzero(buff,320000);
	while(read(book_file,&buff,320000)==320000)
	{

		if (write (client, buff, 320000) <= 0)
		{
		perror ("[server]Eroare la write() catre client.\n");
		}
		else
		printf ("[server]Un buffer de %d a fost trasmis cu succes.\n",strlen(buff));
	bzero(buff,320000);
	}
	write(client,buff,strlen(buff));
	update_downloads(user_id,atoi(book_id));

}
