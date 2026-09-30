#include "fingers.h"
#include "words.h"
#include <math.h>
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <curses.h>
#include <string.h>
#include <unistd.h>

extern WINDOW *win;
extern const int win_w;
extern const int win_h;

extern const char *name;

// seperator line properties
#define SEPLINE_W 45
#define SEPLINE_Y 7
#define SEPLINE_X 4

// time variables
extern const int test_time; // will be read from config file later
extern int currSec; // which second we are in
#define TIMER_Y 1
#define TIMER_X 45

// type indicator
#define IND_X 4
#define IND_Y 9
#define IND_CH '>'

extern FILE *d;

// result blocks
const int blocks_size = 9;
const wchar_t blocks[] = {
      L' ',
      L'▁',
      L'▂',
      L'▃',
      L'▄',
      L'▅',
      L'▆',
      L'▇',
      L'█'
};

#define WRONG COLOR_RED
#define CORRECT COLOR_GREEN

// const char* blocks[] = {
//       "0",
//       "1",
//       "2",
//       "3",
//       "4",
//       "5",
//       "6",
//       "7",
//       "8"
// };

#define HIST_W 40

extern Line *lines;
extern int curr_line_idx;

#define LINE_START_Y 4
#define LINE_START_X 5

void printTypingScr(int currSec){
      // seperator line
      mvwhline(win, SEPLINE_Y, SEPLINE_X, ACS_HLINE, SEPLINE_W);
      
      // timer
      mvwprintw(win, TIMER_Y, TIMER_X, "%02d:%02d", (test_time - currSec) / 60, (test_time - currSec) % 60);

      // indicator
      mvwaddch(win, IND_Y, IND_X, IND_CH);
      
      // words
      for(int l = 0; l < 2; l++){
            Line *cur_line = &lines[curr_line_idx + l];
            wmove(win, LINE_START_Y + (2 * l), LINE_START_X); // move start

            for (int w = 0; w < cur_line->count; w++){ // for every word
                  word_view *word = &cur_line->words[w];
                  extern int words_count;
                  if (!word->word) mvwprintw(win, l, 1, "%d.\n", words_count);
                  // waddnwstr(win, word->word, -1); // do not need to print inputs in first state
            }
      }

      wmove(win, IND_Y, IND_X + 2); // move typing space

      wrefresh(win);
}

int roundUppperTen(int val){
      if (val % 10 > 0) return val + (10 - (val % 10));
      return val;
}

int *graph_wps;
int graph_w = 40;

// print result screen
void printResult(int wpm, int max_wps, int accuracy){
      const char *header = "-- RESULTS --";
      mvwprintw(win, 1, (win_w - strlen(header)) / 2, "%s", header);

      // calculate intervals
      double block_wps; // wpm equivalent of '-'
      if (max_wps <= 60)
            block_wps = 60.0 / 4.5;
      else
           block_wps = ceil((double)max_wps / 4.5);
          
      // write left side numbers
      for (int i = 0; i < 3; i++) {
            mvwprintw(win, 5 + (2*i), 3, "%*d -", 3, (int)((9 - 4*i) * block_wps / 2));
      }

      // bottom line (seconds indicator) 
      mvwaddch(win, 10, 8, '0');
      mvwprintw(win, 10, 8 + graph_w / 2, "%2d", test_time / 2);
      mvwprintw(win, 10, 8 + graph_w, "%2d", test_time);
      
      // write wpm
      mvwprintw(win, 2, 3, "WPM: %d", wpm);
      mvwprintw(win, 3, 3, "Accuracy: %d%%", accuracy);

      // write graph
      for(int i = 0; i < graph_w; i++){
            int curr = graph_wps[i];
            fprintf(d, "actual value = %d\n", curr);
            int blockVal = ( ( curr / block_wps ) * ( blocks_size - 1 ) ); // value in blocks
            fprintf(d, "block equivalent %d ",blockVal);
            int l = 0;
            while (blockVal > 0){
                  if(blockVal >= blocks_size - 1){
                        fprintf(d, "->> %d ",blocks_size -1);
                        mvwprintw(win, 9 - l, 9 + i, "%lc", blocks[blocks_size - 1]);
                        blockVal -= blocks_size - 1;
                  }else{
                        fprintf(d, "->> %d ",blockVal);
                        mvwprintw(win, 9 - l, 9 + i, "%lc", blocks[blockVal]);
                        break;
                  }
                  l++;
                  wrefresh(win);
            }
            fprintf(d, "\n");
      }

      // restart info
      const char *restartInfo = " Type 'R' to retry ";
      mvwprintw(win, win_h - 1 , (win_w - strlen(restartInfo)) / 2, "%s", restartInfo);

      wrefresh(win);
}
