#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <inttypes.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "server.h"
#include "httpHeaderManager.h"
#include "httpStatusCodes.h"

void handle_childfork(int singal)
{
    //TODO: wait and handle child exit
    int status = -1;
    int pid = waitpid(-1, &status, WNOHANG);
    if (pid == -1)
    {
        //TODO: handle ERROR with errno etc
        exit(EXIT_FAILURE);
    }

    if (WEXITSTATUS(status) == EXIT_SUCCESS)
    {
        //everything was ok
        debug("Client exit ok!");
    }
    else
    {
        //print error message and kill everything
        fprintf(stderr, "Client exit not ok!\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief registers the SIGCHILD singal for this programm
 * 
 */
void registerWaitSignal()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa)); // initialize sa to 0
    sa.sa_handler = handle_childfork;
    sigaction(SIGCHLD, &sa, NULL);
}

/**
 * @brief registers all the singals in this programm
 * 
 */
void signalRegistry()
{
    registerWaitSignal();
}

/**
 * @brief Prints the usage of this programm to stderr
 * 
 */
void usage(void)
{
    fprintf(stderr, "Usage: %s [-p PORT] [-h]\n", PROGRAMNAME);
}

/**
 * @brief Prings the usage of this programm to stderr and exits with EXIT_FAILURE
 * 
 */
void usageAndExit(void)
{
    usage();
    exit(EXIT_FAILURE);
}

/**
 * @brief Checks if a given port is valid
 * 
 * @param port the port to check if it is valid
 * @return int whether the port was valid or not
 */
int validPort(char *port)
{
    uintmax_t num = strtoumax(port, NULL, 10);
    if (num == UINTMAX_MAX && errno == ERANGE)
    {
        /* Could not convert. */
        return 0;
    }

    // check if in valid range
    if (num < 0 || num > 65535)
    {
        return 0;
    }

    return 1;
}

/**
 * @brief Reads the arguments from the commandline
 * 
 * @param argc the argument counter
 * @param argv the argument vector
 * @param args the container holding the arguments for further processing
 */
void readArguments(int argc, char *argv[], serverarguments_t *args)
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
            if (portFlag == 1)
            {
                usageAndExit();
            }
            if (optarg == NULL)
            {
                //forgot the PORT NUMBER
                usageAndExit();
            }
            args->port = optarg;
            //check Port range
            if (validPort(args->port) != 1)
            {
                fprintf(stderr, "./%s: The port %s you specified is not valid\n", PROGRAMNAME, args->port);
                exit(EXIT_FAILURE);
            }

            break;
        case '?':
        default:
            usageAndExit();
        }
    }
}

/**
 * @brief Starts the server and registers the port on the system
 * 
 * @param args the arguments of this programm
 * @return int the servers filedescriptor
 */
int startServer(serverarguments_t *args)
{
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

char *readClientReqest(int clientFd, char *requestContent)
{
    FILE *clientInput = fdopen(clientFd, "r");

    char line[1024];
    while ((fgets(line, sizeof(line), clientInput)) != 0)
    {
        int newLength = strlen(line) + strlen(requestContent) + 2;
        requestContent = realloc(requestContent, newLength);
        strcat(requestContent, line);
        if (strcmp("\r\n", line) == 0)
        {
            //done
            break;
        }
    }
    return requestContent;
}

void processClientRequest(int clientFd)
{
    // duplicate fd so its safe to close
    int duplicatedClientSocket = dup(clientFd);
    close(clientFd);

    clientFd = duplicatedClientSocket;

    // reading the content
    char *requestContent = calloc(1024, sizeof(char));
    requestContent = readClientReqest(clientFd, requestContent);

    // parsing the header and storing into the header struct
    httpheader_t requestHttpheader;
    parseHttpHeader(requestContent, &requestHttpheader);

    // generating the resposne header
    httpheader_t responseHttpheader = {.methode = "GET", .statuscode = 200};
    char *httpHeaderAsString = calloc(2, sizeof(char));
    httpHeaderAsString = responseheaderToString(&responseHttpheader, httpHeaderAsString);

    //printf("%s", httpHeaderAsString);

    // write response
    FILE *cl = fdopen(clientFd, "w");
    fputs(httpHeaderAsString, cl);
    fputs("It works!\n", cl);
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

    //TODO correct writing back
    free(httpHeaderAsString);
    free(requestContent);
    fflush(stdout);
}

/**
 * @brief Handles the incoming request of a client
 * 
 * @param clientFd the clients filedescriptor
 */
void handleNewClient(int clientFd)
{
    pid_t pid = fork();

    switch (pid)
    {
    case -1:
        fprintf(stderr, "Cannot fork!\n");
        exit(EXIT_FAILURE);
    case 0:
        // child tasks ...
        processClientRequest(clientFd);
        exit(EXIT_SUCCESS);
        break;
    default:
        // parent tasks ...
        // nothing to do
        // handle in sigaction
        close(clientFd);
        break;
    }
}

/**
 * @brief Waits for a client to send a request 
 * 
 * @param serverFd the servers filedescriptor to accept on
 */
void clientWaiting(int serverFd)
{
    while (1)
    {
        int clientFd = accept(serverFd, NULL, NULL);
        if (clientFd < 0)
        {
            if (errno == EINTR)
            {
                //TODO: check if it is really a sigchild
                // maybe a SIGCHILD is incomming
                // so catch it
                continue;
            }
            // error
            fprintf(stderr, "./%s: Error accepting new client: %s\n", PROGRAMNAME, strerror(errno));
            exit(EXIT_FAILURE);
        }

        handleNewClient(clientFd);
    }
}

int main(int argc, char *argv[])
{
    signalRegistry();
    serverarguments_t args = {
        .port = DEFAULTPORT};

    readArguments(argc, argv, &args);
    int serverFd = startServer(&args);
    clientWaiting(serverFd);

    return EXIT_SUCCESS;
}
