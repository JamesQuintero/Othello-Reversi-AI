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

		//Weights for first 0-20 levels or so
	    double weights[3][size][size] =
	    {{
	        {150 ,  0 , 12 ,  3 ,  3 ,  12 ,  0 , 150 },
	        {0  ,  0 , 0.5, 0.5, 0.5, 0.5,  0 ,  0 },
	        { 12 , 0.5,  6 , 1.8, 1.8,  6 , 0.5,  12 },
	        {  3 , 0.5, 1.8, 1.8, 1.8, 1.8, 0.5,  3 },
	        {  3 , 0.5, 1.8, 1.8, 1.8, 1.8, 0.5,  3 },
	        { 12 , 0.5,  6 , 1.8, 1.8,  6 , 0.5,  12 },
	        {0  ,  0 , 0.5, 0.5, 0.5, 0.5,  0 ,  0 },
	        {150 ,  0 , 12 ,  3 ,  3 ,  12 ,  0 , 150 }
	    }, 
	    //Weights for levels 20-40 or so
	    {
	        {  75,    0,    6,  1.5,  1.5,    6,    0,  75},
	        {   0,    0, 0.25, 0.25, 0.25, 0.25,    0,   0},
	        {   6, 0.25,    3,  0.9,  0.9,    3, 0.25,   6},
	        { 1.5, 0.25,  0.9,  0.9,  0.9,  0.9, 0.25, 1.5},
	        { 1.5, 0.25,  0.9,  0.9,  0.9,  0.9, 0.25, 1.5},
	        {   6, 0.25,    3,  0.9,  0.9,    3, 0.25,   6},
	        {   0,    0, 0.25, 0.25, 0.25, 0.25,    0,   0},
	        {  75,    0,    6,  1.5,  1.5,    6,    0,  75}
	    },
	    //weights for levels 40+ or so
	    {
	        {  30,    0,    3,  1.0,  1.0,    3,    0,  30},
	        {   0,    0, 0.15, 0.15, 0.15, 0.15,    0,   0},
	        {   3, 0.15,  1.5,  0.4,  0.4,  1.5, 0.15,   3},
	        { 1.0, 0.15,  0.4,  0.4,  0.4,  0.4, 0.15, 1.0},
	        { 1.0, 0.15,  0.4,  0.4,  0.4,  0.4, 0.15, 1.0},
	        {   3, 0.15,  1.5,  0.4,  0.4,  1.5, 0.15,   3},
	        {   0,    0, 0.15, 0.15, 0.15, 0.15,    0,   0},
	        {  30,    0,    3,  1.0,  1.0,    3,    0,  30}
	    }};

		

	public:
		// int** board;


		Board();
		void resetBoard(char (&board)[size+1][size+1]);
		//places specified piece on the specified board at the specified position, and flips the opponent's pieces
		bool place_piece(char (&board)[size+1][size+1], char piece, int col, int row);
		int countFlips(char (&board)[size+1][size+1], char piece, int col, int row);
		
		vector<double> getMobility(char (&board)[size+1][size+1], char piece);
		double getPieceStabilityScore(char (&board)[size+1][size+1], char piece);
		vector<vector<int>> getPossibleMoveCoordinates(char (&board)[size+1][size+1], char piece);
		// void getPossibleMoveBoards(vector<char**>& boards, char**& board, char piece);

		//callbacks list of pieces surrounding specified position
		void get_neighbors(char (&board)[size+1][size+1], char*& neighbors, int x, int y);
		int countPieces(char (&board)[size+1][size+1], char piece);
		double countPositionWeights(char (&board)[size+1][size+1], int level, char piece);
		double getWeight(int level, int col, int row);
		

		bool isEqual(char (&board)[size+1][size+1], char (&other_board)[size+1][size+1]);
		vector<vector<int>> getDifferenceCoordinates(char (&board)[size+1][size+1], char (&board2)[size+1][size+1]);
		char getPieceAtPosition(char (&board)[size+1][size+1], int col, int row);
		void copyBoard(char (&board)[size+1][size+1], char (&old_board)[size+1][size+1]);

		void printBoard(char (&board)[size+1][size+1], int num_indents=0);

};



#endif


