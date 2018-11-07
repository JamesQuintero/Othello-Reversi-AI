#include <iostream>
#include <string>
#include "Tree.h"



#ifndef OTHELLO_H
#define OTHELLO_H

using namespace std;



class Othello
{
	private:
		int** board;
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
		//returns true if player won the game
		bool playerWon();
		//returns true if AI won the game
		bool AIWon();
		bool place_piece(int piece, string position);
		void convert_to_coordinates(string position, int * coordinates);
		//returns array of possible moves in string format
		void possibleMoves(int** moves);
		//reset's game
		void resetGame();

};



#endif


