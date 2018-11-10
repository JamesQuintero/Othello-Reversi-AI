#include <iostream>
#include <string>
#include <vector>
#include "Board.h"
#include "Tree.h"




#ifndef OTHELLO_H
#define OTHELLO_H

using namespace std;



class Othello
{
	private:
		// int** board;
		Board board_obj;

		int size = 8;
		
		//if true, player goes first. AI otherwise
		bool turn;

		//0, or O
		int player_piece;
		//1, or X
		int AI_piece;

		//AI #1
		Tree tree;

	public:
		Othello();

		int MIN = numeric_limits<int>::min();
		int MAX = numeric_limits<int>::max();
		

		//runs the game
		void run();
		//if it was player's turn, it's now AI's turn
		void changeTurn();
		//returns true if it's player's turn to move. false otherwise
		bool isPlayersTurn();
		//player's turn to move. returns true if successful
		bool playersMove();
		//AI's turn to move. returns true if successful
		bool AIMove(int AI_version);
		//returns 1 if player won, -1 if AI won, and 0 if tie
		int determineWinner();
		
		//converts c4 into (2,3)
		void convert_to_coordinates(string position, int * coordinates);
		//converts (2,3) into c4
		string convert_to_notation(int col, int row);
		
		//reset's game
		void resetGame();

		// int count_flips(vector<int> col_list, int piece, int other_piece);
		// vector<int> get_column(int** board, int col);
		// vector<int> get_row(int** board, int row);
		// vector<int> get_up_diagonal(int** board, int col, int row);
		// vector<int> get_down_diagonal(int** board, int col, int row);

};


#endif


