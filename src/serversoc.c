#include "serversoc.h"
#include "utils.h"

int 
prepare_socket(int portno) 
{
int sockfd, opt;
socklen_t opt_len;
struct sockaddr_in serv_addr, cli_addr;

memset(&serv_addr, 0, sizeof(serv_addr)); 
serv_addr.sin_family = AF_INET;
serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
if (portno < 0) {
	fprintf(stderr, "bad port number\n");
	return -1;
}
serv_addr.sin_port = htons(portno);

if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
	perror("socket");
	return -1;
}
opt = 1;
opt_len = sizeof(opt_len);
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, 
			&opt, opt_len) == -1) {
	perror("setsockopt");
	close(sockfd);
	return -1;
}
if (bind(sockfd, (struct sockaddr *)&serv_addr, 
			sizeof(serv_addr)) == -1) {
	perror("bind");
	close(sockfd);
	return -1;
}
if (listen(sockfd, SOMAXCONN) == -1) {
	perror("listen");
	close(sockfd);
	return -1;
}
return sockfd;
} /* prepare_socket */

void
accept_loop(int sockfd)
{} /* accept_loop */
