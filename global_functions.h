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
inline extern void createMatrix(char**& matrix, int size)
{
	for(int x =0; x <size; x++)
	{
		matrix[x] = new char[size];
		
		//places 0
		for(int y =0; y < size; y++)
			matrix[x][y] = '0';
	}

}

//returns random int from start to end
inline extern int randNum(int start, int end)
{
	int max = end-start;
	return rand() % max + start;
}


// #endif
