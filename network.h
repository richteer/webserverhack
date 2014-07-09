#ifndef _network_h_
#define _network_h_


#define DEBUG 1

int net_connect(char* ip,char* port);
int net_listen(char* port);
void net_close();


#endif
