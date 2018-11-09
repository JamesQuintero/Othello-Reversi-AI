#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Board.h"


#ifndef Tree_H
#define Tree_H

using namespace std;



class Tree
{
	private:
		//side length of the Othello board
		static const int size = 8;
		//percentage of the "neuron" that gets increment if its path results in a good outcome
		// static constexpr double perc = 0.25;

		
		struct node
		{
			//matrix of board state
			//acts like the ID of the node
			// int** board;
			Board board;

			int piece = 0;


			//heuristic
			double h = 0;
			
			//indices for children
			int next_index = 0;
			int prev_index = 0;

			//children
			node** next = new node*[size*size];
			//parent
			node* prev;
		};
		

	public:
		int piece = 0;

		int AI_piece = 0;
		int player_piece = 0;

		//root node of the tree
		node * root;

		//current position in the tree
		node * ptr;

		int max_depth = 3;
		int max_h_depth = 10;


		Tree();
		//links a new node to ptr, with initialized board
		void newNode(node * ptr, Board new_board, int piece);

		void determinePossibleMoves(node* ptr, int piece);
		int getMinHeuristic(node * ptr, int alpha, int beta, int depth_left);
		int getMaxHeuristic(node * ptr, int alpha, int beta, int depth_left);
		int calculateHeuristic(node* ptr);

		bool hasLegalMoves(node* ptr);
		Board getBoardMinHeuristic(node* ptr);

		void iterateTreeDepth(node* ptr, int piece, int cur_depth, int max_depth);


		//returns a new board containing Tree's next move
		// int** AIMove(int** board, int** possible_moves);
		void AIMove(int col, int row);
		void playerMove(int col, int row);
		void move(Board board);

		void printNode(node * ptr, int indent=0);
		void printNet(node * ptr, int indent=0);

		int getOtherPiece(int piece);



};



#endif


