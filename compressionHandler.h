#include <strings.h>

typedef enum
{
    GZIP,
    COMPRESS,
    DEFLATE,
    IDENTITY,
    BR,
    UNDEFINED
} compression_t;

char *compressString(char *content, compression_t type, char *compressed);

compression_t getEnumFromChar(char *type)
{
    if (strcasecmp("gzip", type) == 0)
    {
        return GZIP;
    }
    else if (strcasecmp("compress", type) == 0)
    {
        return COMPRESS;
    }
    else if (strcasecmp("deflate", type) == 0)
    {
        return DEFLATE;
    }
    else if (strcasecmp("br", type) == 0)
    {
        return BR;
    }
    else if (strcasecmp("identity", type) == 0)
    {
        return IDENTITY;
    }
    else if (strcasecmp("*", type) == 0)
    {
        //DEFAULT we just use GZIP
        return GZIP;
    }
    return UNDEFINED;
}
