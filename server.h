#define PROGRAMNAME "httpc"
#define DEFAULTPORT "80"
#define DEFAULT_CLIENT_LISTEN_SIZE 1

#define debug(fmt, ...)                        \
    (void)fprintf(stderr, "[%s:%d] " fmt "\n", \
                  __FILE__, __LINE__,          \
                  ##__VA_ARGS__)

typedef struct
{
    char *port;
} serverarguments_t;

void usage(void);

void usageAndExit(void);
