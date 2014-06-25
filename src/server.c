#include <stdio.h>
#include <unistd.h>
#include "serverconf.h"
#include "serversoc.h"

int
main(int argc, char *argv[])
{
int	sockfd;

if ((sockfd = prepare_socket(HTTP_PORT)) == -1) {
	fprintf(stderr, "socket error\n");
	return -1;
}
fprintf(stderr, "server socket is ready for accept\n");
accept_loop(sockfd);
close(sockfd);
return 0;
} /* main */
