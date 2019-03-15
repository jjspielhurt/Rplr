#ifndef search_h
#define search_h
char *strlwr(char *str);
int callback_books(void *data, int argc, char **argv, char **azColName);
void search_books(char str[100]);

#endif