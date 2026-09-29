#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "fingers.h"
#include "words.h"

extern wchar_t **words;
extern int words_count;
extern int words_capacity;

#define BUFFER_SIZE 100
#define WRITE_LINE_W 45

// const char *wordFileName = "en_US.csv";

// typedef struct {
//       wchar_t *word;
//       int len;
//       int cur_idx;
//       wchar_t inputs[INPUT_BUF_SIZE]; // holds input characters
// } word_view;

// typedef struct {
//       word_view **words;
//       int cur;
//       int cap;
//       int count;
// } Line;

Line *lines;
int lines_count;
int lines_cap;
int curr_line = 0;

// initialize first states of Line *lines;
void init_lines(){
      // initialize lines
      lines_cap = 3;
      lines = malloc(lines_cap * sizeof(Line));
      if (!lines){
            wprintf(L"error: lines could not initialized.\n");
            exit(1);
      }
      lines_count = 0;
      curr_line = 0;

      // create starting lines
      for (int i = 0; i < 3; i++) new_line(); // start with 3 lines
}

// initialize a word
void init_wview(word_view *wv) {

      wv->cur_idx = 0;

      wchar_t *word = words[rand() % words_count];
      wv->word = word; // set random word
      wv->len = wcslen(word); // returns size of wide string

}

// create brand new line
Line* new_line(){
      if (lines_count >= lines_cap) {
            lines->cap *= 2;
            lines = realloc(lines, lines_cap * sizeof(Line));
            if (!lines){
                  wprintf(L"error: memory allocation failed.\n");
                  exit(1);
            }
      }
      Line *line = lines + lines_count;
      line->count = 0;
      line->cap = 3;
      line->cur = 0;
      line->words = malloc(line->cap * sizeof(word_view));

      // insert words
      int written = 0;
      while (written <=  WRITE_LINE_W){

            // if line's word array full, expand
            if (line->count >= line->cap) {
                  line->cap += 3;
                  line->words = realloc(line->words, sizeof(Line[line->cap]));
                  if (!line->words){
                        wprintf(L"memory allocation failed.\n");
                        exit(1);
                  }
            }

            word_view *wv = &line->words[line->cur];
            init_wview(wv);

            written += wcslen(wv->word) + 1; // 1 for adjusting space
            line->count++; // increase word count of line
      }

      lines_count++;
      return 0;
}

// look input and change state of words and lines
void handle_input(int wch){
      Line *cur_line = &lines[curr_line];
      word_view *cur_word = &cur_line->words[cur_line->cur];

      if (wch == KEY_ENTER || wch == L' '){ // if pressed ENTER or SPACE go next word
            if (cur_line->cur >= cur_line->count - 1){ // if last word of line
                  curr_line++;
                  return;
            }
            cur_line->cur++;
      }else if (wch == KEY_BACKSPACE){ // if pressed BACKSPACE go one char
            if (cur_word->cur_idx > 0) cur_word->cur_idx--;
      }else{ // save input to word's inputs
            if (cur_word->cur_idx >= INPUT_BUF_SIZE) return; // if buffer is full, ignore
            cur_word->inputs[++cur_word->cur_idx] = wch;
      }

}
