#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"
#include "mimeTypeManager.h"

#endif

httpheader_t getDefaultResponseHeader()
{
    httpheader_t responseHttpheader = {.httpVersion = "HTTP/1.1", .server = "httpc", .connection = "close"};
    return responseHttpheader;
}

char *getCurrentGMTTime()
{
    time_t rawtime;
    time(&rawtime);
    struct tm *info = gmtime(&rawtime);
    /* Get GMT time */
    char *currentGmtTime = asctime(info);
    currentGmtTime[strlen(currentGmtTime) - 1] = 0;
    return currentGmtTime;
}

char *responseheaderToString(httpheader_t *header, char *headerString)
{
    // status code is not longer than 3 digits
    char statuscode[4];
    sprintf(statuscode, "%d", header->statuscode);

    // getting the current timestamp
    char *gmtTime = getCurrentGMTTime();

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
                 sizeof(header->server) +
                 sizeof(statuscode) +
                 sizeof(statusText) +
                 sizeof(gmtTime) +
                 10 + // for "GMT"
                 2;

    headerString = realloc(headerString, length);

    if (header->httpVersion == NULL)
    {
        fprintf(stderr, "Error you forgot to set the httpVersion in the header!\n");
        exit(EXIT_FAILURE);
    }

    sprintf(headerString, "%s %s %s\r\n", header->httpVersion, statuscode, statusText);
    sprintf(headerString, "%sDate: %s GMT\r\n", headerString, gmtTime);

    if (header->connection != NULL)
    {
        sprintf(headerString, "%sConnection: %s\r\n", headerString, header->connection);
    }
    if (header->server != NULL)
    {
        sprintf(headerString, "%sServer: %s\r\n", headerString, header->server);
    }
    if (header->content_type != NULL)
    {
        sprintf(headerString, "%sContent-Type: %s\r\n", headerString, header->content_type);
    }
    if (header->content_length != -1)
    {
        sprintf(headerString, "%sContent-Length: %d\r\n", headerString, header->content_length);
    }
    if (header->last_modified != NULL)
    {
        sprintf(headerString, "%sLast-Modified: %s\r\n", headerString, header->last_modified);
    }

    sprintf(headerString, "%s\r\n", headerString);

    return headerString;
}

void processFirstLine(char *firstLine, httpheader_t *parseHttpHeader)
{
    int position = 0;
    char *delimiterFirstLine = " ";
    char *end_token;
    char *firstLineToken = strtok_r(firstLine, delimiterFirstLine, &end_token);
    while (firstLineToken != NULL)
    {
        if (position == 0)
        {
            //httpmethod
            parseHttpHeader->methode = firstLineToken;
        }
        else if (position == 1)
        {
            //requested file
            parseHttpHeader->file = firstLineToken;
        }
        else if (position == 2)
        {
            // httpVersion
            parseHttpHeader->httpVersion = firstLineToken;
        }
        firstLineToken = strtok_r(NULL, delimiterFirstLine, &end_token);
        position++;
    }
    if (position - 1 != 2)
    {
        // header format not correct
        //thow error
        //TODO: implement error engine
        fprintf(stderr, "ERROR http header in wrong format!\n");
    }
}

void processLine(char *key, char *value, httpheader_t *parseHttpHeader)
{
    if (strcasecmp(key, HTTPHEADER_KEY_HOST) == 0)
    {
        parseHttpHeader->host = value;
    }
    else if (strcasecmp(key, HTTPHEADER_KEY_CONNECTION) == 0)
    {
        parseHttpHeader->connection = value;
    }
    else if (strcasecmp(key, HTTPHEADER_KEY_USER_AGENT) == 0)
    {
        parseHttpHeader->user_agent = value;
    }
    else if (strcasecmp(key, HTTPHEADER_KEY_ENCODING) == 0)
    {
        parseHttpHeader->accept_encoding = value;
    }
    else
    {
        // not supported header key
        // do nothing just ignore
    }
}

void parseHttpHeader(char *requestContent, httpheader_t *parseHttpHeader)
{
    char *end_str;
    char *currentLine = strtok_r(requestContent, HTTPHEADER_DELIMITER, &end_str);

    int firstLine = 1;
    while (currentLine != NULL)
    {
        if (firstLine == 1)
        {
            processFirstLine(currentLine, parseHttpHeader);
            firstLine = 0;
            currentLine = strtok_r(NULL, HTTPHEADER_DELIMITER, &end_str);
            continue;
        }
        else
        {
            char *value;
            char *key = strtok_r(currentLine, " ", &value);
            processLine(key, value, parseHttpHeader);
        }

        currentLine = strtok_r(NULL, HTTPHEADER_DELIMITER, &end_str);
    }
}
