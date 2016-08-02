#ifndef HTTP_READ_H
#define HTTP_READ_H

#include <stdint.h>

extern const uint8_t FLAG_NONE; // 0x00
extern const uint8_t FLAG_ENVI; // 0x01
extern const uint8_t FLAG_QUET; // 0x02

/**
 * \brief read http packet sequentially from stream
 *
 * \param file stream to read from
 *
 * \return success indicator
 */
int http_read_file(FILE *file, uint8_t flags);

#endif // HTTP_READ_H
