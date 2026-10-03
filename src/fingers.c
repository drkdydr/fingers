#include "fingers.h"
#include "words.h"
#include <math.h>
#include <signal.h>
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

const int test_time = 60; // will be read from config file later
extern int *graph_wps;
extern int graph_w;
// int wpm = 0;
// int max_wps = 0; // for graph generation

extern Line *lines;
extern int curr_line_idx;

// TEST PROPERTIES
// seconds
int currSec = 0; // which second we are in

// int wpm = 0; 
// int max_wps = 0; // for graph generation

// int key_stroke = 0;
// int accuracy = 0;


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

// look input and change state of words and lines
void handle_input(int status, int wch){
      // handle input
      if (status == KEY_CODE_YES){ // special characters
            if (wch == KEY_RESIZE){
                  alignWin();
                  printTypingScr(currSec);
            } else if (wch == KEY_ENTER){

            } else if (wch == KEY_BACKSPACE) {

            }
      } else if (status == OK) {
            if (wch == L'\n' || wch == L'\r'){ // ENTER

            } else if (wch == 127 || wch == L'\b') { // BACKSPACE

            } else {
                  

            }
      } else ; // ERR (error)

      // wadd_wch(win, input);
      // ignore mouse inputs
      // printInput(input, L'a', 10, 0);
}

// typing test part
void test(){
      wclear(win);
      printBorder();
                                    
      // initialize lines
      init_lines();

      // TODO calculate accuracy (correct characters / total typed characters) * 100
      // wpm formula : (total characters typed / 5) / time (in minutes)

      // Line *cur_line = &lines[curr_line_idx];
      // word_view *cur_word = &cur_line->words[cur_line->cur];

      wint_t input;
      int status; // input status (KEY_CODE_YES / OK / ERR)

      while (true){
            printTypingScr(currSec);
            while (true){
                  status = wget_wch(win, &input);

            }

      }

      // // TODO save if its is a record
      // if (wpm > record_wpm){
      //          lb = fopen("leaderboard.txt", "a"); // [timestamp] : user : { "wpm" : "36", "key-stroke" : "178", "test_time" : "60" }
      //          saveRecord();
      //          fclose(lb); // close leaderboard file
      // }

}

int result(){ 
      wclear(win);
      printBorder();

      wint_t input;
      int status;

      // you can calculate wpm like variables in here
      
      int wpm = 0, max_wps = 0, key_stroke = 0, accuracy = 0;
      
      // auto input (temporary)
      fillRandom(graph_wps, graph_w, &wpm, &max_wps);
      // printWPS();

      while (true){
            status = wget_wch(win, &input);
            if (status == KEY_CODE_YES && input == KEY_RESIZE) {
                  alignWin();
                  printResult(wpm, max_wps, accuracy);
            }
            if (status == OK && input == L'r') break;
            if (status == OK && input == 27) return 1; // exit program
      }
      return 0; // no need to exit program
}

void signalHandler(int sig){
      destroy();
      exit(sig);
}

void init(){
      signal(SIGINT, signalHandler);

      // screen properties
      set_escdelay(25);   // milisaniye cinsinden, initscr()'dan ÖNCE çağrılmalı
      initscr();
	cbreak();
      noecho();
	keypad(stdscr, TRUE);

      if (has_colors()) {
            start_color();
            use_default_colors();
      }
      refresh();

      init_pair(CORRECT, COLOR_GREEN /*foreground*/, -1 /*background*/);
      init_pair(WRONG, COLOR_RED /*foreground*/, -1 /*background*/); 

      d = fopen("debug.log", "w");
      
      // // get high score
      // lb = fopen("leaderboard.txt", "r"); 
      // int record_wpm = readRecordFile();

      // initializations:
      win_y = (LINES - win_h) / 2;
      win_x = (COLS - win_w) / 2;

      // read words from locale file
      readWordFile(WORD_FILE_PATH); // fopen çalıştırdığın konuma göre path'i alır

      // initialize wps arrays
      graph_wps = (int*)calloc(graph_w, sizeof(double));
      if (!graph_w){
            wprintf(L"error: allocation failed.\n");
            exit(1);
      }
}

void destroy(){
      
      fclose(d); // close debug file
      endwin();

}

void start(int duration){
      init();

      bool isTest = true;
      while (true) {
            if (isTest){
                  test();
            }else{ // result screen
                  if (result()) break; // if result() returns 1 end program
            }
            isTest = !isTest;
      }
      
      destroy();
}

