#include "http.h"

void main() {
req_header* req;
req = parse_message(justscan, "GET http://hoge.com 1.0");
printf(req->method);
printf("\n");
printf(req->path);
printf("\n");
printf(req->version);
printf("\n");
free(req);
}
