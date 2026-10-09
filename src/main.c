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
// line'daki son kelimeyi overflow yapıp enter'layınca donuyor

int main(int argc, char **argv){
      setlocale(LC_ALL, "");
      srand(time(0)); // for randomness

      int time_limit = 0; // default time is 60 seconds

      // flags for configuration
      hoist(name, "tui typing test");

      char *time_lim_str = "\0";
      new_opt_arged("-d", "--duration", "DURATION", "custom test duration in seconds (default 60 seconds)", &time_lim_str);
      char *language = "\0"; // turkish by default
      new_opt_arged("-l", "--lang", "LANGUAGE", "select language", &language);

      parse_args(argc, argv);
      strike();

      if (time_lim_str[0] != '\0')
            time_limit = atoi(time_lim_str);

      if (time_limit < 0){
            printf("error: invalid test time\n");
            exit(1);
      }
      
      start(&time_limit, language);

      endwin();
}
