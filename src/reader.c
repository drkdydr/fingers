#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <ctype.h>
#include "fingers.h"

wchar_t **words;
int words_count;
int words_capacity;

#define BUFFER_SIZE 100

const char *wordFileName = "tr.csv";

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
      
      // // debug:
      // for(int i = 0; i < words_count; i++){
      //       wprintf(L"%ls, ", words[i]);
      // }
      // wprintf(L"\n");

      return;
}

#define RECORD_FORMAT "[%s] ~ %s ~ [ \"wpm\" : \"%d\", \"accuracy\" : \"%d%%\", \"test_time\" : \"%d\" ]\n"

int read_RecordFile(){
      return 0;

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

      FILE *r_file = fopen("records.txt", "a");
      if (!r_file){
            fprintf(stderr, "warning: record cannot be opened.\n");
            return;
      }
      fprintf(r_file, RECORD_FORMAT, /*timestamp*/ "" , username, new_record, accuracy, test_time);
      fclose(r_file);
}

// meraklısına
// char *my_strdup(const char *s) {
//     char *copy = malloc(strlen(s) + 1);
//     if (copy) strcpy(copy, s);
//     return copy;
// }

