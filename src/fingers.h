#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>
#include <wchar.h>

#define RESTART_KEY R

#define LINE_START_Y 3
#define LINE_START_X 4

#define CORRECT 1
#define WRONG 2

typedef struct {
      int wrongs;
      int corrects;
} second;

extern int duration;
extern second *seconds;

void readWordFile(const char *filename);
// starts program
void start(int duration); // no parameter after config file added.
      void init(int duration);
      void destroy();

      void test();
      int result(); // returns 1 if user wanted to exit

      // draws typing screen
      void alignWin();
      void printBorder();
      void printTypingScr(int cur_sec);
      void printTimer(int cur_sec);
      void printLines();
      void printInput(int inp_status, wchar_t inp_wch); 
      
// print result screen
void printResult(int wpm, int max_wps, int accuracy, int duration);
      // // prints speed graph (no need over simplification)
      // void printGraph();

// LATER:
// reads settings from config (language, time etc.)
void readConfig();

#endif
