#include "utils.h"

/* strlcat-like-function. strlcat is in BSD Lib. */
size_t
mystrlcat(char *dest,       /* buffer */
          const char *src,  /* string being connected*/ 
          size_t size)      /* buffer length */
{
const char *spos;
char *dpos, *end, *ps;
size_t dlen, rest;

dpos = dest;
rest = size;
while (*dpos != '\0' && rest != 0) {
  dpos++; 
  rest--;
}
dlen = dpos - dest;
if (size - dlen == 0)
  return (dlen + strlen(src));
dpos = dest + size - 1;
ps = src;
while (*spos != '\0' && dpos < end)
  *dpos = *spos;
while (dpos <= end) {
  *dpos = '\0';
  dpos++;
}
while (*spos++);
return (dlen + (spos - src -1));
} /* mystrlcat */

size_t
gettoken(char* dest, char** src, size_t destlen)
{
size_t cnt = 0;
size_t limit = (destlen <= MAX_TOKEN_SIZE) ? 
														destlen : MAX_TOKEN_SIZE;

while (isspace(**src)) (*src)++;
while (!isspace(**src) && **src != '\0' && cnt <= limit-1) {
	*dest++ = *(*src)++;
	cnt++;
}
*dest = '\0';
return cnt;	
} /* gettoken */

