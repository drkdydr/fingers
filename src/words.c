#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fingers.h"

extern char **words;
extern int words_count;
extern int words_capacity;

#define BUFFER_SIZE 100

const char *wordFileName = "en_US.csv";

void add_word(char *word){
      // allocate memory if unallocated or insufficient
      if (!words) {
           words_capacity = 10;
           words = malloc(words_capacity * sizeof(char*));
      }
      if (words_count >= words_capacity){
           words_capacity *= 2;
           words = realloc(words, words_capacity * sizeof(char*));
      }
      if (!words) {
           printf("error: mem allocation failed.\n");
           exit(1);
      }

      words[words_count++] = word;
}

void readWordFile(const char *filename){
      FILE* fp = fopen(filename, "r");

      char * line = NULL;
      size_t len = 0;
      ssize_t read;

      fp = fopen(filename, "r");
      if (!fp){
            printf("error: cannot open file '%s'.\n", filename);
            exit(1);
      }

      char buffer[BUFFER_SIZE];

      while (fscanf(fp, "%99s", buffer) == 1){
            char *word = strdup(buffer);  // string'in heap üzerinde kopyasını alır
            if (!word) {
                  printf("error: mem allocation failed.\n");
                  exit(1);
            }
            add_word(word);
      }

      fclose(fp);
      
      // debug:
      // for(int i = 0; i < words_count; i++){
      //       printf("%s, ", words[i]);
      // }
      // printf("\n");

      return;
}

// meraklısına
// char *my_strdup(const char *s) {
//     char *copy = malloc(strlen(s) + 1);
//     if (copy) strcpy(copy, s);
//     return copy;
// }

