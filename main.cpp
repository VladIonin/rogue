#include <pdcurses.h>
#include "Screen.h"

#include <string.h>

// map variable
const int MAP_HEIGHT = 20;
const int MAP_WIDTH = 40;
int map[MAP_HEIGHT][MAP_WIDTH] = {
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
{0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

int items[MAP_HEIGHT][MAP_WIDTH];

int inventory[10];

void initItems();
void drawMap();
void drawTile();
bool isPassable(int mapX, int mapY);
void interactDoor(char action);
void getCommand();
void dropCommand();
void useCommand();
void showInventory();

int nPlayerX, nPlayerY;

struct TILE_TYPE{
	char nCharacter;	// ASCII character
	short nColorCode;	// Colour code
	bool isBold;		// Bolded font?
	bool bPassable;		// Can you walk on this tile?
};

struct ITEM_TYPE{
	char nCharacter;	// ASCII character code
	short nColorCode;	// Colour code
	char *p_szName;		// Name of the item
};

TILE_TYPE sTileIndex[] = {
	{'.', 7, false, true},    // 0 TILE_ROCKFLOOR
	{'#', 7, true, false},	  // 1 TILE_WALL
	{'+', 3, true, false},	  // 2 TILE_CLOSEDDOOR
	{'/', 3, false, true},	  // 3 TILE_OPENDOOR
};

ITEM_TYPE sItemIndex[] = {
	{' ', 7, "EMPTY"},			// 0 ITEM_EMPTY
	{'!', 6, "Potion"},			// 1 ITEM_POTION
	{'*', 7, "Rock"}			// 2 ITEM_ROCK
};

// tile types
const int TILE_ROCKFLOOR = 0;
const int TILE_WALL = 1;
const int TILE_CLOSEDDOOR = 2;
const int TILE_OPENDOOR = 3;
const int TILE_WATER = 4;

// item types
const int ITEM_EMPTY = 0;
const int ITEM_POTION = 1;
const int ITEM_ROCK = 2;

const int INVENTORY_SLOTS = sizeof(inventory) / sizeof(inventory[0]);

int main(void)
{
	// initialize ncurses
	Screen scr;
	scr.add("Press any key to continue.\n\nExcept 'q', that will quit the game.");
	int ch = getch();

	// initialize player character and position
	nPlayerX = MAP_WIDTH / 2; 
	nPlayerY = MAP_HEIGHT / 2; 
	int nDeltaX, nDeltaY;
	nDeltaX = nDeltaY = 0;

	char player = '@';

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	
	initItems();
	memset(inventory, ITEM_EMPTY, sizeof(inventory));
	
	// add stuff to item map
	items[5][7] = ITEM_ROCK;
	items[4][4] = ITEM_POTION;

	while(true){
		clear();
		drawMap();
		showInventory();
		attron(COLOR_PAIR(3));
			mvaddch(nPlayerY, nPlayerX, player);
		attroff(COLOR_PAIR(3));
		nDeltaY = nDeltaX = 0;
		ch = getch();
		switch(ch){
			// // upper-left
			// case 'r':
			// 	nDeltaX = -1;
			// 	nDeltaY = -1;
			// 	break;
			// // upper-right
			// case 'y':
			// 	nDeltaX = 1;
			// 	nDeltaY = -1;
			// 	break;
			// left
			case 'a':
				nDeltaX = -1;
				nDeltaY = 0;
				break;
			// down
			case 's':
				nDeltaX = 0;
				nDeltaY = 1;
				break;
			// up
			case 'w':
				nDeltaX = 0;
				nDeltaY = -1;
				break;
			// right
			case 'd':
				nDeltaX = 1;
				nDeltaY = 0;
				break;
			// // bottom-left
			// case 'v':
			// 	nDeltaX = -1;
			// 	nDeltaY = 1;
			// 	break;
			// // bottom-right
			// case 'n':
			// 	nDeltaX = 1;
			// 	nDeltaY = 1;
			// 	break;
			// open door
			case 'o':
				interactDoor(ch);
				break;
			// close door
			case 'c':
				interactDoor(ch);
				break;
			case 'q':
				return 0;
			case '1':
				getCommand();
				break;
			case '2':
				dropCommand();
				break;
			case '3':
				useCommand();
				break;
			default:
				break;
		}
		if(isPassable(nPlayerX + nDeltaX, nPlayerY + nDeltaY)){
			nPlayerX += nDeltaX;
			nPlayerY += nDeltaY;
		}
	}
	return 0;
}

void initItems(){
	for(int y = 0; y < MAP_HEIGHT; y++){
		for(int x = 0; x < MAP_WIDTH; x++){
			items[y][x] = ITEM_EMPTY;
		}
	}
}

void showInventory(){
	mvaddstr(1, MAP_WIDTH + 2, "INVENTORY");		
	mvaddstr(2, MAP_WIDTH + 2, "---------");

	for(int i = 0; i < INVENTORY_SLOTS; i++){
		int nType = inventory[i];
		mvaddch(3+i, MAP_WIDTH + 2, 'A'+i);
		mvaddch(3+i, MAP_WIDTH + 3, ':');
		mvprintw(3+i, MAP_WIDTH + 5, sItemIndex[nType].p_szName);
	}
}

void drawTile(int x, int y){
	int nColor;
	int nType;
	char nCharacter;
	bool isBold = false;
	if(items[y][x] != ITEM_EMPTY)
	{
		nType = items[y][x];
		nColor = sItemIndex[nType].nColorCode;
		nCharacter = sItemIndex[nType].nCharacter;
	}
	else
	{
		nType = map[y][x];
		nColor = sTileIndex[nType].nColorCode;
		nCharacter = sTileIndex[nType].nCharacter;
		isBold = sTileIndex[nType].isBold;
	}
	attron(COLOR_PAIR(nColor));
	if(isBold){attron(A_BOLD);}else{attroff(A_BOLD);}
	mvaddch(y, x, nCharacter);
	attroff(COLOR_PAIR(nColor));
}

void drawMap(){
	for(int y = 0; y < MAP_HEIGHT; y++){
		for(int x = 0; x < MAP_WIDTH; x++){
			drawTile(x, y);
		}
	}
}

bool isPassable(int nMapX, int nMapY){
	if(nMapX < 0 || nMapX >= MAP_WIDTH || nMapY < 0 || nMapY >= MAP_HEIGHT)
		return false;

	int nTileValue = map[nMapY][nMapX];
	return sTileIndex[nTileValue].bPassable;
}

void interactDoor(char action){
	mvprintw(MAP_HEIGHT + 1, 0, "What direction?");
	int ch = getch();
	int nDeltaX, nDeltaY;
	int nTileValue;

	switch(ch){
		// left
		case 'a':
			nDeltaX = -1;
			nDeltaY = 0;
			break;
		// down
		case 's':
			nDeltaX = 0;
			nDeltaY = 1;
			break;
		// up
		case 'w':
			nDeltaX = 0;
			nDeltaY = -1;
			break;
		// right
		case 'd':
			nDeltaX = 1;
			nDeltaY = 0;
			break;
		default:
			clear();
			return;
	}

	nTileValue = map[nPlayerY + nDeltaY][nPlayerX + nDeltaX];
	if(action == 'o' && nTileValue == TILE_CLOSEDDOOR){
		map[nPlayerY + nDeltaY][nPlayerX + nDeltaX] = TILE_OPENDOOR;
	}
	else if(action == 'o' && nTileValue == TILE_OPENDOOR)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "The door is already open!");
		getch();
	}
	
	if(action == 'c' && nTileValue == TILE_OPENDOOR){
		map[nPlayerY + nDeltaY][nPlayerX + nDeltaX] = TILE_CLOSEDDOOR;
	}
	else if(action == 'c' && nTileValue == TILE_CLOSEDDOOR)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "The door is already closed!");
		getch();
	}
}

void getCommand(){ 
	if(items[nPlayerY][nPlayerX] == ITEM_EMPTY){ 
		return;
	}

	for(int i = 0; i < INVENTORY_SLOTS; i++){
		if(inventory[i] == ITEM_EMPTY){
			inventory[i] = items[nPlayerY][nPlayerX];
			items[nPlayerY][nPlayerX] = ITEM_EMPTY;
			return;
		}
	}
	return;
}

void dropCommand(){
	mvprintw(MAP_HEIGHT + 1, 0, "Drop from which slot?");
	int ch = getch();
	
	int nSlot = (char)ch - 'A'; 

	if(nSlot < 0 || nSlot >= INVENTORY_SLOTS){
		mvprintw(MAP_HEIGHT + 3, 0, "Invalid inventory slot");	
		getch();
	}
	else if(inventory[nSlot] == ITEM_EMPTY)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "Nothing in this item slot");
		getch();
	}
	else if(items[nPlayerY][nPlayerX] != ITEM_EMPTY)
	{
		mvprintw(MAP_HEIGHT + 3, 0, "There's no room to drop this item");
		getch();
	}
	else
	{
		items[nPlayerY][nPlayerX] = inventory[nSlot];
		inventory[nSlot] = ITEM_EMPTY;
	}
}

void useCommand(){
	mvprintw(MAP_HEIGHT + 1, 0, "Use which item?");
	int ch = getch();

	int nSlot = (char)ch - 'A';

	if(nSlot < 0 || nSlot >= INVENTORY_SLOTS){
		mvprintw(MAP_HEIGHT + 3, 0, "Invalid inventory slot");
		getch();
	}

	switch(inventory[nSlot])
	{
		case ITEM_EMPTY:
			mvprintw(MAP_HEIGHT + 3, 0, "Nothing in this item slot to use");
			getch();
			break;
		case ITEM_POTION:
			mvprintw(MAP_HEIGHT + 3, 0, "You quaff the potion");
			inventory[nSlot] = ITEM_EMPTY;
			getch();
			break;
		default:
			mvprintw(MAP_HEIGHT + 3, 0, "Not sure how to use this item");
			getch();
			break;
	}
}

