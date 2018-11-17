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
			// Board board;

			char** board = new char*[size];

			char piece = '0';

			//heuristic
			float player_mobility = -1.0;
			float player_potential_mobility = -1.0;
			float AI_mobility = -1.0;
			float AI_potential_mobility = -1.0;
			float h = 0;


			short level = 0;

			//weights for reinforement learning
			float good = 0;
			float bad = 0;

			//indices for children
			short next_index = 0;

			//children
			// node** next = new node*[size*size];
			node** next = new node*[4*(size-2)];

			//parent
			node* prev;
		};
		

	public:
		Board board_obj;

		char piece = '0';

		char AI_piece = '0';
		char player_piece = '0';
		//the piece that will get the worse heuristic. 0 by default. 
		char worse_heuristic_piece = '0';

		//root node of the tree
		node * root = NULL;

		//current position in the tree
		node * ptr;

		//depth should be odd so as to not give advance to the side in minimax that wants to maximize or minimize. 
		int max_depth = 2;
		int max_h_depth = 2;

		int good_weight = 100;


		Tree();

		void eraseParentNodes(node * ptr);
		void eraseBranch(node * ptr, node * ptr_to_keep);
		void resetTree();
		void eraseTree(node * ptr);
		//links a new node to ptr, with initialized board
		void newNode(node * ptr, char**& new_board, char piece);

		void determinePossibleMoves(node* ptr, char piece);
		double negamax(node* start, node* ptr, int depth_left, double alpha, double beta, int is_maximizing);
		double getMinHeuristic(node* start, node * ptr, double alpha, double beta, int depth_left);
		double getMaxHeuristic(node* start, node * ptr, double alpha, double beta, int depth_left);
		double calculateHeuristic(node* start, node* ptr);

		bool hasLegalMoves(node* ptr);
		//returns the child board with the minimum heuristic. Uesd for AI
		char** getBoardMinHeuristic(node* ptr);
		//returns the child board with the maximum heuristic. Uesd for player
		char** getBoardMaxHeuristic(node* ptr);

		void iterateTreeDepth(node* ptr, char piece, int cur_depth, int max_depth);


		//returns a new board containing Tree's next move
		// int** AIMove(int** board, int** possible_moves);
		void AIMove(int col, int row);
		void playerMove(int col, int row);
		void move(char**& board);

		//reinforcement methods
		void reinforceGood(node * ptr);
		void reinforceBad(node * ptr);


		void printNode(node * ptr, int indent=0);
		void printNet(node * ptr, int indent=0);

		char getOtherPiece(char piece);



};



#endif


