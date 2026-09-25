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

// TODO revise wview and queue functions

// typedef struct {
//       wchar_t *word;
//       int len;
//       int cur_idx;
//       wchar_t inputs[INPUT_BUF_SIZE]; // holds input characters
// } word_view;

// typedef struct {
//       word_view **words;
//       int cap;
//       int count;
// } Line;

void init_wview(word_view *wv, wchar_t *w) {
      wv->cur_idx = 0;
      wv->word = w;
      wv->len = wcslen(w); // returns size of wide string
      // initialize typos in heap
      // wv->inputs = malloc(wv->len * sizeof(bool));
      // if (!wv->inputs){
      //       printf("error: memory allocation failed.\n");
      //       exit(1);
      // }
}

int add_input_line(int w){

}

int add_input_wc(word_view *wv, int w){ //enter basıldıysa true döndürecek (başka kelimeye geç manasında)

      if (w == KEY_ENTER || w == L' ') return 1;

      if (w == KEY_BACKSPACE) {
            if (wv->cur_idx > 0) wv->cur_idx--;
         // return 0;
      }

      // write input to inputs
      if (wv->cur_idx < 20) 
            wv->inputs[wv->cur_idx++] = w;
      return 0;
}

