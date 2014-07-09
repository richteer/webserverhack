#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <netdb.h>

#include "network.h"

int sfd = -1;
struct sockaddr_storage rem_addr;

int net_connect(char* ip, char* port)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int retval = 0;

	memset(&hints,0,sizeof(hints));
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((NULL == ip) || (NULL == port)) {
		fprintf(stderr,"net_connect: Invalid server or port!\n");
		return -1;
	}

	if ((rv = getaddrinfo(ip,port,&hints,&servinfo)) != 0) {
		fprintf(stderr, "net_connect: getaddrinfo: %s\n", gai_strerror(rv));
		return -2;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if (-1 != (sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
			break;
	}

	if (NULL == p) {
		fprintf(stderr, "net_connect: Could not create a socket!\n");
		retval = -3;
		goto cleanup;
	}

	if (0 != connect(sfd, p->ai_addr, p->ai_addrlen)) {
		perror("net_connect: connect error");
		retval = -4;
		close(sfd);
		sfd = -1;
	}

cleanup:

	freeaddrinfo(servinfo);

	return retval;
}

int net_listen(char* port)
{
	struct addrinfo hints, *servinfo, *p;
	socklen_t storage_size = sizeof(struct sockaddr_storage);
	int rv;
	int retval = 0;
	int tempfd;

	memset(&hints,0,sizeof(hints));
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    #if DEBUG
    	printf("Trying to listen on port %s\n",port);
	#endif

	if (NULL == port) {
		fprintf(stderr,"net_listen: Invalid server or port!\n");
		return -1;
	}

	if ((rv = getaddrinfo(NULL,port,&hints,&servinfo)) != 0) {
		fprintf(stderr, "net_listen: getaddrinfo: %s\n", gai_strerror(rv));
		return -2;
	}

	for (p = servinfo; p != NULL; p = p->ai_next) {
		if (-1 != (sfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)))
			break;
	}

	if (NULL == p) {
		fprintf(stderr, "net_listen: Could not create a socket!\n");
		retval = -3;
		goto cleanup;
	}

	if (0 != bind(sfd, p->ai_addr, p->ai_addrlen)) {
		perror("net_listen: bind error");
		retval = -4;
		close(sfd); 
		sfd = -1;
		goto cleanup;
	}

	if (0 != listen(sfd, 1)) {
		perror("net_listen: listen error");
		retval = -5;
		close(sfd);
		goto cleanup;
	}

cleanup:
	freeaddrinfo(servinfo);

	return retval;
}

void net_close()
{
	close(sfd);
}

