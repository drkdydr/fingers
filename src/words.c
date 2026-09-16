#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fingers.h"

extern char **words;
extern int words_count;
extern int words_capacity;

#define BUFFER_SIZE 100

const char *wordFileName = "en_US.csv";

// Circular Queue implementation for test words list 
#define TCQ_CAP 50
typedef struct {
      char *words[TCQ_CAP];
      int front;
      int rear;
} testCirQueue;

void init_tcQueue(testCirQueue *tcq){
      tcq->front = tcq->rear = -1;
}

bool isEmpty(testCirQueue *tcq){
      return tcq->front == -1;
}

bool isFull(testCirQueue *tcq){
      return tcq->front = ( tcq->rear + 1 ) % TCQ_CAP;
}

char* dequeue(testCirQueue *tcq){
      if (isEmpty(tcq)) return 0;
      char *res = tcq->words[tcq->front];
      if (tcq->front == tcq->rear) tcq->front = tcq->rear = -1;
      else tcq->front = ( tcq->front + 1 ) % TCQ_CAP;
      
      return res;
}

void enqueue(testCirQueue *tcq, char *val){
      if (!tcq) init_tcQueue(tcq);
      if (isFull(tcq)) return;
      tcq->rear = ( tcq->rear + 1 ) % TCQ_CAP;
      if (tcq->front == -1) tcq->front++;
      tcq->words[tcq->rear] = val;
}

// add read word to memory
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

// read words from word file
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

