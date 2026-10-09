#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <ctype.h>
#include <string.h>
#include "fingers.h"

wchar_t **words;
int words_count;
int words_capacity;

#define BUFFER_SIZE 200

char leaderboard_path[PATH_BUFFERSIZE];
FILE *lbf; // leaderboard file

// add read word to memory
void add_word(wchar_t *word){
      // allocate memory if unallocated or insufficient
      if (!words) {
           words_capacity = 10;
           words = malloc(words_capacity * sizeof(wchar_t*));
      }
      if (words_count >= words_capacity){
           words_capacity *= 2;
           words = realloc(words, words_capacity * sizeof(wchar_t*));
      }
      if (!words) {
           wprintf(L"error: mem allocation failed.\n");
           exit(1);
      }

      words[words_count++] = word;
}

// read words from word file
void readWordFile(const char *filename){
      // char * line = NULL;
      // size_t len = 0;
      // ssize_t read;

      FILE* fp = fopen(filename, "r");
      if (!fp){
            wprintf(L"error: cannot open file '%s'.\n", filename);
            exit(1);
      }

      wchar_t buffer[BUFFER_SIZE];

      while (fwscanf(fp, L"%99ls", buffer) == 1){
            wchar_t *word = wcsdup(buffer);  // string'in heap üzerinde kopyasını alır
            if (!word) {
                  wprintf(L"error: mem allocation failed.\n");
                  exit(1);
            }
            add_word(word);
      }

      fclose(fp);

      return;
}

#define RECORD_FORMAT "[%s] ~ %s ~ [ \"wpm\" : \"%d\", \"accuracy\" : \"%d%%\", \"test_time\" : \"%d\" ]\n"
#define RECORD_SCAN_FORMAT \
  "[%*[^]]] ~ %*[^~]~ [ \"wpm\" : \"%d\", \"accuracy\" : \"%*d%%\", \"test_time\" : \"%*d\" ]"

// read function will be always started before write so I initialized leaderboard_path here
int read_RecordFile(){
      int record = 0;
      // read file will work
      strcpy(leaderboard_path, bin_folder_buffer);
      strcat(leaderboard_path, "leaderboard.txt"); // otomatik ~ ($HOME) algılama yok

      lbf = fopen(leaderboard_path, "rb");
      if (!lbf){
            fprintf(stderr, "warning: record file cannot be opened.\n");
            return record;
      }

      if (fseek(lbf, -1, SEEK_END) != 0) { /* file empty */ }

      // skip last \n characters (cause there can be multiple)
      while (fgetc(lbf) == '\n') {
      if (fseek(lbf, -2, SEEK_CUR) != 0) break;
      }

      // find start of last line
      do {
      if (fseek(lbf, -2, SEEK_CUR) != 0) {   // start of file (fseek returns 0 if fails -> fails if nothing to seek)
        rewind(lbf);
        break;
      }
      } while (fgetc(lbf) != '\n');

      fscanf(lbf, RECORD_SCAN_FORMAT, &record); // scanf okumazsa pointer değerine dokunmuyormuş

      fclose(lbf);

      return record;
}

// [timestamp] ~ USER ~ [ "wpm" : "36", "accuracy" : "78%", "test_time" : "60" ]
void write_RecordFile(int new_record, int accuracy, int test_time){

      char *username = getenv("USER");

      // upper username
      char *un_cpy = username;
      while (*un_cpy) {
            *un_cpy = toupper(*un_cpy);
            un_cpy++;
      }
      lbf = fopen(leaderboard_path, "a");
      if (!lbf){
            fprintf(stderr, "warning: record cannot be opened.\n");
            return;
      }

      time_t now = time(NULL);
      char *time_str = ctime(&now);
      char *t_str_cpy = time_str;
      while (*t_str_cpy != '\n') t_str_cpy++;
      *t_str_cpy = '\0';

      fprintf(lbf, RECORD_FORMAT, time_str , username, new_record, accuracy, test_time);
      fclose(lbf);
}

// meraklısına
// char *my_strdup(const char *s) {
//     char *copy = malloc(strlen(s) + 1);
//     if (copy) strcpy(copy, s);
//     return copy;
// }

