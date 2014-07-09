/************************************************************
 * http-premitive server main routine.                      *
 * Usage : ./server [ option(help) | port-number ]          *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "conf.h"
#include "soc.h"

int
main(int argc, char *argv[])
{
int sockfd;
int portno;

if (argc > 1) {
  if (!strcmp(argv[1], "--help")) {
    printf("Usage: ./server port-number\n");
    return 1;
  }
  else
    portno = atoi(argv[1]);
}
else
  portno = HTTP_PORT;

signal(SIGCHLD, sig_child_handler);
if ((sockfd = prepare_socket(portno)) == -1) {
  fprintf(stderr, "socket error\n");
  return -1;
}
fprintf(stderr, "server socket is ready for accept\n");
accept_routine(sockfd);
close(sockfd);
return 0;
} /* main */
