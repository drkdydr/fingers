#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>


void readWordFile(const char *filename);
// starts program
void start(int duration); // no parameter after config file added.
      // draws typing screen
      void printTypingScr();

// print result screen
void printResult();
      // prints speed graph
      void printGraph();

// LATER:
// reads settings from config (language, time etc.)
void readConfig();

#endif
