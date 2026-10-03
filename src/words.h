#ifndef WORDS_H
#define WORDS_H

#include <wchar.h>

#define INPUT_BUF_SIZE 20
#define WRITE_LINE_W 45

typedef struct {
      wchar_t *word;
      int len;
      int cur_idx; // current character idx
      wchar_t inputs[INPUT_BUF_SIZE]; // holds input characters
      int x; // x position in win
} word_view;

void init_wview(word_view *wv, wchar_t *word, int written);

typedef struct {
      word_view *words;
      int cur; // current word's index
      int cap;
      int count;
} Line;

extern Line *lines;
extern int lines_count;
extern int lines_cap;
extern int curr_line_idx;

extern Line *cur_line;
extern word_view *cur_word;

// initialize first states of Line *lines;
void init_lines();
void print_lines(); // debug

// create brand new line
Line* new_line();

void move_nextLine();

// look input and change state of words and lines
void handle_input(int status, wint_t wch);

#endif
