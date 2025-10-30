#ifndef S21_CAT_H
#define S21_CAT_H
#define TAB 9
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int E;
  int T;
} opt;

static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                       {"number", 0, 0, 'n'},
                                       {"squeeze-blank", 0, 0, 's'},
                                       {0, 0, 0, 0}};

void reading_from_file(int argc, char *argv[], opt *options);
void reading_from_stdin(opt *options, int *nonblank_counter, int *str_counter);
int parsing_args(int argc, char *argv[], opt *options,
                 struct option *long_options);
void printing_data(opt *options, int *nonblank_counter, int *str_counter,
                   FILE *file);
void v_flag(int *current);
void closing_file(FILE *file);
void usage();
#endif