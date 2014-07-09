#include "http.h"
#include "utils.h"

void
http_server_routine(int sockfd)
{
int readfd;
ssize_t len;
char* filename;
char sbuf[SEND_BUF_SIZE], rbuf[RECV_BUF_SIZE];
/* const char* ok = "HTTP/1.0 200 OK\ntext/html\n\n";  */
req_header* req;
ssize_t send_message(int, const char*);

if ((len = recv(sockfd, rbuf, sizeof(rbuf), 0)) == -1) { 
  perror("recv");
  return; 
}
rbuf[len] = '\0';

if ((req = parse_message(scanline, rbuf)) == NULL) { 
  perror("parse_message error\n");
  return; 
} 

if (strcmp(req->method, "GET") != 0) {  
  send_message(sockfd, "501 Not Implemented");
	free(req);
  return; 
}

filename = req->path + 1;
if ((readfd = open(filename, O_RDONLY, 0666)) == -1) {
  send_message(sockfd, "404 Not Found");
  perror("open");
  return;  
} else {
/*  if (send_message(sockfd, ok) == -1) {
		perror("send_message is failed\n");
    close(readfd);
		free(req);
		return; 
	} */

	send_message(sockfd, "HTTP/1.0 200 OK\r\n");
	send_message(sockfd, "text/html\r\n");
	send_message(sockfd, "\r\n");

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

/*
req_header*
justscan(req_header* header, char* buf)
{
if (sscanf(buf, "%s %s %s", header->method, 
            header->path, header->version) == EOF) {
  perror("sscanf");
  return NULL;
} 

return header; 
} */ /* justscan */

ssize_t 
send_message(int sockfd, const char* buf)
{
ssize_t len;
if ((len = send(sockfd, buf, sizeof(buf),0)) < 0) {
  perror("send");
  return -1;
} 
return len;
} /* send_message */

req_header*
scanline(req_header* header, char* buf)
{
int i;
char method[METHOD_NAME_LENGTH], 
		 uri[URI_ADDRESS_SIZE],
		 version[HTTP_VERSION_SIZE];
char *req_header_elms[3] = {
	method, uri, version
};		
size_t req_header_elmsizes[3] = {
	METHOD_NAME_LENGTH, URI_ADDRESS_SIZE, HTTP_VERSION_SIZE
};	

for (i = 0; i < 3; i++) {  
	if (!gettoken(req_header_elms[i], &buf, req_header_elmsizes[i])) { 
		perror("gettoken error");
		return NULL;
	}
}
strcpy(header->method, method);
strcpy(header->path, uri);
strcpy(header->version, version);
return header;
} /* scanline */
