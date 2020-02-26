#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

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

void sendTextWithHeader(char *message, char *httpHeaderAsString, int clientFd)
{

    // write response
    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fputs(message, cl);
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
}

void sentTextFileContent(char *filename, char *httpHeaderAsString, int clientFd)
{
    // reading the file content
    char *fileContent = calloc(2, sizeof(char));
    fileContent = readFileContent(filename, fileContent);

    sendTextWithHeader(fileContent, httpHeaderAsString, clientFd);

    free(fileContent);
}

void sendBinaryFile(char *filename, char *httpHeaderAsString, int clientFd)
{
    // read binary file
    struct stat fileInfo;
    stat(filename, &fileInfo);

    int binData[fileInfo.st_size];

    FILE *file = fopen(filename, "r");
    fread(binData, 1, fileInfo.st_size, file);
    if (fclose(file) != 0)
    {
        //error
        fprintf(stderr, "Error closing file bin read!\n");
    }

    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fwrite(binData, 1, fileInfo.st_size, cl);
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
}

void sentFileContent(httpheader_t responseHttpheader, char *filename, int clientFd)
{
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);

    if (isBinaryMimeType(responseHttpheader.content_type) == 1)
    {
        // send binary data to the client
        sendBinaryFile(filename, httpHeaderAsString, clientFd);
    }
    else
    {
        // send text data to the client
        sentTextFileContent(filename, httpHeaderAsString, clientFd);
    }

    free(httpHeaderAsString);
}
