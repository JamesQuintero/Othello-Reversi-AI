#include <iostream>
#include <string>



#ifndef Board_H
#define Board_H

using namespace std;



class Board
{
	private:
		//side length of the Othello board
		static const int size = 8;
		//percentage of the "neuron" that gets increment if its path results in a good outcome
		// static constexpr double perc = 0.25;

		int white_piece = 2;
		int black_piece = 1;

	public:
		int** board;


		Board();
		void resetBoard();
		//places specified piece on the specified board at the specified position, and flips the opponent's pieces
		bool place_piece(int piece, int col, int row);
		//returns array of possible moves in string format
		vector<vector<int>> getPossibleMoveCoordinates(int piece);
		vector<Board> getPossibleMoveBoards(int piece);
		//callbacks list of pieces surrounding specified position
		void get_neighbors(int* neighbors, int x, int y);
		int countPieces(int piece);
		void printBoard();

		int getPieceAtPosition(int col, int row);
		void copyBoard(Board *old_board);

};



#endif


