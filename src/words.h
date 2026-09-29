#ifndef WORDS_H
#define WORDS_H

#define INPUT_BUF_SIZE 20

typedef struct {
      wchar_t *word;
      int len;
      int cur_idx;
      wchar_t inputs[INPUT_BUF_SIZE]; // holds input characters
} word_view;

void init_wview(word_view *wv);

typedef struct {
      word_view *words;
      int cur; // current word's index
      int cap;
      int count;
} Line;

// Line *lines;
// int lines_count;
// int lines_cap;
// int curr_line = 0;

// initialize first states of Line *lines;
void init_lines();

// create brand new line
Line* new_line();

// look input and change state of words and lines
int handle_input(int wch);

#endif
