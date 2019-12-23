#define PROGRAMNAME "httpc"
#define DEFAULTPORT "80"

#define debug(fmt, ...) \
(void) fprintf(stderr, "[%s:%d] " fmt "\n", \
                 __FILE__, __LINE__, \
                 ##__VA_ARGS__)



typedef struct
{
    char *port;
} serverarguments;

void usage(void);

void usageAndExit(void);
