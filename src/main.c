#include <curses.h>
#include <hoist.h>
#include <stdio.h>
#include <stdlib.h>
#include "fingers.h"
#include <unistd.h>

extern const char *name; // fingers

int main(int argc, char **argv){
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
      sleep(10000000);

      endwin();
}
