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
		Board board_obj;

		int size = 8;
		
		//if true, player goes first. AI otherwise
		bool turn;

		//2
		char player_piece;
		//1
		char AI_piece;

		//AI #1
		Tree tree;

	public:
		Othello();

		double MIN = numeric_limits<double>::lowest();
		double MAX = numeric_limits<double>::max();


		vector<string> moves;
		

		//runs the game
		void run();
		//if it was player's turn, it's now AI's turn
		void changeTurn();
		//returns true if it's player's turn to move. false otherwise
		bool isPlayersTurn();
		//player's turn to move. returns true if successful
		bool playersMove(int player_type);
		//AI's turn to move. returns true if successful
		vector<int> AIMove(int AI_version, bool verbose=true);
		//returns 1 if player won, -1 if AI won, and 0 if tie
		int determineWinner();
		
		//converts c4 into (2,3)
		void convert_to_coordinates(string position, int * coordinates);
		//converts (2,3) into c4
		string convert_to_notation(int col, int row);
		
		//reset's game
		void resetGame(bool reset_tree=true);


};


#endif


