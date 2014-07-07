#include "http.h"

void
http_server_routine(int sockfd)
{
int readfd;
ssize_t len;
char* filename;
char sbuf[SEND_BUF_SIZE],rbuf[RECV_BUF_SIZE];
req_header* req;
static ssize_t send_message(int, const char*);

if (recv(sockfd, rbuf, sizeof(rbuf), 0) == -1) { 
  perror("recv");
  return;
}
rbuf[len] = '\0';

if ((req = parse_message(justscan, rbuf)) == NULL) { 
  perror("parse_message error\n");
  return;
}

if (strcmp(req->method, "GET") != 0) {  
  send_message(sockfd, "501 Not Implemented")
  return;
}

filename = req->path + 1;
if ((readfd = open(filename, O_RDONLY)) == -1) {
  send_message(sockfd, "404 Not Found");
  perror("open");
  return; 
} else {
  /* this procedure below may be optimized by the compiler. */
  send_message(sockfd, "HTTP/1.0 200 OK\n");
  send_message(sockfd, "text/html\n");
  send_message(sockfd, "\n");

  while ((len = read(readfd, sbuf, sizeof(sbuf))) > 0) 
    if (send(sockfd, sbuf, len) != len) {
      perror("send");
      free(req);
      close(readfd);
      break;
    } 
  close(readfd);
}

free(req);
} /* http_server_routine */

req_header*
parse_message(req_handler_t handler, char* buf)
{
req_header* req;

if ((req = malloc(sizeof(req))) == NULL)  {
  perror("malloc");
  return NULL;
}

if ((req = handler(req, buf)) == NULL) {
  perror("request handler");
  free(req);
  return NULL;
}
return req;
} /* parse_message */

req_header*
justscan(req_header* header, char* buf)
{
if (sscanf(buf, "%s %s %s", header->method, 
            header->path, header->version) != 3) {
  perror("sscanf");
  return NULL;
} 
return header; 
} /* justscan */

static ssize_t 
send_message(int sockfd, const char* buf)
{
ssize_t len;
if ((len = send(sockfd, buf, sizeof(buf), 0)) < 0) {
  perror("send");
  free(req); /* req_header 'req' is declared in parse_message */
  return -1;
} 
return len;
} /* send_message */

