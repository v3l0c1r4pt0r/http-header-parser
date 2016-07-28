#ifndef UTILS_H
#define UTILS_H

static const unsigned int BUFFER_SIZE = 10;//0x1000;
static const char CRLF[3] = "\r\n";

/**
 * \brief Get next line from stream
 *
 * \param file that line will be downloaded from
 * \param outbuf pointer that will be alloc'd and filled with line
 *
 * \return whole line ending with CRLF
 *
 * \remark after doing stuff, function must be called again with NULL as output
 * buffer to cleanup after itself and recover file cursor to AFTER the last CRLF
 */
int next_line(FILE *file, char **outbuf);

#endif // UTILS_H
