#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>

#define RESTART_KEY R

void readWordFile(const char *filename);
// starts program
void start(int duration); // no parameter after config file added.
      // draws typing screen
      void printTypingScr(int curr_sec);

// print result screen
void printResult(int wpm, int max_wps);
      // // prints speed graph (no need over simplification)
      // void printGraph();

// LATER:
// reads settings from config (language, time etc.)
void readConfig();

#endif
