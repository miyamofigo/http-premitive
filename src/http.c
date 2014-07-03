#include "http.h"

void
http_server_routine(int sockfd)
{
size_t len;
char buf[RECV_BUF_SIZE];
req_header* req;
req_header* justscan(req_header*, char*);

if (recv(sockfd, buf, sizeof(buf), 0) == -1) { 
	perror("recv");
	return;
}
buf[len] = '\0';

if ((req = parse_message(justscan, buf, sizeof(buf))) == NULL) { 
	perror("parse_message error\n");
	return;
}
} /* http_server_routine */

req_header*
parse_message(req_header* (*handler)(req_header*, char*),
							char* buf, size_t bufsize) {
}	

req_header*
justscan(req_header* header, char* buf)
{
char method[METHOD_NAME_LENGTH];
char uri[URI_ADDRESS_SIZE];
char http_ver[HTTP_VERSION_SIZE];

sscanf(buf, "%s %s %s", method, uri, http_ver);
strcpy(header->method, method);
strcpy(header->path, uri);
strcpy(header->version, http_ver);
return header;
}
} /* justscan */

