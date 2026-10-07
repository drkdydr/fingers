#include "fingers.h"
#include "words.h"
#include <ctype.h>
#include <math.h>
#ifndef _XOPEN_SOURCE_EXTENDED
#define _XOPEN_SOURCE_EXTENDED
#endif
#include <curses.h>
#include <string.h>
#include <unistd.h>
#include <sys/param.h>
#include <stdlib.h>
#include <stdio.h>

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
#define IND_OFFSET 2 // right margin of indicator

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

void alignWin(){
      extern int win_x, win_y;

      win_y = (LINES - win_h) / 2;
      win_x = (COLS - win_w) / 2;

      werase(win);
      wnoutrefresh(win); // saves changes to memory and doupdate prints these changes
      mvwin(win, win_y, win_x);
      printBorder();
}

void printBorder(){
      // write application name as header
      mvwhline(win, 0, 2, ' ', strlen(name) + 2);
      wmove(win, 0, 3); // move cursor
      size_t nameLen = strlen(name);
      for(int i = 0; i < nameLen; i++){
         waddch(win, toupper((unsigned char) name[i]));}

      // draw box
      extern int win_x, win_y;
      win = newwin(win_h, win_w, win_y, win_x);
      box(win, 0, 0);

      wnoutrefresh(win);
}

// extern Line *lines;
// extern int curr_line_idx;

// change time in every second
void printTimer(int cur_sec){
      mvwprintw(win, TIMER_Y, TIMER_X, "%02d:%02d", (test_time - cur_sec) / 60, (test_time - cur_sec) % 60);
}

// just print input dont handle anything more
void printInput(int inp_stat, int inp_wch){
      int cur_idx = cur_line->cur;
      wchar_t *cur_wch = &cur_line->content[cur_idx];
      int inp_line_idx = cur_idx - cur_line->cur_word;

      // print both input line and test line
      if (inp_wch == KEY_BACKSPACE || inp_wch == 127 || inp_wch == L'\b') { 
            if (!(cur_wch - 1) || *(cur_wch - 1) == L'\0')
                  mvwaddnwstr(win, LINE_START_Y, LINE_START_X + cur_idx - 1, L" ", 1);
            else 
                  mvwprintw(win, LINE_START_Y, LINE_START_X + cur_idx - 1, "%lc", *(cur_wch - 1)); // clear true/false info
            mvwprintw(win, IND_Y, IND_X + 1 + IND_OFFSET + inp_line_idx - 1, " "); // clear from input line
            wmove(win, IND_Y, IND_X + 1 + IND_OFFSET + MAX(inp_line_idx - 1, 0)); 

      } else if (inp_wch == L' ' || inp_wch == L'\n' || inp_wch == L'\r' || inp_wch == KEY_ENTER){ // enter and space ends input for current word
            // eğer kelimede harf kaldıysa yanlış olarak yaz
            wattron(win, COLOR_PAIR(WRONG));
            while (cur_line->content[cur_idx] != L' ' && cur_line->content[cur_idx] != L'\0') 
                    mvwaddnwstr(win, LINE_START_Y, LINE_START_X + cur_idx, &cur_line->content[cur_idx++], 1);
            wattroff(win, COLOR_PAIR(WRONG));

            int next_word_idx = cur_line->cur_word;
            while (cur_line->content[next_word_idx] != L' ' && cur_line->content[next_word_idx] != L'\0') next_word_idx++;
            next_word_idx++; // skip space
            if (next_word_idx < WRITE_LINE_W){ // if line is not ended
                  // reprint rest of line (if overflow this prevents overwrite later words)
                  mvwprintw(win, LINE_START_Y, LINE_START_X + next_word_idx, "%-*ls", WRITE_LINE_W - next_word_idx, cur_line->content + next_word_idx);
            }

            // clear whole input line
            mvwprintw(win, IND_Y, IND_X + 1 + IND_OFFSET, "%-*ls", inp_line_idx + 1, L"");

            wnoutrefresh(win);

            // move cursor to first pos
            wmove(win, IND_Y, IND_X + 1 + IND_OFFSET);

      }else if (inp_stat != KEY_CODE_YES){ // normal input
            // print input to input line
            int pair = inp_wch != *cur_wch ? COLOR_PAIR(WRONG) : cur_line->overflow ? COLOR_PAIR(WRONG) : COLOR_PAIR(CORRECT);

            wattron(win, pair);
            mvwprintw(win, LINE_START_Y, LINE_START_X + cur_idx, "%lc", inp_wch); 
            wattroff(win, pair);

            // print input line
            mvwprintw(win, IND_Y, IND_X + 1 + IND_OFFSET + inp_line_idx, "%lc", inp_wch); 
      }
}

void printLines() {
      for(int l = 0; l < 2; l++){
            Line *line = &lines[curr_line_idx + l]; // next lines
            mvwprintw(win,LINE_START_Y + (2 * l), LINE_START_X, "%-*ls ", WRITE_LINE_W, line->content); // this also clears prev line (- aligns left)
      }

}

void printTypingScr(int cur_sec){
      curs_set(1); // makes cursor visible
                   
      // seperator line
      mvwhline(win, SEPLINE_Y, SEPLINE_X, ACS_HLINE, SEPLINE_W);
      
      // timer
      printTimer(0);

      // indicator
      mvwaddch(win, IND_Y, IND_X, IND_CH);
      
      // words
      for(int l = 0; l < 2; l++){
            Line *line = &lines[l]; // next lines
            mvwprintw(win,LINE_START_Y + (2 * l), LINE_START_X, "%-*ls ", WRITE_LINE_W, line->content); // this also clears prev line
      }

      wmove(win, IND_Y, IND_X + 2); // move typing space
                                    
      // word_view *cur_word = 
      // for (int i = 0; i < ) 

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
      curs_set(0); // makes cursor invisible
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
