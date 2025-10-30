#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    char *patterns[argc], *files[argc];
    Options options = {0};
    Variables variables = {0};
    int error = parsing_args(argc, argv, &options, patterns, files, &variables);
    if (!error) {
      reading_from_file(argc, argv, options, &variables, patterns, files);
    }
  } else
    usage();
  return 0;
}

int parsing_args(int argc, char *argv[], Options *options, char *patterns[],
                 char *files[], Variables *variables) {
  int flag, error = 0;
  while (!error && (flag = getopt_long(argc, argv, "e:ivclnhsf:", long_options,
                                       NULL)) != -1) {
    switch (flag) {
      case 'e':
        options->e = 1;
        if (strlen(optarg) < MAX_SIZE)
          patterns[variables->patterns_cnt++] = optarg;
        else {
          printf("s21_grep: Argument is too long\n");
          error = 1;
        }
        break;
      case 'i':
        options->i = 1;
        break;
      case 'v':
        options->v = 1;
        break;
      case 'c':
        options->c = 1;
        break;
      case 'l':
        options->l = 1;
        break;
      case 'n':
        options->n = 1;
        break;
      case 'h':
        options->h = 1;
        break;
      case 's':
        options->s = 1;
        break;
      case 'f':
        options->f = 1;
        files[variables->files_cnt++] = optarg;
        break;
      default:
        error = 1;
        usage();
    }
  }

  if (options->l || options->c) options->n = 0;
  return error;
}

void make_single_pattern(Variables *variables, char *patterns[], int cnt) {
  for (int i = 0; i < cnt; i++) {
    size_t max_length = MAX_SIZE - strlen(variables->reg_pattern);
    printf("max_length = %lu\n", max_length);
    if (strlen(patterns[i]) < max_length) {
      if ((strlen(variables->reg_pattern) != 0)) {
        strcat(variables->reg_pattern, "|");
        strcat(variables->reg_pattern, patterns[i]);
      } else
        strcat(variables->reg_pattern, patterns[i]);
    }
  }
}

int check_files(Variables *variables, char *files[]) {
  int error = 0, size_error = 0;
  for (int i = 0; i < variables->files_cnt && !error; i++) {
    FILE *file = fopen(files[i], "r");
    if (file != NULL) {
      int n = 0;
      char *str = NULL;
      char *patterns_from_file[MAX_SIZE];
      size_t len = 0;
      ssize_t size;

      while ((size = getline(&str, &len, file)) != EOF && !size_error) {
        if (size < MAX_SIZE && n < MAX_SIZE) {
          str[size - 1] = '\0';
          patterns_from_file[n++] = str;
          str = NULL;
        } else {
          printf("s21_grep: out of memory\n");
          size_error = 1;
        }
      }

      if (str) free(str);

      if (!size_error && !error) {
        make_single_pattern(variables, patterns_from_file, n);

        for (int j = 0; j < n; j++) free(patterns_from_file[j]);
      } else
        error = 1;
      closing_file(file);
    } else {
      printf("s21_grep: %s: No such file or directory\n", files[i]);
      error = 1;
    }
  }
  return error;
}

void reading_from_file(int argc, char *argv[], Options options,
                       Variables *variables, char *patterns[], char *files[]) {
  int error = 0;
  if (options.f) error = check_files(variables, files);
  if (!error) {
    if (options.e)
      make_single_pattern(variables, patterns, variables->patterns_cnt);
    printf("reg_pattern = %s\n", variables->reg_pattern);
    printf("strlen reg_pattern = %lu\n", strlen(variables->reg_pattern));
    variables->total_files =
        options.e || options.f ? argc - optind : argc - optind - 1;
    for (variables->index = !options.e && !options.f ? optind + 1 : optind;
         variables->index < argc; variables->index++) {
      FILE *file = fopen(argv[variables->index], "r");
      if (file != NULL) {
        variables->str_counter = 0;
        search_in_file(
            argv, file, options,
            options.e || options.f ? variables->reg_pattern : argv[optind],
            variables);
        closing_file(file);
      } else if (!options.s)
        printf("s21_grep: %s: No such file or directory\n",
               argv[variables->index]);
    }
  }
}

void search_in_file(char *argv[], FILE *file, Options options, char pattern[],
                    Variables *variables) {
  printf("pattern in search in file = %s\n", pattern);
  int stop = 0;
  int regex_flag = REG_EXTENDED;
  if (options.i) regex_flag = REG_EXTENDED | REG_ICASE;
  regex_t reg;
  int reg_expr =
      regcomp(&reg, !options.e && !options.f ? pattern : variables->reg_pattern,
              regex_flag);
  if (reg_expr) {
    printf("s21_grep: Invalid regular expression\n");
  } else {
    int str_number = 0;
    char *str = NULL;
    size_t len = 0;
    while (getline(&str, &len, file) != EOF && !stop) {
      str_number++;

      int match = regexec(&reg, str, 0, NULL, 0);

      printing_data(argv, match, options, str, str_number, variables, &stop);
    }
    // !options.l удалить после тестирования
    if (options.c /* && !options.l*/) {
      if (variables->total_files == 1 || options.h)
        printf("%d\n", variables->str_counter);
      else
        printf("%s:%d\n", argv[variables->index], variables->str_counter);
    }

    if (options.l && stop) printf("%s\n", argv[variables->index]);

    if (str) free(str);
    regfree(&reg);
  }
}

void printing_data(char *argv[], int match, Options options, char str[],
                   int str_number, Variables *variables, int *stop) {
  if ((!options.v && match == 0) || (options.v && match == REG_NOMATCH)) {
    if (options.l) *stop = 1;
    if (options.c) variables->str_counter++;

    if ((options.n || options.h) && !options.l && !options.c) {
      if (options.h && !options.n)
        printf("%s", str);
      else if (variables->total_files == 1 || options.h)
        printf("%d:%s", str_number, str);
      else if (variables->total_files > 1)
        printf("%s:%d:%s", argv[variables->index], str_number, str);
    }

    else if (!options.c && !options.n && !options.l && !options.h) {
      if (variables->total_files == 1)
        printf("%s", str);
      else if (variables->total_files > 1)
        printf("%s:%s", argv[variables->index], str);
    }
  }
}

void closing_file(FILE *file) {
  if (fclose(file) == EOF) printf("Error closing file\n");
}

void usage() { printf("Usage: s21_grep [-benstvET] [file]\n"); }
