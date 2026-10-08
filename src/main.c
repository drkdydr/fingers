#include <locale.h>
#include <curses.h>
#include <hoist.h>
#include <stdio.h>
#include <stdlib.h>
#include "fingers.h"
#include "words.h"
#include <unistd.h>
#include <time.h>

extern const char *name; // fingers
                         
// TODO
// fingers'taki gereksiz constant'ları temizle
// fix overflow (sanırım hallettim)
// write line kısmında 45 genişlikte yazabilelim en fazla fakat kalan kelimeleri aşağı (input line) yazalım
// bazen (sadece overflow durumunda değil) line düzgün temizlenmiyor.
// init_lines'ı düzenle (testi tekrar başlattığımızda baştan memory allocate etcem diye uğraşmasın)

int main(int argc, char **argv){
      setlocale(LC_ALL, "");
      srand(time(0)); // for randomness

      int time_limit = 10;

      // flags for configuration
      hoist(name, "tui typing test");

      char *time_lim_str;
      new_opt_arged("-d", "--duration", "DURATION", "custom test duration in seconds (default 60 seconds)", &time_lim_str);

      parse_args(argc, argv);
      strike();

      if (time_lim_str)
            time_limit = atoi(time_lim_str);

      start(time_limit);

      endwin();
}
