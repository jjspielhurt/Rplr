#include "global.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>
float average_rating;
int num_ratings;

 int callback_average_rating(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: ", (const char*)data);
  average_rating=average_rating+atoi(argv[0]);
  num_ratings++;
  return 0;
}
 int callback_average_rating_update(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: ", (const char*)data);
  return 0;
}



void compute_average_rating(int book_id)
{
	average_rating=0;
	num_ratings=0;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[5000],book_string[100];
	const char* data = "Callback function called";
	/* Open database */
	rc = sqlite3_open("database.db", &db);

	if( rc ) {
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	exit(0);
	} else {
	fprintf(stderr, "Opened database successfully\n");
	}

	sprintf( book_string, "%d",book_id);

	/* Create SQL statement */
	strcpy(sql,"SELECT rating from ratings where id_book=");
	strcat(sql,book_string);

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_average_rating, (void*)data, &zErrMsg);

	if( rc != SQLITE_OK ) {
	  fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	} else {
	  fprintf(stdout, "Operation done successfully\n");
	}

  sqlite3_close(db);

}
void update_average_rating(int book_id)
{
	compute_average_rating(book_id);
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char sql[5000],book_string[100],average_string[100];
	const char* data = "Callback function called";
	/* Open database */
	rc = sqlite3_open("database.db", &db);

	if( rc ) {
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	exit(0);
	} else {
	fprintf(stderr, "Opened database successfully\n");
	}

	average_rating=average_rating/num_ratings;

	sprintf( book_string, "%d",book_id);
	sprintf( average_string, "%f",average_rating);


	/* Create SQL statement */
	strcpy(sql,"UPDATE books set average_rating=");
	strcat(sql,average_string);
	strcat(sql," where id_book=");
	strcat(sql,book_string);
	
	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_average_rating_update, (void*)data, &zErrMsg);

	if( rc != SQLITE_OK ) {
	  fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	} else {
	  fprintf(stdout, "[Update average rating]Operation done successfully\n");
	}

  sqlite3_close(db);


}

int callback_update_ratings(void *NotUsed, int argc, char **argv, char **azColName) 
{
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

void update_ratings(int book_id,int rating)
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	const char* data = "Callback function called";
	char sql[5000];
	char user_string[10],book_string[10],rating_string[10];
	//convert from integer to string
	sprintf( user_string, "%d", user_id);
	sprintf( book_string, "%d",book_id);
	sprintf( rating_string, "%d",rating);

	/* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "Opened database successfully\n");
  }

	/* Create SQL statement */
	strcpy(sql,"INSERT INTO ratings(id_book,rating,id_user) values(");
	strcat(sql,book_string);

	strcat(sql,",");
	strcat(sql,rating_string);	
	strcat(sql,",");	
	strcat(sql,user_string);
	strcat(sql,")");

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql,callback_update_ratings, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	sqlite3_free(zErrMsg);
	} else {
	fprintf(stdout, "[Update ratings]Records created successfully\n");
	}
	sqlite3_close(db);

}
void submit_rating(int book_id,int rating)
{
	update_ratings(book_id,rating);
	update_average_rating(book_id);

}
