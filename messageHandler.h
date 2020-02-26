#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"
#include "mimeTypeManager.h"
#include "messageHandler.h"

#endif

void sentFileContent(httpheader_t responseHttpheader, char *filename, int clientFd);

void sendTextWithHeader(char* message,char *httpHeaderAsString, int clientFd);
