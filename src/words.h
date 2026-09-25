#ifndef WORDS_H
#define WORDS_H

#define INPUT_BUF_SIZE 20

typedef struct {
      wchar_t *word;
      int len;
      int cur_idx;
      wchar_t inputs[INPUT_BUF_SIZE]; // holds input characters
} word_view;

void init_wview(word_view *wv, wchar_t *w);

typedef struct {
      word_view **words;
      int cap;
      int count;
} Line;

// Line currentLines[2];
// Line *written;

Line* new_line();

int add_input_line(int w);

int add_input_wc(word_view *wv, int w); //enter basıldıysa true döndürecek (başka kelimeye geç manasında)

#endif
