#include <stdio.h>
#include <stdlib.h>
#include "http.h"

void 
main()
{
req_header* req;
req = malloc(sizeof(req));
req = justscan(req, "GET http://hoge.com 1.0");
printf(req->method);
printf("\n");
printf(req->path);
printf("\n");
printf(req->version);
printf("\n");
free(req);
}
