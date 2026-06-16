#ifndef _LOGS_H
#define _LOGS_H

#define LOG_PREFIX "damas-server: "

#define printerror(x) _printerror(__FILE__, __LINE__, __func__, x)
void _printerror(const char *fileName, int lineNumber, const char *funcName,
                 const char *calledFunction);

#define errorout(x) _errorout(__FILE__, __LINE__, __func__, #x, (x))
#define errorout_msg(x, y) _errorout(__FILE__, __LINE__, __func__, y, (x))
void _errorout(const char *fileName, int lineNumber, const char *funcName,
               const char *calledFunction, int err);

extern int sockfd;

#endif // _LOGS_H
