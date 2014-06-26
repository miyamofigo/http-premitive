#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef __SERVER_SOC__
#define __SERVER_SOC__

int prepare_socket(int);
void accept_routine(int);

#endif
