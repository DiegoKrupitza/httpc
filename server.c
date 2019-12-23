#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

void startServer(serverarguments* args) {

}

int main(int argc, char *argv[])
{
    serverarguments args = {
        .port = DEFAULTPORT};

    readArguments(argc, argv, &args);
    startServer(&args);

    return EXIT_SUCCESS;
}
