/*
This program in an implementation of a variation of the game Chutes and Ladders in C and showcases the use 
of pointer and pointer arithmetics.
Here, we have a game board represented as a string, and all of the position of the string will represent 
either a blank space, a chute, ladder, a haven or two chars that represents a movement to the nearest haven.
This is a two-player game -the players board position will be represented by pointers to the string board and
the movement of the players will be done solely by using pointer arithmetics. 
The players alternate turns, with a randomly generated die number 1 to 6, and depending on where the player
lands, the position may change again as described:
- If a player lands on 'B', they are moved backward to the nearest 'Haven' space on the board.
- If a player lands on 'F', they advance to the nearest 'Haven' space forward on the board.
- If no Haven is found on 'B', player moves back to start, if no haven found on 'F', player stays where it is.
- If a player lands on a chute (a lowercase char from 'a' to 'm'), they slide back by a number of spaces.
- If a player lands on a ladder (a lowercase char from 'o' to 'z'), they climb forward by a number of spaces.
- If a player lands on the other player, it is a collision, and the player backs up 1 space.
The first player to reach or exceed the final position on the board wins the game, and the winner is declared!
The program uses various functions which deals with these different cases and updates the player and board 
acoordingly, and after each move the gameboard is output to a file.
*/

#define _CRT_SECURE_NO_WARNINGS		//to handle errors
// C libraries
#include <stdio.h>		
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 99	//constant for the size of board

// function prototypes
void printBoard(char* board, char* p1, char* p2, FILE* fp);	//output game board to file
char* move(char* current, char* other, int turnNum, char* board);	//handle player movements
char* findHaven(char* current, char* board);	//locate and move player to nearest Haven
char* chuteLadder(char* current);	//move player upward or downward 

// In the main function, we initialize the board, declare and initialize the two players to have them start
// at the beginning of the board. We seed the random number generator and open file pointer for our output file.
// We start a while loop with a condition that it won't stop until neither player has gone beyond the board,
// meaning none has won. Then, in the loop, we start p1's move by calling the move() function, then start p2's move
// then we output the board to our file by printBoard() function. After a player reaches end of the board, the loop 
// exits and we close the file. Then, we check for who won the game, by looking for the higher location and output
// the winner.
int main() {
	char board[] = "  mHk nH l B He Flq p H  hByHlho H B  jr H B ir j H  F ku gd  H pjB mH x  BF i H  m oB HlHF hoH B  ";
	char* p1 = board;	//pointer to starting board position of player1
	char* p2 = board;	//pointer to starting board position of player2
	//seeding  the random number generator
	srand(time(NULL));

	FILE* fp = fopen("board.txt", "w");
	while (p1 < board + SIZE && p2 < board + SIZE) {
		p1 = move(p1, p2, 1, board);	//player 1's turn
		p2 = move(p2, p1, 2, board);	//player 2's turn
		printBoard(board, p1, p2, fp);
	}
	fclose(fp);
	printf("\nPlayer %d wins!!", (p1 > p2) ? 1 : 2);
	return 0;
}

// This function receives the pointer to both players location, the board, and output file pointer.
// This function updates and prints the board to our output file after the players move. Here, we have
// a board pointer to iterate each char over the board till end, and for each char, we check for the
// position of both players and update the char if we match the player's location, else we just let the
// char as it is. Then, we put a new line at the end so we each time we have the board on a seperate line.
void printBoard(char* board, char* p1, char* p2, FILE* fp) {
	char* boardPointer;
	for (boardPointer = board; boardPointer < board + SIZE; boardPointer++) {		//iterate each char till end of board
		if (boardPointer == p1) {		//if pointer matches location of p1
			putc('1', fp);
		}
		else if (boardPointer == p2) {	//if pointer matches location of p1
			putc('2', fp);
		}
		else {
			putc(*boardPointer, fp);
		}
	}
	putc('\n', fp);
}
// This function receives both players location pointer, the turn players number and the board, and this is the 
// primary function that handles all the movement of the players. Here, we use rand to generate a random number between 
// 1 to 6, acting as dice, and we add that number to the player's location so the player moves. As we are using a 
// pointer to the player's location, when we add the dice number to it, the player's location is moved by that number.
// Then, after the move, we check if the player is still within the board, if it is, we check for where it landed on the
// board. First, we check if the player landed on the other player's position, if it did then it is a collision and we move
// the player one step back. Then, we check if the player landed on any speical character on the board like B/F/a-m/o-z.
// We check for each for those character in the player's position, and if it landed on any of it, we call the appropiate
// function. If it landed on B/F, we call the findHaven() function which finds a Haven and moves the player to that Haven,
// and returns the new position. If it landed on any of the lower-case alphabet, we call the chuteLadder() function,
// which moves the player forward or backward and returns the new location. If the player didn't land on any of the
// special char, nothing happens, the player remains at the current location. After the special spaces are checked,
// we again check if the new location of the player collided with the other player and if it did, we move the player
// one step back. After all these conditions, we return the players updated location. If the player was already out of 
// board after the die move, we simply return the player's location without having to check for any special chars.
// Parameters: current represents the player's position whose turn it is to move, and other represent the position of the 
// other player, tNum is the player's number 1 or 2 whosevers turn it is and we have the board
char* move(char* current, char* other, int turnNum, char* board) {
	//getting a random number from 1 to 6 like a rolling die
	int die = (rand() % 6) + 1;

	//moving current player by the die number
	current = current + die;
	printf("\nPlayer %d moves %d to %d", turnNum, die, (int)(current - board));

	//checking if the current player's position is within the board after the move
	if (current < board + SIZE) {
		//if the position of both players are same, we move the current player one step back
		if (current == other) {
			current = current - 1;
			printf(" collision, moving back to %d", (int)(current - board));
		}
		//the following condition check if player landed on B or F and calls findHaven,
		//and the player's position is updated from the returned position value
		if (*current == 'B') {
			char* moved_position = findHaven(current, board);	//assigning the new moved player position temporarily
			//if we find a H, then the player is moved, new position is updated, else player goes back at start position
			
			if (moved_position == board) {
				// Player is moved to the start of the board
				printf(" and lands on a 'B', no haven-moving back to the beginning of the board at 0");
			}
			else {
				printf(" and lands on a 'B', moving back to the nearest haven at %ld", (int)(moved_position - board));
			}
			current = moved_position;

		}
		else if (*current == 'F') {
			char* moved_position = findHaven(current, board);	//assigning the new moved player position temporarily
			//if we find a H, then the player is moved, and new position is updated, // else player stays at the same position
			if (moved_position != current) {
				printf(" and lands on a 'F' moving forward to %d", (int)(moved_position - board));
				current = moved_position;
			}
			else {
				printf(" and land on a 'F', no haven found, remains at %d", (int)(current - board));
			}

		}
		//the following two condtions check if the player landed on a lower case a to m, chute, or a lower case 
		// o to z, ladder, and the player's position is reassigned the value returned from the function
		else if (*current >= 'a' && *current <= 'm') {	//check and update if player landed on chute
			current = chuteLadder(current);
			printf(" and lands on a chute, sliding down to %d", (int)(current - board));
		}
		else if (*current >= 'o' && *current <= 'z') {	//check and update if player landed on ladder
			current = chuteLadder(current);
			printf(" and lands on a ladder, climbing up to %d", (int)(current - board));
		}
		else {	//if player doesn't land on any special char
			printf("");
		}
		//rechecking if any collision after B/F/chute/ladder move
		if (current == other) {
			current = current - 1;
			printf(" collision, moving back to %d", (int)(current - board));
		}
		return current;
	}
	//if the player is out of board, we return the player's position directly
	else {
		return current;
	}
}
// This function receives the pointer to a player's current location and board if the player landed
// on a B/F. Here, we declare a pointer to traverse through each character in the board forward or 
// backward from its current location depending on its landing on B (backward) or F (forward). On both
// cases, we use a WHILE loop to traverse the board until we reach the start/end of the board or we find
// a H (haven). When the player lands on B, we search back till the start of board, and vice-versa when 
// on F. While searching, if any conditions met, the loop exits then we again check if we found a H, if we 
// do then we replace the H with '*' and return that location, if we do not find a H, we simply return the 
// players current location in F, and in B we move the player back to begining of the board.
char* findHaven(char* current, char* board) {
	char* point = current;		// pointer to traverse the board
	if (*current == 'B') {
		//search the board backwards till we find a H, or we reach the start of the board. 
		while (*point != 'H' && point > board) {
			point--;
		}
		//if no 'H' found, move player back to location 0
		if (*point != 'H') {
			point = board;
		}
	}
	else if (*current == 'F') {
		//search the board forward till we find a H, or we reach the end of the board.
		while (*point != 'H' && point < board + SIZE) {
			point++;
		}
		//if no 'H' found, player stays at current location
		if (*point != 'H') {
			return current;
		}
	}
	// if we find a H, a haven, we change the H to * in the board 
	if (*point == 'H') {
		*point = '*';
	}
	return point;
}
// This function receives the pointer to a player's current location if it landed on a lowercase a-m, which 
// represents chute, or o-z, which represents ladder. We use a temorary location pointer to store the
// new location of the player after climbing a ladder or sliding a chute, and then use it to return the
// new location as well. The new location is computated by subtracting the lowercase letter with ASCII value  
// of n which gives a value then it is added to the current location so the player moves up or down accordingly.
// We also remove the used chute/ladder and replace it with '-' so it won't be used again.
char* chuteLadder(char* current) {
	//computing the new location of player by storing in a temporary pointer 
	char* location = current + (int)(*current - 'n');
	//setting the used chute/ladder to '-'
	*current = '-';
	//return the changed location of player from using chute/ladder back to move()
	return location;
}