#include <string.h>

#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"
#include "mimeTypeManager.h"
#include "messageHandler.h"

#endif

void sendFileNotExistsMessage(int clientFd)
{
    httpheader_t responseHttpheader = {.statuscode = 404, .httpVersion = "HTTP/1.1", .server = "httpc"};
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);
    
    sendTextWithHeader(FILE_NOT_FOUND_MESSAGE,httpHeaderAsString, clientFd);

    free(httpHeaderAsString);
}

void sendNoPermissionMessage(int clientFd)
{
    httpheader_t responseHttpheader = {.statuscode = 403, .httpVersion = "HTTP/1.1", .server = "httpc"};
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);

    sendTextWithHeader(FILE_NO_PERMISSION_MESSAGE,httpHeaderAsString, clientFd);

    free(httpHeaderAsString);
}

permission_t checkFileForPermissionAndExistence(httpheader_t *requestHeader)
{
    if (access(requestHeader->file, F_OK) != -1)
    {
        // file exists
        if (access(requestHeader->file, R_OK) != -1)
        {
            // permission ok
            return FILE_EXISTS;
        }
    }
    else
    {
        // file doesn't exist
        return FILE_NOT_EXISTS;
    }

    return PERMISSION_DENIED;
}

const char *getExt(const char *fspec)
{
    char *e = strrchr(fspec, '.');
    if (e == NULL)
        e = ""; // fast method, could also use &(fspec[strlen(fspec)]).
    return e;
}
