#include "fingers.h"
#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *name = "fingers";

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

WINDOW *fingersWin;
const int win_w = 53;
const int win_h = 12;

int win_x, win_y; // absolute position of program window

const int test_time = 60; // will be read from config file later
// int duration_mins, duration_secs;

int *speedHistory;
int currSec = 0; // which second we are in

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
      fingersWin = newwin(win_h, win_w, win_y, win_x);
      box(fingersWin, 0, 0);

      // write application name as header
      mvwhline(fingersWin, 0, 2, ' ', strlen(name) + 2);
      wmove(fingersWin, 0, 3); // move cursor
      size_t nameLen = strlen(name);
      for(int i = 0; i < nameLen; i++){
         waddch(fingersWin, toupper((unsigned char) name[i]));}

      wrefresh(fingersWin);

      printTypingScr();
      

// while(true){
//       speedHistory = (int*)calloc(duration, sizeof(int));
//       if (!speedHistory){
//             printf("error: memory cannot allocated.\n");
//             exit(1);
//       }
// }
}

