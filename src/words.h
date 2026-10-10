#ifndef WORDS_H
#define WORDS_H

#include <stdbool.h>
#include <wchar.h>

#define INPUT_BUF_SIZE 60
#define WRITE_LINE_W 45
#define INPUT_LEN_LIM 20

typedef struct {
      wchar_t content[WRITE_LINE_W + 1]; // for \0
      wchar_t inputs[INPUT_BUF_SIZE];
      int cur; // current char's index
      int cur_word; // current word's index
      bool overflow;
      int length;
} Line;

extern Line *lines;
extern int lines_count;
extern int lines_cap;
extern int curr_line_idx;

extern Line *cur_line;
// extern word_view *cur_word;

// initialize first states of Line *lines;
void init_lines();
void print_lines(); // debug

// create brand new line
Line* new_line();

void move_nextLine();

// look input and change state of words and lines
void handle_input(int status, wint_t wch);

#endif
