#include "global.h"
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define NUM_USERS 5
#define NUM_BOOKS 11
#define NUM_GENRES 8
#include "extract_genres.h"

float X[NUM_USERS][NUM_GENRES];
int max_feature[NUM_GENRES];

void update_matrix(int user,int rating,int genre_id)
{
  X[user][genre_id]=X[user][genre_id]+rating;
  if(X[user][genre_id]>max_feature[genre_id])
    max_feature[genre_id]=X[user][genre_id];
  int parents;
  parents=genres_parents[genre_id];

  while(parents!=0)
  {
    X[user][parents]=X[user][parents]+rating;
    if(X[user][parents]>max_feature[parents])
      max_feature[parents]=X[user][parents];
    parents=genres_parents[parents];
  }

}

int callback_ratings(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: ", (const char*)data);
  update_matrix(atoi(argv[0]),atoi(argv[1]),atoi(argv[2]));

  return 0;
}


void features_ratings() 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char* data = "[f_ratings]Callback function called\n";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[f_ratings]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[f_ratings]Opened database successfully\n");
  }
 /* Create SQL statement */
    sql = "SELECT id_user,rating,genre_id from ratings join books where ratings.id_book=books.id_book";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_ratings, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[f_ratings]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[f_ratings]Operation done successfully\n");
    }
  sqlite3_close(db);

}

int callback_searches(void *data, int argc, char **argv, char **azColName)
{
  
  fprintf(stderr, "%s: ", (const char*)data);
  update_matrix(atoi(argv[0]),1,atoi(argv[1]));

  return 0;
}

void features_searches() 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char* data = "[f_searches]Callback function called\n";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[f_searches]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[f_searches]Opened database successfully\n");
  }
 /* Create SQL statement */
    sql = "SELECT user_id,genre_id from searches join books where searches.searched_id=books.id_book";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_searches, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[f_searches]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[f_searches]Operation done successfully\n");
    }
  sqlite3_close(db);

}

int callback_downloads(void *data, int argc, char **argv, char **azColName)
{
  
  fprintf(stderr, "%s: ", (const char*)data);
  update_matrix(atoi(argv[0]),2.5,atoi(argv[1]));

  return 0;
}

void features_downloads() 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char* data = "[f_download]Callback function called";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[f_download]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[f_download]Opened database successfully\n");
  }
 /* Create SQL statement */
    sql = "SELECT user_id,genre_id from downloads join books where downloads.book_id=books.id_book";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_downloads, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[f_download]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[f_download]Operation done successfully\n");
    }
  sqlite3_close(db);

}
//int main()
void extract_features()
{
  genres_compute();
  // for(int genre_id=0;genre_id<NUM_GENRES;genre_id++)
  //   printf("genre %d -> %d\n",genre_id,genres_parents[genre_id]);
  // fflush(stdout);
  for(int i=1;i<NUM_GENRES;i++)
  max_feature[i]=0;
  for(int i=0;i<NUM_USERS;i++)
    for(int j=0;j<NUM_GENRES;j++)
      X[i][j]=0;
  features_ratings();
  features_searches();
  features_downloads();
}