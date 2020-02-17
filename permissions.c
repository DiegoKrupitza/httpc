#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"

#endif

void sendFileNotExistsMessage(int clientFd) {
    httpheader_t responseHttpheader = {.statuscode = 404, .httpVersion = "HTTP/1.1", .server = "httpc"};
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);


    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fputs(FILE_NOT_FOUND_MESSAGE, cl);
    if (fflush(cl) != 0)
    {
        //error
        fprintf(stderr, "Error flushing to client!\n");
    }
    if (fclose(cl) != 0)
    {
        //error
        fprintf(stderr, "Error closing connection to client!\n");
    }
    free(httpHeaderAsString);
}

void sendFileNoPermissionMessage(int clientFd) {
    httpheader_t responseHttpheader = {.statuscode = 403, .httpVersion = "HTTP/1.1", .server = "httpc"};
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);


    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fputs(FILE_NO_PERMISSION_MESSAGE, cl);
    if (fflush(cl) != 0)
    {
        //error
        fprintf(stderr, "Error flushing to client!\n");
    }
    if (fclose(cl) != 0)
    {
        //error
        fprintf(stderr, "Error closing connection to client!\n");
    }
    free(httpHeaderAsString);
}

permission_t checkFileForPermissionAndExistence(httpheader_t* requestHeader) {
    if( access( requestHeader->file, F_OK) != -1 ) {
        // file exists
        if( access( requestHeader->file, R_OK) != -1 ) {
            // permission ok
            return FILE_EXISTS;
        }
    } else {
        // file doesn't exist
        return FILE_NOT_EXISTS;
    }

    return PERMISSION_DENIED;
}
