#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "network.h"
#include "index.h"

extern int sfd;

void get(char * buf, int replyto)
{
	if (!strncmp(buf, get_index, sizeof(index))) {
		send(replyto,index_data,sizeof(index_data),0);
	}
}

int serve()
{
	int tempfd;
	int retval = 0;
	struct sockaddr_storage rem_addr;
	socklen_t storage_size;

	char buf[513];
	while(1) {

		if (-1 == (tempfd = accept(sfd,(struct sockaddr *) &rem_addr, &storage_size))) {
			perror("net_listen: accept error");
			retval = -6;
			continue;
		}
		memset(buf,0,sizeof(buf));
		if (-1 == recv(tempfd, buf, sizeof(buf), 0)) perror("recv failed");
		printf("%s",buf);
		if (!strncmp(buf, "GET", 3)) get(buf, tempfd);
		close(tempfd);
	}

	return 0;
}

int main(int argc, char ** argv)
{

	if (net_listen(argv[1])) exit(1);
	serve();

	


}
