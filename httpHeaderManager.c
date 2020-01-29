#include <stdlib.h>
#include <stdio.h>

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"

void responseheaderToString(httpheader_t *header, char *headerString)
{
    // status code is not longer than 3 digits
    char *statuscode = calloc(4, sizeof(char));
    sprintf(statuscode, "%d", header->statuscode);

    // getting the reason for a given status code
    const char *statusText = HttpStatus_reasonPhrase(header->statuscode);

    int length = sizeof(header->methode) +
                 sizeof(header->file) +
                 sizeof(header->httpVersion) +
                 sizeof(header->host) +
                 sizeof(header->connection) +
                 sizeof(header->user_agent) +
                 sizeof(header->accept_encoding) +
                 sizeof(header->content_length) +
                 sizeof(header->content_type) +
                 sizeof(header->date) +
                 sizeof(header->last_modified) +
                 sizeof(statuscode) +
                 sizeof(statusText) +
                 2;

    headerString = realloc(headerString, length);

    sprintf(headerString, "%s %s %s\r\n", header->methode, statuscode, statusText);
    //TODO: implement
}
