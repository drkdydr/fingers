#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "fingers.h"

extern wchar_t **words;
extern int words_count;
extern int words_capacity;

#define BUFFER_SIZE 100

const char *wordFileName = "en_US.csv";

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
           printf("error: mem allocation failed.\n");
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
            printf("error: cannot open file '%s'.\n", filename);
            exit(1);
      }

      wchar_t buffer[BUFFER_SIZE];

      while (fwscanf(fp, L"%99s", buffer) == 1){
            wchar_t *word = wcsdup(buffer);  // string'in heap üzerinde kopyasını alır
            if (!word) {
                  printf("error: mem allocation failed.\n");
                  exit(1);
            }
            add_word(word);
      }

      fclose(fp);
      
      // debug:
      for(int i = 0; i < words_count; i++){
            wprintf(L"%s, ", words[i]);
      }
      wprintf(L"\n");

      return;
}

// meraklısına
// char *my_strdup(const char *s) {
//     char *copy = malloc(strlen(s) + 1);
//     if (copy) strcpy(copy, s);
//     return copy;
// }

