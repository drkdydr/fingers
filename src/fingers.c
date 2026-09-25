#include "fingers.h"
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

const char *name = "fingers";
#define WORD_FILE_PATH "/home/dydr/projects/fingers/words/tr.csv"

WINDOW *win;
const int win_w = 53;
const int win_h = 12;

int win_x, win_y; // absolute position of program window
                  
// words
wchar_t **words;
int words_count;
int words_capacity;

const int test_time = 60; // will be read from config file later
extern int *graph_wps;
extern int graph_w;
// int wpm = 0;
// int max_wps = 0; // for graph generation



FILE *d; // debug log file
FILE *lb; // leaderboard

// debug tool to fill wps values randomly
void fillRandom(int *arr, int arr_size, int* wpm, int* max_wps){
      // FILE *f = fopen("debug.log", "a");
      // fprintf(f, "graph_wps=%p, graph_w=%d\n", (void*)graph_wps, graph_w);
      // fclose(f);

      if (!arr) return;
      // printf("filling random...\n");
      int lower = 0, upper = 200;

      double total = 0;

      for (int i = 0; i < arr_size; i++){
            int random_int = rand() % ( upper + 1 );
            // printf("random-int : %d", random_int);
            if (*max_wps < random_int) *max_wps = random_int;
            arr[i] = random_int;
            total += random_int;
      }
      // printf("filled random\n");
      *wpm = ceil(total / (double)arr_size);
}

// test for debugging function
void printWPS(){
      if (!graph_wps) return;
      printf("graph_wps:\n");
      for(int i = 0; i < graph_w; i++){
            printf("%d, ", graph_wps[i]);
      }
}

void run(int duration){

      d = fopen("debug.log", "w");
      
      // // get high score
      // lb = fopen("leaderboard.txt", "r"); 
      // int record_wpm = readRecordFile();

      // initializations:
      win_y = (LINES - win_h) / 2;
      win_x = (COLS - win_w) / 2;

      int currSec = 0; // which second we are in

      int wpm = 0; 
      int max_wps = 0; // for graph generation

      int key_stroke = 0;
      int accuracy = 0;

      // read words from locale file
      readWordFile(WORD_FILE_PATH); // fopen çalıştırdığın konuma göre path'i alır

      // initialize wps arrays
      graph_wps = (int*)calloc(graph_w, sizeof(double));
      if (!graph_w){
            wprintf(L"error: allocation failed.\n");
            exit(1);
      }
      
      // auto input (temporary)
      fillRandom(graph_wps, graph_w, &wpm, &max_wps);
      // printWPS();

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

      // TODO calculate accuracy (correct characters / total typed characters) * 100
      // wpm formula : (total characters typed / 5) / time (in minutes)

      printTypingScr(currSec);
      // printResult(wpm, max_wps, accuracy);

      // // TODO save if its is a record
      // if (wpm > record_wpm){
               // lb = fopen("leaderboard.txt", "a"); // [timestamp] : user : { "wpm" : "36", "key-stroke" : "178", "test_time" : "60" }
      //       saveRecord();
               // fclose(lb); // close leaderboard file
      // }
      
      fclose(d); // close debug file
}

