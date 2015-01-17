****************************************************
Yash Shah	0876928
Cis 2500	A1-Obstruction Game
Jan 16/15
****************************************************
************
Compilation
************
Two ways to compile it.
1)
-You can go to the src folder and type,
gcc a1.c -Wall -std=c99 -pedantic -lncurses
-To run the file, type 
./a.out
2) You can type "make" to compile it
and then you can type bin/runMe to run the program
***********************
Running the program(s)
***********************
- To play the game, use "i,j,k,l" to move, e to select position and type q to quit the game
*****************
Known Limitations
*****************
- One limitation is that it can only take maximum of 49 characters for a persons name
- another limitation is I havent done error testing meaning if user pressses anything
except 1, they can play again
- another is that the scores file cant read in multiple entrees


*****************
Testing Done
*****************
- I checked input errors such as checking if the user goes out of bounds.
- I checked if user presses any other keys during game play

*****************
Bonuses
*****************
- I created an AI that is not smart. It moves in the first open spot. 
- I have reading from and writing to a file working meaning if two players play the game
couple times and use different names to play, it wont show up. Game keeps score if 
user plays again
- I gave hints so if the user goes out of bounds or selects position 
that is already taken, it warns them
- I added color to my board and also added Red Color to my hints so it stands out
to user and so they can see it
- Instructions to play the game are given below the board




