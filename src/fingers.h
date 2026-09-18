#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>

#define RESTART_KEY R

typedef struct {
      int idx;
      char val;
      bool isTrue;
} pair;

typedef struct {
      char *word;
      int wlen;
      int cur_idx;
      pair **trues;
      pair **falses;
} word_view;

void init_wview(word_view *wv, char *w);

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

void enqueue(TCQ *tcq, char *val);


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
