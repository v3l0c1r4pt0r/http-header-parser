#include <bsd/string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char *FN_STDIN = "-";
static const unsigned int BUFFER_SIZE = 10;//0x1000;
static const char CRLF[3] = "\r\n";

int create_env(const char *line);
int next_line(FILE *file, char **buffer);

int main(int argc, char **argv)
{
  FILE *file = stdin;
  if(argc < 2)
  {
    fprintf(stderr, "%s: insufficient argument\n", argv[0]);
    return 1;
  }
  char *filename = argv[1];

  /* if fname != "-" then open file for reading */
  if(strcmp(filename, FN_STDIN) != 0)
  {
    file = fopen(filename, "r");
    if(!file)
    {
      perror(argv[0]);
      return -1;
    }
  }

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
  if(res != 0)
  {
    fprintf(stderr, "%s: something gone wrong, bye!", argv[0]);
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

  /* finally close file if it is not stdin */
  if(file != stdin)
  {
    fclose(file);
  }
  return 0;
}

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
        inbuf = (char*)realloc(inbuf, max_len + BUFFER_SIZE);
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
      strncpy(inbuf, crlf + sizeof(CRLF) - 1, len - (match_size));
      *(inbuf + len) = '\0';
      len -= match_size + (sizeof(CRLF) -1);
      crlf = strstr(inbuf, CRLF);
      return 0;
    }
  }
  return -1;
}
