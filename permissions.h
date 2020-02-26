#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef COMMON_HEADERS
#define COMMON_HEADERS

#include "httpHeaderManager.h"
#include "httpStatusCodes.h"
#include "permissions.h"
#include "mimeTypeManager.h"
#include "messageHandler.h"

#endif

#define FILE_NOT_FOUND_MESSAGE "<html><body><h1>404 - File not found!</h1></body></html>\n"
#define FILE_NO_PERMISSION_MESSAGE "<html><body><h1>403 - Permission denied!</h1></body></html>\n"

typedef enum
{
    FILE_EXISTS,
    FILE_NOT_EXISTS,
    PERMISSION_DENIED,
} permission_t;

/**
 * @brief Checks if a given file requested by the requestheader exsits and of the read permission is given
 * 
 * @param requestHeader the requestheader struct containing the filename
 * @return permission_t the permission status of the given file
 */
permission_t checkFileForPermissionAndExistence(httpheader_t *requestHeader);

/**
 * @brief Sends the File not exists error message to the client
 * 
 * @param clientFd the clients file descriptor
 */
void sendFileNotExistsMessage(int clientFd);

/**
 * @brief Sends the no permission error message to the client
 * 
 * @param clientFd the clients file descriptor
 */
void sendNoPermissionMessage(int clientFd);

/**
 * @brief Get the Extension of a file
 * 
 * @see https://stackoverflow.com/questions/3035225/getting-file-extension-in-c-language
 * @param fspec the filename
 * @return const char* the extension including the point
 */
const char *getExt(const char *fspec);
