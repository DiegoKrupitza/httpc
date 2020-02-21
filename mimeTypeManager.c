#include <string.h>

#include "mimeTypeManager.h"

char *getMimeTypFromFilename(char *filename)
{
    const char *ext = getExt(filename);
    ext++; // jumping the '.'

    FILE *mimeTypeFile = fopen(MIME_TYPE_FILE_LOCATION, "r");
    if (mimeTypeFile == NULL)
    {
        fprintf(stderr, "Cannot open the File (%s) containing the mimetypes!", MIME_TYPE_FILE_LOCATION);
    }

    char *line = NULL;
    size_t linecap = 0;
    ssize_t linelen;
    while ((linelen = getline(&line, &linecap, mimeTypeFile)) > 0)
    {
        if (strncmp(ext, line, strlen(ext)) == 0)
        {
            // we found the extension
            break;
        }
    }

    //TODO find problem
    char *ptr = strtok(line, " ");
    strtok(NULL, " "); // jumping the first part we already know
    printf("%s\n", line);
    return ptr;

}
