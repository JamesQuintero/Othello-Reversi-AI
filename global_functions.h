#include <iostream>

// #ifndef GLOBAL_FUNCTIONS_H
// #define GLOBAL_FUNCTIONS_H

using namespace std;

//prints the contents of a 2D array into an Othello board pattern
//    A  B  C  D  E  F  G  H
// 1 [ ][ ][ ][ ][ ][ ][ ][ ]
// 2 [ ][ ][ ][ ][ ][ ][ ][ ]
// 3 [ ][ ][ ][ ][ ][ ][ ][ ]
// 4 [ ][ ][ ][W][B][ ][ ][ ]
// 5 [ ][ ][ ][B][W][ ][ ][ ]
// 6 [ ][ ][ ][ ][ ][ ][ ][ ]
// 7 [ ][ ][ ][ ][ ][ ][ ][ ]
// 8 [ ][ ][ ][ ][ ][ ][ ][ ]

//creates a size x size 2D array
inline extern char** copyBoard(char** board, int size)
{
	char** new_board = new char*[size];
	for(int x =0; x <size; x++)
	{
		char* col = new char[size];

		//places 0
		for(int y =0; y < size; y++)
			col[y] = board[x][y];

		new_board[x] = col;
	}

	return new_board;
}

//creates a size x size 2D array
inline extern char** createMatrix(int size)
{
	char** rows = new char*[size];
	for(int x =0; x <size; x++)
	{
		char* col = new char[size];

		//places 0
		for(int y =0; y < size; y++)
			col[y] = '0';

		rows[x] = col;
	}

	return rows;
}

//returns random int from start to end
inline extern int randNum(int start, int end)
{
	int max = end-start;
	return rand() % max + start;
}


// #endif
