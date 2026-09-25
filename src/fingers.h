#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>
#include <wchar.h>

#define RESTART_KEY R

void readWordFile(const char *filename);
// starts program
void run(int duration); // no parameter after config file added.
      void init();
      // draws typing screen
      void printTypingScr(int curr_sec);

// print result screen
void printResult(int wpm, int max_wps, int accuracy);
      // // prints speed graph (no need over simplification)
      // void printGraph();

// LATER:
// reads settings from config (language, time etc.)
void readConfig();

#endif
