#ifndef connect_server_h
#define connect_server_h

void client_quit();
int client_login(char username[100]);
void client_download(char book_id[100]);
void client_rate(char book_id[10],char rating_value[2]);
void client_search(char book_name[100]);
void client_get_recommendations();
void client_connect(char adresa_server_char[100],char port_char[50]);

#endif
