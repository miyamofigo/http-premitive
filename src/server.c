/*****************************************************
 * http-premitive server main routine.               *
 * Usage : ./server [ option(help) | port-number ]   *
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "serverconf.h"
#include "serversoc.h"

int
main(int argc, char *argv[])
{
int	sockfd;
int portno;

if (argc > 0) {
	if (strcmp(argv[0], "--help")) {
		printf("Usage: ./server port-number\n");
		return 1;
	}
	else
		portno = atoi(argv[0]);
}
else
	portno = HTTP_PORT;

if ((sockfd = prepare_socket(portno)) == -1) {
	fprintf(stderr, "socket error\n");
	return -1;
}
fprintf(stderr, "server socket is ready for accept\n");
accept_loop(sockfd);
close(sockfd);
return 0;
} /* main */
