#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>
#include <wchar.h>

#define RESTART_KEY R

#define LINE_START_Y 3
#define LINE_START_X 4

#define CORRECT 1
#define WRONG 2

void readWordFile(const char *filename);
// starts program
void run(int duration); // no parameter after config file added.
      void init();
      // draws typing screen
      void printTypingScr();
      void printTimer(int cur_sec);
      void printInput(int inp_wch, int word_cur_wch, int x, int cur);

// print result screen
void printResult(int wpm, int max_wps, int accuracy);
      // // prints speed graph (no need over simplification)
      // void printGraph();

// LATER:
// reads settings from config (language, time etc.)
void readConfig();

#endif
