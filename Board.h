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

		int white_piece = '2';
		int black_piece = '1';




		// double weights[size][size] =
	 //    {
	 //        {10 , -2 ,  2 , 0.5, 0.5,  2 , -2 , 10 },
	 //        {-2 , -4 ,-0.5,-0.5,-0.5,-0.5, -4 , -2 },
	 //        {2  ,-0.5, 1.5, 0.3, 0.3, 1.5,-0.5,  2 },
	 //        {0.5,-0.5, 0.3, 0.3, 0.3, 0.3,-0.5, 0.5},
	 //        {0.5,-0.5, 0.3, 0.3, 0.3, 0.3,-0.5, 0.5},
	 //        {2  ,-0.5, 1.5, 0.3, 0.3, 1.5,-0.5,  2 },
	 //        {-2 , -4 ,-0.5,-0.5,-0.5,-0.5, -4 , -2 },
	 //        {10 , -2 ,  2 , 0.5, 0.5,  2 , -2 , 10 }
	 //    };

		// double weights[size][size] =
	 //    {
	 //        {10 ,  1 ,  2 , 0.5, 0.5,  2 ,  1 , 10 },
	 //        {1  ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 },
	 //        {2  ,  0 , 1.5, 0.3, 0.3, 1.5,  0 ,  2 },
	 //        {0.5,  0 , 0.3, 0.3, 0.3, 0.3,  0 , 0.5},
	 //        {0.5,  0 , 0.3, 0.3, 0.3, 0.3,  0 , 0.5},
	 //        {2  ,  0 , 1.5, 0.3, 0.3, 1.5,  0 ,  2 },
	 //        {1  ,  0 ,  0 ,  0 ,  0 ,  0 ,  0 ,  1 },
	 //        {10 ,  1 ,  2 , 0.5, 0.5,  2 ,  1 , 10 }
	 //    };

	    double weights[size][size] =
	    {
	        {150 ,  0 , 12 ,  3 ,  3 ,  12 ,  0 , 150 },
	        {0  ,  0 , 0.5, 0.5, 0.5, 0.5,  0 ,  0 },
	        { 12 , 0.5,  6 , 1.8, 1.8,  6 , 0.5,  12 },
	        {  3 , 0.5, 1.8, 1.8, 1.8, 1.8, 0.5,  3 },
	        {  3 , 0.5, 1.8, 1.8, 1.8, 1.8, 0.5,  3 },
	        { 12 , 0.5,  6 , 1.8, 1.8,  6 , 0.5,  12 },
	        {0  ,  0 , 0.5, 0.5, 0.5, 0.5,  0 ,  0 },
	        {150 ,  0 , 12 ,  3 ,  3 ,  12 ,  0 , 150 }
	    };

	public:
		// int** board;


		Board();
		void resetBoard(char**& board);
		//places specified piece on the specified board at the specified position, and flips the opponent's pieces
		bool place_piece(char**& board, char piece, int col, int row);
		
		double getPossibleMovesCount(char**& board, char piece);
		vector<vector<int>> getPossibleMoveCoordinates(char**& board, char piece);
		vector<char**> getPossibleMoveBoards(char**& board, char piece);

		//callbacks list of pieces surrounding specified position
		void get_neighbors(char**& board, char* neighbors, int x, int y);
		int countPieces(char**& board, char piece);
		double getWeight(int col, int row);
		

		bool isEqual(char**& board, char**& other_board);
		vector<vector<int>> getDifferenceCoordinates(char**& board, char**& board2);
		char getPieceAtPosition(char**& board, int col, int row);
		void copyBoard(char**& board, char**& old_board);

		void printBoard(char**& board, int num_indents=0);

};



#endif


