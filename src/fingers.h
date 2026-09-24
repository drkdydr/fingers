#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>
#include <wchar.h>

#define RESTART_KEY R

typedef struct {
      wchar_t *word;
      int len;
      int cur_idx;
      bool *inputs; // boolean array to hold each index is true or false typed
} word_view;

void init_wview(word_view *wv, wchar_t *w);

// circular queue for test words list
#define TCQ_CAP 100
typedef struct {
      word_view *words[TCQ_CAP];
      int front;
      int rear;
} TCQ; // test circular queue

void init_tcQueue(TCQ *tcq);

bool isEmpty(TCQ *tcq);

bool isFull(TCQ *tcq);

word_view* dequeue(TCQ *tcq);

void enqueue(TCQ *tcq, wchar_t *val);


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
