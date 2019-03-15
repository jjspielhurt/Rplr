//extract the genres ontology from the database into a structure datatype.
#include "global.h"
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>
#define NUM_USERS 5
#define NUM_BOOKS 10
#define NUM_GENRES 8

int genres_parents[NUM_GENRES];

 int callback_genres(void *data, int argc, char **argv, char **azColName)
{
  fprintf(stderr, "%s: ", (const char*)data);

  genres_parents[atoi(argv[0])]=atoi(argv[1]);

  return 0;
}


void genres_compute() 
{
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	const char* data = "Callback function called";
	/* Open database */
	rc = sqlite3_open("database.db", &db);

	if( rc ) {
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	exit(0);
	} else {
	fprintf(stderr, "Opened database successfully\n");
	}
	/* Create SQL statement */
	sql = "SELECT genre_id,parent_id from genres";

	/* Execute SQL statement */
	rc = sqlite3_exec(db, sql, callback_genres, (void*)data, &zErrMsg);

	if( rc != SQLITE_OK ) {
	  fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	} else {
	  fprintf(stdout, "Operation done successfully\n");
	}

  sqlite3_close(db);

}