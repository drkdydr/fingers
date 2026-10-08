#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "words.h"
#include "fingers.h"

#define BUFFER_SIZE 100

// const char *wordFileName = "en_US.csv";

// typedef struct {
      // wchar_t content[WRITE_LINE_W + 1]; // for \0
      // wchar_t inputs[WRITE_LINE_W + 1];
      // int cur; // current char's index
// } Line;

Line *lines;
int lines_count;
int lines_cap;
int curr_line_idx;


Line *cur_line;
// word_view *cur_word;

// initialize first states of Line *lines;
void init_lines(){
      lines_count = 0;
      curr_line_idx = 0;

      // create starting lines
      for (int i = 0; i < 5; i++) new_line(); // start with 5 lines
      
      cur_line = &lines[0]; // first line's
      // cur_word = &cur_line->words[0]; //first word
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
      line->cur = 0;
      line->cur_word = 0;
      line->overflow = false;

      // insert words
      int written = 0;
      while (true){
            // get random word from word list
            wchar_t *word = words[rand() % words_count];
            size_t wlen = wcslen(word);

            if (written + wlen >  WRITE_LINE_W) break;

            wmemcpy(&line->content[written], word, wlen);

            written += wcslen(word); // 1 for adjusting space
            line->content[written++] = L' '; // seperate words with space
      }

      // null terminator
      line->content[written - 1] = L'\0'; // -1 koymasam "s" + " " + "\0"

      return 0;
}
