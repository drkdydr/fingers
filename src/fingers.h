#ifndef FINGERS_H
#define FINGERS_H

#include <curses.h>
#include <wchar.h>

#define PATH_BUFFERSIZE 200

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

extern wchar_t **words;
extern int words_count;
extern int words_capacity;

extern char bin_folder_buffer[PATH_BUFFERSIZE];

void readWordFile(const char *filename);
// starts program
void start(int *duration, char *language); // no parameter after config file added.
      void init(int *duration, char *language);
      void destroy();

      int test();
            int result(); // returns 1 if user wanted to exit

      // draws typing screen
      void alignWin();
      void printBorder();
      void printTypingScr(int cur_sec);
      void printTimer(int cur_sec);
      void printLines();
      void printInput(int inp_status, wchar_t inp_wch); 
      // move cursor to end of input line
      void returnCursor(int idx);
      
// print result screen
void printResult(int wpm, int max_wps, int accuracy, int duration, bool isRecord);
      // // prints speed graph (no need over simplification)
      // void printGraph();

// LATER:
// reads record file and returns record
int read_RecordFile(); 
// write record to end of file
void write_RecordFile(int new_record, int accuracy, int test_time);

#endif
