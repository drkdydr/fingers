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
// init_lines'ı düzenle (testi tekrar başlattığımızda baştan memory allocate etcem diye uğraşmasın)
// birkaç tekrardan sonra wpm cozutuyor

int main(int argc, char **argv){
      setlocale(LC_ALL, "");
      srand(time(0)); // for randomness

      int time_limit = 10; // default time is 60 seconds

      // flags for configuration
      hoist(name, "tui typing test");

      char *time_lim_str;
      new_opt_arged("-d", "--duration", "DURATION", "custom test duration in seconds (default 60 seconds)", &time_lim_str);
      char *language = "tr"; // turkish by default
      new_opt_arged("-l", "--lang", "LANGUAGE", "select language", &language);

      parse_args(argc, argv);
      strike();

      if (time_lim_str)
            time_limit = atoi(time_lim_str);

      
      start(&time_limit, language);

      endwin();
}
