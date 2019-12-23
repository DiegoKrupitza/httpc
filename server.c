#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "server.h"

void usage(void)
{
    fprintf(stderr, "Usage: %s [-p PORT] [-h]\n", PROGRAMNAME);
}

void usageAndExit(void)
{
    usage();
    exit(EXIT_FAILURE);
}

int validPort(char* port) {
    uintmax_t num = strtoumax(port, NULL, 10);
    if (num == UINTMAX_MAX && errno == ERANGE) {
        /* Could not convert. */
        return 0;
    }
    
    // check if in valid range
    if(num <0 || num > 65535) {
        return 0;
    }

    return 1;
}

void readArguments(int argc, char *argv[], serverarguments* args)
{
    int portFlag = 0;
    int ch = -1;
    while ((ch = getopt(argc, argv, "hp:")) != -1)
    {
        switch (ch)
        {
        case 'h':
            // help message and exit with success
            usage();
            exit(EXIT_SUCCESS);
            break;
        case 'p':
            if(portFlag == 1) {
                usageAndExit();   
            }
            if(optarg == NULL) {
                //forgot the PORT NUMBER
                usageAndExit();
            }
            args->port = optarg;
            //check Port range
            if(validPort(args->port) != 1) {
                fprintf(stderr,"./%s: The port %s you specified is not valid\n",PROGRAMNAME,args->port);
                exit(EXIT_FAILURE);
            }

            break;
        case '?':
        default:
            usageAndExit();
        }
    }
}

int startServer(serverarguments* args) {

    struct addrinfo hints, *ai;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int res = getaddrinfo(NULL, args->port, &hints, &ai);
    if (res != 0)
    {
        // error
        fprintf(stderr, "./%s: Error while getaddrinfo: %s\n", PROGRAMNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }
    int sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (sockfd < 0)
    {
        // error
        fprintf(stderr, "./%s: Error while socket generation: %s\n", PROGRAMNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    // allow to reuse the port
    int optval = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval) < 0)
    {
        // error
        fprintf(stderr, "./%s: Error while setting socketoptions: %s\n", PROGRAMNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, ai->ai_addr, ai->ai_addrlen) < 0)
    {
        // error
        fprintf(stderr, "./%s: Error while binding socket to port: %s\n", PROGRAMNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, DEFAULT_CLIENT_LISTEN_SIZE) < 0)
    {
        // error
        fprintf(stderr, "./%s: Error while listening to port: %s\n", PROGRAMNAME, strerror(errno));
        exit(EXIT_FAILURE);
    }
    freeaddrinfo(ai);
    return sockfd;
}

void clientWaiting(int serverFd) {
    while(1) {
        debug("In request\n");
        int clientFD = accept(serverFd, NULL, NULL);
        if (clientFD < 0)
        {
            // error
            fprintf(stderr, "./%s: Error accepting new client: %s\n", PROGRAMNAME, strerror(errno));
            exit(EXIT_FAILURE);
        }

        FILE *clientFd = fdopen(clientFD, "r");

        char *line = NULL;
        size_t linecap = 0;
        ssize_t linelen;
        while ((linelen = getline(&line, &linecap, clientFd)) > 0) {
                fwrite(line, linelen, 1, stdout);
                fflush(stdout);
        }

        fclose(clientFd);
    }
}

int main(int argc, char *argv[])
{
    serverarguments args = {
        .port = DEFAULTPORT};

    readArguments(argc, argv, &args);
    int serverFd = startServer(&args);
    clientWaiting(serverFd);

    return EXIT_SUCCESS;
}
