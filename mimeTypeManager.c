#include <string.h>

#include "mimeTypeManager.h"

char *getMimeTypFromFilename(char *filename)
{
    char *mimeType = NULL;

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
        //printf("%s\n", line);
        if (strncmp(ext, line, strlen(ext)) == 0)
        {
            // we found the extension
            char *e = strrchr(line, ' ');
            e++;
            mimeType = e;
            break;
        }
    }

    // removing the \n
    char *pos;
    if ((pos = strchr(mimeType, '\n')) != NULL)
        *pos = '\0';
    else
        /* input too long for buffer, flag error */

        if (fclose(mimeTypeFile) < 0)
    {
        fprintf(stderr, "Cannot close the File (%s) containing the mimetypes!", MIME_TYPE_FILE_LOCATION);
    }

    return mimeType;
}
