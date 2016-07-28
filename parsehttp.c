#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "http_read.h"
#include "utils.h"

static const char *FN_STDIN = "-";

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

  uint8_t flags = FLAG_NONE;
  /* TODO: do getopt */

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

  int res = http_read_file(file, flags);
  if(res)
  {
    fprintf(stderr, "%s: something gone wrong, bye!", argv[0]);
    return res;
  }

  /* finally close file if it is not stdin */
  if(file != stdin)
  {
    fclose(file);
  }
  return 0;
}
