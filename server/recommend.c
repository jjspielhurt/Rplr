#include "global.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sqlite3.h>
#define NUM_BOOKS 11
#define NUM_RECOMM 3
#define NUM_USERS 5
#include "kmeans.h"

int user_to_recommend,num_recommendations;

struct book_struct{int id;float rating;} candidate_books[NUM_BOOKS],tem;
int books_read[NUM_BOOKS];
//update scores so that the books already read are not recommended
void update_scores(char type,int user,int rating,int book_id)
{

  if(user!=user_to_recommend && books_read[book_id]!=1)
  {
    candidate_books[book_id].rating=candidate_books[book_id].rating+rating;
    candidate_books[book_id].id=book_id;
  }
  else if(type!='s' && type!='a' && user==user_to_recommend)
  {
    printf("%d user%d read book %d\n",user_to_recommend,user,book_id);
    fflush(stdout);
    books_read[book_id]=1;
  }
}
int callback_score_author(void *data, int argc, char **argv, char **azColName)
{
  
  fprintf(stderr, "%s: ", (const char*)data);
  update_scores('a',atoi(argv[0]),1.5,atoi(argv[1]));

  return 0;
}

void book_score_author(int user) 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[5000];
  const char* data = "[score_author]Callback function called";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[score_author]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[score_author]Opened database successfully\n");
  }
 /* Create SQL statement */
    strcpy(sql ,"SELECT r.id_user,b.id_book from author a join books b on a.genre_id=b.genre_id join ratings r on r.id_book=b.id_book where (r.id_user=");
    char user_string[10];
    sprintf( user_string, "%d", user);
    strcat(sql,user_string);
    strcat(sql,")");

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_score_author, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[score_author]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[score_author]Operation done successfully\n");
    }
  sqlite3_close(db);

}


int callback_score_ratings(void *data, int argc, char **argv, char **azColName)
{
  int i;
  fprintf(stderr, "%s: ", (const char*)data);
  update_scores('r',atoi(argv[0]),atoi(argv[1]),atoi(argv[2]));

  return 0;
}

void book_score_ratings(int user) 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
   char sql[5000];
  const char* data = "[b_ratings]Callback function called\n";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[b_ratings]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[b_ratings]Opened database successfully\n");
  }
 /* Create SQL statement */
    strcpy(sql,"SELECT id_user,rating,id_book from ratings where id_user=");
    char user_string[10];
    sprintf( user_string, "%d", user);
    strcat(sql,user_string);
  
  /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_score_ratings, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[b_ratings]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[b_ratings]Operation done successfully\n");
    }
  sqlite3_close(db);

}

int callback_score_searches(void *data, int argc, char **argv, char **azColName)
{
  
  fprintf(stderr, "%s: ", (const char*)data);
  update_scores('s',atoi(argv[0]),1,atoi(argv[1]));

  return 0;
}

void book_score_searches(int user) 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[5000];
  const char* data = "[b_searches]Callback function called\n";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[b_searches]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[b_searches]Opened database successfully\n");
  }
 /* Create SQL statement */
    strcpy(sql ,"SELECT user_id,searched_id from searches where user_id=");
    char user_string[10];
    sprintf( user_string, "%d", user);
    strcat(sql,user_string);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_score_searches, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[b_searches]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[b_searches]Operation done successfully\n");
    }
  sqlite3_close(db);

}


int callback_score_downloads(void *data, int argc, char **argv, char **azColName)
{
  
  fprintf(stderr, "%s: ", (const char*)data);
  update_scores('d',atoi(argv[0]),2.5,atoi(argv[1]));

  return 0;
}

void book_score_downloads(int user) 
{
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char sql[5000];
  const char* data = "[b_download]Callback function called";
  /* Open database */
  rc = sqlite3_open("database.db", &db);

  if( rc ) {
    fprintf(stderr, "[b_download]Can't open database: %s\n", sqlite3_errmsg(db));
    exit(0);
  } else {
    fprintf(stderr, "[b_download]Opened database successfully\n");
  }
 /* Create SQL statement */
    strcpy(sql ,"SELECT user_id,book_id from downloads where user_id=");
    char user_string[10];
    sprintf( user_string, "%d", user);
    strcat(sql,user_string);

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback_score_downloads, (void*)data, &zErrMsg);

    if( rc != SQLITE_OK ) {
      fprintf(stderr, "[b_download]SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
    } else {
      fprintf(stdout, "[b_download]Operation done successfully\n");
    }
  sqlite3_close(db);

}

void update_book_scores(int user)
{
  book_score_ratings(user);
  book_score_searches(user);
  book_score_downloads(user);
  book_score_author(user);

}

void recommend(int user_id)
{
  int cluster,max=0;
  int c_min[NUM_USERS];

  float cost,cost_min=-1;
  for(int it=0;it<10;it++)
  {
      //kmeans num_iterations,num_centroids,num_users
    cost=kmeans(1000,2,5);
    if(cost<cost_min || cost_min == -1)
    {
      cost_min=cost;
      for(int i=0;i<NUM_USERS;i++)
      {
        c_min[i]=c[i];
      }
    }
  }

  cluster=c_min[user_id];
  user_to_recommend=user_id;
  //marcam cititele
  for(int i=0;i<NUM_BOOKS;i++)
  {
    books_read[i]=0;
  }
  update_book_scores(user_to_recommend);

  for(int user=1;user<=NUM_USERS;user++)
  {
    if(c_min[user]==cluster&&user!=user_to_recommend)
    {
    //users in the same cluster as the user we want to make recommendations to 
      update_book_scores(user);
    }
  }
  for(int i=0;i<NUM_BOOKS-1;i++)
  {

    for(int j=i+1;j<NUM_BOOKS;j++)
    {
      if(candidate_books[i].rating<candidate_books[j].rating)
      {
        tem.rating=candidate_books[i].rating;
        tem.id=candidate_books[i].id;
        candidate_books[i].rating=candidate_books[j].rating;
        candidate_books[i].id=candidate_books[j].id;
        candidate_books[j].id=tem.id;
        candidate_books[j].rating=tem.rating;
      }
    }
  }

  num_recommendations=0;
  while(candidate_books[num_recommendations].rating>0)
  {
    num_recommendations++;
  }
}
int callback_send_info(void *data, int argc, char **argv, char **azColName)
{
  int i,id_book;
  fprintf(stderr, "%s: ", (const char*)data);
  char msgrasp[500];
  bzero(msgrasp,100);
  id_book=atoi(argv[0]);

  strcat(msgrasp,argv[0]);
  strcat(msgrasp,".");

  for(i = 1; i<argc; i++)
  {
  strcat(msgrasp,argv[i]);
  strcat(msgrasp,"|");
  }
      /* returnam mesajul clientului */
      if (write (client, msgrasp, 100) <= 0)
      {
        perror ("[server]Eroare la write() catre client.\n");
      }
      else
        printf ("[server]Mesajul a fost trasmis cu succes.\n");
      /* am terminat cu acest client, inchidem conexiunea */
   
  return 0;
}
void send_info(int book_id)
{
  char sql[5000];
  char book_string[10];
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  //convert from integer to string
  sprintf( book_string, "%d",book_id);

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
  strcpy(sql,"select id_book,title,author,isbn,publish_year,isbn,average_rating from books where id_book=");
  strcat(sql,book_string);
 

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback_send_info, 0, &zErrMsg);

  if( rc != SQLITE_OK ){
  fprintf(stderr, "SQL error: %s\n", zErrMsg);
  sqlite3_free(zErrMsg);
  } else {
  fprintf(stdout, "[downloads]Records created successfully\n");
  }
  sqlite3_close(db);

}

void send_recommendations(int user_id)
{
    for(int i=0;i<NUM_BOOKS;i++)
  {
    candidate_books[i].rating=0;
    candidate_books[i].id=0;

  }
  recommend(user_id);
  char num_recommendations_str[10];
  sprintf(num_recommendations_str,"%d",num_recommendations);
  if (write (client,num_recommendations_str , 10) <= 0)
  {
    perror ("[server]Eroare la write() catre client.\n");
  }
  for(int i=0;i<num_recommendations;i++)
  {
    send_info(candidate_books[i].id);
  }


}
