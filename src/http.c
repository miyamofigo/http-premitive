#include "http.h"

void
http_server_routine(int sockfd)
{
size_t len;
char buf[RECV_BUF_SIZE];
req_header* req;

if (recv(sockfd, buf, sizeof(buf), 0) == -1) { 
  perror("recv");
  return;
}
buf[len] = '\0';

if ((req = parse_message(justscan, buf)) == NULL) { 
  perror("parse_message error\n");
  return;
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
