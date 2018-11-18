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


//returns random int from start to end
inline extern int randNum(int start, int end)
{
	return rand() % (end-start) + start;
}


// #endif
