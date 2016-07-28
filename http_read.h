#ifndef HTTP_READ_H
#define HTTP_READ_H

/**
 * \brief read http packet sequentially from stream
 *
 * \param file stream to read from
 *
 * \return success indicator
 */
int http_read_file(FILE *file);

#endif // HTTP_READ_H
