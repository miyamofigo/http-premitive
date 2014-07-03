#ifndef __HTTP_SERVER_DEF__
#define __HTTP_SERVER_DEF__

#define RECV_BUF_SIZE 512
#define METHOD_NAME_LENGTH 16
#define URI_ADDRESS_SIZE 256
#define HTTP_VERSION_SIZE 64

typedef struct _req_header {
	char method[METHOD_NAME_LENGTH],
	char path[URI_ADDRESS_SIZE],
	char version[HTTP_VERSION_SIZE]
} req_header;	

void http_server_routine(int);

#endif

