#include "fingers.h"
#include <curses.h>
#include <string.h>

extern WINDOW *fingersWin;
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


void printTypingScr(){
      // seperator line
      mvwhline(fingersWin, SEPLINE_Y, SEPLINE_X, ACS_HLINE, SEPLINE_W);
      
      // words
      
      // timer
      mvwprintw(fingersWin, TIMER_Y, TIMER_X, "%02d:%02d", test_time / 60, test_time % 60);

      // indicator
      mvwaddch(fingersWin, IND_Y, IND_X, IND_CH);

      wrefresh(fingersWin);
}

// print result screen
void printResult(){

}

// prints speed graph
void printGraph();
