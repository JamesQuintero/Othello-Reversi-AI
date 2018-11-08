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
inline extern int** copyBoard(int** board, int size)
{
	int** new_board = new int*[size];
	for(int x =0; x <size; x++)
	{
		int* col = new int[size];

		//places 0
		for(int y =0; y < size; y++)
			col[y] = board[x][y];

		new_board[x] = col;
	}

	return new_board;
}

//creates a size x size 2D array
inline extern int** createMatrix(int size)
{
	int** rows = new int*[size];
	for(int x =0; x <size; x++)
	{
		int* col = new int[size];

		//places 0
		for(int y =0; y < size; y++)
			col[y]=0;

		rows[x] = col;
	}

	return rows;
}

//returns matrix version of the board if provided with string version
inline extern int** matrixBoard(string board, int side_size)
{
	int** rows = createMatrix(side_size);

	int row=0;
	int col=0;
	for(int x =0; x < board.length(); x++)
	{
		int item = std::stoi(board.substr(x,1));
		rows[row][col] = item;

		col++;

		//goes to next row
		if(col == side_size)
		{
			col = 0;
			row++;
		}
	}


	return rows;
}


//returns string version of the board if provided with matrix version
inline extern string stringBoard(int** board, int side_size)
{
	string new_board="";

	int row=0;
	int col=0;
	for(int x =0; x < side_size; x++)
	{
		for(int y =0; y < side_size; y++)
			new_board += to_string(board[x][y]);
	}

	return new_board;
}

//returns random int from start to end
inline extern int randNum(int start, int end)
{
	int max = end-start;
	return rand() % max + start;
}


// #endif
