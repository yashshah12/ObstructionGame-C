#include <ncurses.h>

//Define statements for player movement
#define MOVE_UP 'i'
#define MOVE_RIGHT 'l'
#define MOVE_DOWN 'k'
#define MOVE_LEFT 'j'

//Function Prototypes
void playGame();
void drawBoard();
int canweMove(int *gridData,int currentSpace);
void neighbors(int y,int x, int*gridData,int currentSpace);
int didWin(int *gridData, char *currentName);
void printMessage(char *str, int y, int x);
void clearMessage(int y, int x);
void printRules();
void clearBoard();
