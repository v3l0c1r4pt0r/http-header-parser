#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

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
int next_line(FILE *file, char **outbuf)
{
  static char *inbuf = NULL;
  static size_t len = 0, max_len = 0;
  static char *crlf = NULL;
  int bytes_read = 0;
  /* if no buffer given, do cleanup */
  if(!outbuf)
  {
    /* set file cursor at the end of processed data */
    int seek = fseek(file, -len, SEEK_CUR);
    if(inbuf)
    {
      free(inbuf);
      inbuf = NULL;
    }
    max_len = len = 0;
    crlf = NULL;
    return seek * 2;
  }
  while(1)
  {
    if(!crlf)
    {
      if(max_len < BUFFER_SIZE + len)
      {
        inbuf = (char*)realloc(inbuf, max_len + BUFFER_SIZE + 1);
        inbuf[max_len + BUFFER_SIZE] = '\0';
        max_len += BUFFER_SIZE;
      }
      bytes_read = fread(inbuf + len, 1, max_len - len, file);
      if(bytes_read == 0)
      {
        *outbuf = NULL;
        return -1;
      }
      len += bytes_read;
    }
    while(crlf || ((crlf = strstr(inbuf, CRLF)) != NULL))
    {
      size_t match_size = crlf - inbuf;
      *outbuf = (char*)malloc(match_size + 1);
      strncpy(*outbuf, inbuf, match_size);
      *(*outbuf + match_size) = '\0';
      strncpy(inbuf, crlf + sizeof(CRLF) - 1, len - match_size);
      *(inbuf + len) = '\0';
      len -= match_size + (sizeof(CRLF) -1);
      crlf = strstr(inbuf, CRLF);
      return 0;
    }
  }
  return -1;
}

int set_env(char *line)
{
  char *name, *value;
  size_t value_len = 0;

  /* find first colon in line */
  char *colon = strchr(line, ':');
  if(!colon)
  {
    return -1;
  }

  /* extract key */
  name = (char*)malloc((colon - line) + 1);
  strncpy(name, line, colon - line);
  name[colon-line]='\0';

  /* set cursor to start of value */
  colon += 1;
  while(isspace(*colon))
  {
    colon += 1;
  }

  /* extract value */
  value_len = strlen(colon);
  value = (char*)malloc(value_len + 1);
  strncpy(value, colon, value_len);
  value[value_len] = '\0';

  /* set it to env */
  /* TODO */
  setenv(name, value, 1);

  /* clean up */
//free(name);
//free(value);
  return 0;
}
