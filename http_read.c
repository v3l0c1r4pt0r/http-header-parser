#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "http_read.h"
#include "utils.h"

/**
 * \brief read http packet sequentially from stream
 *
 * \param file stream to read from
 *
 * \return success indicator
 */
int http_read_file(FILE *file)
{
  /* read header line by line */
  char *buffer = NULL;
  int res = 0;
  while(!(res = next_line(file, &buffer)))
  {
    /* break on empty line */
    if(*buffer == '\0')
    {
      free(buffer);
      buffer = NULL;
      break;
    }
    /* extract variable and set it in environment */
    fprintf(stderr, "%s\n", buffer);
    free(buffer);
    buffer = NULL;
  }

  /* cleanup file descriptor */
  res = next_line(file, NULL);
  if(res)
  {
    return res;
  }

  /* print rest of file to stdout */
  buffer = (char*)malloc(BUFFER_SIZE);
  size_t bytes_read = 0;
  while(bytes_read = fread(buffer, 1, BUFFER_SIZE, file))
  {
    printf("%.*s", bytes_read, buffer);
  }
  setvbuf(stdout, (char*)NULL, _IONBF, 0);

  return 0;
}
