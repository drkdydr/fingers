#include "fingers.h"
#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *name = "fingers";

WINDOW *win;
const int win_w = 53;
const int win_h = 12;

int win_x, win_y; // absolute position of program window

const int test_time = 60; // will be read from config file later
// int duration_mins, duration_secs;

int *wps;



// char* upper(const char *str){
//       char *res;
//       strcpy(res, str);
//       for(int i = 0; )
// }

void start(int duration){
      // initializations:
      win_y = (LINES - win_h) / 2;
      win_x = (COLS - win_w) / 2;

      // draw box
      win = newwin(win_h, win_w, win_y, win_x);
      box(win, 0, 0);

      // write application name as header
      mvwhline(win, 0, 2, ' ', strlen(name) + 2);
      wmove(win, 0, 3); // move cursor
      size_t nameLen = strlen(name);
      for(int i = 0; i < nameLen; i++){
         waddch(win, toupper((unsigned char) name[i]));}

      wrefresh(win);

      int currSec = 0; // which second we are in

      int wpm = 0;
      int max_wps = 0;

      // printTypingScr();
      printResult(wpm, max_wps);
      

// while(true){
//       speedHistory = (int*)calloc(duration, sizeof(int));
//       if (!speedHistory){
//             printf("error: memory cannot allocated.\n");
//             exit(1);
//       }
// }
}

