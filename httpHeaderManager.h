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
    char *content_encoding;
    int content_length;
    char *content_type;
    char *date;
    char *last_modified;
    char *server;
    int statuscode;
} httpheader_t;

/**
 * @brief Converts a httpheader_t struct into a string
 * 
 * @param header the header to convert into a string
 * @param headerString the location of the converted httpheader_t
 * @return char* the converted header as string
 */
char *responseheaderToString(httpheader_t *header, char *headerString);

/**
 * @brief Parses the request for given http header fields and stores their value into the struct
 * 
 * @param requestContent the content of the request
 * @param parseHttpHeader the parsed reuqest in a struct
 */
void parseHttpHeader(char *requestContent, httpheader_t *parseHttpHeader);

/**
 * @brief Get the Default Response Header
 * 
 * @return httpheader_t the default responseheader
 */
httpheader_t getDefaultResponseHeader();
