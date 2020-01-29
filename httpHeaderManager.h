#include "httpStatusCodes.h"

#define HTTPHEADER_DELIMITER "\r\n"

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
    int statuscode;
} httpheader_t;

void responseheaderToString(httpheader_t *header, char *headerString);
