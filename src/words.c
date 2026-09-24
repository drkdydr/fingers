#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "fingers.h"

extern wchar_t **words;
extern int words_count;
extern int words_capacity;

#define BUFFER_SIZE 100

// const char *wordFileName = "en_US.csv";

// TODO revise wview and queue functions

// typedef struct{
// } word_view;

// typedef struct {
//       int idx;
//       char val;
//       bool isTrue;
// } pair;

// typedef struct {
//       char *word;
//       int wsize;
//       int cur_idx;
//       pair** typos;
// } word_view;

void init_wview(word_view *wv, wchar_t *w) {
      wv->cur_idx = 0;
      wv->word = w;
      wv->len = wcslen(w); // returns size of wide string
      // initialize typos in heap
      // wv->trues = ;
}

void add_pair(word_view *wv, int idx, wchar_t c, bool isTrue){
}

int add_input(word_view *wv, char w){ //enter basıldıysa true döndürecek (başka kelimeye geç manasında)

      // if (w == KEY_ENTER) return 1;
      // if (w == BACKSPACE) {
      //    wv->cur_idx--;
      //    return 0;
      // }
      if (wv->word[wv->cur_idx] == w){
            add_pair(wv, wv->cur_idx, w, true);
      }else{
            add_pair(wv, wv->cur_idx, w, false);
      }
      return 0;
}

// ==== CIRCULAR QUEUE FOR TEST WORDS ====

// Circular Queue implementation for test words list 
void init_tcQueue(TCQ *tcq){
      tcq->front = tcq->rear = -1;

      // fill with random words
      for (int i = 0; i < TCQ_CAP; i++){
            int idx = rand() % words_count;
            enqueue(tcq, words[idx]);
      }
}

// FFM (for future me): kelimeler rastgele şekilde burada tutulacak sonra her space'te dequeue olup da içinde doğru ve yanlış basış bilgisi bulunan word_view nesnesine dönüştürülecek (ya da satır bitince görünmez hale gelince tüm satırı otomatik olarak dequeue yapsın)

bool isEmpty(TCQ *tcq){
      return tcq->front == -1;
}

bool isFull(TCQ *tcq){
      return tcq->front = ( tcq->rear + 1 ) % TCQ_CAP;
}

word_view* dequeue(TCQ *tcq){
      if (isEmpty(tcq)) return 0;
      word_view *res = tcq->words[tcq->front];
      if (tcq->front == tcq->rear) tcq->front = tcq->rear = -1;
      else tcq->front = ( tcq->front + 1 ) % TCQ_CAP;
      
      return res;
}

void enqueue(TCQ *tcq, wchar_t *val){
      if (!tcq) init_tcQueue(tcq);
      if (isFull(tcq)) return;
      tcq->rear = ( tcq->rear + 1 ) % TCQ_CAP;
      if (tcq->front == -1) tcq->front++;
      word_view *wv;
      
      return;
      // to be completed
      // tcq->words[tcq->rear] = val;
}

