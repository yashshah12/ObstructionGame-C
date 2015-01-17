#include "a1.h"
#include <string.h>
#include <stdlib.h>

int main() {
	initscr();
	cbreak();

	playGame();

	refresh();
	endwin();

	return 0;
}

/*
The most important function of the whole game
It calls all other functions from here
This is where the whole gameplay happens
IN:Nothing
OUT:Nothing
*/
void playGame() {
	//FILEIO Variables
	char stringName1[50];
	char stringName2[50]; 
	int playerScore1 = 0;
	int playerScore2 = 0;
	FILE* file;
	//A variable that will read an int from file that will determine if its the first game or not
	int secondGame =0;

	//Grid Info
	int gridData[36] = {0};
	int currentSpace = 1;
	int ableMove = 0;
	int winGame = 0;
	//Coordinates
	int xCoord = 2;
	int yCoord = 1;

	int i = 0;
	//If user wants to play again
	int playingAgain;
	//Keyboard Listener
	int inputCh;
	//Player Variables
	char player1Name[50];
	char player2Name[50];
	char playerAI[50];
	char currentName[50];
	int currentPlayer = 1;
	char currentChar = 'O';
	//Messages
	char* strMessage = "Good Luck";
	//Find out if user ants to play with two players or not
	int gameType = 0;


	mvwprintw(stdscr,20,0,"Do you want to play with 1 player(1) or two players(2)");
	gameType = getch();
	clearMessage(20,0);

	//If the player choose to play with 1 player
	if(gameType==49) {
		strcpy(playerAI,"AI");
		mvwprintw(stdscr,20,0,"Player 1 Name?");
	    getstr(player1Name);
	    noecho();
	    strcpy(player2Name,playerAI);
        clearMessage(20,0);
	} else if (gameType ==50) {
		//Getting players Names
		mvwprintw(stdscr,20,0,"Player 1 Name?");
		getstr(player1Name);
		clearMessage(20,0);    
		mvwprintw(stdscr,20,0,"Player 2 Name?");
		getstr(player2Name);
		clearMessage(20,0);
		noecho();
	}
	/*Reading the file and determining if the current user have played before. I read 5 variables. 
	1st varable - int that determines if the user is playing the game for first time
	2nd - 5th variable are the users name and scores
	*/
	file = fopen("scores.txt","r");
	fscanf(file,"%d",&secondGame);
    if (secondGame==1) {
		fscanf(file,"%s",stringName1);
		fscanf(file,"%d",&playerScore1);
		fscanf(file,"%s",stringName2);
		fscanf(file,"%d",&playerScore2);
		fclose(file);
	    //Compare the current names to names in the file, if they same, show scores
		if (strcmp(stringName1,player1Name)==0 && strcmp(stringName2,player2Name)==0) {
			mvwprintw(stdscr,0,33,"Scores");
			mvwprintw(stdscr,1,33,"------");
			mvwprintw(stdscr,3,33,"%s: %d",player1Name,playerScore1);
			mvwprintw(stdscr,4,33,"%s: %d",player2Name,playerScore2);
		} else {
			playerScore1 = 0;
			playerScore2 = 0;
		}
	}
	

	//drawing the board
	drawBoard();
	printRules();
	mvwprintw(stdscr,21,0,"Player 1: %s",player1Name);
	mvwprintw(stdscr,22,0,"Player 2: %s",player2Name);
	
	refresh();

    //LOOP that keeps playing the game until user quits
    do {
	    
	   
	    //Determining who is moving now
	    if (currentPlayer==3) {
			currentPlayer = 1;
			currentChar = 'O';
		}
		if (currentPlayer==1) {
			strcpy(currentName,player1Name);

		} else if (currentPlayer==2) {
			strcpy(currentName,player2Name);
			
		}

		//Clears players name as player changes
		clearMessage(18,0);
		mvwprintw(stdscr,18,0,"%s's turn (%c):",currentName,currentChar);
		move(yCoord,xCoord);
		//Only let players from game type 2 play or player 1 from game type 1
		if (gameType ==50 || currentPlayer==1) {
			//currentPlayer =1;
			//User Input
			inputCh = getch();
			/*
			Check what key they pressed and also check if they go out of bounds by clickingthat key
			As the user moves, keep track of what grid space they are in currently(currentSpace) and keep track of position
			*/
			if (inputCh==MOVE_RIGHT) {
				if(xCoord+4<=22) {
					currentSpace+=1;
					xCoord+=4;
				} else {
					strMessage = "Cant move there!";
				}
			} else if (inputCh==MOVE_LEFT) {
				if (xCoord-4>=2) {
					currentSpace-=1;
					xCoord-=4;
				} else {
					strMessage = "Cant move there!";
				}

			} else if (inputCh==MOVE_UP) {
				if (yCoord-2>=1) {
					currentSpace-=6;
					yCoord-=2;
				} else {
					strMessage = "Cant move there!";
				}

			} else if (inputCh==MOVE_DOWN) {
				if (yCoord+2<=11) {
					currentSpace+=6;
					yCoord+=2;
				} else {
					strMessage = "Cant move there!";
				}

			} else if (inputCh=='e') {
			    //check if player can move at the current spot.Returns 1 if its safe
				ableMove = canweMove(gridData,currentSpace);
				if (ableMove==1) {
					//add the current spot as well as neighboring spot and update the gridData array to tell that current position is filled
					mvaddch(yCoord,xCoord,currentChar);
					gridData[currentSpace-1]=1;
					//fill the neighboring spaces up too, and update grid data
					neighbors(yCoord,xCoord,gridData,currentSpace);
					strMessage = "Keep playing";
				} else {
					strMessage = "Cant move there!";
				}

				// //check if someone won, returns 1 for true
				winGame = didWin(gridData, currentName);
				if (winGame!=1) {
					currentPlayer++;
					currentChar = 'X';
				} else if (winGame==1) {
					if (currentPlayer==1) {
						playerScore1++;
					} else {
						playerScore2++;
					}
					
				}
			}
		}

	 //AI Movement
	 if (gameType==49 && currentPlayer ==2) {
	 	strcpy(currentName,"AI");
		//Clears players name as player changes
		clearMessage(18,0);
		mvwprintw(stdscr,18,0,"%s's turn (%c):",currentName,currentChar);
		move(yCoord,xCoord);
	 	//The AI will start at top left position
		yCoord = 1;
		xCoord = 2;
		ableMove =0;
		currentSpace = 1;
		//Check the array data, and find the first available open space and put AI's position there
        for (i = 0;i<36;i++) {
        	ableMove = canweMove(gridData,currentSpace);
	        if (ableMove==1) {
	        	mvaddch(yCoord,xCoord,'X');
				gridData[currentSpace-1]=1;
				//fill the neighboring spaces up too, and update grid data
				neighbors(yCoord,xCoord,gridData,currentSpace);
				winGame = didWin(gridData, currentName);
				if (winGame==1) {
					playerScore2++;
				}
				break;
	        }
			/*
			As you moving through the array and you dont find an open space, change the x and 
			width of the current position to position of next space
	        */
	        xCoord+=4;
	        currentSpace++;
	        if (xCoord >22) {
	        	xCoord=2;
	        	yCoord+=2;
	        }        
    	}

		currentPlayer++;
		i = 0;
		ableMove = 0;
		refresh();

	
	
	}
	if (winGame==1) {
		
		mvwprintw(stdscr,7,30,"Play Again: Yes(1) No(any key)");
		playingAgain = getch();
		
		//If They want to play again
		if (playingAgain == 49) {
			file = fopen("scores.txt","w");
			fprintf(file,"1 \n");
			fprintf(file, "%s %d \n",player1Name,playerScore1);
			fprintf(file, "%s %d",player2Name,playerScore2);
			fclose(file);
		
			//If player wnats to play again, clear everything
			clearBoard();
			clearMessage(15,0);
			clearMessage(18,0);
			clearMessage(7,30);
			///clearing the array that holds info about if a character is stored at a positon  or not
			for (i=0;i<36;i++) {
				gridData[i] = 0;
			}
			currentSpace = 1;
			ableMove = 0;
			winGame = 0;
			yCoord = 1;
			xCoord = 2;
			currentPlayer = 1;
			currentChar = 'O';
			strMessage = "Good Luck";
			drawBoard();

			mvwprintw(stdscr,0,33,"Scores");
			mvwprintw(stdscr,1,33,"------");
			mvwprintw(stdscr,3,33,"%s: %d",player1Name,playerScore1);
			mvwprintw(stdscr,4,33,"%s: %d",player2Name,playerScore2);
					
		} else if (playingAgain==50) {
			inputCh = 'q';
		}

		
	}
			
		//Ading color  for hint messages
		start_color();
		init_pair(2,COLOR_RED,COLOR_BLACK);
		attron(COLOR_PAIR(2));
		clearMessage(34,50);
		printMessage(strMessage,34,50);

		attroff(COLOR_PAIR(2));
		move(yCoord,xCoord);
		refresh();

	} while (inputCh!='q');
}


/*
This function draws the board out
In: Nothing, 
Out Nothing, Will just print stuff
*/
void drawBoard() {
	int xCoord,yCoord;
	start_color();
	init_pair(1,COLOR_BLUE,COLOR_BLACK);
	attron(COLOR_PAIR(1));
	/*
	I make the height of each squre 2
	and width 4
	*/
	for (yCoord=1;yCoord<=11;yCoord+=2) { 
		for (xCoord=0;xCoord<=24;xCoord+=4) {
			mvaddch(yCoord,xCoord,'|');
		}
	}
	for (yCoord=0;yCoord<=12;yCoord+=2) {
		for (xCoord=0;xCoord<=24;xCoord++) {
			mvaddch(yCoord,xCoord,'-');
		}
	}
	attroff(COLOR_PAIR(1));
	move(1,2);
	refresh();

}

/*
Function that determines if user is allowed to place a character at their current spot
IN: the grid data array and their current grid space
OUT: returs an interger - 1 if its safe to put a character there, 0 if its not safe
*/
int canweMove(int *gridData, int currentSpace) {
	int i = 0;
	for (i = 0;i<36;i++) {
		//Checks if there is an empty space at your positon
		if (gridData[currentSpace-1]==0) {
			// gridData[currentSpace-1]=1;
			return 1;
		} else {
			return 0;
		}
	}
	return 0; 
}

/*
Function that fills neigboring squares and updates grid data so tha no one can move in those spaces
IN: the x and y position, the grid data array and the current grid space the user is on
OUT: nothing
*/
void neighbors(int yCoord, int xCoord, int *gridData, int currentSpace) {
	
	//Color for messages
	start_color();
	init_pair(3,COLOR_RED,COLOR_RED);
	attron(COLOR_PAIR(3));
	
	/*
	Check for above space
	So any space above current space, is 6 spaces less so check
	that index which is how many spaces -1. you check if that space
	is empty(!=1) and you also check if its not out of bounds. 
	If those statments are true, then you can put a neighboring space and update the grid data
	This is same way I did for all the other neighboring space
	*/
	if (gridData[currentSpace-6-1]!=1 && (yCoord-2)>=1) {
			mvaddch(yCoord-2,xCoord,'!');
			gridData[currentSpace-6-1]=1;
	}
	//Check for below space. Below space would be 6 spaces more than current space
	if (gridData[currentSpace+6-1]!=1 && (yCoord+2)<=11) {
			mvaddch(yCoord+2,xCoord,'!');
			gridData[currentSpace+6-1]=1;
	}
	//Check for left space. Left space would be 1 space less than current space
	if (gridData[currentSpace-1-1]!=1 && (xCoord-4)>0) {
			mvaddch(yCoord,xCoord-4,'!');
			gridData[currentSpace-1-1]=1;
	}
	//Check for right space. RIght space would be 1 space more than current space
	if (gridData[currentSpace+1-1]!=1 && (xCoord+4)<24) {
			mvaddch(yCoord,xCoord+4,'!');
			gridData[currentSpace+1-1]=1;
	}
	//Check for topleft neighboring space. topleft space would be 7 spaces less than current space
	if (gridData[currentSpace-7-1]!=1 && (yCoord-2)>=1 && (xCoord-4)>0) {
			mvaddch(yCoord-2,xCoord-4,'!');
			gridData[currentSpace-7-1]=1;
	}
	//Check for topright neighboring space. topright space would be 5 spaces less than current space
	if (gridData[currentSpace-5-1]!=1 && (yCoord-2)>=1 && (xCoord+4)<24) {
			mvaddch(yCoord-2,xCoord+4,'!');
			gridData[currentSpace-5-1]=1;
	}
	//Check for bottomleft neighboring space. bottmleft space would be 5 spaces more  than current space
	if (gridData[currentSpace+5-1]!=1 && (yCoord+2)<=11 && (xCoord-4)>0) {
			mvaddch(yCoord+2,xCoord-4,'!');
			gridData[currentSpace+5-1]=1;
	}
	//Check for bottomright neighboring space. bottomright space would be 7 spaces more than current space
	if (gridData[currentSpace+7-1]!=1 && (yCoord+2)<=11 && (xCoord+4)<24) {
			mvaddch(yCoord+2,xCoord+4,'!');
			gridData[currentSpace+7-1]=1;
	}
	attroff(COLOR_PAIR(3));
	refresh();
}

/*
Function that determines if someone has won
IN: the grid data array
OUT: returs an interger - 1 if someone won, 0 if no one has won yet
*/
int didWin(int *gridData, char *currentName) {
	int i = 0;
	int winGame = 1;
	//Going through the loop to check if all spaces have been filled up
	for (i=0;i<36;i++) {
		//check if there is an empty space
		if (gridData[i]==0) {
			winGame = 0;
			return 0;
			// return 0;//means there is a space left
		}
	}
	if (winGame == 1) {
		mvwprintw(stdscr,15,0,"Congratulations, %s Won!!!",currentName);
	}
	
	return 1;//If there is no empty space, then someone wins
}

/*
Source - Professor Judi's findTheM example with modifications of my own
Prints hint messages
IN: the x position, and y position and the string to be printed
OUT: nothing
*/
void printMessage(char *str, int yCoord, int xCoord) {
	clearMessage(yCoord,xCoord);
	mvwprintw(stdscr,yCoord,xCoord,str);
	refresh();
}

/*
Source - Professor Judi's findTheM example with modifications of my own
Clears messages
IN: the x position, and y position
OUT: nothing
*/
void clearMessage(int yCoord, int xCoord) {
	mvwprintw(stdscr,yCoord,xCoord,"                                                                                                                                                                                                                      ");
	refresh();
}

/*
Just prints out instructions for the game
IN:Nothing
OUT:Nothing
*/
void printRules() {

	mvwprintw(stdscr,30,0,"Rules");
	mvwprintw(stdscr,31,0,"-----");
	mvwprintw(stdscr,32,0,"Arrow Keys to move");
	mvwprintw(stdscr,33,0,"e to select position");
	mvwprintw(stdscr,34,0,"q to quit");

	mvwprintw(stdscr,32,50,"Hints");
	mvwprintw(stdscr,33,50,"-----");
	refresh();

}

/*
This function clears the board out if the user wants to play again
In: Nothing, 
Out Nothing
*/
void clearBoard() {
	int xCoord,yCoord;
	
	for (yCoord=1;yCoord<=12;yCoord++) {
		for (xCoord=0;xCoord<=24;xCoord++) {
			mvwprintw(stdscr,yCoord,xCoord," ");
		}
	}
	
	
	refresh();

}
