#include "httpStatusCodes.h"

#define HTTPHEADER_DELIMITER "\r\n"

#define HTTPHEADER_KEY_HOST "Host:"
#define HTTPHEADER_KEY_CONNECTION "Connection:"
#define HTTPHEADER_KEY_USER_AGENT "User-Agent:"
#define HTTPHEADER_KEY_ENCODING "Accept-Encoding:"

typedef struct
{
    char *methode;
    char *file;
    char *httpVersion;
    char *host;
    char *connection;
    char *user_agent;
    char *accept_encoding;
    char *content_length;
    char *content_type;
    char *date;
    char *last_modified;
    char *server;
    int statuscode;
} httpheader_t;

char *responseheaderToString(httpheader_t *header, char *headerString);

void parseHttpHeader(char *requestContent, httpheader_t *parseHttpHeader);
