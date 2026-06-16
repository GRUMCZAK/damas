#include "logs.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void _printerror(const char *fileName, int lineNumber, const char *funcName,
                 const char *calledFunction) {
  char buf[256];
  snprintf(buf, 256, "%s%s:%d in %s: %s -> %d", LOG_PREFIX, fileName,
           lineNumber, funcName, calledFunction, errno);
  perror(buf);
}
void _errorout(const char *fileName, int lineNumber, const char *funcName,
               const char *calledFunction, int err) {
  if (err < 0) {
    _printerror(fileName, lineNumber, funcName, calledFunction);
#ifdef _SERVER
    if (sockfd != -1) {
      close(sockfd);
    }
#endif // ifdef _SERVER
    exit(1);
  }
}
