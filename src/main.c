#include <locale.h>
#include <curses.h>
#include <hoist.h>
#include <stdio.h>
#include <stdlib.h>
#include "fingers.h"
#include <unistd.h>
#include <time.h>

extern const char *name; // fingers
                         
// TODO
// fingers'taki gereksiz constant'ları temizle

int main(int argc, char **argv){
      setlocale(LC_ALL, "");
      srand(time(0)); // for randomness

      int time_limit = 60;

      // flags for configuration
      hoist(name, "tui typing test");

      char *time_lim_str;
      new_opt_arged("-d", "--duration", "DURATION", "custom test duration in seconds (default 60 seconds)", &time_lim_str);

      parse_args(argc, argv);
      strike();

      if (time_lim_str)
            time_limit = atoi(time_lim_str);

      // screen properties
      initscr();
	cbreak();
	keypad(stdscr, TRUE);
      refresh();

      curs_set(0); // makes cursor invisible

      start(time_limit);
      sleep(11115);

      endwin();
}
