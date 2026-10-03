#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "words.h"
#include "fingers.h"

// words
wchar_t **words;
int words_count;
int words_capacity;

#define BUFFER_SIZE 100

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
int curr_line_idx;


Line *cur_line;
word_view *cur_word;

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
      curr_line_idx = 0;

      // create starting lines
      for (int i = 0; i < 3; i++) new_line(); // start with 3 lines
      
      cur_line = &lines[0]; // first line's
      cur_word = &cur_line->words[0]; //first word
}

// initialize a word
void init_wview(word_view *wv, wchar_t *word, int written) {

      wv->cur_idx = 0;
      wv->word = word; // set random word
      wv->len = wcslen(word); // returns size of wide string
      wv->x = LINE_START_X + written;
}

// create brand new line
Line* new_line(){

      if (lines_count >= lines_cap) {
            lines_cap *= 2;
            lines = realloc(lines, lines_cap * sizeof(Line));
            if (!lines){
                  wprintf(L"error: memory allocation failed.\n");
                  exit(1);
            }
      }

      Line *line = &lines[lines_count++];
      line->count = 0;
      line->cap = 3;
      line->cur = 0;
      line->words = malloc(line->cap * sizeof(word_view));

      // insert words
      int written = 0;
      wchar_t *word = words[rand() % words_count];
      while (written + wcslen(word) <=  WRITE_LINE_W){

            // if line's word array full, expand
            if (line->count >= line->cap) {
                  line->cap += 3;
                  line->words = realloc(line->words, sizeof(word_view[line->cap]));
                  if (!line->words){
                        wprintf(L"memory allocation failed.\n");
                        exit(1);
                  }
            }

            word_view *wv = &line->words[line->count++];
            init_wview(wv, word, written);
            written += wcslen(word) + 1; // 1 for adjusting space

                                             
            word = words[rand() % words_count]; // generate new word for next cycle
      }
      return 0;
}

extern FILE *d;
void print_lines(){
      for (int l = 0; l < lines_count; l++){
            Line *line = &lines[l];
                  fwprintf(stderr, L"cur line word count: %d\n", line->count);
            for (int w = 0; w < line->count; w++){
                  fwprintf(stderr, L"cur word: %ls len: %d\n",line->words[w].word, wcslen(line->words[w].word));
                  fflush(stderr);
            }
            fwprintf(stderr, L"\n");
      }
}
