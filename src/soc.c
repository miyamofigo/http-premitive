#include "http.h"
#include "soc.h"
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
accept_routine(int sockfd)
{
char hostname[NI_MAXHOST], servname[NI_MAXSERV];
struct sockaddr_storage cli;
int acc, status;
pid_t pid;
socklen_t len;

while(1) {
  len = (socklen_t)sizeof(cli);
  if ((acc = accept(sockfd, (struct sockaddr *)&cli, &len)) == -1) {
    if (errno != EINTR)
      perror("accept");
  } else {
    getnameinfo((struct sockaddr *)&cli, len,
                hostname, sizeof(hostname),
                servname, sizeof(servname),
                NI_NUMERICHOST | NI_NUMERICSERV);
    fprintf(stderr, "accept:%s:%s\n", hostname, servname);
    if ((pid = fork()) == 0) {
      close(sockfd);
      http_server_routine(acc); 
      close(acc);
      _exit(1);  
    } else if (pid > 0) {
      close(acc);
      acc = -1;
    } else {
      perror("fork");
      close(acc);
      acc = -1;
    }
    if ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      fprintf(stderr,
              "accept_routine:waitpid:pid=%d.status=%d\n",
              pid,
              status);
      fprintf(stderr, 
              "WIFEXITED:%d,WEXITSTATUS:%d,WIFSIGNALED:%d,"
              "WTERMSIG:%d,WIFSTOPPED:%d,WSTOPSIG:%d\n",
              WIFEXITED(status),
              WEXITSTATUS(status),
              WIFSIGNALED(status),
              WTERMSIG(status),
              WIFSTOPPED(status),
              WSTOPSIG(status));
    }
  }
}
} /* accept_loop */

void
sig_child_handler(int sig)
{
int status;
pid_t pid;
pid = wait(&status);
fprintf(stderr, "sig_child_handler:wait:pid=%d,status=%d\n",pid,status);
fprintf(stderr, 
        "WIFEXITED:%d,WEXITSTATUS:%d,WIFSIGNALED:%d,"
        "WTERMSIG:%d,WIFSTOPPED:%d,WSTOPSIG:%d\n",
        WIFEXITED(status),
        WEXITSTATUS(status),
        WIFSIGNALED(status),
        WTERMSIG(status),
        WIFSTOPPED(status),
        WSTOPSIG(status));
}

