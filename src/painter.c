#include "fingers.h"
#include <math.h>
#include <curses.h>
#include <string.h>

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

// result blocks
#define LVL0_BLOCK " "
#define LVL1_BLOCK "▁"
#define LVL2_BLOCK "▂"
#define LVL3_BLOCK "▃"
#define LVL4_BLOCK "▄"
#define LVL5_BLOCK "▅"
#define LVL6_BLOCK "▆"
#define LVL7_BLOCK "▇"
#define LVL8_BLOCK "█"

#define BLOCK_LEVELS 8

#define HIST_W 40

void printTypingScr(int currSec){
      // seperator line
      mvwhline(win, SEPLINE_Y, SEPLINE_X, ACS_HLINE, SEPLINE_W);
      
      // words
      
      // timer
      mvwprintw(win, TIMER_Y, TIMER_X, "%02d:%02d", (test_time - currSec) / 60, (test_time - currSec) % 60);

      // indicator
      mvwaddch(win, IND_Y, IND_X, IND_CH);

      wrefresh(win);
}

int roundUppperTen(int val){
      if (val % 10 > 0) return val + (10 - (val % 10));
      return val;
}

extern int *wps;
extern int max_wps;

// print result screen
void printResult(int wpm, int max_wps){
      const char *header = "-- RESULTS --";
      mvwprintw(win, 1, (win_w - strlen(header)) / 2, "%s", header);

      // calculate intervals
      int halfblock_wps; // wpm equivalent of '-'
      if (max_wps <= 45)
            halfblock_wps = 5;
      else
           halfblock_wps = ceil((double)max_wps / 9.0);
      //     
      // write left side numbers
      for (int i = 0; i < 3; i++) {
            mvwprintw(win, 5 + (2*i), 3, "%*d -", 3, (9 - 4*i) * halfblock_wps);
      }

      // bottom line (seconds indicator) 
      mvwaddch(win, 10, 8, '0');
      mvwprintw(win, 10, 28, "%2d", test_time / 2);
      mvwprintw(win, 10, 48, "%2d", test_time);
      
      // write wpm
      mvwprintw(win, 3, 3, "WPM: %d", wpm);

      // write graph
      for(int i = 0; i < 40; i++){
            continue;
      }

      // restart info
      const char *restartInfo = " Type 'R' to retry ";
      mvwprintw(win, win_h - 1 , (win_w - strlen(restartInfo)) / 2, "%s", restartInfo);

      wrefresh(win);
}

// prints speed graph
void printGraph();
