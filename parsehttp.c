#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "http_read.h"
#include "utils.h"

static const char *FN_STDIN = "-";

int create_env(const char *line);
int next_line(FILE *file, char **buffer);

int main(int argc, char **argv)
{
  FILE *file = stdin;
  FILE *errs = stderr;

  uint8_t flags = FLAG_NONE;
  int opt;
  while ((opt = getopt(argc, argv, "eqh")) != -1) {
    switch (opt) {
      case 'e':
        flags |= FLAG_ENVI;
        break;
      case 'q':
        flags |= FLAG_QUET;
        errs = fopen("/dev/null","w");
        break;
      case 'h':
        printf("Usage: %s [-eqh] filename|-\n", argv[0]);
        printf(
            "\t-e\tset header variables as environment variables and spawn"
            " new shell\n"
            "\t\tProgram might be called using 'exec %s [args]' to\n"
            "\t\tsubstitute shell with new one\n"
            "\t-q\tbe quite on errors\n"
            "\t-h\tprint this help message\n",
            argv[0]
        );
        return 0;
      default:
        /* '?' */
        fprintf(errs, "Usage: %s [-eqh] filename|-\n", argv[0]);
        return 2;
    }
  }

  if (optind >= argc) {
    fprintf(errs, "%s: Insufficient argument\n", argv[0]);
    return 1;
  }

  char *filename = argv[optind];

  /* if fname != "-" then open file for reading */
  if(strcmp(filename, FN_STDIN) != 0)
  {
    file = fopen(filename, "r");
    if(!file)
    {
      if(!(flags & FLAG_QUET))
      {
        perror(argv[0]);
      }
      return -1;
    }
  }

  int res = http_read_file(file, flags);
  if(res)
  {
    fprintf(errs, "%s: something gone wrong, bye!", argv[0]);
    return res;
  }

  /* finally close file if it is not stdin */
  if(file != stdin)
  {
    fclose(file);
  }

  /* and execute new shell to preserve environment in case of updating it */
  if(flags & FLAG_ENVI)
  {
    system("$SHELL -i");
  }
  return 0;
}
