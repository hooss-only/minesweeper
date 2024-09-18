#include <ncurses.h>
#include <locale.h>

const char *LOGO[] = {
	"███╗   ███╗██╗███╗   ██╗███████╗███████╗██╗    ██╗███████╗███████╗██████╗ ███████╗██████╗ ",
	"████╗ ████║██║████╗  ██║██╔════╝██╔════╝██║    ██║██╔════╝██╔════╝██╔══██╗██╔════╝██╔══██╗",
	"██╔████╔██║██║██╔██╗ ██║█████╗  ███████╗██║ █╗ ██║█████╗  █████╗  ██████╔╝█████╗  ██████╔╝",
	"██║╚██╔╝██║██║██║╚██╗██║██╔══╝  ╚════██║██║███╗██║██╔══╝  ██╔══╝  ██╔═══╝ ██╔══╝  ██╔══██╗",
	"██║ ╚═╝ ██║██║██║ ╚████║███████╗███████║╚███╔███╔╝███████╗███████╗██║     ███████╗██║  ██║",
	"╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚══════╝╚══════╝ ╚══╝╚══╝ ╚══════╝╚══════╝╚═╝     ╚══════╝╚═╝  ╚═╝"
};

int menu() {
	int ch;
	int sel = 0;
	
	size_t len = sizeof(LOGO)/sizeof(char*);
	
	const char *options[] = {
		"PLAY EASY (10x10, 10 mines)",
		"PLAY HARD (40x40, 60 mines)",
		"EXIT"
	};
	const size_t length = sizeof(options)/sizeof(char*);

	for (int i=0; i<len; i++) {
		mvprintw(i, 0, "%s", LOGO[i]);
	}
	
	char *color = "\033[37m";
	for (int i=0; i<length; i++) {
		if (i == sel) {
			attron(COLOR_PAIR(2));
			mvprintw(len+i, 0, "%s", options[i]);
			attroff(COLOR_PAIR(2));
		} else {
			attron(COLOR_PAIR(1));
			mvprintw(len+i, 0, "%s", options[i]);
			attroff(COLOR_PAIR(1));
		}
	}

	while ((ch = getch()) != '\n') {
		switch (ch) {
			case KEY_UP:
				sel -= 1;
				break;
			
			case KEY_DOWN:
				sel += 1;
				break;
		}
		clear();
		if (sel < 0) sel = 0;
		else if (sel >= length) sel = length-1;

		for (int i=0; i<len; i++) {
			mvprintw(i, 0, "%s", LOGO[i]);
		}
		
		char *color = "\033[37m";
		for (int i=0; i<length; i++) {
			if (i == sel) {
				attron(COLOR_PAIR(2));
				mvprintw(len+i, 0, "%s", options[i]);
				attroff(COLOR_PAIR(2));
			} else {
				attron(COLOR_PAIR(1));
				mvprintw(len+i, 0, "%s", options[i]);
				attroff(COLOR_PAIR(1));
			}
		}
		refresh();
	}
	return sel;
}

void init() {
	initscr();
	start_color();
  raw();
  keypad(stdscr, true);
  noecho();
	curs_set(0);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_WHITE);
}

int main() {
	setlocale(LC_CTYPE, "");
	init();

	menu();

	endwin();
	return 0;
}
