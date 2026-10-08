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

char bin_folder_buffer[PATH_BUFFERSIZE];

const char *name = "fingers";

WINDOW *win;
const int win_w = 53;
const int win_h = 12;

int win_x, win_y; // absolute position of program window

// const int test_time = 60; // will be read from config file later
extern int *graph_wps;
extern int graph_w;

// TEST PROPERTIES
int duration;

second *seconds;
int currSec; // which second we are in
             
struct timespec start_ts;

int record = 0;

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
      exit(1);
}

// look input and change state of words and lines
void handle_input(int status, wint_t input){
      wchar_t inp_wch = (wchar_t) input;

      // handle input
      if (input == KEY_RESIZE){
            alignWin();
            printTypingScr(currSec);
            return;
      }

      printInput(status, inp_wch);

      if (inp_wch == KEY_BACKSPACE || inp_wch == 127 || inp_wch == L'\b') { 
            if (cur_line->cur > cur_line->cur_word){
                  wchar_t *cur_wch = &cur_line->content[cur_line->cur];
                  if (*cur_wch == L'\0' || *cur_wch == L' ')
                        cur_line->overflow = false;
                  cur_line->cur--;
            }

      } else if (inp_wch == L' ' || inp_wch == L'\n' || inp_wch == L'\r' || inp_wch == KEY_ENTER){ 
            wchar_t *cur_wchar = &cur_line->content[cur_line->cur_word];

            // skip next word
            while (*cur_wchar != L' ' && *cur_wchar != L'\0') cur_wchar++; // we cannot use cur because of overflow condition

            if (*cur_wchar == L'\0'){
                  // fprintf(stderr, "null char found!\n");
                  // fflush(stderr);
                  if (curr_line_idx + 3 >= lines_count) // erişmeye çalıştığımız line initialize edilmiş mi?
                        for(int i = 0; i < 2; i++) new_line(); // create 2 new lines

                  // cur_line++; // realloc kullandığımız için lines adresi değişebilir bu da seg fault'a sebep olur
                  cur_line = &lines[++curr_line_idx];
                  printLines();
            } else { // if current char is space
                  cur_line->overflow = false;
                  cur_wchar++; // skip space
                  cur_line->cur = cur_line->cur_word = cur_wchar - cur_line->content;
            }

      } else{ // normal input
            // do not store any input, just store true/false info (for wpm falan filan)
            if (cur_line->cur >= cur_line->cur_word + INPUT_LEN_LIM) return;
            cur_line->cur++;
            wchar_t *cur_wch = &cur_line->content[cur_line->cur];
            if (*cur_wch == L'\0' || *cur_wch == L' ') 
                  cur_line->overflow = true;
      }

      // if (cur_wchar == L' '){
      //       // skip space
      //       cur_line->cur_word = ++cur_line->cur;
      // } else if (cur_wchar == L'\0'){ // if end of line
      //       cur_line = &lines[++curr_line_idx];
      //       // cur_wchar = cur_line->content[cur_line->cur];
      // }

      // ignore mouse inputs
}

// typing test part
int test(){
      wclear(win);
      printBorder();
                                    
      // initialize lines
      init_lines();

      // CREATE INITIAL DATA / CLEAR PREVIOUS TEST'S DATA
      // clear seconds
      for (int s = 0; s < duration; s++){
            second *cur = &seconds[s];
            cur->corrects = 0;
            cur->wrongs = 0;
      }

      // clear graph_wps array
      for (int i = 0; i < graph_w; i++) graph_wps[i] = 0;

      // recreate lines
      
      //

      wint_t input;
      int status; // input status (KEY_CODE_YES / OK / ERR)

      currSec = 0;
      printTypingScr(currSec);

      // wait for user input
      wtimeout(win, -1); // indefinitely waits for user input
      status = wget_wch(win, &input);
      clock_gettime(CLOCK_MONOTONIC, &start_ts);
      handle_input(status, input);
      wtimeout(win, 0);
      struct timespec now_ts;
      while (currSec < duration){
            // if current second changed reprint timer
            clock_gettime(CLOCK_MONOTONIC, &now_ts);
            int newcurrSec = now_ts.tv_sec - start_ts.tv_sec;
            if (newcurrSec > currSec) {
                  // fprintf(stderr, "current second = %d", currSec);
                  currSec = newcurrSec;
                  printTimer(currSec);
                  returnCursor(cur_line->cur - cur_line->cur_word);
            }

            status = wget_wch(win, &input);
            if (status != ERR) // if no input provided wget_wch returns ERR
                  handle_input(status, input);
      }

      return result();
}

int result(){ 
      wclear(win);
      wnoutrefresh(win);
      printBorder();

      wint_t input;
      int status;

      int wpm = 0, max_wps = 0, accuracy = 0;
      // int keystrokes; // added very long time ago but cant remember why (deprecated)
      double total_corrects, total_wrongs;
      int cur_wpm;

      // calculate wpm like variables in here
      for (int s = 0; s < duration; s++){
            second *cur_sec = &seconds[s];
            int cur_correct = cur_sec->corrects;
            int cur_wrong = cur_sec->wrongs;

            cur_wpm = (cur_correct + cur_wrong) / 5.0
                  * 60.0; // because it is one second data

            // also add to graph wps array
            int graph_idx = (int)ceil(((double)s / (double)(duration - 1)) * (double)(graph_w - 1)); // find a better way

            int *cur_graph = &graph_wps[graph_idx];
            *cur_graph += cur_wpm;
            if (*cur_graph > max_wps) max_wps = *cur_graph;

            total_corrects += cur_correct;
            total_wrongs += cur_wrong;
      }

      wpm = (total_corrects + total_wrongs) / (double) duration * 60.0 / 5.0;
      accuracy = total_corrects / (total_corrects + total_wrongs) * 100;

      bool isRecord = wpm > record;

      // save if it is a record
      if (isRecord){
            record = wpm;
            write_RecordFile(record, accuracy, duration);
      }
      
      printResult(wpm, max_wps, accuracy, duration, isRecord);

      wtimeout(win, -1);
      while (true){
            status = wget_wch(win, &input);
            if (status == KEY_CODE_YES && input == KEY_RESIZE) {
                  alignWin();
                  printResult(wpm, max_wps, accuracy, duration, isRecord);
            }
            if (status == OK && input == L'r') break;
            if (status == OK && input == 27) return 1; // exit program
      }

      return 0; // no need to exit program
}

void signalHandler(int sig){
      destroy();
      if (sig == SIGSEGV){
            printf("seg fault yedin aga :(\n");
      }
      exit(sig);
}

void init(int *_duration, char *_language){
      signal(SIGINT, signalHandler);
      signal(SIGSEGV, signalHandler);

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

      duration = *_duration;

      // allocate seconds array
      seconds = malloc(duration * sizeof(second));
      if (!seconds){
            printf("error: memory allocation failed.\n");
            exit(1);
      }

      // initializations:
      win_y = (LINES - win_h) / 2;
      win_x = (COLS - win_w) / 2;

      strcpy(bin_folder_buffer, getenv("HOME"));
      strcat(bin_folder_buffer, "/bin/fingers/");

      char words_path[200];
      strcpy(words_path, bin_folder_buffer);
      strcat(words_path, "words/");
      strcat(words_path, _language);
      strcat(words_path, ".csv");

      // read words from locale file
      readWordFile(words_path); // fopen çalıştırdığın konuma göre path'i alır

      // read config file
      readConfig(&duration, _language);

      // allocate wps arrays
      graph_wps = (int*)malloc(graph_w * sizeof(int));
      if (!graph_w){
            wprintf(L"error: allocation failed.\n");
            exit(1);
      }

      record = read_RecordFile();
}

void destroy(){
      
      free(seconds);
      free(graph_wps);
      free(lines);
      free(words);

      endwin();
}

void start(int *duration, char *language){
      init(duration, language);

      while (!test()) {
      }
      
      destroy();
}

