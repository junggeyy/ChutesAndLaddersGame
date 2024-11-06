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

#define _CRT_SECURE_NO_WARNINGS		
#include <stdio.h>		
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SIZE 99

// function prototypes
void printBoard(char* board, char* p1, char* p2, FILE* fp);	//output game board to file
char* move(char* current, char* other, int turnNum, char* board);	//handle player movements
char* findHaven(char* current, char* board);	//locate and move player to nearest Haven
char* chuteLadder(char* current);	//move player upward or downward 


int main() {
	char board[] = "  mHk nH l B He Flq p H  hByHlho H B  jr H B ir j H  F ku gd  H pjB mH x  BF i H  m oB HlHF hoH B  ";
	char* p1 = board;	
	char* p2 = board;	
	//seeding  the random number generator for die
	srand(time(NULL));

	FILE* fp = fopen("board.txt", "w");
	while (p1 < board + SIZE && p2 < board + SIZE) {
		p1 = move(p1, p2, 1, board);	
		p2 = move(p2, p1, 2, board);	
		printBoard(board, p1, p2, fp);
	}
	fclose(fp);
	printf("\nPlayer %d wins!!", (p1 > p2) ? 1 : 2);
	return 0;
}

void printBoard(char* board, char* p1, char* p2, FILE* fp) {
	char* boardPointer;
	for (boardPointer = board; boardPointer < board + SIZE; boardPointer++) {		
		if (boardPointer == p1) {		
			putc('1', fp);
		}
		else if (boardPointer == p2) {	
			putc('2', fp);
		}
		else {
			putc(*boardPointer, fp);
		}
	}
	putc('\n', fp);
}

char* move(char* current, char* other, int turnNum, char* board) {
	char roll;
	do {
		printf("\nPlayer %d, enter r to roll the die: ", turnNum);
		scanf(" %c", &roll);
		if ((roll != 'r') && (roll != 'R'))
		printf("Invalid input. Please press 'r' or 'R' to roll: ");
	} while ((roll != 'r') && (roll != 'R'));
	int die = (rand() % 6) + 1;
	current = current + die;
	printf("\nPlayer %d moves %d to %d", turnNum, die, (int)(current - board));
	if (current < board + SIZE) {
		if (current == other) {
			current = current - 1;
			printf(" collision, moving back to %d", (int)(current - board));
		}
		if (*current == 'B') {
			char* moved_position = findHaven(current, board);			
			if (moved_position == board) {
				printf(" and lands on a 'B', no haven-moving back to the beginning of the board at 0");
			}
			else {
				printf(" and lands on a 'B', moving back to the nearest haven at %ld", (int)(moved_position - board));
			}
			current = moved_position;

		}
		else if (*current == 'F') {
			char* moved_position = findHaven(current, board);	
			if (moved_position != current) {
				printf(" and lands on a 'F' moving forward to %d", (int)(moved_position - board));
				current = moved_position;
			}
			else {
				printf(" and land on a 'F', no haven found, remains at %d", (int)(current - board));
			}

		}
		else if (*current >= 'a' && *current <= 'm') {	
			current = chuteLadder(current);
			printf(" and lands on a chute, sliding down to %d", (int)(current - board));
		}
		else if (*current >= 'o' && *current <= 'z') {	
			current = chuteLadder(current);
			printf(" and lands on a ladder, climbing up to %d", (int)(current - board));
		}
		else {	
			printf("");
		}
		if (current == other) {
			current = current - 1;
			printf(" collision, moving back to %d", (int)(current - board));
		}
		return current;
	}
	else {
		return current;
	}
}
char* findHaven(char* current, char* board) {
	char* point = current;		
	if (*current == 'B') {
		while (*point != 'H' && point > board) {
			point--;
		}
		if (*point != 'H') {
			point = board;
		}
	}
	else if (*current == 'F') {
		while (*point != 'H' && point < board + SIZE) {
			point++;
		}
		if (*point != 'H') {
			return current;
		}
	}
	if (*point == 'H') {
		*point = '*';
	}
	return point;
}
char* chuteLadder(char* current) {
	char* location = current + (int)(*current - 'n');
	*current = '-';
	return location;
}
