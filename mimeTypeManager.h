#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"

#endif

#define MIME_TYPE_FILE_LOCATION "mimetypes.txt"

char *getMimeTypFromFilename(char *extension);
