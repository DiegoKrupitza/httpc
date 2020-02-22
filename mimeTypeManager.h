#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"

#endif

#define MIME_TYPE_FILE_LOCATION "mimetypes.txt"
#define MIME_TYPE_FILE_TEXT_SEPERATOR " "

/**
 * @brief Get the Mime Typ From Filename 
 * 
 * @param filename the filename
 * @return char* the mimetype
 */
char *getMimeTypFromFilename(char *filename);
