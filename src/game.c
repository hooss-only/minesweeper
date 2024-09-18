#include <stdbool.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

int WIDTH, HEIGHT, MINE_AMOUNT;
bool loop_con = true; bool end = false;
void init_mine_map(bool mine_map[WIDTH][HEIGHT]) {
	unsigned int x, y, mine_amount = 0;

	while (true) {
		if (mine_amount >= MINE_AMOUNT) return;

		x = rand() % WIDTH;
		y = rand() % HEIGHT;

		if (mine_map[x][y] == true) continue;

		mine_map[x][y] = true;
		mine_amount++;
	}
}

void make_board(unsigned int game_board[WIDTH][HEIGHT], bool mine_map[WIDTH][HEIGHT]) {
	int dx[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
	int dy[8] = { 0, 1, -1, 1, -1, 0, 1, -1 };
	int nx, ny, found = 0;

	for (int i=0; i<WIDTH; i++) {
		for (int j=0; j<HEIGHT; j++) {
			if (mine_map[i][j]) {
				game_board[i][j] = '*';
				continue;
			}

			found = 0;
			for (int k=0; k<8; k++) {
				nx = i+dx[k];
				ny = j+dy[k];
				if (nx < 0 || nx >= WIDTH || ny < 0 || ny >= HEIGHT) continue;
				if (mine_map[nx][ny]) found++;
			}
			game_board[i][j] = found;
		}
	}
}

void print_board(int x, int y, int width, int height, char playing_board[width][height]) {
	attron(COLOR_PAIR(2));
	for (int i=0; i<width; i++) {
		mvprintw(0, 2*i+2, "%d", i);
	}

	for (int i=0; i<width; i++) {
		mvprintw(i+1, 0, "%d", i);
	}
	attroff(COLOR_PAIR(2));
	
	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			bool selected = (i==x && j==y);
			if (selected) attron(COLOR_PAIR(5));
			else if (playing_board[i][j] == '*') attron(COLOR_PAIR(4));
			else if (playing_board[i][j] == 'X') attron(COLOR_PAIR(6));
			else if (playing_board[i][j] == '#') attron(COLOR_PAIR(3));
			else attron(COLOR_PAIR(1));

			if (playing_board[i][j] != '*' && playing_board[i][j] != '#' && playing_board[i][j] != 'X') mvprintw(i+1, 2*j+2, "%d", playing_board[i][j]);
			else mvprintw(i+1, 2*j+2, "%c", playing_board[i][j]);

			if (selected) attroff(COLOR_PAIR(5));
			else if (playing_board[i][j] == '*') attroff(COLOR_PAIR(4));
			else if (playing_board[i][j] == 'X') attroff(COLOR_PAIR(6));
			else if (playing_board[i][j] == '#') attroff(COLOR_PAIR(3));
			else attroff(COLOR_PAIR(1));
		}
	}
	
	int s = height + 6;
	mvprintw(s, 0, "KEYMAP");
	mvprintw(s+1, 0, "Arrows: move");
	mvprintw(s+2, 0, "c: open");
	mvprintw(s+3, 0, "x: mark");
	mvprintw(s+4, 0, "q: exit");
}


void dfs(int width, int height, char playing_board[width][height], unsigned int game_board[width][height], int x, int y, bool visited[width][height]) {
  if (x < 0 || x >= width || y < 0 || y >= height || visited[x][y] || playing_board[x][y] == 'X') return;

  visited[x][y] = true;
	playing_board[x][y] = game_board[x][y];

	int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
	int dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int nx, ny;
	if (game_board[x][y] == 0) {
		for (int k=0; k<8; k++) {
			nx = x+dx[k];
			ny = y+dy[k];
			dfs(width, height, playing_board, game_board, nx, ny, visited);
		}
	}
}

void check_win(int width, int height, char playing_board[width][height], unsigned int mine_amount) {
	unsigned int not_open = 0;
	unsigned int marked = 0;
	for (int i=0; i<height; i++)
		for (int j=0; j<width; j++) {
			if (playing_board[i][j] == '#') not_open++;
			else if (playing_board[i][j] == 'X') marked++;
		}
	
	mvprintw(height+2, 0, "mine count: %d", mine_amount);
	mvprintw(height+3, 0, "marked: %d", marked);
	mvprintw(height+4, 0, "remain cells: %d", not_open);
	if (not_open+marked == mine_amount) {
		print_board(-1, -1, width, height, playing_board);
		attron(COLOR_PAIR(2));
		mvprintw(height+1, 0, "YOU WIN!!");
		attroff(COLOR_PAIR(2));
		end = true;
	}
}

void check(int x, int y, int width, int height, unsigned int game_board[width][height], char playing_board[width][height]) {
	if (playing_board[x][y] == 'X') return;
	if (game_board[x][y] == 0) {
		bool visited[width][height];
		memset(visited, false, sizeof(visited));
		dfs(width, height, playing_board, game_board, x, y, visited);
	}

	if (game_board[x][y] == '*') {
		for (int i=0; i<height; i++)
			for (int j=0; j<width; j++)
				if (game_board[i][j] == '*')
					playing_board[i][j] = game_board[i][j];

		print_board(-1, -1, width, height, playing_board);
		attron(COLOR_PAIR(4));
		mvprintw(height+1, 0, "YOU LOSE!!");
		attroff(COLOR_PAIR(4));
		end = true;
	}

	playing_board[x][y] = game_board[x][y];
}

void mark(int x, int y, int width, int height, char playing_board[width][height]) {
	if (playing_board[x][y] == '#') playing_board[x][y] = 'X';
	else if (playing_board[x][y] == 'X') playing_board[x][y] = '#';
	
}

int x = 0, y = 0;
void loop(int width, int height, unsigned int game_board[width][height], char playing_board[width][height]) {
	print_board(x, y, width, height, playing_board);
	check_win(width, height, playing_board, MINE_AMOUNT);

	int ch;
	ch = getch();
	if (ch == 'q') loop_con = false;
	else if (end) return;
	else if (ch == KEY_UP) x--;
	else if (ch == KEY_DOWN) x++;
	else if (ch == KEY_LEFT) y--;
	else if (ch == KEY_RIGHT) y++;
	else if (ch == 'c') check(x, y, width, height, game_board, playing_board);
	else if (ch == 'x') mark(x, y, width, height, playing_board);

	if (x < 0) x = 0;
	else if (x >= height) x = height-1;
	if (y < 0) y = 0;
	else if (y >= width) y = width-1;
}

void init_color_game() {
 	init_pair(1, COLOR_WHITE, COLOR_BLACK); // normal
 	init_pair(2, COLOR_YELLOW, COLOR_BLACK); // coordinates
	init_pair(3, COLOR_CYAN, COLOR_BLACK); // blocks
	init_pair(4, COLOR_RED, COLOR_BLACK); // bombs
	init_pair(5, COLOR_BLACK, COLOR_WHITE); // selected 
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK); // marker
}

void game(int sel) {
	srand(time(NULL));
	switch (sel) {
		case 0:
			WIDTH = 10;
			HEIGHT = 10;
			MINE_AMOUNT = 10;
			break;
		case 1:
			WIDTH = 10;
			HEIGHT = 10;
			MINE_AMOUNT = 30;
			break;
		case 2:
			WIDTH = 10;
			HEIGHT = 10;
			MINE_AMOUNT = 1;
			break;
	}

	init_color_game();

	bool mine_map[WIDTH][HEIGHT];
	memset(mine_map, 0, sizeof(mine_map));
	init_mine_map(mine_map);

	unsigned int game_board[WIDTH][HEIGHT];
	memset(game_board, 0, sizeof(game_board));
	make_board(game_board, mine_map);

	char playing_board[WIDTH][HEIGHT];
	memset(playing_board, 35, sizeof(playing_board));
	
	while (loop_con) {
		clear();
		loop(WIDTH, HEIGHT, game_board, playing_board);
		refresh();
	}
}
