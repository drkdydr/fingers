#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fingers.h"

char **words;
int words_count;
int words_capacity;

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

      if (!fp){
            printf("error: cannot open file\n");
            exit(1);
      }

      char buffer[BUFFER_SIZE];

      char *word =  fgets(buffer, BUFFER_SIZE, fp);


}

