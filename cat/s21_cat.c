#include "s21_cat.h"

int main(int argc, char *argv[]) {
  int str_counter = 0, nonblank_counter = 0;
  opt options = {0};
  int error = parsing_args(argc, argv, &options, long_options);
  if (!error) {
    if (argv[optind] != NULL)
      reading_from_file(argc, argv, &options);
    else {
      reading_from_stdin(&options, &nonblank_counter, &str_counter);
    }
  }
  return 0;
}

int parsing_args(int argc, char *argv[], opt *options,
                 struct option *long_options) {
  int flag, error = 0;
  while ((flag = getopt_long(argc, argv, "benstvET", long_options, NULL)) !=
         -1) {
    switch (flag) {
      case 'b':
        options->b = 1;
        break;
      case 'e':
        options->e = 1;
        options->v = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 't':
        options->t = 1;
        options->v = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'E':
        options->E = 1;
        break;
      case 'T':
        options->T = 1;
        break;
      default:
        error = 1;
        usage();
    }
  }
  if (options->b) options->n = 0;
  return error;
}

void reading_from_file(int argc, char *argv[], opt *options) {
  for (int index = optind; index < argc; index++) {
    FILE *file = fopen(argv[index], "r");
    if (file != NULL) {
      int str_counter = 0, nonblank_counter = 0;
      printing_data(options, &nonblank_counter, &str_counter, file);
      closing_file(file);
    } else
      printf("./s21_cat: %s: No such file or directory\n", argv[index]);
  }
}

void reading_from_stdin(opt *options, int *nonblank_counter, int *str_counter) {
  printing_data(options, nonblank_counter, str_counter, stdin);
}

void printing_data(opt *options, int *nonblank_counter, int *str_counter,
                   FILE *file) {
  int current, blank = 0, last = '\n';

  while ((current = fgetc(file)) != EOF) {
    if (last == '\n') {
      if (options->s && current == '\n') {
        if (blank) continue;
        blank = 1;
      } else
        blank = 0;
      if (options->n) printf("%6d\t", ++(*str_counter));
      if (options->b && current != '\n') printf("%6d\t", ++(*nonblank_counter));
    }

    if ((options->e || options->E) && current == '\n') printf("$");
    if (options->v) v_flag(&current);
    if ((options->t || options->T) && current == TAB)
      printf("^I");
    else
      printf("%c", current);
    last = current;
  }
}

void v_flag(int *current) {
  if (*current >= 0 && *current < 32 && *current != 9 && *current != 10) {
    *current += 64;
    printf("^");
  } else if (*current == 127) {
    *current -= 64;
    printf("^");
  } else if (*current > 127 && *current < 160) {
    *current -= 64;
    printf("M-^");
  }
}

void closing_file(FILE *file) {
  if (fclose(file) == EOF) printf("Error closing file\n");
}

void usage() { printf("Usage: ./s21_cat [-benstvET] [file]\n"); }