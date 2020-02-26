#include <stdio.h>
#include <string.h>

#include "messageHandler.h"

char *readFileContent(char *filename, char *fileContent)
{
    //size_t fileLength = -1;
    FILE *fp = fopen(filename, "r");

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, fp)) > 0)
    {
        int newLenght = linelen + strlen(fileContent) + 2;
        fileContent = realloc(fileContent, newLenght);
        strcat(fileContent, line);
    }

    if (fclose(fp) < 0)
    {
        fprintf(stderr, "Cannot close the file (%s) the client requested!", filename);
        exit(EXIT_FAILURE);
    }

    return fileContent;
}

void sentTextFileContent(char *filename, char *httpHeaderAsString, int clientFd)
{
    // reading the file content
    char *fileContent = calloc(2, sizeof(char));
    fileContent = readFileContent(filename, fileContent);

    // write response
    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fputs(fileContent, cl);
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

    free(fileContent);
}

void sentFileContent(httpheader_t responseHttpheader, char *filename, int clientFd)
{
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);

    if (isBinaryMimeType(responseHttpheader.content_type) == 1)
    {
        // send binary data to the client
        // TODO: implement
        fprintf(stderr, "Binary file serving currently not implemented!\n");
        fflush(stderr);
    }
    else
    {
        sentTextFileContent(filename, httpHeaderAsString, clientFd);
    }

    free(httpHeaderAsString);
}
